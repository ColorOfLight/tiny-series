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

#include <array>

#include "./geometry/vec.h"

template <size_t n, size_t m, class t>
class Mat {
  // row-first order

 public:
  Mat() : data(std::array<std::array<t, m>, n>{0}) {}
  Mat(const std::initializer_list<std::initializer_list<t>> &list) {
    if (list.size() != n) {
      throw std::invalid_argument("Invalid initializer list size");
    }

    int i = 0;
    for (const auto &row : list) {
      if (row.size() != m) {
        throw std::invalid_argument("Invalid initializer list size");
      }

      int j = 0;
      for (const auto &elem : row) {
        data[i][j] = elem;
        j++;
      }
      i++;
    }
  }

  std::array<t, m> &operator[](int i) {
    if (i < 0 || i >= n) {
      throw std::out_of_range("Index out of range");
    }
    return data[i];
  }
  const std::array<t, m> &operator[](int i) const {
    if (i < 0 || i >= n) {
      throw std::out_of_range("Index out of range");
    }
    return data[i];
  }

  inline Mat<n, m, t> &operator+=(const Mat<n, m, t> &mat) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < mat; j++) {
        data[i][j] += mat.data[i][j];
      }
    }
    return *this;
  }
  inline Mat<n, m, t> &operator-=(const Mat<n, m, t> &mat) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        data[i][j] -= mat.data[i][j];
      }
    }
    return *this;
  }

  inline Mat<n, m, t> operator+(const Mat<n, m, t> &mat) const {
    Mat<n, m, t> result = *this;
    result += mat;
    return result;
  }
  inline Mat<n, m, t> operator-(const Mat<n, m, t> &mat) const {
    Mat<n, m, t> result = *this;
    result -= mat;
    return result;
  }

  template <size_t l>
  inline Mat<n, l, t> operator*(const Mat<m, l, t> &mat) const {
    Mat<n, l, t> result;

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < l; j++) {
        for (int k = 0; k < m; k++) {
          result[i][j] += data[i][k] * mat[k][j];
        }
      }
    }

    return result;
  }

  inline Vec<n, t> operator*(const Vec<m, t> &vec) const {
    Vec<n, t> result;

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        result[i] += data[i][j] * vec[j];
      }
    }

    return result;
  }

  inline std::array<t, n> GetColumn(int j) const {
    if (j < 0 || j >= m) {
      throw std::out_of_range("Index out of range");
    }

    std::array<t, n> column;
    for (int i = 0; i < n; i++) {
      column[i] = data[i][j];
    }
    return column;
  }

  inline Vec<n, t> GetColumnVector(int j) const {
    if (j < 0 || j >= m) {
      throw std::out_of_range("Index out of range");
    }

    Vec<n, t> column;
    for (int i = 0; i < n; i++) {
      column[i] = data[i][j];
    }
    return column;
  }

  inline void SetColumn(int j, const Vec<n, t> &vec) {
    for (int i = 0; i < n; i++) {
      data[i][j] = vec[i];
    }
  }

  inline const std::array<t, n> &GetRow(int i) const {
    if (i < 0 || i >= n) {
      throw std::out_of_range("Index out of range");
    }

    return data[i];
  }

  inline const Vec<m, t> &GetRowVector(int i) const {
    return Vec<m, t>(GetRow(i));
  }

  inline void SetRow(int i, const Vec<m, t> &vec) {
    if (i < 0 || i >= n) {
      throw std::out_of_range("Index out of range");
    }

    for (int j = 0; j < m; j++) {
      data[i][j] = vec[j];
    }
  }

 private:
  std::array<std::array<t, m>, n> data;
};

template <size_t n, size_t m, class t>
std::ostream &operator<<(std::ostream &s, const Mat<n, m, t> &mat) {
  if (n == 1) {
    s << "[ ";
    for (int j = 0; j < m; j++) {
      s << mat[0][j] << " ";
    }
    s << "]";
    return s;
  }

  for (int i = 0; i < n; i++) {
    if (i == 0) {
      s << "\u250C ";
    } else if (i == n - 1) {
      s << "\u2514 ";
    } else {
      s << "\u2502 ";
    }

    for (int j = 0; j < m; j++) {
      s << mat[i][j] << " ";
    }

    if (i == 0) {
      s << "\u2510";
    } else if (i == n - 1) {
      s << "\u2518";
    } else {
      s << "\u2502";
    }

    s << std::endl;
  }

  return s;
}
