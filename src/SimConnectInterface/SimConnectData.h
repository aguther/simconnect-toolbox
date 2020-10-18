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

#include <any>
#include <deque>
#include <Windows.h>
#include <SimConnect.h>
#include "MemoryAccessor.h"
#include "SimConnectDataDefinition.h"

namespace simconnect::toolbox::connection {
class SimConnectData;
}

class simconnect::toolbox::connection::SimConnectData {
 public:
  explicit SimConnectData(
      const SimConnectDataDefinition &dataDefinition
  );

  ~SimConnectData();

  [[nodiscard]] size_t size() const;

  char *getBuffer();

  std::any get(
      size_t index
  );

  void set(
      size_t index,
      std::any value
  );

  void copy(
      char *pBuffer
  );

 private:
  struct MemberCount {
    size_t countBoolean = 0;
    size_t countInt32 = 0;
    size_t countFloat32 = 0;
    size_t countFloat64 = 0;
    size_t countLatLonAlt = 0;
    size_t countXYZ = 0;
  };

  SimConnectDataDefinition dataDefinition;
  std::deque<size_t> indexMapping;
  MemberCount memberCount = {};

  size_t totalSize = 0;
  char *buffer = nullptr;

  std::shared_ptr<MemoryAccessor<int>> memoryAccessorBoolean = nullptr;
  std::shared_ptr<MemoryAccessor<long>> memoryAccessorInt32 = nullptr;
  std::shared_ptr<MemoryAccessor<float>> memoryAccessorFloat32 = nullptr;
  std::shared_ptr<MemoryAccessor<double>> memoryAccessorFloat64 = nullptr;
  std::shared_ptr<MemoryAccessor<SIMCONNECT_DATA_LATLONALT>> memoryAccessorLatLonAlt = nullptr;
  std::shared_ptr<MemoryAccessor<SIMCONNECT_DATA_XYZ>> memoryAccessorXYZ = nullptr;

  MemberCount getMemberCountFromDataDefinition(
      SimConnectDataDefinition _dataDefinition
  );

  static size_t getTotalSize(
      MemberCount counts
  );

  void setupMemoryAccessors();
};
