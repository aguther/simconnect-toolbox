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
#include <SimConnectDataInterface.h>

using namespace std;
using namespace simconnect::toolbox::connection;

int main() {
  // data definition defines what to read / write
  auto dataDefinition = SimConnectDataDefinition();
  // add variables to definition
  dataDefinition.add(SimConnectVariable("RUDDER TRIM PCT", "Percent Over 100"));
  // create data object holding the actual data
  auto simConnectData = make_shared<SimConnectData>(dataDefinition);

  // connect to sim
  SimConnectDataInterface simConnectInterface;
  bool connected = simConnectInterface.connect(
      0,
      "example-write",
      dataDefinition,
      simConnectData
  );
  cout << connected << endl;

  bool lightOn = false;
  unsigned int runningIndex = 0;

  while (true) {
    if (runningIndex++ % 5 == 0) {
      lightOn = !lightOn;
    }

    simConnectData->set(0, lightOn ? -1.0 : 1.0);
    if (simConnectInterface.sendData()) {
      cout << "Success writing" << endl;
    } else {
      cout << "Failed writing" << endl;
    }

    this_thread::sleep_for(chrono::milliseconds(500));
  }

  return 0;
}
