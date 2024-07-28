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

#include "./shape.h"

#include "geometry/utils.h"

bool Sphere::GetIsIntersecting(const Vec<3, float> &origin,
                               const Vec<3, float> &direction,
                               float length) const {
  if (std::fabsf(direction.length() - 1.f) > kEpsilon) {
    throw new std::out_of_range(
        "The length of the direction vector must be 1.");
  }

  // 1. If the ray is already inside the sphere, return true.
  float distance_between_origins = (origin - _center).length();
  if (distance_between_origins < _radius) {
    return true;
  }

  // 2. If the ray direction is not pointing to the sphere, return false.
  Vec<3, float> origin_to_center = _center - origin;
  if (origin_to_center * direction <= 0) {
    return false;
  }

  // 3. Get the nearest point from the ray to the sphere.
  float distance_to_nearest = origin_to_center * direction;
  Vec<3, float> nearest_point = origin + direction * distance_to_nearest;

  // 4. If the distance between the nearest point and the sphere is more than
  // the radius, return false.
  float distance_nearest_center = (nearest_point - _center).length();
  if (distance_nearest_center > _radius) {
    return false;
  }

  // 5. If the length of the ray is less than the distance between the nearest
  // point and the sphere, check the distance between the latest point of ray
  // and the center of the sphere.
  if (length < distance_to_nearest) {
    Vec<3, float> latest_point = origin + direction * length;
    float distance_latest_center = (latest_point - _center).length();
    return distance_latest_center <= _radius;
  }

  // 6. Else, return true.
  return true;
}
