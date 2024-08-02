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
#include <functional>
#include <variant>
#include <vector>

#include "./geometry/utils.h"
#include "./light.h"
#include "./material.h"
#include "./shape.h"

struct CastRayOptions {
  int current_reflection;
};

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

bool GetIsShadowed(const Vec<3, float> &intersection_point,
                   const Vec<3, float> &normal,
                   const std::vector<std::reference_wrapper<Shape>> &shapes,
                   const Shape &target_shape, const Light &light) {
  Vec<3, float> light_direction =
      (intersection_point - light.GetPosition()).Normalize();

  Vec<3, float> shadow_origin = light_direction * (-1) * normal >= 0
                                    ? intersection_point + normal * kEpsilon
                                    : intersection_point - normal * kEpsilon;

  for (const Shape &shape : shapes) {
    if (&shape == &target_shape) {
      continue;
    }

    float shadow_distance = shape.GetIntersectionDistance(
        shadow_origin, light_direction * (-1),
        (light.GetPosition() - shadow_origin).length());
    if (shadow_distance >= 0) {
      return true;
    }
  }

  return false;
}

float GetSpecularIntensity(const Vec<3, float> &intersection_point,
                           const Vec<3, float> &direction,
                           const Vec<3, float> &normal,
                           const std::vector<Light> &lights) {
  float specular_intensity_sum = 0;

  for (const Light &light : lights) {
    Vec<3, float> light_direction =
        (intersection_point - light.GetPosition()).Normalize();

    Vec<3, float> reflection = Reflect(light_direction, normal);

    float specular_intensity =
        std::pow(std::max(0.f, reflection * (-1) * direction), 30) *
        light.GetIntensity() * 0.5;

    specular_intensity_sum += specular_intensity;
  }

  return specular_intensity_sum;
}

float GetDiffuseIntensity(
    const Vec<3, float> &intersection_point, const Vec<3, float> &normal,
    const Shape &target_shape,
    const std::vector<std::reference_wrapper<Shape>> &shapes,
    const std::vector<Light> &lights) {
  float diffuse_intensity_sum = 0;

  for (const Light &light : lights) {
    Vec<3, float> light_direction =
        (intersection_point - light.GetPosition()).Normalize();

    bool is_shadowed =
        GetIsShadowed(intersection_point, normal, shapes, target_shape, light);

    if (is_shadowed) {
      continue;
    }

    float diffuse_intensity =
        std::max(0.f, (light_direction * (-1) * normal)) * light.GetIntensity();

    diffuse_intensity_sum += diffuse_intensity;
  }

  return diffuse_intensity_sum;
}

RgbaColor GetBackgroundColorFromImage(
    Vec<3, float> direction, const Image<RgbaColor> &background_image) {
  float u = std::atan2(direction[0], -direction[2]) / (kPi * 2) + 0.5f;
  float v = 1.0f - std::acos(direction[1]) / (kPi);

  int x = static_cast<int>(u * background_image.GetWidth());
  int y = static_cast<int>(v * background_image.GetHeight());

  return background_image.at(x, y);
}

RgbaColor CastRay(const Vec<3, float> &origin, const Vec<3, float> &direction,
                  const std::vector<std::reference_wrapper<Shape>> &shapes,
                  const std::vector<Light> &lights,
                  const Image<RgbaColor> &background_image,
                  const CastRayOptions options = {}) {
  float ray_length = std::numeric_limits<float>::max();

  int target_shape_index = -1;
  float nearest_distance = std::numeric_limits<float>::max();

  for (int i = 0; i != shapes.size(); ++i) {
    const auto &shape = shapes[i].get();

    float current_distance =
        shape.GetIntersectionDistance(origin, direction, ray_length);
    if (current_distance >= 0 && current_distance < nearest_distance) {
      target_shape_index = i;
      nearest_distance = current_distance;
    }
  }

  if (target_shape_index == -1) {
    return GetBackgroundColorFromImage(direction, background_image);
  }

  const auto &target_shape = shapes[target_shape_index].get();

  Vec<3, float> intersection_point = origin + direction * nearest_distance;
  Vec<3, float> normal = target_shape.GetNormal(intersection_point);

  MaterialVariant target_material = target_shape.GetMaterial();

  RgbaColor base_color;
  bool has_diffuse = false;
  bool has_specular = false;

  RgbaColor material_color;

  if (const auto &solid_material =
          std::get_if<SolidMaterial>(&target_material)) {
    RgbaColor base_color = solid_material->color;
    float diffuse_intensity = GetDiffuseIntensity(intersection_point, normal,
                                                  target_shape, shapes, lights);

    material_color = base_color * std::min(diffuse_intensity, 1.f);
  } else if (const auto *reflective_material =
                 std::get_if<ReflectiveMaterial>(&target_material)) {
    if (options.current_reflection >= reflective_material->max_reflection) {
      material_color = GetBackgroundColorFromImage(direction, background_image);
    }

    Vec<3, float> reflect_direction = Reflect(direction, normal);
    Vec<3, float> reflect_origin = reflect_direction * normal < 0
                                       ? intersection_point - normal * kEpsilon
                                       : intersection_point + normal * kEpsilon;

    RgbaColor reflect_color =
        CastRay(reflect_origin, reflect_direction, shapes, lights,
                background_image, {options.current_reflection + 1});
    material_color = reflect_color * 0.9;
  }

  float specular_intensity =
      GetSpecularIntensity(intersection_point, direction, normal, lights);
  RgbaColor specular_color = white * std::min(specular_intensity, 1.f);

  return material_color + specular_color;
}

Image<RgbaColor> render(int width, int height, float y_fov,
                        const Vec<3, float> camera_position,
                        const Image<RgbaColor> &background_image) {
  Image<RgbaColor> image(width, height);

  std::vector<std::reference_wrapper<Shape>> shapes;

  Sphere sphere1 =
      Sphere(SolidMaterial(cement_gray), 0.5f, Vec<3, float>({-1, 0, -3.5}));
  Sphere sphere2 =
      Sphere(ReflectiveMaterial(4), 0.5f, Vec<3, float>({-0.5, -0.375, -2.5}));
  Sphere sphere3 =
      Sphere(SolidMaterial(coral_red), 0.5f, Vec<3, float>({0.125, 0, -3}));
  Sphere sphere4 =
      Sphere(ReflectiveMaterial(4), 0.5f, Vec<3, float>({1, 0.5, -1.5}));

  Plane plane = Plane(SolidMaterial(white), Vec<3, float>({0, 1, 0}),
                      Vec<3, float>({0, -1.25, -3}), 5, 5);

  shapes.push_back(sphere1);
  shapes.push_back(sphere2);
  shapes.push_back(sphere3);
  shapes.push_back(sphere4);

  shapes.push_back(plane);

  std::vector<Light> lights;

  lights.push_back(Light(Vec<3, float>({-1, 2, 0}), 0.75));
  lights.push_back(Light(Vec<3, float>({3, 2, 0}), 0.5));

  float tan_y_fov_half = std::tan((y_fov * kPi / 180) / 2);

  for (int j = 0; j != height; ++j) {
    for (int i = 0; i != width; ++i) {
      float x = (2 * (i + 0.5f) / static_cast<float>(width) - 1) *
                tan_y_fov_half * width / height;
      float y =
          (2 * (j + 0.5f) / static_cast<float>(height) - 1) * tan_y_fov_half;

      Vec<3, float> ray_direction = Vec<3, float>({x, y, -1.f}).Normalize();

      image.set(i, j,
                CastRay(camera_position, ray_direction, shapes, lights,
                        background_image));
    }
  }

  return image;
}
