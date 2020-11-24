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
#include <thread>
#include <SimConnectData.h>
#include <SimConnectDataDefinition.h>
#include <SimConnectInputInterface.h>

using namespace std;
using namespace simconnect::toolbox::connection;

int main() {
  // data definition defines what to read / write
  auto dataDefinition = SimConnectDataDefinition(SimConnectVariableLookupTable::Input);
  // add variables to definition
  dataDefinition.add(SimConnectVariable("AXIS_THROTTLE1_SET", "FALSE"));
  // create data object holding the actual data
  auto simConnectData = make_shared<SimConnectData>(dataDefinition);

  // connect to sim
  SimConnectInputInterface simConnectInterface;
  bool connected = simConnectInterface.connect(
      0,
      "example-input",
      dataDefinition,
      simConnectData
  );
  cout << connected << endl;

  while (simConnectInterface.readData()) {
    for (int kI = 0; kI < dataDefinition.size(); ++kI) {
      switch (dataDefinition.getType(kI)) {
        case SIMCONNECT_VARIABLE_TYPE_FLOAT64:
          cout << dataDefinition.get(kI).name << ": ";
          cout << any_cast<double>(simConnectData->get(kI)) << " ";
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
