
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
class SimConnectSourceLocalVariablesAutothrust;
}

class simconnect::toolbox::blocks::SimConnectSourceLocalVariablesAutothrust : public blockfactory::core::Block {
 public:
  static const std::string ClassName;

  SimConnectSourceLocalVariablesAutothrust() = default;

  ~SimConnectSourceLocalVariablesAutothrust() override = default;

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
  struct LocalVariablesAutothrust {
    double ATHR_push;
    double ATHR_disconnect;
    double TLA_1;
    double TLA_2;
    double V_c_kn;
    double V_LS_kn;
    double V_MAX_kn;
    double thrust_limit_REV_percent;
    double thrust_limit_IDLE_percent;
    double thrust_limit_CLB_percent;
    double thrust_limit_MCT_percent;
    double thrust_limit_FLEX_percent;
    double thrust_limit_TOGA_percent;
    double flex_temperature_degC;
    double mode_requested;
    double is_mach_mode_active;
    double alpha_floor_condition;
    double is_approach_mode_active;
    double is_SRS_TO_mode_active;
    double is_SRS_GA_mode_active;
    double is_LAND_mode_active;
    double thrust_reduction_altitude;
    double thrust_reduction_altitude_go_around;
    double flight_phase;
    double is_soft_alt_mode_active;
  };

  int configurationIndex = 0;
  std::string connectionName;
  HANDLE simConnectHandle = nullptr;
  LocalVariablesAutothrust data = {};

  void processDispatch();

  void SimConnectSourceLocalVariablesAutothrust::dispatchProcedure(
      SIMCONNECT_RECV *pData,
      DWORD *cbData
  );
};
