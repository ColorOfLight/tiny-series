/*
 * Copyright Dmitry V. Sokolov
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software. Permission is granted to anyone to use this software
 * for any purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 * that you wrote the original software.
 * If you use this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Modified by Seongho Park in 2024
 */

// Source:
// https://github.com/ssloy/tinyrenderer/blob/f6fecb7ad493264ecd15e230411bfb1cca539a12/geometry.h

#pragma once

#include <cmath>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace geometry {

const float PI = 3.14159265358979323846f;

template <class t>
struct Vec2 {
  union {
    struct {
      t u, v;
    };
    struct {
      t x, y;
    };
    t raw[2];
  };
  Vec2() : u(0), v(0) {}
  Vec2(t _u, t _v) : u(_u), v(_v) {}
  Vec2(const Vec2<t> &v) : u(v.u), v(v.v) {}
  inline Vec2<t> operator+(const Vec2<t> &V) const {
    return Vec2<t>(u + V.u, v + V.v);
  }
  inline Vec2<t> operator-(const Vec2<t> &V) const {
    return Vec2<t>(u - V.u, v - V.v);
  }
  inline Vec2<t> operator*(float f) const { return Vec2<t>(u * f, v * f); }
  inline Vec2<t> operator^(const Vec2<t> &V) const {
    return Vec2<t>(u * V.v - v * V.u);
  }
  template <class>
  friend std::ostream &operator<<(std::ostream &s, Vec2<t> &v);
};

template <class t>
struct Vec3 {
  union {
    struct {
      t x, y, z;
    };
    struct {
      t ivert, iuv, inorm;
    };
    t raw[3];
  };
  Vec3() : x(0), y(0), z(0) {}
  Vec3(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
  Vec3(const Vec3<t> &v) : x(v.x), y(v.y), z(v.z) {}
  inline Vec3<t> operator^(const Vec3<t> &v) const {
    return Vec3<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
  }
  inline Vec3<t> operator+(const Vec3<t> &v) const {
    return Vec3<t>(x + v.x, y + v.y, z + v.z);
  }
  inline Vec3<t> operator-(const Vec3<t> &v) const {
    return Vec3<t>(x - v.x, y - v.y, z - v.z);
  }
  inline Vec3<t> operator*(float f) const {
    return Vec3<t>(x * f, y * f, z * f);
  }
  inline t operator*(const Vec3<t> &v) const {
    return x * v.x + y * v.y + z * v.z;
  }
  float norm() const { return std::sqrt(x * x + y * y + z * z); }
  Vec3<t> &normalize(t l = 1) {
    *this = (*this) * (l / norm());
    return *this;
  }
  template <class>
  friend std::ostream &operator<<(std::ostream &s, Vec3<t> &v);
};

template <class t>
struct Vec4 {
  union {
    struct {
      t x, y, z, w;
    };
    t raw[4];
  };
  Vec4() : x(0), y(0), z(0), w(0) {}
  Vec4(t _x, t _y, t _z, t _w) : x(_x), y(_y), z(_z), w(_w) {}
  Vec4(const Vec4<t> &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
  inline Vec4<t> &operator+=(const Vec4<t> &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
  }
  inline Vec4<t> operator+(const Vec4<t> &v) const {
    return Vec4<t>(x + v.x, y + v.y, z + v.z, w + v.w);
  }
  inline Vec4<t> &operator-=(const Vec4<t> &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
  }
  inline Vec4<t> operator-(const Vec4<t> &v) const {
    return Vec4<t>(x - v.x, y - v.y, z - v.z, w - v.w);
  }
  inline Vec4<t> operator*(float f) const {
    return Vec4<t>(x * f, y * f, z * f, w * f);
  }
  inline Vec4<t> &operator*=(const Vec4<t> &v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
  }
  inline t operator*(const Vec4<t> &v) const {
    return x * v.x + y * v.y + z * v.z + w * v.w;
  }
  Vec3<t> ToNDC() const {
    if (w == 0) {
      throw std::runtime_error("Division by zero");
    }
    return Vec3<t>(x / w, y / w, z / w);
  }
  float norm() const { return std::sqrt(x * x + y * y + z * z + w * w); }
  Vec4<t> &normalize(t l = 1) {
    *this = (*this) * (l / norm());
    return *this;
  }
  template <class>
  friend std::ostream &operator<<(std::ostream &s, Vec4<t> &v);
};

template <class t>
struct Mat4x4 {
  // row-major order
  t m[4][4] = {0};
  Mat4x4() {}
  Mat4x4(t m00, t m01, t m02, t m03, t m10, t m11, t m12, t m13, t m20, t m21,
         t m22, t m23, t m30, t m31, t m32, t m33) {
    m[0][0] = m00;
    m[0][1] = m01;
    m[0][2] = m02;
    m[0][3] = m03;
    m[1][0] = m10;
    m[1][1] = m11;
    m[1][2] = m12;
    m[1][3] = m13;
    m[2][0] = m20;
    m[2][1] = m21;
    m[2][2] = m22;
    m[2][3] = m23;
    m[3][0] = m30;
    m[3][1] = m31;
    m[3][2] = m32;
    m[3][3] = m33;
  }

