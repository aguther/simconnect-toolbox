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

#include "SimConnectSourceLocalVariablesAutothrust.h"

#include <BlockFactory/Core/Log.h>
#include <BlockFactory/Core/Parameter.h>
#include <BlockFactory/Core/Signal.h>

using namespace blockfactory::core;
using namespace simconnect::toolbox::blocks;

unsigned SimConnectSourceLocalVariablesAutothrust::numberOfParameters() {
  return Block::numberOfParameters() + 2;
}

bool SimConnectSourceLocalVariablesAutothrust::parseParameters(
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

bool SimConnectSourceLocalVariablesAutothrust::configureSizeAndPorts(
    BlockInformation *blockInfo
) {
  if (!Block::configureSizeAndPorts(blockInfo)) {
    return false;
  }

  // parse the parameters
  if (!SimConnectSourceLocalVariablesAutothrust::parseParameters(blockInfo)) {
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
    outputPortInfo.push_back(
        {
            8,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            9,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            10,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            11,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            12,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            13,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            14,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            15,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            16,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            17,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            18,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            19,
            {1},
            Port::DataType::DOUBLE
        }
    );
    outputPortInfo.push_back(
        {
            20,
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

bool SimConnectSourceLocalVariablesAutothrust::initialize(
    BlockInformation *blockInfo
) {
  // the base Block class need to be initialized first
  if (!Block::initialize(blockInfo)) {
    return false;
  }

  // parse the parameters
  if (!SimConnectSourceLocalVariablesAutothrust::parseParameters(blockInfo)) {
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
        simConnectHandle, "A32NX_CLIENT_DATA_LOCAL_VARIABLES_AUTOTHRUST", 0);

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

bool SimConnectSourceLocalVariablesAutothrust::output(
    const BlockInformation *blockInfo
) {
  // vector for output signals
  std::vector<OutputSignalPtr> outputSignals;
  for (int kI = 0; kI < 21; ++kI) {
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
  outputSignals[0]->set(0, data.ATHR_push);
  outputSignals[1]->set(0, data.TLA_1);
  outputSignals[2]->set(0, data.TLA_2);
  outputSignals[3]->set(0, data.V_c_kn);
  outputSignals[4]->set(0, data.V_LS_kn);
  outputSignals[5]->set(0, data.V_MAX_kn);
  outputSignals[6]->set(0, data.thrust_limit_REV_percent);
  outputSignals[7]->set(0, data.thrust_limit_IDLE_percent);
  outputSignals[8]->set(0, data.thrust_limit_CLB_percent);
  outputSignals[9]->set(0, data.thrust_limit_MCT_percent);
  outputSignals[10]->set(0, data.thrust_limit_FLEX_percent);
  outputSignals[11]->set(0, data.thrust_limit_TOGA_percent);
  outputSignals[12]->set(0, data.flex_temperature_degC);
  outputSignals[13]->set(0, data.mode_requested);
  outputSignals[14]->set(0, data.is_mach_mode_active);
  outputSignals[15]->set(0, data.alpha_floor_condition);
  outputSignals[16]->set(0, data.is_approach_mode_active);
  outputSignals[17]->set(0, data.is_SRS_TO_mode_active);
  outputSignals[18]->set(0, data.is_SRS_GA_mode_active);
  outputSignals[19]->set(0, data.thrust_reduction_altitude);
  outputSignals[20]->set(0, data.thrust_reduction_altitude_go_around);

  // return result
  return true;
}

bool SimConnectSourceLocalVariablesAutothrust::terminate(
    const BlockInformation *blockInfo
) {
  // disconnect
  SimConnect_Close(simConnectHandle);
  simConnectHandle = nullptr;

  // success
  return true;
}

void SimConnectSourceLocalVariablesAutothrust::processDispatch() {
  DWORD cbData;
  SIMCONNECT_RECV *pData;
  while (SUCCEEDED(SimConnect_GetNextDispatch(simConnectHandle, &pData, &cbData))) {
    dispatchProcedure(pData, &cbData);
  }
}

void SimConnectSourceLocalVariablesAutothrust::dispatchProcedure(
    SIMCONNECT_RECV *pData,
    DWORD *cbData
) {
  switch (pData->dwID) {
    case SIMCONNECT_RECV_ID_CLIENT_DATA: {
      auto *event = (SIMCONNECT_RECV_CLIENT_DATA *) pData;
      switch (event->dwRequestID) {
        case 0:
          data = *reinterpret_cast<LocalVariablesAutothrust *>(&event->dwData);
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
