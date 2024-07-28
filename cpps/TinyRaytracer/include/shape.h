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

#include "./image.h"
#include "./material.h"
#include "geometry/vec.h"

class Sphere {
 public:
  Sphere() : _material(SolidMaterial()), _radius(1), _center(Vec<3, float>()) {}
  explicit Sphere(const MaterialVariant &material)
      : _material(material), _radius(1), _center(Vec<3, float>()) {}
  explicit Sphere(const MaterialVariant &material, float radius)
      : _material(material), _radius(radius), _center(Vec<3, float>()) {
    if (radius < 0) {
      throw new std::out_of_range("The radius must be positive.");
    }
  }
  Sphere(const MaterialVariant &material, float radius,
         const Vec<3, float> &center)
      : _material(material), _radius(radius), _center(center) {
    if (radius < 0) {
      throw new std::out_of_range("The radius must be positive.");
    }
  }

  MaterialVariant GetMaterial() const { return _material; }

  float GetIntersectionDistance(const Vec<3, float> &origin,
                                const Vec<3, float> &direction,
                                float rayLength) const;

  Vec<3, float> GetNormal(const Vec<3, float> &point) const {
    return (point - _center).Normalize();
  }

 private:
  float _radius;
  Vec<3, float> _center;
  MaterialVariant _material;
};
