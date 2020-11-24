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

#include <deque>
#include <memory>
#include <Windows.h>
#include <SimConnect.h>
#include "SimConnectVariable.h"
#include "SimConnectVariableLookupTable.h"

namespace simconnect::toolbox::connection {
class SimConnectDataDefinition;
}

class simconnect::toolbox::connection::SimConnectDataDefinition {
 public:
  explicit SimConnectDataDefinition(
      SimConnectVariableLookupTable::Type type = SimConnectVariableLookupTable::Type::Data
  );

  SimConnectDataDefinition(
      const SimConnectDataDefinition &other
  );

  SimConnectDataDefinition &operator=(
      const SimConnectDataDefinition &other
  );

  ~SimConnectDataDefinition();

  void add(
      const SimConnectVariable &item
  );

  SimConnectVariable get(
      size_t index
  );

  size_t size();

  SIMCONNECT_VARIABLE_TYPE getType(
      size_t index
  );

  SIMCONNECT_VARIABLE_TYPE getType(
      const SimConnectVariable &item
  );

 private:
  SimConnectVariableLookupTable::Type type;
  std::deque<SimConnectVariable> variables;
};
