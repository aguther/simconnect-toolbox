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

#include "SimConnectSourceAutopilotEvents.h"

#include <BlockFactory/Core/Log.h>
#include <BlockFactory/Core/Parameter.h>
#include <BlockFactory/Core/Signal.h>

using namespace blockfactory::core;
using namespace simconnect::toolbox::blocks;

unsigned SimConnectSourceAutopilotEvents::numberOfParameters() {
  return Block::numberOfParameters() + 2;
}

bool SimConnectSourceAutopilotEvents::parseParameters(
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

bool SimConnectSourceAutopilotEvents::configureSizeAndPorts(
    BlockInformation *blockInfo
) {
  if (!Block::configureSizeAndPorts(blockInfo)) {
    return false;
  }

  // parse the parameters
  if (!SimConnectSourceAutopilotEvents::parseParameters(blockInfo)) {
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
    outputPortInfo.push_back(
        {
            4,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            5,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            6,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            7,
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

bool SimConnectSourceAutopilotEvents::initialize(
    BlockInformation *blockInfo
) {
  // the base Block class need to be initialized first
  if (!Block::initialize(blockInfo)) {
    return false;
  }

  // parse the parameters
  if (!SimConnectSourceAutopilotEvents::parseParameters(blockInfo)) {
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
    bool boolResult = addEvent(0, "A32NX.FCU_AP_1_PUSH", true);
    boolResult &= addEvent(1, "A32NX.FCU_AP_2_PUSH", false);
    boolResult &= addEvent(2, "AUTOPILOT_OFF", false);
    boolResult &= addEvent(3, "HEADING_SLOT_INDEX_SET", false);
    boolResult &= addEvent(4, "ALTITUDE_SLOT_INDEX_SET", false);
    boolResult &= addEvent(5, "AP_PANEL_VS_ON", false);
    boolResult &= addEvent(6, "A32NX.FCU_LOC_PUSH", false);
    boolResult &= addEvent(7, "A32NX.FCU_APPR_PUSH", false);

    HRESULT result = SimConnect_SetNotificationGroupPriority(
        simConnectHandle,
        0,
        SIMCONNECT_GROUP_PRIORITY_HIGHEST_MASKABLE
    );

    if (FAILED(result) || !boolResult) {
      bfError << "Failed to initialize events";
      return false;
    }

  } catch (std::exception &ex) {
    bfError << "Failed to parse variables: " << ex.what();
    return false;
  }

  return true;
}

bool SimConnectSourceAutopilotEvents::output(
    const BlockInformation *blockInfo
) {
  // vector for output signals
  std::vector<OutputSignalPtr> outputSignals;
  for (int kI = 0; kI < 8; ++kI) {
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
  outputSignals[0]->set(0, data.ap_1);
  outputSignals[1]->set(0, data.ap_2);
  outputSignals[2]->set(0, data.apOff);
  outputSignals[3]->set(0, data.headingSlotIndexSet);
  outputSignals[4]->set(0, data.altitudeSlotIndexSet);
  outputSignals[5]->set(0, data.apPanelVsOn);
  outputSignals[6]->set(0, data.apLocHold);
  outputSignals[7]->set(0, data.apAprHold);

  // reset signals
  data.ap_1 = 0;
  data.ap_2 = 0;
  data.apOff = 0;
  data.headingSlotIndexSet = 0;
  data.altitudeSlotIndexSet = 0;
  data.apPanelVsOn = 0;
  data.apLocHold = 0;
  data.apAprHold = 0;

  // return result
  return true;
}

bool SimConnectSourceAutopilotEvents::terminate(
    const BlockInformation *blockInfo
) {
  // disconnect
  SimConnect_Close(simConnectHandle);
  simConnectHandle = nullptr;

  // success
  return true;
}

bool SimConnectSourceAutopilotEvents::addEvent(
    int eventId,
    const std::string &eventName,
    bool shouldMask
) {
  if (FAILED(SimConnect_MapClientEventToSimEvent(simConnectHandle, eventId, eventName.c_str()))) {
    return false;
  }
  if (FAILED(SimConnect_AddClientEventToNotificationGroup(simConnectHandle, 0, eventId, shouldMask ? 1 : 0))) {
    return false;
  }
  return true;
}

void SimConnectSourceAutopilotEvents::processDispatch() {
  DWORD cbData;
  SIMCONNECT_RECV *pData;
  while (SUCCEEDED(SimConnect_GetNextDispatch(simConnectHandle, &pData, &cbData))) {
    dispatchProcedure(pData, &cbData);
  }
}

void SimConnectSourceAutopilotEvents::dispatchProcedure(
    SIMCONNECT_RECV *pData,
    DWORD *cbData
) {
  switch (pData->dwID) {
    case SIMCONNECT_RECV_ID_EVENT: {
      auto *event = (SIMCONNECT_RECV_EVENT *) pData;
      switch (event->uEventID) {
        case 0: {
          data.ap_1 = 1;
          break;
        }

        case 1: {
          data.ap_2 = 1;
          break;
        }

        case 2: {
          data.apOff = 1;
          break;
        }

        case 3: {
          data.headingSlotIndexSet = event->dwData;
          break;
        }

        case 4: {
          data.altitudeSlotIndexSet = event->dwData;
          break;
        }

        case 5: {
          data.apPanelVsOn = 1;
          break;
        }

        case 6: {
          data.apLocHold = 1;
          break;
        }

        case 7: {
          data.apAprHold = 1;
          break;
        }

        default:
          break;
      };
      break;
    }

    default:
      break;
  }
}
