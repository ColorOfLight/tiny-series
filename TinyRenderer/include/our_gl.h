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

#include "./geometry/mat.h"
#include "./geometry/vec.h"
#include "./model.h"
#include "./tgaimage.h"

namespace our_gl {
struct Vertex {
  geometry::Vec<3, float> position;
  geometry::Vec<3, float> normal;
  geometry::Vec<2, float> texture_coords;
};

class IShader {
 public:
  virtual our_gl::Vertex ShadeVertex(model::Vertex model_vertex) const = 0;
  virtual TGAColor ShadeFragment(const our_gl::Vertex& vertex) const = 0;
};

geometry::Vec<3, float> GetBarycentric(const geometry::Vec<2, float>& target,
                                       const geometry::Vec<2, float>& p0,
                                       const geometry::Vec<2, float>& p1,
                                       const geometry::Vec<2, float>& p2);

bool IsPointInTriangle(const geometry::Vec<2, int>& edge1,
                       const geometry::Vec<2, int>& edge2,
                       const geometry::Vec<2, int>& origin_from_point);

TGAColor GetPhongColor(const geometry::Vec<3, float>& normal,
                       const geometry::Vec<3, float>& view_vector,
                       const geometry::Vec<3, float>& light_dir,
                       const TGAColor& texture_color, float diffuse = 1,
                       float specular = 0.5, float alpha = 16);

TGAColor FindNearestTextureColor(const geometry::Vec<2, float>& st,
                                 const TGAImage& texture);

void DrawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);

void DrawTriangle(const std::vector<our_gl::Vertex>& vertices,
                  const IShader& shader, TGAImage& image, TGAImage& z_buffer);
}  // namespace our_gl
