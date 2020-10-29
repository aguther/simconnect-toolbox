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

#include <string>
#include <vector>
#include <Windows.h>
#include <SimConnect.h>
#include "SimConnectDataDefinition.h"
#include "SimConnectData.h"

namespace simconnect::toolbox::connection {
class SimConnectDataInterface;
}

class simconnect::toolbox::connection::SimConnectDataInterface {
 public:
  SimConnectDataInterface() = default;

  ~SimConnectDataInterface() = default;

  bool connect(
      int configurationIndex,
      const std::string &name,
      const SimConnectDataDefinition &dataDefinition,
      const std::shared_ptr<SimConnectData> &simConnectData
  );

  void disconnect();

  bool requestReadData();

  bool requestData();

  bool readData();

  bool sendData();

 private:
  bool isConnected = false;
  HANDLE hSimConnect = nullptr;
  std::string connectionName;
  std::shared_ptr<SimConnectData> data;

  void simConnectProcessDispatchMessage(
      SIMCONNECT_RECV *pData,
      DWORD *cbData
  );

  void simConnectProcessSimObjectDataByType(
      const SIMCONNECT_RECV *pData
  );

  static bool prepareDataDefinition(
      HANDLE connectionHandle,
      SIMCONNECT_DATA_DEFINITION_ID id,
      SimConnectDataDefinition dataDefinition
  );

  static bool addDataDefinition(
      HANDLE connectionHandle,
      SIMCONNECT_DATA_DEFINITION_ID id,
      SIMCONNECT_VARIABLE_TYPE dataType,
      const std::vector<SimConnectVariable> &variables
  );
};
