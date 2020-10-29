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
#include "SimConnectInputInterface.h"

using namespace std;
using namespace simconnect::toolbox::connection;

bool SimConnectInputInterface::connect(
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

void SimConnectInputInterface::disconnect() {
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

bool SimConnectInputInterface::readData() {
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

void SimConnectInputInterface::simConnectProcessDispatchMessage(
    SIMCONNECT_RECV *pData,
    DWORD *cbData
) {
  switch (pData->dwID) {
    case SIMCONNECT_RECV_ID_OPEN:
      // connection established
      cout << "SimConnect connection established ('" << connectionName << "')" << endl;
      break;

    case SIMCONNECT_RECV_ID_EVENT:
      // get event
      simConnectProcessEvent(pData);
      break;

    case SIMCONNECT_RECV_ID_QUIT:
      // connection lost
      cout << "Closed SimConnect connection ('" << connectionName << "')" << endl;
      disconnect();
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

void SimConnectInputInterface::simConnectProcessEvent(
    const SIMCONNECT_RECV *pData
) {
  // get event
  auto *event = (SIMCONNECT_RECV_EVENT *) pData;

  // process depending on event id
  data->set(event->uEventID, static_cast<long>(event->dwData) / 16384.0);
}

bool SimConnectInputInterface::prepareDataDefinition(
    HANDLE connectionHandle,
    SIMCONNECT_DATA_DEFINITION_ID id,
    SimConnectDataDefinition dataDefinition
) {
  // iterate over data definitions
  for (int i = 0; i < dataDefinition.size(); ++i) {
    bool result = addDataDefinition(
        connectionHandle,
        id,
        i,
        dataDefinition.get(i)
    );
    if (!result) return false;
  }

  // success
  return true;
}

bool SimConnectInputInterface::addDataDefinition(
    HANDLE connectionHandle,
    SIMCONNECT_DATA_DEFINITION_ID groupId,
    SIMCONNECT_CLIENT_EVENT_ID eventId,
    const SimConnectVariable &variable
) {
  HRESULT result = SimConnect_MapClientEventToSimEvent(
      connectionHandle,
      eventId,
      variable.name.c_str()
  );
  if (result != S_OK) {
    // failed -> abort
    return false;
  }

  result = SimConnect_AddClientEventToNotificationGroup(
      connectionHandle,
      groupId,
      eventId,
      variable.unit == "TRUE" ? TRUE : FALSE
  );
  if (result != S_OK) {
    // failed -> abort
    return false;
  }

  result = SimConnect_SetNotificationGroupPriority(
      connectionHandle,
      groupId,
      SIMCONNECT_GROUP_PRIORITY_HIGHEST_MASKABLE
  );
  if (result != S_OK) {
    // failed -> abort
    return false;
  }

  // success
  return true;
}
