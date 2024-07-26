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

#include "./geometry/utils.h"

namespace our_gl {

void OurGL::DrawModel(const model::Model& model, IShader& shader,
                      Image<RgbaColor>& image,
                      Image<GrayscaleColor>& z_buffer) {
  for (int i = 0; i != model.size(); ++i) {
    auto face = model.get(i);

    std::array<our_gl::gl_Position, 3> gl_Positions;
    for (int v_idx = 0; v_idx != 3; ++v_idx) {
      gl_Positions[v_idx] = shader.ShadeVertex(*this, face[v_idx], v_idx);
    }

    DrawTriangle(gl_Positions, shader, image, z_buffer);
  }
}

Vec<3, float> GetBarycentric(const Vec<2, float>& target,
                             const Vec<2, float>& p0, const Vec<2, float>& p1,
                             const Vec<2, float>& p2) {
  Vec<2, float> edge1 = p1 - p0;
  Vec<2, float> edge2 = p2 - p0;
  Vec<2, float> origin_from_point = p0 - target;

  Vec<3, float> vec_x{edge1[0], edge2[0], origin_from_point[0]};
  Vec<3, float> vec_y{edge1[1], edge2[1], origin_from_point[1]};

  Vec<3, float> vec_crossed = vec_x ^ vec_y;

  if (vec_crossed[2] == 0) {
    return Vec<3, float>({-1, 1, 1});
  }

  return Vec<3, float>(
      {1.f - (vec_crossed[0] + vec_crossed[1]) / vec_crossed[2],
       vec_crossed[0] / vec_crossed[2], vec_crossed[1] / vec_crossed[2]});
}

bool IsPointInTriangle(const Vec<2, int>& edge1, const Vec<2, int>& edge2,
                       const Vec<2, int>& origin_from_point) {
  Vec<3, int> vec_x = Vec<3, int>({edge1[0], edge2[0], origin_from_point[0]});
  Vec<3, int> vec_y = Vec<3, int>({edge1[1], edge2[1], origin_from_point[1]});

  Vec<3, int> vec_crossed = vec_x ^ vec_y;

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

RgbaColor GetPhongColor(const Vec<3, float>& normal,
                        const Vec<3, float>& view_vector,
                        const Vec<3, float>& light_dir,
                        const RgbaColor& texture_color, float diffuse,
                        float specular, float alpha) {
  Vec<3, float> light_vec = Vec<3, float>(light_dir).Normalize() * (-1);
  Vec<3, float> reflection = Reflect(light_vec, normal);
  Vec<3, float> normalized_view = Vec<3, float>(view_vector).Normalize();

  RgbaColor diffuse_color =
      texture_color * (diffuse * std::max(0.f, normal * light_vec));
  RgbaColor specular_color =
      RgbaColor(255, 255, 255, 255) *
      (specular * pow(std::max(0.f, reflection * normalized_view), alpha));

  return diffuse_color + specular_color;
}

void OurGL::DrawTriangle(const std::array<gl_Position, 3>& gl_Positions,
                         IShader& shader, Image<RgbaColor>& image,
                         Image<GrayscaleColor>& z_buffer) {
  const Vec<3, float>& p0 = gl_Positions[0];
  const Vec<3, float>& p1 = gl_Positions[1];
  const Vec<3, float>& p2 = gl_Positions[2];

  // Give some margin to avoid the edge of the triangle
  int min_x = std::min(p0[0], std::min(p1[0], p2[0])) - 1;
  int max_x = std::max(p0[0], std::max(p1[0], p2[0])) + 1;
  int min_y = std::min(p0[1], std::min(p1[1], p2[1])) - 1;
  int max_y = std::max(p0[1], std::max(p1[1], p2[1])) + 1;

  const int z_buffer_width = z_buffer.GetWidth();
  const int z_buffer_height = z_buffer.GetHeight();

  // To handle the case where the coord is same with width or height
  min_x = std::clamp(min_x, 0, z_buffer_width - 1);
  min_y = std::clamp(min_y, 0, z_buffer_height - 1);
  max_x = std::clamp(max_x, 0, z_buffer_width - 1);
  max_y = std::clamp(max_y, 0, z_buffer_height - 1);

  for (int x = min_x; x != max_x; ++x) {
    for (int y = min_y; y != max_y; ++y) {
      Vec<3, float> barycentric = GetBarycentric(
          Vec<2, float>({static_cast<float>(x), static_cast<float>(y)}),
          Vec<2, float>({static_cast<float>(p0[0]), static_cast<float>(p0[1])}),
          Vec<2, float>({static_cast<float>(p1[0]), static_cast<float>(p1[1])}),
          Vec<2, float>(
              {static_cast<float>(p2[0]), static_cast<float>(p2[1])}));

      if (barycentric[0] < 0 || barycentric[1] < 0 || barycentric[2] < 0) {
        continue;
      }

      if (auto z = barycentric[0] * p0[2] + barycentric[1] * p1[2] +
                   barycentric[2] * p2[2];
          // TODO(Seongho Park): Make 255.f as a constant
          static_cast<float>(z_buffer.at(x, y).value) / 255.f < z && z <= 1) {
        float bar_x = std::max(barycentric[0] * p0[0] + barycentric[1] * p1[0] +
                                   barycentric[2] * p2[0],
                               0.f);
        float bar_y = std::max(barycentric[0] * p0[1] + barycentric[1] * p1[1] +
                                   barycentric[2] * p2[1],
                               0.f);

        Vec<3, float> gl_FragCoord({
            bar_x,
            bar_y,
            z,
        });

        RgbaColor fragment_color =
            shader.ShadeFragment(*this, gl_FragCoord, barycentric);

        z_buffer.set(x, y, GrayscaleColor(static_cast<uint8_t>(z * 255.f)));
        image.set(x, y, fragment_color);
      }
    }
  }
}

Vec<3, float> ConvertColorToVec(const RgbaColor& color) {
  return Vec<3, float>({static_cast<float>(color.r) / 255.f - .5f,
                        static_cast<float>(color.g) / 255.f - .5f,
                        static_cast<float>(color.b) / 255.f - .5f});
}

}  // namespace our_gl
