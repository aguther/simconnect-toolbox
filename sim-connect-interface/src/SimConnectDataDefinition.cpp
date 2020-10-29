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

#include <iostream>
#include <utility>
#include "SimConnectDataDefinition.h"

using namespace std;
using namespace simconnect::toolbox::connection;

SimConnectDataDefinition::SimConnectDataDefinition() : variables() {
}

SimConnectDataDefinition::SimConnectDataDefinition(
    const SimConnectDataDefinition &other
) {
  variables.clear();
  for (const auto &variable : other.variables) {
    variables.push_back(variable);
  }
}

SimConnectDataDefinition &SimConnectDataDefinition::operator=(
    const SimConnectDataDefinition &other
) {
  variables.clear();
  for (const auto &variable : other.variables) {
    variables.push_back(variable);
  }
  return *this;
}

SimConnectDataDefinition::~SimConnectDataDefinition() = default;

void SimConnectDataDefinition::add(
    const SimConnectVariable &item
) {
  if (!SimConnectVariableLookupTable::isKnown(item)) {
    throw std::invalid_argument("Variable is not known!");
  }
  variables.push_back(item);
}

SimConnectVariable SimConnectDataDefinition::get(
    size_t index
) {
  return variables[index];
}

size_t SimConnectDataDefinition::size() {
  return variables.size();
}

SIMCONNECT_VARIABLE_TYPE SimConnectDataDefinition::getType(
    size_t index
) {
  return getType(get(index));
}

SIMCONNECT_VARIABLE_TYPE SimConnectDataDefinition::getType(
    const SimConnectVariable &item
) {
  return SimConnectVariableLookupTable::getDataType(item);
}
