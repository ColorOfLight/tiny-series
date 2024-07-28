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

#include "./shape.h"

RgbaColor sky_blue(static_cast<uint8_t>(255 * 0.2),
                   static_cast<uint8_t>(255 * 0.7),
                   static_cast<uint8_t>(255 * 0.8));
RgbaColor coral_red(static_cast<uint8_t>(255 * 0.8),
                    static_cast<uint8_t>(255 * 0.2),
                    static_cast<uint8_t>(255 * 0.2));

RgbaColor CastRay(const Vec<3, float> &origin, const Vec<3, float> &direction,
                  const Sphere &sphere) {
  float ray_length = std::numeric_limits<float>::max();
  if (sphere.GetIntersectionDistance(origin, direction, ray_length) >= 0) {
    return sphere.GetColor();
  }
  return RgbaColor(0, 0, 0);
}

Image<RgbaColor> render(int width, int height, float y_fov,
                        const Vec<3, float> camera_position) {
  Image<RgbaColor> image(width, height);
  Sphere sphere(coral_red, 0.5f, Vec<3, float>({0.5, 0.5, 0}));

  float tan_y_fov_half = std::tan((y_fov * kPi / 180) / 2);

  for (int j = 0; j != height; ++j) {
    for (int i = 0; i != width; ++i) {
      float x = (2 * (i + 0.5f) / static_cast<float>(width) - 1) *
                tan_y_fov_half * width / height;
      float y =
          (2 * (j + 0.5f) / static_cast<float>(height) - 1) * tan_y_fov_half;

      Vec<3, float> ray_direction = Vec<3, float>({x, y, -1.f}).Normalize();

      image.set(i, j, CastRay(camera_position, ray_direction, sphere));
    }
  }

  return image;
}
