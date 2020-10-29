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

#include <memory>
#include <iostream>

template<class T>
class MemoryAccessor {
 public:
  MemoryAccessor(
      char *_data,
      size_t _count
  );

  size_t getCount();

  T get(
      size_t index
  ) const;

  void set(
      size_t index,
      T value
  );

  static size_t getSizeWithPadding(
      size_t _count,
      size_t _alignment
  );

  static size_t getSize(
      size_t _count
  );

  static size_t getPadding(
      size_t _count,
      size_t _alignment
  );

 private:
  T *data = nullptr;
  size_t count = 0;
};

template<class T>
MemoryAccessor<T>::MemoryAccessor(
    char *_data,
    const size_t _count
) {
  data = reinterpret_cast<T *>(_data);
  count = _count;
}

template<class T>
size_t MemoryAccessor<T>::getCount() {
  return count;
}

template<class T>
T MemoryAccessor<T>::get(
    size_t index
) const {
  if (index >= count) {
    throw std::out_of_range("Index is out of range!");
  }
  return data[index];
}

template<class T>
void MemoryAccessor<T>::set(
    size_t index,
    T value
) {
  if (index >= count) {
    throw std::out_of_range("Index is out of range!");
  }
  data[index] = value;
}

template<class T>
size_t MemoryAccessor<T>::getSizeWithPadding(
    const size_t _count,
    const size_t _alignment
) {
  return getSize(_count) + getPadding(_count, _alignment);
}

template<class T>
size_t MemoryAccessor<T>::getSize(
    const size_t _count
) {
  // variable for size
  size_t size = 0;
  // add size of doubles
  size += _count * sizeof(T);
  // return result
  return size;
}

template<class T>
size_t MemoryAccessor<T>::getPadding(
    const size_t _count,
    const size_t _alignment
) {
  // return result
  return (_alignment - (getSize(_count) % _alignment)) % _alignment;
}
