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

#include "./geometry/mat.h"
#include "./geometry/vec.h"

namespace geometry {

inline Vec<3, float> GetNDC(const Vec<4, float>& vec) {
  if (vec[3] == 0) {
    throw std::runtime_error("GetNDC: Division by zero");
  }

  return Vec<3, float>({vec[0] / vec[3], vec[1] / vec[3], vec[2] / vec[3]});
}

inline Vec<3, float> Reflect(const Vec<3, float>& incident,
                             const Vec<3, float>& normal) {
  return (incident - normal * 2 * (incident * normal)) * (-1);
}

inline bool IsParallel(const Vec<3, float>& a, const Vec<3, float>& b) {
  return (a ^ b).length() == 0;
}

template <size_t n, class t>
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
  Mat<4, 4, float> matrix = GetIdentityMat<4, float>();
  for (int i = 0; i < 3; i++) {
    matrix[0][i] = x[i];
    matrix[1][i] = y[i];
    matrix[2][i] = z[i];
  }

  matrix[0][3] = -(x * (eye - center));
  matrix[1][3] = -(y * (eye - center));
  matrix[2][3] = -(z * (eye - center));

  return matrix;
}

inline Mat<4, 4, float> Perspective(float distance) {
  if (distance == 0) {
    throw std::runtime_error("Distance cannot be zero");
  }

  Mat<4, 4, float> perspective = GetIdentityMat<4, float>();
  perspective[2][2] = 1.0f / distance;
  perspective[3][2] = -1.0f / distance;
  return perspective;
}

inline Mat<4, 4, float> Orthographic(float width, float height,
                                     float distance) {
  if (width == 0) {
    throw std::runtime_error("Width cannot be zero");
  }

  if (height == 0) {
    throw std::runtime_error("Height cannot be zero");
  }

  if (distance == 0) {
    throw std::runtime_error("Distance cannot be zero");
  }

  Mat<4, 4, float> projection_matrix = geometry::GetIdentityMat<4, float>();

  projection_matrix[0][0] = 2.0f / width;
  projection_matrix[1][1] = 2.0f / height;
  projection_matrix[2][2] = 2.0f / distance;
  projection_matrix[2][3] = 1.0f;

  return projection_matrix;
}

inline Mat<4, 4, float> Viewport(float x, float y, float width, float height,
                                 float depth) {
  Mat<4, 4, float> viewport;
  viewport[0][0] = width / 2;
  viewport[0][3] = x + width / 2;
  viewport[1][1] = height / 2;
  viewport[1][3] = y + height / 2;
  viewport[2][2] = depth / 2;
  viewport[2][3] = depth / 2;
  viewport[3][3] = 1.0f;
  return viewport;
}

inline Mat<3, 3, float> Inverse(const Mat<3, 3, float> mat) {
  float det =
      mat[0][0] * mat[1][1] * mat[2][2] + mat[0][1] * mat[1][2] * mat[2][0] +
      mat[0][2] * mat[1][0] * mat[2][1] - mat[0][2] * mat[1][1] * mat[2][0] -
      mat[0][1] * mat[1][0] * mat[2][2] - mat[0][0] * mat[1][2] * mat[2][1];

  if (det == 0) {
    throw std::runtime_error("Determinant is zero");
  }

  Mat<3, 3, float> result;
  result[0][0] = (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) / det;
  result[0][1] = (mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2]) / det;
  result[0][2] = (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]) / det;
  result[1][0] = (mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2]) / det;
  result[1][1] = (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]) / det;
  result[1][2] = (mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2]) / det;
  result[2][0] = (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]) / det;
  result[2][1] = (mat[0][1] * mat[2][0] - mat[0][0] * mat[2][1]) / det;
  result[2][2] = (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]) / det;

  return result;
}

inline float smoothstep(float edge0, float edge1, float x) {
  // Clamp x to the [0, 1] range
  x = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
  // Evaluate the smoothstep function
  return x * x * (3 - 2 * x);
}

}  // namespace geometry
