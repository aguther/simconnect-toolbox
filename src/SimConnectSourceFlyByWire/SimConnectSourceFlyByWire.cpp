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

#include "SimConnectSourceFlyByWire.h"

#include <BlockFactory/Core/Log.h>
#include <BlockFactory/Core/Parameter.h>
#include <BlockFactory/Core/Signal.h>

using namespace blockfactory::core;
using namespace simconnect::toolbox::blocks;

unsigned SimConnectSourceFlyByWire::numberOfParameters() {
  return Block::numberOfParameters() + 2;
}

bool SimConnectSourceFlyByWire::parseParameters(
    BlockInformation *blockInfo
) {
  // get base index
  unsigned int index = Block::numberOfParameters();

  // define parameters
  const std::vector<ParameterMetadata> metadata{
      {ParameterType::INT, index++, 1, 1, "ConfigurationIndex"},
      {ParameterType::STRING, index++, 1, 1, "ConnectionName"},
  };

  // add parameters
  for (const auto &md : metadata) {
    if (!blockInfo->addParameterMetadata(md)) {
      bfError << "Failed to store parameter metadata";
      return false;
    }
  }

  return blockInfo->parseParameters(m_parameters);
}

bool SimConnectSourceFlyByWire::configureSizeAndPorts(
    BlockInformation *blockInfo
) {
  if (!Block::configureSizeAndPorts(blockInfo)) {
    return false;
  }

  // parse the parameters
  if (!SimConnectSourceFlyByWire::parseParameters(blockInfo)) {
    bfError << "Failed to parse parameters.";
    return false;
  }
  // store together the port information objects
  InputPortsInfo inputPortInfo;
  OutputPortsInfo outputPortInfo;

  // get output count
  try {
    outputPortInfo.push_back(
        {
            0,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            1,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            2,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            3,
            {1},
            Port::DataType::DOUBLE
        }
    );
  } catch (std::exception &ex) {
    bfError << "Failed to parse variables: " << ex.what();
    return false;
  }

  // store the port information into the BlockInformation
  if (!blockInfo->setPortsInfo(inputPortInfo, outputPortInfo)) {
    bfError << "Failed to configure input / output ports";
    return false;
  }

  return true;
}

bool SimConnectSourceFlyByWire::initialize(
    BlockInformation *blockInfo
) {
  // the base Block class need to be initialized first
  if (!Block::initialize(blockInfo)) {
    return false;
  }

  // parse the parameters
  if (!SimConnectSourceFlyByWire::parseParameters(blockInfo)) {
    bfError << "Failed to parse parameters.";
    return false;
  }

  // read the Operation parameter and store it as a private member
  if (!m_parameters.getParameter("ConfigurationIndex", configurationIndex)) {
    bfError << "Failed to parse ConfigurationIndex parameter";
    return false;
  }

  // read the Operation parameter and store it as a private member
  if (!m_parameters.getParameter("ConnectionName", connectionName)) {
    bfError << "Failed to parse ConnectionName parameter";
    return false;
  }

  // connect to FS
  HRESULT connected = SimConnect_Open(
      &simConnectHandle,
      connectionName.c_str(),
      nullptr,
      0,
      nullptr,
      0
  );
  if (FAILED(connected)) {
    bfError << "Failed to connect to SimConnect";
    return false;
  }

  try {
    HRESULT result;
    result = SimConnect_MapClientDataNameToID(
        simConnectHandle, "A32NX_CLIENT_DATA_FLY_BY_WIRE", 0);

    result &= SimConnect_CreateClientData(
        simConnectHandle,
        0,
        sizeof(data),
        SIMCONNECT_CREATE_CLIENT_DATA_FLAG_DEFAULT
    );

    result &= SimConnect_AddToClientDataDefinition(
        simConnectHandle,
        0,
        SIMCONNECT_CLIENTDATAOFFSET_AUTO,
        SIMCONNECT_CLIENTDATATYPE_FLOAT64
    );
    result &= SimConnect_AddToClientDataDefinition(
        simConnectHandle,
        0,
        SIMCONNECT_CLIENTDATAOFFSET_AUTO,
        SIMCONNECT_CLIENTDATATYPE_FLOAT64
    );
    result &= SimConnect_AddToClientDataDefinition(
        simConnectHandle,
        0,
        SIMCONNECT_CLIENTDATAOFFSET_AUTO,
        SIMCONNECT_CLIENTDATATYPE_FLOAT64
    );
    result &= SimConnect_AddToClientDataDefinition(
        simConnectHandle,
        0,
        SIMCONNECT_CLIENTDATAOFFSET_AUTO,
        SIMCONNECT_CLIENTDATATYPE_FLOAT64
    );

    result &= SimConnect_RequestClientData(
        simConnectHandle,
        0,
        0,
        0,
        SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET
    );

    if (FAILED(result)) {
      bfError << "Failed to initialize client data";
      return false;
    }

  } catch (std::exception &ex) {
    bfError << "Failed to parse variables: " << ex.what();
    return false;
  }

  return true;
}

bool SimConnectSourceFlyByWire::output(
    const BlockInformation *blockInfo
) {
  // vector for output signals
  std::vector<OutputSignalPtr> outputSignals;
  for (int kI = 0; kI < 4; ++kI) {
    // get output signal
    auto outputSignal = blockInfo->getOutputPortSignal(kI);
    // check if output is ok
    if (!outputSignal) {
      bfError << "Signals not valid";
      return false;
    }
    // store signal
    outputSignals.emplace_back(outputSignal);
  }

  // get data from simconnect
  processDispatch();

  // write output value to all signals
  outputSignals[0]->set(0, data.alpha_floor_command);
  outputSignals[1]->set(0, data.protection_ap_disc);
  outputSignals[2]->set(0, data.v_alpha_prot_kn);
  outputSignals[3]->set(0, data.v_alpha_max_kn);

  // return result
  return true;
}

bool SimConnectSourceFlyByWire::terminate(
    const BlockInformation *blockInfo
) {
  // disconnect
  SimConnect_Close(simConnectHandle);
  simConnectHandle = nullptr;

  // success
  return true;
}

void SimConnectSourceFlyByWire::processDispatch() {
  DWORD cbData;
  SIMCONNECT_RECV *pData;
  while (SUCCEEDED(SimConnect_GetNextDispatch(simConnectHandle, &pData, &cbData))) {
    dispatchProcedure(pData, &cbData);
  }
}

void SimConnectSourceFlyByWire::dispatchProcedure(
    SIMCONNECT_RECV *pData,
    DWORD *cbData
) {
  switch (pData->dwID) {
    case SIMCONNECT_RECV_ID_CLIENT_DATA: {
      auto *event = (SIMCONNECT_RECV_CLIENT_DATA *) pData;
      switch (event->dwRequestID) {
        case 0:
          data = *reinterpret_cast<FlyByWire *>(&event->dwData);
          break;

        default:
          break;
      }
      break;
    }

    default:
      break;
  }
}
