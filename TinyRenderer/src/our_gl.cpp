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

#include "./our_gl.h"

#include <math.h>

#include <algorithm>

namespace our_gl {

Vertex FirstShader::ShadeVertex(model::Vertex model_vertex) const {
  geometry::Vec3f new_position =
      (g_viewport_mat * u_vpm_mat *
       geometry::Vec4f(model_vertex.position.x, model_vertex.position.y,
                       model_vertex.position.z, 1))
          .ToNDC();

  return {new_position, model_vertex.normal, model_vertex.texture_coords};
}
TGAColor FirstShader::ShadeFragment(const our_gl::Vertex& vertex) const {
  return TGAColor(255, 255, 255, 255);
}

geometry::Vec3f GetBarycentric(const geometry::Vec2f& target,
                               const geometry::Vec2f& p0,
                               const geometry::Vec2f& p1,
                               const geometry::Vec2f& p2) {
  geometry::Vec2f edge1 = p1 - p0;
  geometry::Vec2f edge2 = p2 - p0;
  geometry::Vec2f origin_from_point = p0 - target;

  geometry::Vec3f vec_x =
      geometry::Vec3f(edge1.x, edge2.x, origin_from_point.x);
  geometry::Vec3f vec_y =
      geometry::Vec3f(edge1.y, edge2.y, origin_from_point.y);

  geometry::Vec3f vec_crossed = vec_x ^ vec_y;

  if (vec_crossed.z == 0) {
    return geometry::Vec3f(-1, 1, 1);
  }

  return geometry::Vec3f(1.f - (vec_crossed.x + vec_crossed.y) / vec_crossed.z,
                         vec_crossed.x / vec_crossed.z,
                         vec_crossed.y / vec_crossed.z);
}

bool IsPointInTriangle(const geometry::Vec2i& edge1,
                       const geometry::Vec2i& edge2,
                       const geometry::Vec2i& origin_from_point) {
  geometry::Vec3i vec_x =
      geometry::Vec3i(edge1.x, edge2.x, origin_from_point.x);
  geometry::Vec3i vec_y =
      geometry::Vec3i(edge1.y, edge2.y, origin_from_point.y);

  geometry::Vec3i vec_crossed = vec_x ^ vec_y;

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

TGAColor FindNearestTextureColor(const geometry::Vec2f& st,
                                 const TGAImage& texture) {
  if (st.x < 0 || st.x > 1 || st.y < 0 || st.y > 1) {
    throw std::out_of_range("st should be in range [0, 1]");
  }

  int width = texture.width();
  int height = texture.height();

  int x = std::min(static_cast<int>(st.x * width), width - 1);
  int y = std::min(static_cast<int>(st.y * height), height - 1);

  return texture.get(x, y);
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

void DrawTriangle(const geometry::Vec3f& v0, const geometry::Vec3f& v1,
                  const geometry::Vec3f& v2, TGAColor color, TGAImage& image,
                  std::vector<std::vector<float>>& z_buffer) {
  // Give some margin to avoid the edge of the triangle
  int min_x = std::min(v0.x, std::min(v1.x, v2.x)) - 1;
  int max_x = std::max(v0.x, std::max(v1.x, v2.x)) + 1;
  int min_y = std::min(v0.y, std::min(v1.y, v2.y)) - 1;
  int max_y = std::max(v0.y, std::max(v1.y, v2.y)) + 1;

  // To handle the case where the coord is same with width or height
  min_x = std::clamp(min_x, 0, static_cast<int>(z_buffer[0].size()) - 1);
  min_y = std::clamp(min_y, 0, static_cast<int>(z_buffer.size()) - 1);
  max_x = std::clamp(max_x, 0, static_cast<int>(z_buffer[0].size()) - 1);
  max_y = std::clamp(max_y, 0, static_cast<int>(z_buffer.size()) - 1);

  for (int x = min_x; x != max_x; ++x) {
    for (int y = min_y; y != max_y; ++y) {
      geometry::Vec3f barycentric = GetBarycentric(
          geometry::Vec2f(x, y), geometry::Vec2f(v0.x, v0.y),
          geometry::Vec2f(v1.x, v1.y), geometry::Vec2f(v2.x, v2.y));

      if (barycentric.x < 0 || barycentric.y < 0 || barycentric.z < 0) {
        continue;
      }

      if (auto z = barycentric.x * v0.z + barycentric.y * v1.z +
                   barycentric.z * v2.z;
          z_buffer[y][x] < z) {
        z_buffer[y][x] = z;
        image.set(x, y, color);
      }
    }
  }
}

void DrawTriangle(const geometry::Vec3f& p0, const geometry::Vec3f& p1,
                  const geometry::Vec3f& p2, const geometry::Vec3f& n0,
                  const geometry::Vec3f& n1, const geometry::Vec3f& n2,
                  const TGAColor& color, const geometry::Vec3f& light_dir,
                  TGAImage& image, std::vector<std::vector<float>>& z_buffer) {
  // Give some margin to avoid the edge of the triangle
  int min_x = std::min(p0.x, std::min(p1.x, p2.x)) - 1;
  int max_x = std::max(p0.x, std::max(p1.x, p2.x)) + 1;
  int min_y = std::min(p0.y, std::min(p1.y, p2.y)) - 1;
  int max_y = std::max(p0.y, std::max(p1.y, p2.y)) + 1;

  // To handle the case where the coord is same with width or height
  min_x = std::clamp(min_x, 0, static_cast<int>(z_buffer[0].size()) - 1);
  min_y = std::clamp(min_y, 0, static_cast<int>(z_buffer.size()) - 1);
  max_x = std::clamp(max_x, 0, static_cast<int>(z_buffer[0].size()) - 1);
  max_y = std::clamp(max_y, 0, static_cast<int>(z_buffer.size()) - 1);

  geometry::Vec2f edge1_2d = geometry::Vec2f(p1.x - p0.x, p1.y - p0.y);
  geometry::Vec2f edge2_2d = geometry::Vec2f(p2.x - p0.x, p2.y - p0.y);

  for (int x = min_x; x != max_x; ++x) {
    for (int y = min_y; y != max_y; ++y) {
      geometry::Vec3f barycentric = GetBarycentric(
          geometry::Vec2f(x, y), geometry::Vec2f(p0.x, p0.y),
          geometry::Vec2f(p1.x, p1.y), geometry::Vec2f(p2.x, p2.y));

      if (barycentric.x < 0 || barycentric.y < 0 || barycentric.z < 0) {
        continue;
      }

      if (auto z = barycentric.x * p0.z + barycentric.y * p1.z +
                   barycentric.z * p2.z;
          z_buffer[y][x] < z) {
        geometry::Vec3f normal =
            n0 * barycentric.x + n1 * barycentric.y + n2 * barycentric.z;

        float intensity =
            std::max(0., (-1.) * (normal.normalize() *
                                  geometry::Vec3f(light_dir).normalize()));

        z_buffer[y][x] = z;
        image.set(x, y, color * intensity);
      }
    }
  }
}

void DrawTriangle(const geometry::Vec3f& p0, const geometry::Vec3f& p1,
                  const geometry::Vec3f& p2, const geometry::Vec3f& n0,
                  const geometry::Vec3f& n1, const geometry::Vec3f& n2,
                  const geometry::Vec2f& st0, const geometry::Vec2f& st1,
                  const geometry::Vec2f& st2, const TGAImage& texture,
                  const geometry::Vec3f& light_dir, TGAImage& image,
                  std::vector<std::vector<float>>& z_buffer) {
  // Give some margin to avoid the edge of the triangle
  int min_x = std::min(p0.x, std::min(p1.x, p2.x)) - 1;
  int max_x = std::max(p0.x, std::max(p1.x, p2.x)) + 1;
  int min_y = std::min(p0.y, std::min(p1.y, p2.y)) - 1;
  int max_y = std::max(p0.y, std::max(p1.y, p2.y)) + 1;

  // To handle the case where the coord is same with width or height
  min_x = std::clamp(min_x, 0, static_cast<int>(z_buffer[0].size()) - 1);
  min_y = std::clamp(min_y, 0, static_cast<int>(z_buffer.size()) - 1);
  max_x = std::clamp(max_x, 0, static_cast<int>(z_buffer[0].size()) - 1);
  max_y = std::clamp(max_y, 0, static_cast<int>(z_buffer.size()) - 1);

  for (int x = min_x; x != max_x; ++x) {
    for (int y = min_y; y != max_y; ++y) {
      geometry::Vec3f barycentric = GetBarycentric(
          geometry::Vec2f(x, y), geometry::Vec2f(p0.x, p0.y),
          geometry::Vec2f(p1.x, p1.y), geometry::Vec2f(p2.x, p2.y));

      if (barycentric.x < 0 || barycentric.y < 0 || barycentric.z < 0) {
        continue;
      }

      if (auto z = barycentric.x * p0.z + barycentric.y * p1.z +
                   barycentric.z * p2.z;
          z_buffer[y][x] < z) {
        geometry::Vec3f normal =
            n0 * barycentric.x + n1 * barycentric.y + n2 * barycentric.z;

        float intensity =
            std::max(0., (-1.) * (normal.normalize() *
                                  geometry::Vec3f(light_dir).normalize()));

        const auto& st =
            st0 * barycentric.x + st1 * barycentric.y + st2 * barycentric.z;

        const auto& texture_color = FindNearestTextureColor(st, texture);

        z_buffer[y][x] = z;
        image.set(x, y, texture_color * intensity);
      }
    }
  }
}

void DrawTriangle(const std::vector<our_gl::Vertex>& vertices,
                  const IShader& shader, TGAImage& image, TGAImage& z_buffer) {
  const geometry::Vec3f& p0 = vertices[0].position;
  const geometry::Vec3f& p1 = vertices[1].position;
  const geometry::Vec3f& p2 = vertices[2].position;

  // Give some margin to avoid the edge of the triangle
  int min_x = std::min(p0.x, std::min(p1.x, p2.x)) - 1;
  int max_x = std::max(p0.x, std::max(p1.x, p2.x)) + 1;
  int min_y = std::min(p0.y, std::min(p1.y, p2.y)) - 1;
  int max_y = std::max(p0.y, std::max(p1.y, p2.y)) + 1;

  const int z_buffer_width = z_buffer.width();
  const int z_buffer_height = z_buffer.height();

  // To handle the case where the coord is same with width or height
  min_x = std::clamp(min_x, 0, z_buffer_width - 1);
  min_y = std::clamp(min_y, 0, z_buffer_height - 1);
  max_x = std::clamp(max_x, 0, z_buffer_width - 1);
  max_y = std::clamp(max_y, 0, z_buffer_height - 1);

  for (int x = min_x; x != max_x; ++x) {
    for (int y = min_y; y != max_y; ++y) {
      geometry::Vec3f barycentric = GetBarycentric(
          geometry::Vec2f(x, y), geometry::Vec2f(p0.x, p0.y),
          geometry::Vec2f(p1.x, p1.y), geometry::Vec2f(p2.x, p2.y));

      if (barycentric.x < 0 || barycentric.y < 0 || barycentric.z < 0) {
        continue;
      }

      if (auto z = barycentric.x * p0.z + barycentric.y * p1.z +
                   barycentric.z * p2.z;
          static_cast<float>(z_buffer.get(x, y).bgra[0]) < z) {
        geometry::Vec3f position = vertices[0].position * barycentric.x +
                                   vertices[1].position * barycentric.y +
                                   vertices[2].position * barycentric.z;

        geometry::Vec3f normal = vertices[0].normal * barycentric.x +
                                 vertices[1].normal * barycentric.y +
                                 vertices[2].normal * barycentric.z;

        geometry::Vec2f st = vertices[0].texture_coords * barycentric.x +
                             vertices[1].texture_coords * barycentric.y +
                             vertices[2].texture_coords * barycentric.z;

        TGAColor fragment_color =
            shader.ShadeFragment(our_gl::Vertex{position, normal, st});

        z_buffer.set(x, y,
                     TGAColor(static_cast<int>(z), static_cast<int>(z),
                              static_cast<int>(z), 255, TGAImage::GRAYSCALE));
        image.set(x, y, fragment_color);
      }
    }
  }
}

}  // namespace our_gl
