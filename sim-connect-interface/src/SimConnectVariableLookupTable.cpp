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
#include <regex>
#include "SimConnectVariableLookupTable.h"

using namespace std;
using namespace simconnect::toolbox::connection;

bool SimConnectVariableLookupTable::isKnown(
    const SimConnectVariable &item,
    Type type
) {
  switch (type) {
    case Type::Data:
      return LOOKUP_TABLE_DATA.find(normalizeName(item.name)) != LOOKUP_TABLE_DATA.end();

    case Type::Input:
      return LOOKUP_TABLE_INPUT.find(normalizeName(item.name)) != LOOKUP_TABLE_INPUT.end();

    case Type::Event:
      return LOOKUP_TABLE_EVENT.find(normalizeName(item.name)) != LOOKUP_TABLE_EVENT.end();

    default:
      return false;
  }
}

SIMCONNECT_VARIABLE_TYPE SimConnectVariableLookupTable::getDataType(
    const SimConnectVariable &item,
    Type type
) {
  // check if variable is known
  if (!isKnown(item, type)) {
    throw invalid_argument("The variable is not known!");
  }
  // return data type that is mapped to variable
  switch (type) {
    case Type::Data:
      return LOOKUP_TABLE_DATA.at(normalizeName(item.name));

    case Type::Input:
      return LOOKUP_TABLE_INPUT.at(normalizeName(item.name));

    case Type::Event:
      return LOOKUP_TABLE_EVENT.at(normalizeName(item.name));

    default:
      return SIMCONNECT_VARIABLE_TYPE::SIMCONNECT_VARIABLE_TYPE_INVALID;
  }
}

string SimConnectVariableLookupTable::normalizeName(
    const string &itemName
) {
  // this is needed to support indexed variables
  return regex_replace(itemName, regex("(.*)(:[0-9]+)"), "$1:index");
}
