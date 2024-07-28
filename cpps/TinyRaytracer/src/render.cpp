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

#include "./render.h"

#include <algorithm>
#include <vector>

#include "./geometry/utils.h"
#include "./light.h"
#include "./shape.h"

RgbaColor sky_blue(static_cast<uint8_t>(255 * 0.2),
                   static_cast<uint8_t>(255 * 0.7),
                   static_cast<uint8_t>(255 * 0.8));
RgbaColor coral_red(static_cast<uint8_t>(255 * 0.8),
                    static_cast<uint8_t>(255 * 0.2),
                    static_cast<uint8_t>(255 * 0.2));
RgbaColor cement_gray(static_cast<uint8_t>(255 * 0.5),
                      static_cast<uint8_t>(255 * 0.5),
                      static_cast<uint8_t>(255 * 0.5));
RgbaColor white(255, 255, 255);

RgbaColor CastRay(const Vec<3, float> &origin, const Vec<3, float> &direction,
                  const std::vector<Sphere> &spheres,
                  const std::vector<Light> &lights,
                  const RgbaColor &background_color) {
  float ray_length = std::numeric_limits<float>::max();

  int target_sphere_index = -1;
  float nearest_distance = std::numeric_limits<float>::max();

  for (int i = 0; i != spheres.size(); ++i) {
    const Sphere &sphere = spheres[i];

    float current_distance =
        sphere.GetIntersectionDistance(origin, direction, ray_length);
    if (current_distance >= 0 && current_distance < nearest_distance) {
      target_sphere_index = i;
      nearest_distance = current_distance;
    }
  }

  if (target_sphere_index == -1) {
    return background_color;
  }

  Sphere target_sphere = spheres[target_sphere_index];

  Vec<3, float> intersection_point = origin + direction * nearest_distance;
  Vec<3, float> normal = target_sphere.GetNormal(intersection_point);

  RgbaColor base_color = target_sphere.GetColor();

  float diffuse_intensity_sum = 0;
  float specular_intensity_sum = 0;

  for (const Light &light : lights) {
    Vec<3, float> light_direction =
        (intersection_point - light.GetPosition()).Normalize();
    float diffuse_intensity =
        std::max(0.f, (light_direction * (-1) * normal)) * light.GetIntensity();

    diffuse_intensity_sum += diffuse_intensity;

    Vec<3, float> reflection = Reflect(light_direction, normal);
    float specular_intensity =
        std::pow(std::max(0.f, reflection * (-1) * direction), 30) *
        light.GetIntensity() * 0.5;

    specular_intensity_sum += specular_intensity;
  }

  return base_color * std::min(diffuse_intensity_sum, 1.f) +
         white * std::min(specular_intensity_sum, 1.f);
}

Image<RgbaColor> render(int width, int height, float y_fov,
                        const Vec<3, float> camera_position) {
  Image<RgbaColor> image(width, height);

  std::vector<Sphere> spheres;

  spheres.push_back(Sphere(cement_gray, 0.5f, Vec<3, float>({-1, 0, -3.5})));
  spheres.push_back(
      Sphere(coral_red, 0.5f, Vec<3, float>({-0.5, -0.375, -2.5})));
  spheres.push_back(Sphere(coral_red, 0.5f, Vec<3, float>({0.125, 0, -3})));
  spheres.push_back(Sphere(cement_gray, 0.5f, Vec<3, float>({1, 0.5, -1.5})));

  std::vector<Light> lights;

  lights.push_back(Light(Vec<3, float>({-1, 2, 0})));
  lights.push_back(Light(Vec<3, float>({3, 2, 0}), 0.5));

  float tan_y_fov_half = std::tan((y_fov * kPi / 180) / 2);

  for (int j = 0; j != height; ++j) {
    for (int i = 0; i != width; ++i) {
      float x = (2 * (i + 0.5f) / static_cast<float>(width) - 1) *
                tan_y_fov_half * width / height;
      float y =
          (2 * (j + 0.5f) / static_cast<float>(height) - 1) * tan_y_fov_half;

      Vec<3, float> ray_direction = Vec<3, float>({x, y, -1.f}).Normalize();

      image.set(
          i, j,
          CastRay(camera_position, ray_direction, spheres, lights, sky_blue));
    }
  }

  return image;
}
