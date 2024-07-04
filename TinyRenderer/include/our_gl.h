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

#include "./geometry.h"
#include "./model.h"
#include "./tgaimage.h"

namespace our_gl {
struct Vertex {
  geometry::Vec3f position;
  geometry::Vec3f normal;
  geometry::Vec2f texture_coords;
};

class IShader {
 public:
  virtual our_gl::Vertex ShadeVertex(model::Vertex model_vertex) const = 0;
  virtual TGAColor ShadeFragment(const our_gl::Vertex& vertex) const = 0;
};

class FirstShader : public IShader {
 public:
  geometry::Mat4x4f g_viewport_mat;

  geometry::Mat4x4f u_vpm_mat;  // view * projection * model
  geometry::Vec3f u_light_dir;

  our_gl::Vertex ShadeVertex(model::Vertex model_vertex) const override;
  TGAColor ShadeFragment(const our_gl::Vertex& vertex) const override;
};

void DrawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);

void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2,
                  TGAImage& image, TGAColor color);

void DrawTriangle(const geometry::Vec3f& v0, const geometry::Vec3f& v1,
                  const geometry::Vec3f& v2, TGAColor color, TGAImage& image,
                  std::vector<std::vector<float>>& z_buffer);

void DrawTriangle(const geometry::Vec3f& p0, const geometry::Vec3f& p1,
                  const geometry::Vec3f& p2, const geometry::Vec3f& n0,
                  const geometry::Vec3f& n1, const geometry::Vec3f& n2,
                  const TGAColor& color, const geometry::Vec3f& light_dir,
                  TGAImage& image, std::vector<std::vector<float>>& z_buffer);

void DrawTriangle(const geometry::Vec3f& p0, const geometry::Vec3f& p1,
                  const geometry::Vec3f& p2, const geometry::Vec3f& n0,
                  const geometry::Vec3f& n1, const geometry::Vec3f& n2,
                  const geometry::Vec2f& st0, const geometry::Vec2f& st1,
                  const geometry::Vec2f& st2, const TGAImage& texture,
                  const geometry::Vec3f& light_dir, TGAImage& image,
                  std::vector<std::vector<float>>& z_buffer);

void DrawTriangle(const std::vector<our_gl::Vertex>& vertices,
                  const IShader& shader, TGAImage& image, TGAImage& z_buffer);
}  // namespace our_gl
