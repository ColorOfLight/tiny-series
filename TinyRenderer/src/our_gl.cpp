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
#include <initializer_list>

#include "./geometry_new/utils.h"

// Internal functions

geometry_new::Vec<3, float> GetBarycentric(
    const geometry_new::Vec<2, float>& target,
    const geometry_new::Vec<2, float>& p0,
    const geometry_new::Vec<2, float>& p1,
    const geometry_new::Vec<2, float>& p2) {
  geometry_new::Vec<2, float> edge1 = p1 - p0;
  geometry_new::Vec<2, float> edge2 = p2 - p0;
  geometry_new::Vec<2, float> origin_from_point = p0 - target;

  geometry_new::Vec<3, float> vec_x{edge1[0], edge2[0], origin_from_point[0]};
  geometry_new::Vec<3, float> vec_y{edge1[1], edge2[1], origin_from_point[1]};

  geometry_new::Vec<3, float> vec_crossed = vec_x ^ vec_y;

  if (vec_crossed[2] == 0) {
    return geometry_new::Vec<3, float>({-1, 1, 1});
  }

  return geometry_new::Vec<3, float>(
      {1.f - (vec_crossed[0] + vec_crossed[1]) / vec_crossed[2],
       vec_crossed[0] / vec_crossed[2], vec_crossed[1] / vec_crossed[2]});
}

bool IsPointInTriangle(const geometry_new::Vec<2, int>& edge1,
                       const geometry_new::Vec<2, int>& edge2,
                       const geometry_new::Vec<2, int>& origin_from_point) {
  geometry_new::Vec<3, int> vec_x =
      geometry_new::Vec<3, int>({edge1[0], edge2[0], origin_from_point[0]});
  geometry_new::Vec<3, int> vec_y =
      geometry_new::Vec<3, int>({edge1[1], edge2[1], origin_from_point[1]});

  geometry_new::Vec<3, int> vec_crossed = vec_x ^ vec_y;

  if (vec_crossed[2] == 0) {
    return false;
  }

  float normalized_x =
      static_cast<float>(vec_crossed[0]) / static_cast<float>(vec_crossed[2]);
  float normalized_y =
      static_cast<float>(vec_crossed[1]) / static_cast<float>(vec_crossed[2]);

  if (normalized_x >= 0 && normalized_y >= 0 &&
      normalized_x + normalized_y <= 1) {
    return true;
  }

  return false;
}

TGAColor FindNearestTextureColor(const geometry_new::Vec<2, float>& st,
                                 const TGAImage& texture) {
  if (st[0] < 0 || st[0] > 1 || st[1] < 0 || st[1] > 1) {
    throw std::out_of_range("st should be in range [0, 1]");
  }

  int width = texture.width();
  int height = texture.height();

  int x = std::min(static_cast<int>(st[0] * width), width - 1);
  int y = std::min(static_cast<int>(st[1] * height), height - 1);

  return texture.get(x, y);
}

TGAColor GetPhongColor(const geometry_new::Vec<3, float>& normal,
                       const geometry_new::Vec<3, float>& view_vector,
                       const geometry_new::Vec<3, float>& light_dir,
                       const TGAColor& texture_color, float diffuse = 1,
                       float specular = 0.5, float alpha = 16) {
  geometry_new::Vec<3, float> light_vec =
      geometry_new::Vec<3, float>(light_dir).Normalize() * (-1);
  geometry_new::Vec<3, float> reflection =
      geometry_new::Reflect(light_vec, normal);
  geometry_new::Vec<3, float> normalized_view =
      geometry_new::Vec<3, float>(view_vector).Normalize();

  TGAColor diffuse_color =
      texture_color * (diffuse * std::max(0.f, normal * light_vec));
  TGAColor specular_color =
      TGAColor(255, 255, 255, 255) *
      (specular * pow(std::max(0.f, reflection * normalized_view), alpha));

  return diffuse_color + specular_color;
}

namespace our_gl {

Vertex FirstShader::ShadeVertex(model::Vertex model_vertex) const {
  geometry_new::Vec<4, float> pos_4 = geometry_new::Vec<4, float>(
      {model_vertex.position[0], model_vertex.position[1],
       model_vertex.position[2], 1});

  geometry_new::Vec<3, float> new_position =
      GetNDC(g_viewport_mat * u_vpm_mat * pos_4);

  return {new_position, model_vertex.normal, model_vertex.texture_coords};
}
TGAColor FirstShader::ShadeFragment(const our_gl::Vertex& vertex) const {
  geometry_new::Vec<3, float> normal = vertex.normal;
  geometry_new::Vec<3, float> light_dir = u_light_dir;

  TGAColor texture_color =
      FindNearestTextureColor(vertex.texture_coords, u_texture);

  TGAColor phong_color =
      GetPhongColor(normal, u_view_vector, light_dir, texture_color);

  return phong_color;
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

void DrawTriangle(const std::vector<our_gl::Vertex>& vertices,
                  const IShader& shader, TGAImage& image, TGAImage& z_buffer) {
  const geometry_new::Vec<3, float>& p0 = vertices[0].position;
  const geometry_new::Vec<3, float>& p1 = vertices[1].position;
  const geometry_new::Vec<3, float>& p2 = vertices[2].position;

  // Give some margin to avoid the edge of the triangle
  int min_x = std::min(p0[0], std::min(p1[0], p2[0])) - 1;
  int max_x = std::max(p0[0], std::max(p1[0], p2[0])) + 1;
  int min_y = std::min(p0[1], std::min(p1[1], p2[1])) - 1;
  int max_y = std::max(p0[1], std::max(p1[1], p2[1])) + 1;

  const int z_buffer_width = z_buffer.width();
  const int z_buffer_height = z_buffer.height();

  // To handle the case where the coord is same with width or height
  min_x = std::clamp(min_x, 0, z_buffer_width - 1);
  min_y = std::clamp(min_y, 0, z_buffer_height - 1);
  max_x = std::clamp(max_x, 0, z_buffer_width - 1);
  max_y = std::clamp(max_y, 0, z_buffer_height - 1);

  for (int x = min_x; x != max_x; ++x) {
    for (int y = min_y; y != max_y; ++y) {
      geometry_new::Vec<3, float> barycentric = GetBarycentric(
          geometry_new::Vec<2, float>(
              {static_cast<float>(x), static_cast<float>(y)}),
          geometry_new::Vec<2, float>(
              {static_cast<float>(p0[0]), static_cast<float>(p0[1])}),
          geometry_new::Vec<2, float>(
              {static_cast<float>(p1[0]), static_cast<float>(p1[1])}),
          geometry_new::Vec<2, float>(
              {static_cast<float>(p2[0]), static_cast<float>(p2[1])}));

      if (barycentric[0] < 0 || barycentric[1] < 0 || barycentric[2] < 0) {
        continue;
      }

      if (auto z = barycentric[0] * p0[2] + barycentric[1] * p1[2] +
                   barycentric[2] * p2[2];
          static_cast<float>(z_buffer.get(x, y).bgra[0]) < z) {
        geometry_new::Vec<3, float> position =
            vertices[0].position * barycentric[0] +
            vertices[1].position * barycentric[1] +
            vertices[2].position * barycentric[2];

        geometry_new::Vec<3, float> normal =
            vertices[0].normal * barycentric[0] +
            vertices[1].normal * barycentric[1] +
            vertices[2].normal * barycentric[2];

        geometry_new::Vec<2, float> st =
            vertices[0].texture_coords * barycentric[0] +
            vertices[1].texture_coords * barycentric[1] +
            vertices[2].texture_coords * barycentric[2];

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
