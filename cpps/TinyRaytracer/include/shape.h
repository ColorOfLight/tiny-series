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

class Shape {
 public:
  explicit Shape(const MaterialVariant &material) : _material(material) {}

  MaterialVariant GetMaterial() const { return _material; }
  virtual float GetIntersectionDistance(const Vec<3, float> &origin,
                                        const Vec<3, float> &direction,
                                        float rayLength) const {
    throw std::runtime_error("GetIntersectionDistance is not implemented.");
  }
  virtual Vec<3, float> GetNormal(const Vec<3, float> &point) const {
    throw std::runtime_error("GetNormal is not implemented.");
  }

 protected:
  MaterialVariant _material;
};

class Sphere : public Shape {
 public:
  Sphere() : Shape(SolidMaterial()), _radius(1), _center(Vec<3, float>()) {}
  explicit Sphere(const MaterialVariant &material)
      : Shape(material), _radius(1), _center(Vec<3, float>()) {}
  explicit Sphere(const MaterialVariant &material, float radius)
      : Shape(material), _radius(radius), _center(Vec<3, float>()) {
    if (radius < 0) {
      throw new std::out_of_range("The radius must be positive.");
    }
  }
  Sphere(const MaterialVariant &material, float radius,
         const Vec<3, float> &center)
      : Shape(material), _radius(radius), _center(center) {
    if (radius < 0) {
      throw new std::out_of_range("The radius must be positive.");
    }
  }

  float GetIntersectionDistance(const Vec<3, float> &origin,
                                const Vec<3, float> &direction,
                                float rayLength) const override;

  Vec<3, float> GetNormal(const Vec<3, float> &point) const override {
    return (point - _center).Normalize();
  }

 private:
  float _radius;
  Vec<3, float> _center;
};

class Plane : public Shape {
 public:
  Plane()
      : Shape(SolidMaterial()),
        _normal(Vec<3, float>({0, 1, 0})),
        _center(Vec<3, float>()),
        _width(2.f),
        _height(2.f) {}
  explicit Plane(const MaterialVariant &material)
      : Shape(material),
        _normal(Vec<3, float>({0, 1, 0})),
        _center(Vec<3, float>()),
        _width(2.f),
        _height(2.f) {}
  explicit Plane(const MaterialVariant &material, const Vec<3, float> &normal,
                 const Vec<3, float> &center, float width, float height)
      : Shape(material),
        _normal(normal),
        _center(center),
        _width(width),
        _height(height) {
    if (normal.length() != 1) {
      throw new std::out_of_range("The length of the normal vector must be 1.");
    }

    if (width <= 0) {
      throw new std::out_of_range("The width must be positive.");
    }

    if (height <= 0) {
      throw new std::out_of_range("The height must be positive.");
    }
  }

  float GetIntersectionDistance(const Vec<3, float> &origin,
                                const Vec<3, float> &direction,
                                float rayLength) const override;

  Vec<3, float> GetNormal(const Vec<3, float> &point) const override {
    return _normal;
  }

 private:
  Vec<3, float> _normal;
  Vec<3, float> _center;
  float _width;
  float _height;
};
