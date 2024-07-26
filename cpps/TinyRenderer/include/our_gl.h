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

#include <algorithm>
#include <cmath>
#include <vector>

#include "./geometry/mat.h"
#include "./geometry/vec.h"
#include "./image.h"
#include "./model.h"

namespace our_gl {
typedef Vec<3, float> gl_Position;
typedef RgbaColor gl_Fragment;

class OurGL;

class IShader {
 public:
  virtual gl_Position ShadeVertex(const OurGL& gl, Vertex model_vertex,
                                  int vertex_index) = 0;
  virtual gl_Fragment ShadeFragment(const OurGL& gl, Vec<3, float> gl_FragCoord,
                                    const Vec<3, float> barycentric) const = 0;
};

class OurGL {
 public:
  Mat<4, 4, float> g_viewport_mat;
  int g_width;
  int g_height;

  Mat<4, 4, float> u_vpm_mat;  // view * projection * model
  Mat<4, 4, float> u_light_vpm_mat;
  Mat<4, 4, float> u_shadow_vpm_mat;
  Vec<3, float> u_light_dir;
  Vec<3, float> u_view_vector;
  Image<RgbaColor> u_texture;
  Image<RgbaColor> u_tangent_normal_map;
  Image<GrayscaleColor>* u_shadow_depth_map;

  OurGL() : g_width(0), g_height(0) {}

  void DrawModel(const Model& model, IShader& shader, Image<RgbaColor>& image,
                 Image<GrayscaleColor>& z_buffer);

 private:
  void DrawTriangle(const std::array<gl_Position, 3>& gl_Positions,
                    IShader& shader, Image<RgbaColor>& image,
                    Image<GrayscaleColor>& z_buffer);
};

Vec<3, float> GetBarycentric(const Vec<2, float>& target,
                             const Vec<2, float>& p0, const Vec<2, float>& p1,
                             const Vec<2, float>& p2);

bool IsPointInTriangle(const Vec<2, int>& edge1, const Vec<2, int>& edge2,
                       const Vec<2, int>& origin_from_point);

RgbaColor GetPhongColor(const Vec<3, float>& normal,
                        const Vec<3, float>& view_vector,
                        const Vec<3, float>& light_dir,
                        const RgbaColor& texture_color, float diffuse = 1,
                        float specular = 0.5, float alpha = 16);

template <class Color>
Color FindNearestTextureColor(const Vec<2, float>& st,
                              const Image<Color>& texture) {
  float normalized_s = std::fmod(st[0], 1.f);
  if (normalized_s < 0) {
    normalized_s += 1;
  }

  float normalized_t = std::fmod(st[1], 1.f);
  if (normalized_t < 0) {
    normalized_t += 1;
  }

  int width = texture.GetWidth();
  int height = texture.GetHeight();

  int x = std::min(static_cast<int>(normalized_s * width), width - 1);
  int y = std::min(static_cast<int>(normalized_t * height), height - 1);

  return texture.at(x, y);
}

Vec<3, float> ConvertColorToVec(const RgbaColor& color);

}  // namespace our_gl
