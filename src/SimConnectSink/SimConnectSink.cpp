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

#include "SimConnectSink.h"

#include <BlockFactory/Core/Log.h>
#include <BlockFactory/Core/Parameter.h>
#include <BlockFactory/Core/Signal.h>
#include <SimConnectVariableParser.h>

using namespace blockfactory::core;
using namespace simconnect::toolbox::blocks;
using namespace simconnect::toolbox::connection;

unsigned SimConnectSink::numberOfParameters() {
  return Block::numberOfParameters() + 3;
}

bool SimConnectSink::parseParameters(
    BlockInformation *blockInfo
) {
  // get base index
  unsigned int index = Block::numberOfParameters();

  // define parameters
  const std::vector<ParameterMetadata> metadata{
      {ParameterType::INT, index++, 1, 1, "ConfigurationIndex"},
      {ParameterType::STRING, index++, 1, 1, "ConnectionName"},
      {ParameterType::STRING, index++, 1, 1, "Variables"}
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

bool SimConnectSink::configureSizeAndPorts(
    BlockInformation *blockInfo
) {
  if (!Block::configureSizeAndPorts(blockInfo)) {
    return false;
  }

  // parse the parameters
  if (!SimConnectSink::parseParameters(blockInfo)) {
    bfError << "Failed to parse parameters.";
    return false;
  }
  // store together the port information objects
  InputPortsInfo inputPortInfo;
  OutputPortsInfo outputPortInfo;

  // read variables parameter
  std::string parameterVariables;
  if (!m_parameters.getParameter("Variables", parameterVariables)) {
    bfError << "Failed to parse Operation parameter";
    return false;
  }

  // get output count
  try {
    auto variables = SimConnectVariableParser::getSimConnectVariablesFromParameterString(parameterVariables);
    for (unsigned long long kI = 0; kI < variables.size(); ++kI) {
      switch (SimConnectVariableLookupTable::getDataType(variables[kI], SimConnectVariableLookupTable::Type::Data)) {
        case SIMCONNECT_VARIABLE_TYPE_BOOL:
        case SIMCONNECT_VARIABLE_TYPE_INT32:
        case SIMCONNECT_VARIABLE_TYPE_FLOAT32:
        case SIMCONNECT_VARIABLE_TYPE_FLOAT64:
          inputPortInfo.push_back(
              {kI,
               {1},
               Port::DataType::DOUBLE});
          break;

        case SIMCONNECT_VARIABLE_TYPE_LATLONALT:
        case SIMCONNECT_VARIABLE_TYPE_XYZ:
          inputPortInfo.push_back(
              {
                  kI,
                  {3},
                  Port::DataType::DOUBLE
              }
          );
          break;

        default:
          break;
      }
    }
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

bool SimConnectSink::initialize(
    BlockInformation *blockInfo
) {
  // the base Block class need to be initialized first
  if (!Block::initialize(blockInfo)) {
    return false;
  }

  // parse the parameters
  if (!SimConnectSink::parseParameters(blockInfo)) {
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

  // read variables parameter
  std::string parameterVariables;
  if (!m_parameters.getParameter("Variables", parameterVariables)) {
    bfError << "Failed to parse Variables parameter";
    return false;
  }
  try {
    // parse variables and get data definition
    auto simConnectVariables = SimConnectVariableParser::getSimConnectVariablesFromParameterString(parameterVariables);
    simConnectDataDefinition = SimConnectVariableParser::getSimConnectDataDefinitionFromVariables(
        simConnectVariables, SimConnectVariableLookupTable::Type::Data);

    // create data object
    simConnectData = std::make_shared<SimConnectData>(simConnectDataDefinition);

  } catch (std::exception &ex) {
    bfError << "Failed to parse variables: " << ex.what();
    return false;
  }

  // connect to FS
  bool connected = simConnectInterface.connect(
      configurationIndex,
      connectionName,
      simConnectDataDefinition,
      simConnectData
  );
  if (!connected) {
    bfError << "Failed to connect to SimConnect";
    return false;
  }

  return true;
}

bool SimConnectSink::output(
    const BlockInformation *blockInfo
) {
  // vector for output signals
  std::vector<InputSignalPtr> inputSignals;
  for (int kI = 0; kI < simConnectDataDefinition.size(); ++kI) {
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
  for (int kI = 0; kI < inputSignals.size(); ++kI) {
    switch (simConnectDataDefinition.getType(kI)) {
      case SIMCONNECT_VARIABLE_TYPE_BOOL:
        simConnectData->set(kI, static_cast<bool>(inputSignals[kI]->get<double>(0) != 0));
        break;

      case SIMCONNECT_VARIABLE_TYPE_INT32:
        simConnectData->set(kI, static_cast<long>(inputSignals[kI]->get<double>(0)));
        break;

      case SIMCONNECT_VARIABLE_TYPE_FLOAT32:
        simConnectData->set(kI, static_cast<float>(inputSignals[kI]->get<double>(0)));
        break;

      case SIMCONNECT_VARIABLE_TYPE_FLOAT64:
        simConnectData->set(kI, inputSignals[kI]->get<double>(0));
        break;

      case SIMCONNECT_VARIABLE_TYPE_LATLONALT:
        simConnectData->set(
            kI,
            SIMCONNECT_DATA_LATLONALT{
                inputSignals[kI]->get<double>(0),
                inputSignals[kI]->get<double>(1),
                inputSignals[kI]->get<double>(2)
            }
        );
        break;

      case SIMCONNECT_VARIABLE_TYPE_XYZ:
        simConnectData->set(
            kI,
            SIMCONNECT_DATA_XYZ{
                inputSignals[kI]->get<double>(0),
                inputSignals[kI]->get<double>(1),
                inputSignals[kI]->get<double>(2)
            }
        );
        break;

      default:
        break;
    }
  }

  // write data to simconnect
  if (!simConnectInterface.sendData()) {
    bfError << "Failed to write to SimConnect";
    return false;
  }

  // return result
  return true;
}

bool SimConnectSink::terminate(
    const BlockInformation *blockInfo
) {
  // disconnect
  simConnectInterface.disconnect();

  // reset simconnect data
  simConnectData.reset();

  // success
  return true;
}
