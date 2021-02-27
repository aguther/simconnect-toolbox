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
class SimConnectSinkAutothrust;
}

class simconnect::toolbox::blocks::SimConnectSinkAutothrust : public blockfactory::core::Block {
 public:
  static const std::string ClassName;

  SimConnectSinkAutothrust() = default;

  ~SimConnectSinkAutothrust() override = default;

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
  struct Autothrust {
    double N1_TLA_1_percent;
    double N1_TLA_2_percent;
    double is_in_reverse_1;
    double is_in_reverse_2;
    double thrust_limit_type;
    double thrust_limit_percent;
    double N1_c_1_percent;
    double N1_c_2_percent;
    double status;
    double mode;
    double mode_message;
  };

  int configurationIndex = 0;
  std::string connectionName;
  HANDLE simConnectHandle = nullptr;
  Autothrust data;
  Autothrust lastData = {
      -1,
      -1,
      -1,
      -1,
      -1,
      -1,
      -1,
      -1,
      -1,
      -1,
      -1,
  };
};
