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

#include "./geometry_new/mat.h"
#include "./geometry_new/vec.h"

namespace geometry_new {

inline Vec<3, float> GetNDC(const Vec<4, float>& vec) {
  if (vec[3] == 0) {
    throw std::runtime_error("Division by zero");
  }

  return Vec<3, float>({vec[0] / vec[3], vec[1] / vec[3], vec[2] / vec[3]});
}

inline Vec<3, float> Reflect(const Vec<3, float>& incident,
                             const Vec<3, float>& normal) {
  return (incident - normal * 2 * (incident * normal)) * (-1);
}

template <int n, class t>
inline Mat<n, n, t> GetIdentityMat() {
  Mat<n, n, t> identity;
  for (int i = 0; i < n; i++) {
    identity[i][i] = 1;
  }
  return identity;
}

inline Mat<4, 4, float> ViewMatrix(const Vec<3, float>& eye,
                                   const Vec<3, float>& center,
                                   const Vec<3, float>& up) {
  Vec<3, float> z = (eye - center).Normalize();
  Vec<3, float> x = (up ^ z).Normalize();
  Vec<3, float> y = (z ^ x).Normalize();
  Mat<4, 4, float> Minv = GetIdentityMat<4, float>();
  Mat<4, 4, float> Tr = GetIdentityMat<4, float>();
  for (int i = 0; i < 3; i++) {
    Minv[0][i] = x[i];
    Minv[1][i] = y[i];
    Minv[2][i] = z[i];
    Tr[i][3] = -center[i];
  }
  return Minv * Tr;
}

inline Mat<4, 4, float> Perspective(float distance) {
  if (distance == 0) {
    throw std::runtime_error("Distance cannot be zero");
  }

  Mat<4, 4, float> perspective = GetIdentityMat<4, float>();
  perspective[3][2] = -1.0f / distance;
  return perspective;
}

inline Mat<4, 4, float> Viewport(int x, int y, int width, int height,
                                 int depth) {
  Mat<4, 4, float> viewport;
  viewport[0][0] = static_cast<float>(width) / 2;
  viewport[0][3] = x + static_cast<float>(width) / 2;
  viewport[1][1] = static_cast<float>(height) / 2;
  viewport[1][3] = y + static_cast<float>(height) / 2;
  viewport[2][2] = static_cast<float>(depth) / 2;
  viewport[2][3] = static_cast<float>(depth) / 2;
  viewport[3][3] = 1.0f;
  return viewport;
}

}  // namespace geometry_new
