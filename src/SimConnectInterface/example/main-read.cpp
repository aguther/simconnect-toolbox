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

#include <list>
#include <iostream>
#include <chrono>
#include <regex>
#include <thread>
#include <SimConnectData.h>
#include <SimConnectDataDefinition.h>
#include <SimConnectDataInterface.h>

using namespace std;
using namespace simconnect::toolbox::connection;

int main() {
  // data definition defines what to read / write
  auto dataDefinition = SimConnectDataDefinition();
  // add variables to definition
  dataDefinition.add(SimConnectVariable("G FORCE", "GFORCE"));
  dataDefinition.add(SimConnectVariable("PLANE ALTITUDE", "FEET"));
  dataDefinition.add(SimConnectVariable("STRUCT WORLD ROTATION VELOCITY", "SIMCONNECT_DATA_XYZ"));
  dataDefinition.add(SimConnectVariable("LIGHT LANDING ON", "BOOL"));
  dataDefinition.add(SimConnectVariable("TURB ENG N1:1", "PERCENT"));

  // create data object holding the actual data
  auto simConnectData = make_shared<SimConnectData>(dataDefinition);

  // connect to sim
  SimConnectDataInterface simConnectInterface;
  bool connected = simConnectInterface.connect(
      0,
      "example-read",
      dataDefinition,
      simConnectData
  );
  cout << connected << endl;

  while (simConnectInterface.requestReadData()) {
    for (int kI = 0; kI < dataDefinition.size(); ++kI) {
      switch (dataDefinition.getType(kI)) {
        case SIMCONNECT_VARIABLE_TYPE_BOOL:
          cout << dataDefinition.get(kI).name << ": ";
          cout << any_cast<bool>(simConnectData->get(kI)) << " ";
          break;
        case SIMCONNECT_VARIABLE_TYPE_INT32:
          cout << dataDefinition.get(kI).name << ": ";
          cout << any_cast<long>(simConnectData->get(kI)) << " ";
          break;
        case SIMCONNECT_VARIABLE_TYPE_FLOAT32:
          cout << dataDefinition.get(kI).name << ": ";
          cout << any_cast<float>(simConnectData->get(kI)) << " ";
          break;
        case SIMCONNECT_VARIABLE_TYPE_FLOAT64:
          cout << dataDefinition.get(kI).name << ": ";
          cout << any_cast<double>(simConnectData->get(kI)) << " ";
          break;
        case SIMCONNECT_VARIABLE_TYPE_LATLONALT:
          cout << dataDefinition.get(kI).name << ": ";
          cout << any_cast<SIMCONNECT_DATA_LATLONALT>(simConnectData->get(kI)).Latitude << ",";
          cout << any_cast<SIMCONNECT_DATA_LATLONALT>(simConnectData->get(kI)).Longitude << ",";
          cout << any_cast<SIMCONNECT_DATA_LATLONALT>(simConnectData->get(kI)).Altitude << " ";
          break;
        case SIMCONNECT_VARIABLE_TYPE_XYZ:
          cout << dataDefinition.get(kI).name << ": ";
          cout << any_cast<SIMCONNECT_DATA_XYZ>(simConnectData->get(kI)).x << ",";
          cout << any_cast<SIMCONNECT_DATA_XYZ>(simConnectData->get(kI)).y << ",";
          cout << any_cast<SIMCONNECT_DATA_XYZ>(simConnectData->get(kI)).z << " ";
          break;
        default:
          break;
      }
    }
    cout << endl;
    this_thread::sleep_for(chrono::milliseconds(500));
  }

  return 0;
}
