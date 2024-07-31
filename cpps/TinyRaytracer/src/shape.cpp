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

float Sphere::GetIntersectionDistance(const Vec<3, float> &origin,
                                      const Vec<3, float> &direction,
                                      float rayLength) const {
  if (std::fabsf(direction.length() - 1.f) > kEpsilon) {
    throw new std::out_of_range(
        "The length of the direction vector must be 1.");
  }

  // 1. If the ray is already inside the sphere, return the value.
  float distance_between_origins = (origin - _center).length();
  if (distance_between_origins < _radius) {
    return distance_between_origins;
  }

  // 2. If the ray direction is not pointing to the sphere, return -1.
  Vec<3, float> origin_to_center = _center - origin;
  if (origin_to_center * direction <= 0) {
    return -1;
  }

  // 3. Get the nearest point from the ray to the sphere.
  float distance_to_nearest = origin_to_center * direction;
  Vec<3, float> nearest_point = origin + direction * distance_to_nearest;

  // 4. If the distance between the nearest point and the sphere is more than
  // the radius, return -1.
  float distance_nearest_center = (nearest_point - _center).length();
  if (distance_nearest_center > _radius) {
    return -1;
  }

  // 5. Get the distance between the ray origin and the intersecting point.
  float distance_nearest_intersection = std::sqrt(
      _radius * _radius - distance_nearest_center * distance_nearest_center);
  float distance_origin_intersection =
      distance_to_nearest - distance_nearest_intersection;

  // 6. If the distance to intersection is over the ray length, return -1, else
  // return the distance.
  return distance_origin_intersection > rayLength
             ? -1
             : distance_origin_intersection;
}

float Plane::GetIntersectionDistance(const Vec<3, float> &origin,
                                     const Vec<3, float> &direction,
                                     float rayLength) const {
  if (std::fabsf(direction.length() - 1.f) > kEpsilon) {
    throw new std::out_of_range(
        "The length of the direction vector must be 1.");
  }

  // 1. If the ray is parallel to the plane, return -1.
  float denominator = _normal * direction;
  if (std::fabsf(denominator) < kEpsilon) {
    return -1;
  }

  // 2. Get the distance between the origin and the plane.
  float distance = (_center - origin) * _normal / denominator;

  // 3. If the distance is negative, return -1.
  if (distance < 0) {
    return -1;
  }

  // 4. Get the intersection point.
  Vec<3, float> intersection_point = origin + direction * distance;

  // 5. If the intersection point is inside the plane, return the distance.
  if (std::fabsf(intersection_point[0] - _center[0]) <= _width / 2 &&
      std::fabsf(intersection_point[2] - _center[2]) <= _height / 2) {
    return distance;
  }

  // 6. Return -1.
  return -1;
}