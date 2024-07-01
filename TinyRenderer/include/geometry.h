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
  inline Vec2<t> operator+(const Vec2<t> &V) const {
    return Vec2<t>(u + V.u, v + V.v);
  }
  inline Vec2<t> operator-(const Vec2<t> &V) const {
    return Vec2<t>(u - V.u, v - V.v);
  }
  inline Vec2<t> operator*(float f) const { return Vec2<t>(u * f, v * f); }
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

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

template <class t>
std::ostream &operator<<(std::ostream &s, Vec2<t> &v) {
  s << "(" << v.x << ", " << v.y << ")\n";
  return s;
}

template <class t>
std::ostream &operator<<(std::ostream &s, Vec3<t> &v) {
  s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
  return s;
}

template <class t>
inline Vec2<t> Cross(Vec2<t> &vec1, Vec2<t> &vec2) {
  return Vec2<t>(vec1.u * vec2.v - vec1.v * vec2.u);
}

template <class t>
inline Vec3<t> Cross(Vec3<t> &vec1, Vec3<t> &vec2) {
  return Vec3<t>(vec1.y * vec2.z - vec1.z * vec2.y,
                 vec1.z * vec2.x - vec1.x * vec2.z,
                 vec1.x * vec2.y - vec1.y * vec2.x);
}

}  // namespace geometry
