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

#include <Windows.h>
#include <SimConnect.h>
#include <BlockFactory/Core/Block.h>
#include <BlockFactory/Core/BlockInformation.h>

namespace simconnect::toolbox::blocks {
class SimConnectSinkAutopilotStateMachine;
}

class simconnect::toolbox::blocks::SimConnectSinkAutopilotStateMachine : public blockfactory::core::Block {
 public:
  static const std::string ClassName;

  SimConnectSinkAutopilotStateMachine() = default;

  ~SimConnectSinkAutopilotStateMachine() override = default;

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
  struct AutopilotStateMachine {
    unsigned long long enabled;
    double lateral_law;
    double lateral_mode;
    double lateral_mode_armed;
    double vertical_law;
    double vertical_mode;
    double vertical_mode_armed;
    double Psi_c_deg;
    double H_c_ft;
    double H_dot_c_fpm;
    double FPA_c_deg;
  };

  int configurationIndex = 0;
  std::string connectionName;
  HANDLE simConnectHandle = nullptr;
  AutopilotStateMachine data;
  AutopilotStateMachine lastData = {
      false,
      -1,
      -1,
      -1,
      -1,
      -1,
      -1,
      -1,
      -1,
      -1,
      -1
  };
};