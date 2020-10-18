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

namespace simconnect::toolbox::connection {

enum SIMCONNECT_VARIABLE_TYPE {
  SIMCONNECT_VARIABLE_TYPE_INVALID,
  SIMCONNECT_VARIABLE_TYPE_BOOL,
  SIMCONNECT_VARIABLE_TYPE_INT32,
  SIMCONNECT_VARIABLE_TYPE_FLOAT32,
  SIMCONNECT_VARIABLE_TYPE_FLOAT64,
  SIMCONNECT_VARIABLE_TYPE_LATLONALT,
  SIMCONNECT_VARIABLE_TYPE_XYZ,
};

class SimConnectVariableType {
 public:
  SimConnectVariableType() = delete;

  ~SimConnectVariableType() = delete;

  static bool isStruct(
      SIMCONNECT_VARIABLE_TYPE type
  ) {
    switch (type) {
      case SIMCONNECT_VARIABLE_TYPE_LATLONALT:
      case SIMCONNECT_VARIABLE_TYPE_XYZ:
        return true;

      default:
        return false;
    }
    return false;
  }

  static SIMCONNECT_DATATYPE convert(
      SIMCONNECT_VARIABLE_TYPE type
  ) {
    switch (type) {
      case SIMCONNECT_VARIABLE_TYPE_BOOL:
      case SIMCONNECT_VARIABLE_TYPE_INT32:
        return SIMCONNECT_DATATYPE_INT32;

      case SIMCONNECT_VARIABLE_TYPE_FLOAT32:
        return SIMCONNECT_DATATYPE_FLOAT32;

      case SIMCONNECT_VARIABLE_TYPE_FLOAT64:
        return SIMCONNECT_DATATYPE_FLOAT64;

      case SIMCONNECT_VARIABLE_TYPE_LATLONALT:
        return SIMCONNECT_DATATYPE_LATLONALT;

      case SIMCONNECT_VARIABLE_TYPE_XYZ:
        return SIMCONNECT_DATATYPE_XYZ;

      default:
        return SIMCONNECT_DATATYPE_INVALID;
    }
  }

  static SIMCONNECT_VARIABLE_TYPE convert(
      SIMCONNECT_DATATYPE type
  ) {
    switch (type) {
      case SIMCONNECT_DATATYPE_INT32:
        return SIMCONNECT_VARIABLE_TYPE_INT32;

      case SIMCONNECT_DATATYPE_FLOAT32:
        return SIMCONNECT_VARIABLE_TYPE_FLOAT32;

      case SIMCONNECT_DATATYPE_FLOAT64:
        return SIMCONNECT_VARIABLE_TYPE_FLOAT64;

      case SIMCONNECT_DATATYPE_LATLONALT:
        return SIMCONNECT_VARIABLE_TYPE_LATLONALT;

      case SIMCONNECT_DATATYPE_XYZ:
        return SIMCONNECT_VARIABLE_TYPE_XYZ;

      default:
        return SIMCONNECT_VARIABLE_TYPE_INVALID;
    }
  }
};

}
