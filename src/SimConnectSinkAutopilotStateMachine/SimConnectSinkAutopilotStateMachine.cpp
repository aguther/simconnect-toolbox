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

#include "SimConnectSinkAutopilotStateMachine.h"

#include <BlockFactory/Core/Log.h>
#include <BlockFactory/Core/Parameter.h>
#include <BlockFactory/Core/Signal.h>

using namespace blockfactory::core;
using namespace simconnect::toolbox::blocks;

unsigned SimConnectSinkAutopilotStateMachine::numberOfParameters() {
  return Block::numberOfParameters() + 2;
}

bool SimConnectSinkAutopilotStateMachine::parseParameters(
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

bool SimConnectSinkAutopilotStateMachine::configureSizeAndPorts(
    BlockInformation *blockInfo
) {
  if (!Block::configureSizeAndPorts(blockInfo)) {
    return false;
  }

  // parse the parameters
  if (!SimConnectSinkAutopilotStateMachine::parseParameters(blockInfo)) {
    bfError << "Failed to parse parameters.";
    return false;
  }
  // store together the port information objects
  InputPortsInfo inputPortInfo;
  OutputPortsInfo outputPortInfo;

  // get output count
  try {
    inputPortInfo.push_back(
        {0,
         {1},
         Port::DataType::DOUBLE}
    );
    inputPortInfo.push_back(
        {1,
         {1},
         Port::DataType::DOUBLE}
    );
    inputPortInfo.push_back(
        {2,
         {1},
         Port::DataType::DOUBLE}
    );
    inputPortInfo.push_back(
        {3,
         {1},
         Port::DataType::DOUBLE}
    );
    inputPortInfo.push_back(
        {4,
         {1},
         Port::DataType::DOUBLE}
    );
    inputPortInfo.push_back(
        {5,
         {1},
         Port::DataType::DOUBLE}
    );
    inputPortInfo.push_back(
        {6,
         {1},
         Port::DataType::DOUBLE}
    );
    inputPortInfo.push_back(
        {7,
         {1},
         Port::DataType::DOUBLE}
    );
    inputPortInfo.push_back(
        {8,
         {1},
         Port::DataType::DOUBLE}
    );
    inputPortInfo.push_back(
        {9,
         {1},
         Port::DataType::DOUBLE}
    );
    inputPortInfo.push_back(
        {10,
         {1},
         Port::DataType::DOUBLE}
    );
    inputPortInfo.push_back(
        {11,
         {1},
         Port::DataType::DOUBLE}
    );
    inputPortInfo.push_back(
        {12,
         {1},
         Port::DataType::DOUBLE}
    );
    inputPortInfo.push_back(
        {13,
         {1},
         Port::DataType::DOUBLE}
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

bool SimConnectSinkAutopilotStateMachine::initialize(
    BlockInformation *blockInfo
) {
  // the base Block class need to be initialized first
  if (!Block::initialize(blockInfo)) {
    return false;
  }

  // parse the parameters
  if (!SimConnectSinkAutopilotStateMachine::parseParameters(blockInfo)) {
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
    HRESULT result = true;
    result &= SimConnect_MapClientDataNameToID(
        simConnectHandle, "A32NX_CLIENT_DATA_AUTOPILOT_STATE_MACHINE", 0);

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
        SIMCONNECT_CLIENTDATATYPE_INT64
    );
    result &= SimConnect_AddToClientDataDefinition(
        simConnectHandle,
        0,
        SIMCONNECT_CLIENTDATAOFFSET_AUTO,
        SIMCONNECT_CLIENTDATATYPE_INT64
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

bool SimConnectSinkAutopilotStateMachine::output(
    const BlockInformation *blockInfo
) {
  // vector for output signals
  std::vector<InputSignalPtr> inputSignals;
  for (int kI = 0; kI < 14; ++kI) {
    // get output signal
    auto outputSignal = blockInfo->getInputPortSignal(kI);
    // check if output is ok
    if (!outputSignal) {
      bfError << "Signals not valid";
      return false;
    }
    // store signal
    inputSignals.emplace_back(outputSignal);
  }

  // write output value to all signals
  data.enabled_AP1 = inputSignals[0]->get<double>(0) != 0;
  data.enabled_AP2 = inputSignals[1]->get<double>(0) != 0;
  data.lateral_law = inputSignals[2]->get<double>(0);
  data.lateral_mode = inputSignals[3]->get<double>(0);
  data.lateral_mode_armed = inputSignals[4]->get<double>(0);
  data.vertical_law = inputSignals[5]->get<double>(0);
  data.vertical_mode = inputSignals[6]->get<double>(0);
  data.vertical_mode_armed = inputSignals[7]->get<double>(0);
  data.mode_reversion = inputSignals[8]->get<double>(0);
  data.autothrust_mode = inputSignals[9]->get<double>(0);
  data.Psi_c_deg = inputSignals[10]->get<double>(0);
  data.H_c_ft = inputSignals[11]->get<double>(0);
  data.H_dot_c_fpm = inputSignals[12]->get<double>(0);
  data.FPA_c_deg = inputSignals[13]->get<double>(0);

  // only write when needed
  if (data.enabled_AP1 != lastData.enabled_AP1
      || data.enabled_AP2 != lastData.enabled_AP2
      || data.lateral_law != lastData.lateral_law
      || data.lateral_mode != lastData.lateral_mode
      || data.lateral_mode_armed != lastData.lateral_mode_armed
      || data.vertical_law != lastData.vertical_law
      || data.vertical_mode != lastData.vertical_mode
      || data.vertical_mode_armed != lastData.vertical_mode_armed
      || data.mode_reversion != lastData.mode_reversion
      || data.autothrust_mode != lastData.autothrust_mode
      || data.Psi_c_deg != lastData.Psi_c_deg
      || data.H_c_ft != lastData.H_c_ft
      || data.H_dot_c_fpm != lastData.H_dot_c_fpm
      || data.FPA_c_deg != lastData.FPA_c_deg) {
    // write data to simconnect
    HRESULT result = SimConnect_SetClientData(
        simConnectHandle,
        0,
        0,
        SIMCONNECT_CLIENT_DATA_SET_FLAG_DEFAULT,
        0,
        sizeof(data),
        &data
    );

    // check result
    if (FAILED(result)) {
      bfError << "Failed to write to SimConnect";
      return false;
    }

    // remember new settings
    lastData = data;
  }

  // return result
  return true;
}

bool SimConnectSinkAutopilotStateMachine::terminate(
    const BlockInformation *blockInfo
) {
  // disconnect
  SimConnect_Close(simConnectHandle);
  simConnectHandle = nullptr;

  // success
  return true;
}
