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
#include "SimConnectData.h"

using namespace simconnect::toolbox::connection;

SimConnectData::SimConnectData(
    const SimConnectDataDefinition &dataDefinition
) : dataDefinition(dataDefinition), indexMapping() {
  // get counts
  memberCount = getMemberCountFromDataDefinition(dataDefinition);

  // get total size
  totalSize = getTotalSize(memberCount);

  // allocate buffer and set it to zero
  buffer = new char[totalSize];
  std::fill(buffer, buffer + totalSize, 0);

  // setup memory accessors
  setupMemoryAccessors();
}

SimConnectData::~SimConnectData() {
  delete[] buffer;
  buffer = nullptr;
}

SimConnectData::MemberCount SimConnectData::getMemberCountFromDataDefinition(
    SimConnectDataDefinition _dataDefinition
) {
  MemberCount count = {};
  for (int index = 0; index < _dataDefinition.size(); ++index) {
    switch (_dataDefinition.getType(index)) {
      case SIMCONNECT_VARIABLE_TYPE_BOOL:
        indexMapping.push_back(count.countBoolean);
        count.countBoolean++;
        break;
      case SIMCONNECT_VARIABLE_TYPE_INT32:
        indexMapping.push_back(count.countInt32);
        count.countInt32++;
        break;
      case SIMCONNECT_VARIABLE_TYPE_FLOAT32:
        indexMapping.push_back(count.countFloat32);
        count.countFloat32++;
        break;
      case SIMCONNECT_VARIABLE_TYPE_FLOAT64:
        indexMapping.push_back(count.countFloat64);
        count.countFloat64++;
        break;
      case SIMCONNECT_VARIABLE_TYPE_LATLONALT:
        indexMapping.push_back(count.countLatLonAlt);
        count.countLatLonAlt++;
        break;
      case SIMCONNECT_VARIABLE_TYPE_XYZ:
        indexMapping.push_back(count.countXYZ);
        count.countXYZ++;
        break;

      default:
        break;
    }
  }
  return count;
}

size_t SimConnectData::getTotalSize(
    MemberCount counts
) {
  size_t size = 0;
  size += MemoryAccessor<int>::getSizeWithPadding(counts.countBoolean, 4);
  size += MemoryAccessor<long>::getSizeWithPadding(counts.countInt32, 8);
  size += MemoryAccessor<float>::getSizeWithPadding(counts.countFloat32, 8);
  size += MemoryAccessor<double>::getSizeWithPadding(counts.countFloat64, 8);
  size += MemoryAccessor<SIMCONNECT_DATA_LATLONALT>::getSizeWithPadding(counts.countLatLonAlt, 8);
  size += MemoryAccessor<SIMCONNECT_DATA_XYZ>::getSizeWithPadding(counts.countXYZ, 8);
  return size;
}

char *SimConnectData::getBuffer() {
  return buffer;
}

void SimConnectData::setupMemoryAccessors() {
  // variable for offset
  size_t offset = 0;
  if (memberCount.countBoolean > 0) {
    memoryAccessorBoolean = std::make_shared<MemoryAccessor<int>>(
        buffer + offset,
        memberCount.countBoolean
    );
    offset += MemoryAccessor<int>::getSizeWithPadding(memberCount.countBoolean, 4);
  }
  if (memberCount.countInt32 > 0) {
    memoryAccessorInt32 = std::make_shared<MemoryAccessor<long>>(
        buffer + offset,
        memberCount.countInt32
    );
    offset += MemoryAccessor<long>::getSizeWithPadding(memberCount.countInt32, 8);
  }
  if (memberCount.countFloat32 > 0) {
    memoryAccessorFloat32 = std::make_shared<MemoryAccessor<float>>(
        buffer + offset,
        memberCount.countFloat32
    );
    offset += MemoryAccessor<float>::getSizeWithPadding(memberCount.countFloat32, 8);
  }
  if (memberCount.countFloat64 > 0) {
    memoryAccessorFloat64 = std::make_shared<MemoryAccessor<double>>(
        buffer + offset,
        memberCount.countFloat64
    );
    offset += MemoryAccessor<double>::getSizeWithPadding(memberCount.countFloat64, 8);
  }
  if (memberCount.countLatLonAlt > 0) {
    memoryAccessorLatLonAlt = std::make_shared<MemoryAccessor<SIMCONNECT_DATA_LATLONALT>>(
        buffer + offset,
        memberCount.countLatLonAlt
    );
    offset += MemoryAccessor<SIMCONNECT_DATA_LATLONALT>::getSizeWithPadding(memberCount.countLatLonAlt, 8);
  }
  if (memberCount.countXYZ > 0) {
    memoryAccessorXYZ = std::make_shared<MemoryAccessor<SIMCONNECT_DATA_XYZ>>(
        buffer + offset,
        memberCount.countXYZ
    );
    offset += MemoryAccessor<SIMCONNECT_DATA_XYZ>::getSizeWithPadding(memberCount.countXYZ, 8);
  }

  // ensure total size and offset is matching
  if (offset != totalSize) {
    throw std::exception("Offset and total size are different!");
  }
}

size_t SimConnectData::size() const {
  return totalSize;
}

std::any SimConnectData::get(
    size_t index
) {
  switch (dataDefinition.getType(index)) {
    case SIMCONNECT_VARIABLE_TYPE_BOOL:
      return static_cast<bool>(memoryAccessorBoolean->get(indexMapping[index]));
    case SIMCONNECT_VARIABLE_TYPE_INT32:
      return memoryAccessorInt32->get(indexMapping[index]);
    case SIMCONNECT_VARIABLE_TYPE_FLOAT32:
      return memoryAccessorFloat32->get(indexMapping[index]);
    case SIMCONNECT_VARIABLE_TYPE_FLOAT64:
      return memoryAccessorFloat64->get(indexMapping[index]);
    case SIMCONNECT_VARIABLE_TYPE_LATLONALT:
      return memoryAccessorLatLonAlt->get(indexMapping[index]);
    case SIMCONNECT_VARIABLE_TYPE_XYZ:
      return memoryAccessorXYZ->get(indexMapping[index]);
    default:
      throw std::exception("No item found!");
  }
}

void SimConnectData::set(
    size_t index,
    std::any value
) {
  switch (dataDefinition.getType(index)) {
    case SIMCONNECT_VARIABLE_TYPE_BOOL:
      memoryAccessorBoolean->set(indexMapping[index], std::any_cast<bool>(value));
      break;
    case SIMCONNECT_VARIABLE_TYPE_INT32:
      memoryAccessorInt32->set(indexMapping[index], std::any_cast<long>(value));
      break;
    case SIMCONNECT_VARIABLE_TYPE_FLOAT32:
      memoryAccessorFloat32->set(indexMapping[index], std::any_cast<float>(value));
      break;
    case SIMCONNECT_VARIABLE_TYPE_FLOAT64:
      memoryAccessorFloat64->set(indexMapping[index], std::any_cast<double>(value));
      break;
    case SIMCONNECT_VARIABLE_TYPE_LATLONALT:
      memoryAccessorLatLonAlt->set(indexMapping[index], std::any_cast<SIMCONNECT_DATA_LATLONALT>(value));
      break;
    case SIMCONNECT_VARIABLE_TYPE_XYZ:
      memoryAccessorXYZ->set(indexMapping[index], std::any_cast<SIMCONNECT_DATA_XYZ>(value));
      break;
    default:
      throw std::exception("Parameter not known!");
  }
}

void SimConnectData::copy(
    char *pBuffer
) {
  memcpy_s(this->buffer, totalSize, pBuffer, totalSize);
}
