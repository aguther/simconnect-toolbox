/*
 * Copyright 2020 Andreas Guther
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 *     limitations under the License.
 */

#include <map>
#include <vector>
#include "SimConnectDataInterface.h"

using namespace std;
using namespace simconnect::toolbox::connection;

bool SimConnectDataInterface::connect(
    int configurationIndex,
    const string &name,
    const SimConnectDataDefinition &dataDefinition,
    const shared_ptr<SimConnectData> &simConnectData
) {
  // store connection name
  connectionName = name;

  // connect
  HRESULT result = SimConnect_Open(
      &hSimConnect,
      connectionName.c_str(),
      nullptr,
      0,
      nullptr,
      configurationIndex
  );

  if (S_OK == result) {
    // we are now connected
    isConnected = true;
    // store data object
    this->data = simConnectData;
    // add data to definition
    if (!prepareDataDefinition(hSimConnect, 0, dataDefinition)) {
      // failed to add data definition -> disconnect
      disconnect();
      // failed to connect
      return false;
    }
    // success
    return true;
  }
  // fallback -> failed
  return false;
}

void SimConnectDataInterface::disconnect() {
  if (isConnected) {
    // close connection
    SimConnect_Close(hSimConnect);
    // set flag
    isConnected = false;
    // reset data object
    data.reset();
    // reset handle
    hSimConnect = nullptr;
  }
}

bool SimConnectDataInterface::requestReadData() {
  // check if we are connected
  if (!isConnected) {
    return false;
  }

  // request data
  if (!requestData()) {
    return false;
  }

  // read data
  if (!readData()) {
    return false;
  }

  // success
  return true;
}

bool SimConnectDataInterface::requestData() {
  // check if we are connected
  if (!isConnected) {
    return false;
  }

  // request data
  HRESULT result = SimConnect_RequestDataOnSimObjectType(
      hSimConnect,
      0,
      0,
      0,
      SIMCONNECT_SIMOBJECT_TYPE_USER
  );

  // check result of data request
  if (result != S_OK) {
    // request failed
    return false;
  }

  // success
  return true;
}

bool SimConnectDataInterface::readData() {
  // check if we are connected
  if (!isConnected) {
    return false;
  }

  // get next dispatch message(s) and process them
  DWORD cbData;
  SIMCONNECT_RECV *pData;
  while (SUCCEEDED(SimConnect_GetNextDispatch(hSimConnect, &pData, &cbData))) {
    simConnectProcessDispatchMessage(pData, &cbData);
  }

  // success
  return true;
}

bool SimConnectDataInterface::sendData() {
  // check if we are connected
  if (!isConnected) {
    return false;
  }

  // set output data
  HRESULT result = SimConnect_SetDataOnSimObject(
      hSimConnect,
      0,
      SIMCONNECT_OBJECT_ID_USER,
      0,
      0,
      static_cast<DWORD>(data->size()),
      data->getBuffer()
  );

  // check result of data request
  if (result != S_OK) {
    // request failed
    return false;
  }

  // success
  return true;
}

void SimConnectDataInterface::simConnectProcessDispatchMessage(
    SIMCONNECT_RECV *pData,
    DWORD *cbData
) {
  switch (pData->dwID) {
    case SIMCONNECT_RECV_ID_OPEN:
      // connection established
      cout << "SimConnect connection established ('" << connectionName << "')" << endl;
      break;

    case SIMCONNECT_RECV_ID_QUIT:
      // connection lost
      cout << "Closed SimConnect connection ('" << connectionName << "')" << endl;
      disconnect();
      break;

    case SIMCONNECT_RECV_ID_SIMOBJECT_DATA_BYTYPE:
      // process data
      simConnectProcessSimObjectDataByType(pData);
      break;

    case SIMCONNECT_RECV_ID_EXCEPTION:
      // exception
      cout << "Exception in SimConnect connection ('" << connectionName << "'): ";
      cout << ((SIMCONNECT_RECV_EXCEPTION *) pData)->dwException << endl;
      break;

    default:
      break;
  }
}

void SimConnectDataInterface::simConnectProcessSimObjectDataByType(
    const SIMCONNECT_RECV *pData
) {
  // get data object
  auto *simObjectDataByType = (SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE *) pData;

  // process depending on request id
  switch (simObjectDataByType->dwRequestID) {
    case 0:
      // store aircraft data
      data->copy(reinterpret_cast<char *>(&simObjectDataByType->dwData));
      break;

    default:
      // print unknown request id
      cout << "Unknown request id in SimConnect connection ('" << connectionName << "'): ";
      cout << simObjectDataByType->dwRequestID << endl;
      break;
  }
}

bool SimConnectDataInterface::prepareDataDefinition(
    HANDLE connectionHandle,
    SIMCONNECT_DATA_DEFINITION_ID id,
    SimConnectDataDefinition dataDefinition
) {
  // map for right order of data definitions
  map<SIMCONNECT_VARIABLE_TYPE, vector<SimConnectVariable>> dataDefinitionMap;
  for (int i = 0; i < dataDefinition.size(); ++i) {
    dataDefinitionMap[dataDefinition.getType(i)].push_back(dataDefinition.get(i));
  }

  // add data definitions
  bool result = addDataDefinition(
      connectionHandle,
      id,
      SIMCONNECT_VARIABLE_TYPE_BOOL,
      dataDefinitionMap[SIMCONNECT_VARIABLE_TYPE_BOOL]
  );
  if (!result) return false;

  result = addDataDefinition(
      connectionHandle,
      id,
      SIMCONNECT_VARIABLE_TYPE_INT32,
      dataDefinitionMap[SIMCONNECT_VARIABLE_TYPE_INT32]
  );
  if (!result) return false;

  result = addDataDefinition(
      connectionHandle,
      id,
      SIMCONNECT_VARIABLE_TYPE_FLOAT32,
      dataDefinitionMap[SIMCONNECT_VARIABLE_TYPE_FLOAT32]
  );
  if (!result) return false;

  result = addDataDefinition(
      connectionHandle,
      id,
      SIMCONNECT_VARIABLE_TYPE_FLOAT64,
      dataDefinitionMap[SIMCONNECT_VARIABLE_TYPE_FLOAT64]
  );
  if (!result) return false;

  result = addDataDefinition(
      connectionHandle,
      id,
      SIMCONNECT_VARIABLE_TYPE_LATLONALT,
      dataDefinitionMap[SIMCONNECT_VARIABLE_TYPE_LATLONALT]
  );
  if (!result) return false;

  result = addDataDefinition(
      connectionHandle,
      id,
      SIMCONNECT_VARIABLE_TYPE_XYZ,
      dataDefinitionMap[SIMCONNECT_VARIABLE_TYPE_XYZ]
  );
  if (!result) return false;

  // success
  return true;
}

bool SimConnectDataInterface::addDataDefinition(
    HANDLE connectionHandle,
    SIMCONNECT_DATA_DEFINITION_ID id,
    SIMCONNECT_VARIABLE_TYPE dataType,
    const vector<SimConnectVariable> &variables
) {
  for (auto &variable : variables) {
    HRESULT result = SimConnect_AddToDataDefinition(
        connectionHandle,
        id,
        variable.name.c_str(),
        SimConnectVariableType::isStruct(dataType) ? nullptr : variable.unit.c_str(),
        SimConnectVariableType::convert(dataType)
    );
    if (result != S_OK) {
      // failed -> abort
      return false;
    }
  }
  return true;
}
