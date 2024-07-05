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

#include <vector>

#include "./geometry_new/mat.h"
#include "./geometry_new/vec.h"
#include "./model.h"
#include "./tgaimage.h"

namespace our_gl {
struct Vertex {
  geometry_new::Vec<3, float> position;
  geometry_new::Vec<3, float> normal;
  geometry_new::Vec<2, float> texture_coords;
};

class IShader {
 public:
  virtual our_gl::Vertex ShadeVertex(model::Vertex model_vertex) const = 0;
  virtual TGAColor ShadeFragment(const our_gl::Vertex& vertex) const = 0;
};

class FirstShader : public IShader {
 public:
  geometry_new::Mat<4, 4, float> g_viewport_mat;

  geometry_new::Mat<4, 4, float> u_vpm_mat;  // view * projection * model
  geometry_new::Vec<3, float> u_light_dir;
  geometry_new::Vec<3, float> u_view_vector;
  TGAImage u_texture;

  our_gl::Vertex ShadeVertex(model::Vertex model_vertex) const override;
  TGAColor ShadeFragment(const our_gl::Vertex& vertex) const override;
};

void DrawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);

void DrawTriangle(const std::vector<our_gl::Vertex>& vertices,
                  const IShader& shader, TGAImage& image, TGAImage& z_buffer);
}  // namespace our_gl
