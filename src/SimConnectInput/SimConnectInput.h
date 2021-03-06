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

#pragma once

#include <BlockFactory/Core/Block.h>
#include <BlockFactory/Core/BlockInformation.h>
#include <SimConnectData.h>
#include <SimConnectDataDefinition.h>
#include <SimConnectVariable.h>
#include <SimConnectInputInterface.h>
#include <SimConnectVariableLookupTable.h>

namespace simconnect::toolbox::blocks {
class SimConnectInput;
}

class simconnect::toolbox::blocks::SimConnectInput : public blockfactory::core::Block {
 public:
  static const std::string ClassName;

  SimConnectInput() = default;

  ~SimConnectInput() override = default;

  unsigned numberOfParameters() override;

  bool parseParameters(
      blockfactory::core::BlockInformation *blockInfo
  ) override;

  bool configureSizeAndPorts(
      blockfactory::core::BlockInformation *blockInfo
  ) override;

  bool initialize(
      blockfactory::core::BlockInformation *blockInfo
  ) override;

  bool output(
      const blockfactory::core::BlockInformation *blockInfo
  ) override;

  bool terminate(
      const blockfactory::core::BlockInformation *blockInfo
  ) override;

 private:
  int configurationIndex = 0;
  std::string connectionName;
  std::shared_ptr<simconnect::toolbox::connection::SimConnectData> simConnectData;
  simconnect::toolbox::connection::SimConnectDataDefinition simConnectDataDefinition;
  simconnect::toolbox::connection::SimConnectInputInterface simConnectInterface;
};