  static Mat4x4 Perspective(float distance) {
    if (distance == 0) {
      throw std::runtime_error("Division by zero");
    }

    Mat4x4<t> result;

    result.m[0][0] = 1;
    result.m[1][1] = 1;
    result.m[2][2] = 1;
    result.m[3][2] = -1.0f / distance;
    result.m[3][3] = 1;
    return result;
  }

  static Mat4x4 Viewport(int x, int y, int width, int height, int depth) {
    Mat4x4<t> result;

    result.m[0][0] = static_cast<t>(width) / 2;
    result.m[0][3] = x + static_cast<t>(width) / 2;
    result.m[1][1] = static_cast<t>(height) / 2;
    result.m[1][3] = y + static_cast<t>(height) / 2;
    result.m[2][2] = static_cast<t>(depth) / 2;
    result.m[2][3] = static_cast<t>(depth) / 2;
    result.m[3][3] = 1.0f;

    return result;
  }

  inline Mat4x4<t> &operator*=(t scalar) const {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        m[i][j] = m[i][j] * scalar;
      }
    }
    return *this;
  }
  inline Mat4x4<t> operator*(t scalar) const {
    Mat4x4<t> result = *this;
    result *= scalar;
    return result;
  }
  inline Mat4x4<t> &operator*=(const Mat4x4<t> &m2) {
    Mat4x4<t> result;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        result.m[i][j] = m[i][0] * m2.m[0][j] + m[i][1] * m2.m[1][j] +
                         m[i][2] * m2.m[2][j] + m[i][3] * m2.m[3][j];
      }
    }
    *this = result;
    return *this;
  }
  inline Mat4x4<t> operator*(const Mat4x4<t> &m2) const {
    Mat4x4<t> result = *this;
    result *= m2;
    return result;
  }
  inline Vec4<t> operator*(const Vec4<t> &v) const {
    Vec4<t> result;
    for (int i = 0; i < 4; i++) {
      result.raw[i] = v.raw[0] * m[i][0] + v.raw[1] * m[i][1] +
                      v.raw[2] * m[i][2] + v.raw[3] * m[i][3];
    }
    return result;
  }

  template <class>
  friend std::ostream &operator<<(std::ostream &s, const Mat4x4<t> &m);
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
typedef Vec4<float> Vec4f;
typedef Vec4<int> Vec4i;

typedef Mat4x4<float> Mat4x4f;
typedef Mat4x4<int> Mat4x4i;

template <class t>
std::ostream &operator<<(std::ostream &s, const Vec2<t> &v) {
  s << "(" << v.x << ", " << v.y << ")";
  return s;
}

template <class t>
std::ostream &operator<<(std::ostream &s, const Vec3<t> &v) {
  s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return s;
}

template <class t>
std::ostream &operator<<(std::ostream &s, const Vec4<t> &v) {
  s << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
  return s;
}

template <class t>
std::ostream &operator<<(std::ostream &s, const Mat4x4<t> &m) {
  s << "[";
  for (int i = 0; i < 4; i++) {
    s << "[";
    for (int j = 0; j < 4; j++) {
      s << m.m[i][j];
      if (j < 3) {
        s << ", ";
      }
    }
    s << "]";
    if (i < 3) {
      s << std::endl;
    }
  }
  s << "]";
  return s;
}

}  // namespace geometry
