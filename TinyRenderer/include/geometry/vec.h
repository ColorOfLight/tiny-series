/*
 * MIT License
 *
 * Copyright (c) 2024 Seongho Park
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <initializer_list>
#include <iostream>

namespace geometry {

const float kPi = 3.14159265358979323846f;

template <size_t n, class t>
class Vec {
 public:
  Vec() : data(std::array<t, n>{0}) {}
  Vec(const std::initializer_list<t> &list) {
    if (list.size() != n) {
      throw std::invalid_argument("Invalid initializer list size");
    }

    std::copy(list.begin(), list.end(), data.begin());
  }

  t &operator[](int i) {
    if (i < 0 || i >= n) {
      throw std::out_of_range("Index out of range");
    }
    return data[i];
  }
  const t &operator[](int i) const {
    if (i < 0 || i >= n) {
      throw std::out_of_range("Index out of range");
    }
    return data[i];
  }

  inline Vec<n, t> &operator+=(const Vec<n, t> &v) {
    for (int i = 0; i < n; i++) {
      data[i] += v.data[i];
    }
    return *this;
  }
  inline Vec<n, t> &operator-=(const Vec<n, t> &v) {
    for (int i = 0; i < n; i++) {
      data[i] -= v.data[i];
    }
    return *this;
  }

  inline Vec<n, t> &operator*=(t f) {
    for (int i = 0; i < n; i++) {
      data[i] *= f;
    }
    return *this;
  }
  inline Vec<n, t> &operator/=(t f) {
    if (f == 0) {
      throw std::runtime_error("Division by zero");
    }

    for (int i = 0; i < n; i++) {
      data[i] /= f;
    }
    return *this;
  }

  inline Vec<n, t> operator+(const Vec<n, t> &v) const {
    Vec<n, t> result = *this;
    result += v;
    return result;
  }
  inline Vec<n, t> operator-(const Vec<n, t> &v) const {
    Vec<n, t> result = *this;
    result -= v;
    return result;
  }
  inline t operator*(const Vec<n, t> &v) const {
    t result = 0;
    for (int i = 0; i < n; i++) {
      result += data[i] * v.data[i];
    }
    return result;
  }

  inline Vec<n, t> operator*(float f) const {
    Vec<n, t> result = *this;
    result *= f;
    return result;
  }
  inline Vec<n, t> operator/(float f) const {
    Vec<n, t> result = *this;
    result /= f;
    return result;
  }

  // Specialization for cross product when n == 3
  Vec<3, t> &operator^=(const Vec<3, t> &v) {
    *this = Vec<3, t>{data[1] * v[2] - data[2] * v[1],
                      data[2] * v[0] - data[0] * v[2],
                      data[0] * v[1] - data[1] * v[0]};
    return *this;
  }

  // Specialization for cross product when n == 3
  Vec<3, t> operator^(const Vec<3, t> &v) const {
    return Vec<3, t>{data[1] * v[2] - data[2] * v[1],
                     data[2] * v[0] - data[0] * v[2],
                     data[0] * v[1] - data[1] * v[0]};
  }

  float length() const {
    float sum = 0;
    for (int i = 0; i < n; i++) {
      sum += data[i] * data[i];
    }
    return std::sqrt(sum);
  }

  Vec<n, t> &Normalize() {
    float len = length();
    if (len == 0) {
      throw std::runtime_error("Division by zero");
    }

    for (int i = 0; i < n; i++) {
      data[i] /= len;
    }
    return *this;
  }

  const Vec<n, t> &GetNormalized() const {
    Vec<n, t> result = *this;
    return result.Normalize();
  }

  template <class>
  friend std::ostream &operator<<(std::ostream &s, const Vec<n, t> &m);

 private:
  std::array<t, n> data;
};

template <size_t n, class t>
std::ostream &operator<<(std::ostream &s, const Vec<n, t> &v) {
  for (int i = 0; i < n; i++) {
    s << v[i] << " ";
  }

  return s;
}

}  // namespace geometry
