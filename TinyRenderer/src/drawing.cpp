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

#include <math.h>

#include <vector>

#include "./geometry.h"
#include "./tgaimage.h"

bool IsPointInTriangle(const geometry::Vec2i& edge1,
                       const geometry::Vec2i& edge2,
                       const geometry::Vec2i& origin_from_point) {
  geometry::Vec3i vec_x =
      geometry::Vec3i(edge1.x, edge2.x, origin_from_point.x);
  geometry::Vec3i vec_y =
      geometry::Vec3i(edge1.y, edge2.y, origin_from_point.y);

  geometry::Vec3i vec_crossed = geometry::Vec3i(vec_x) ^ vec_y;

  if (vec_crossed.z == 0) {
    return false;
  }

  float normalized_x =
      static_cast<float>(vec_crossed.x) / static_cast<float>(vec_crossed.z);
  float normalized_y =
      static_cast<float>(vec_crossed.y) / static_cast<float>(vec_crossed.z);

  if (normalized_x >= 0 && normalized_y >= 0 &&
      normalized_x + normalized_y <= 1) {
    return true;
  }

  return false;
}

void DrawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
  int x_diff = abs(x1 - x0);
  int y_diff = abs(y1 - y0);

  int larger_diff = std::max(x_diff, y_diff);

  for (int i = 0; i <= larger_diff; ++i) {
    float t = larger_diff == 0
                  ? 0.0f
                  : static_cast<float>(i) / static_cast<float>(larger_diff);
    int x = x0 + (x1 - x0) * t;
    int y = y0 + (y1 - y0) * t;
    image.set(x, y, color);
  }
}

void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2,
                  TGAImage& image, TGAColor color) {
  int min_x = std::min(x0, std::min(x1, x2));
  int max_x = std::max(x0, std::max(x1, x2));
  int min_y = std::min(y0, std::min(y1, y2));
  int max_y = std::max(y0, std::max(y1, y2));

  geometry::Vec2i edge1 = geometry::Vec2i(x1 - x0, y1 - y0);
  geometry::Vec2i edge2 = geometry::Vec2i(x2 - x0, y2 - y0);

  for (int x = min_x; x != max_x; ++x) {
    for (int y = min_y; y != max_y; ++y) {
      if (IsPointInTriangle(edge1, edge2, geometry::Vec2i(x0 - x, y0 - y))) {
        image.set(x, y, color);
      }
    }
  }
}
