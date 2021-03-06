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

#include "SimConnectSource.h"

#include <BlockFactory/Core/Log.h>
#include <BlockFactory/Core/Parameter.h>
#include <BlockFactory/Core/Signal.h>
#include <SimConnectVariableParser.h>

using namespace blockfactory::core;
using namespace simconnect::toolbox::blocks;
using namespace simconnect::toolbox::connection;

unsigned SimConnectSource::numberOfParameters() {
  return Block::numberOfParameters() + 3;
}

bool SimConnectSource::parseParameters(
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

bool SimConnectSource::configureSizeAndPorts(
    BlockInformation *blockInfo
) {
  if (!Block::configureSizeAndPorts(blockInfo)) {
    return false;
  }

  // parse the parameters
  if (!SimConnectSource::parseParameters(blockInfo)) {
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
      switch (SimConnectVariableLookupTable::getDataType(variables[kI])) {
        case SIMCONNECT_VARIABLE_TYPE_BOOL:
        case SIMCONNECT_VARIABLE_TYPE_INT32:
        case SIMCONNECT_VARIABLE_TYPE_FLOAT32:
        case SIMCONNECT_VARIABLE_TYPE_FLOAT64:
          outputPortInfo.push_back(
              {
                  kI,
                  {1},
                  Port::DataType::DOUBLE
              }
          );
          break;

        case SIMCONNECT_VARIABLE_TYPE_LATLONALT:
        case SIMCONNECT_VARIABLE_TYPE_XYZ:
          outputPortInfo.push_back(
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

bool SimConnectSource::initialize(
    BlockInformation *blockInfo
) {
  // the base Block class need to be initialized first
  if (!Block::initialize(blockInfo)) {
    return false;
  }

  // parse the parameters
  if (!SimConnectSource::parseParameters(blockInfo)) {
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
    simConnectDataDefinition = SimConnectVariableParser::getSimConnectDataDefinitionFromVariables(simConnectVariables);

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

bool SimConnectSource::output(
    const BlockInformation *blockInfo
) {
  // vector for output signals
  std::vector<OutputSignalPtr> outputSignals;
  for (int kI = 0; kI < simConnectDataDefinition.size(); ++kI) {
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
  if (!simConnectInterface.requestData()) {
    bfError << "Failed to request data from SimConnect";
    return false;
  }
  if (!simConnectInterface.readData()) {
    bfError << "Failed to read data from SimConnect";
    return false;
  }

  // write output value to all signals
  for (int kI = 0; kI < outputSignals.size(); ++kI) {
    switch (simConnectDataDefinition.getType(kI)) {
      case SIMCONNECT_VARIABLE_TYPE_BOOL:
        outputSignals[kI]->set(0, std::any_cast<bool>(simConnectData->get(kI)) ? 1.0 : 0.0);
        break;

      case SIMCONNECT_VARIABLE_TYPE_INT32:
        outputSignals[kI]->set(0, static_cast<double>(std::any_cast<long>(simConnectData->get(kI))));
        break;

      case SIMCONNECT_VARIABLE_TYPE_FLOAT32:
        outputSignals[kI]->set(0, static_cast<double>(std::any_cast<float>(simConnectData->get(kI))));
        break;

      case SIMCONNECT_VARIABLE_TYPE_FLOAT64:
        outputSignals[kI]->set(0, std::any_cast<double>(simConnectData->get(kI)));
        break;

      case SIMCONNECT_VARIABLE_TYPE_LATLONALT:
        outputSignals[kI]->set(0, std::any_cast<SIMCONNECT_DATA_LATLONALT>(simConnectData->get(kI)).Latitude);
        outputSignals[kI]->set(1, std::any_cast<SIMCONNECT_DATA_LATLONALT>(simConnectData->get(kI)).Longitude);
        outputSignals[kI]->set(2, std::any_cast<SIMCONNECT_DATA_LATLONALT>(simConnectData->get(kI)).Altitude);
        break;

      case SIMCONNECT_VARIABLE_TYPE_XYZ:
        outputSignals[kI]->set(0, std::any_cast<SIMCONNECT_DATA_XYZ>(simConnectData->get(kI)).x);
        outputSignals[kI]->set(1, std::any_cast<SIMCONNECT_DATA_XYZ>(simConnectData->get(kI)).y);
        outputSignals[kI]->set(2, std::any_cast<SIMCONNECT_DATA_XYZ>(simConnectData->get(kI)).z);
        break;

      default:
        break;
    }
  }

  // return result
  return true;
}

bool SimConnectSource::terminate(
    const BlockInformation *blockInfo
) {
  // disconnect
  simConnectInterface.disconnect();

  // reset simconnect data
  simConnectData.reset();

  // success
  return true;
}
