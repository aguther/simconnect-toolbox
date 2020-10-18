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

#include <iostream>
#include <string>
#include <vector>
#include "SimConnectVariable.h"
#include <SimConnectDataDefinition.h>

namespace simconnect::toolbox::connection {
class SimConnectVariableParser;
}

class simconnect::toolbox::connection::SimConnectVariableParser {
 public:
  SimConnectVariableParser(
      SimConnectVariableParser const &
  ) = delete;

  void operator=(
      SimConnectVariableParser const &
  ) = delete;

  static std::vector<SimConnectVariable> getSimConnectVariablesFromParameterString(
      const std::string &parameter
  ) {
    // variable to hold result
    std::vector<SimConnectVariable> simConnectVariables;

    // get lines from parameter
    std::vector<std::string> lines = getVariableLines(parameter);

    simConnectVariables.reserve(lines.size());
    for (const auto &line : lines) {
      simConnectVariables.emplace_back(getSimConnectVariableFromVariableLine(line));
    }

    // need to parse string
    return simConnectVariables;
  }

  static SimConnectDataDefinition getSimConnectDataDefinitionFromVariables(
      const std::vector<SimConnectVariable> &variables
  ) {
    // create data definition
    auto dataDefinition = SimConnectDataDefinition();

    // add variables
    for (const auto &variable : variables) {
      dataDefinition.add(variable);
    }

    // return data definition
    return dataDefinition;
  }

  static std::vector<std::string> getVariableLines(
      const std::string &parameter
  ) {
    // result
    std::vector<std::string> lines;

    // iterate over string and add lines to result
    size_t kPosition = 0;
    std::string kString = parameter;
    while ((kPosition = kString.find(VARIABLE_DELIMITER)) != std::string::npos) {
      lines.push_back(kString.substr(0, kPosition));
      kString.erase(0, kPosition + VARIABLE_DELIMITER.length());
    }

    // return result
    return lines;
  }

  static SimConnectVariable getSimConnectVariableFromVariableLine(
      const std::string &line
  ) {
    // find delimiter
    size_t kPosition = 0;
    if ((kPosition = line.find(VARIABLE_PARAMETER_DELIMITER)) != std::string::npos) {
      // get name and unit
      std::string name = line.substr(0, kPosition);
      std::string unit = line.substr(kPosition + 1, std::string::npos);

      // trim them
      trim(name);
      trim(unit);

      // return result
      return SimConnectVariable(name, unit);
    }
    throw std::invalid_argument("Variable not valid!");
  }

 private:
  inline const static std::string VARIABLE_DELIMITER = ";";
  inline const static std::string VARIABLE_PARAMETER_DELIMITER = ",";

  SimConnectVariableParser() = default;

  ~SimConnectVariableParser() = default;

  static inline void ltrim(
      std::string &s
  ) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
      return !isspace(ch);
    }));
  }

  static inline void rtrim(
      std::string &s
  ) {
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
      return !isspace(ch);
    }).base(), s.end());
  }

  static inline void trim(
      std::string &s
  ) {
    ltrim(s);
    rtrim(s);
  }

};
