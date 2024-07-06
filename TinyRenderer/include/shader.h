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

#include "./our_gl.h"

class MainShader : public our_gl::IShader {
 public:
  our_gl::gl_Position ShadeVertex(const our_gl::OurGL& gl,
                                  model::Vertex model_vertex,
                                  int vertex_index) override;
  our_gl::gl_Fragment ShadeFragment(
      const our_gl::OurGL& gl, geometry::Vec<3, float> gl_FragCoord,
      const geometry::Vec<3, float> barycentric) const override;

 private:
  geometry::Mat<3, 3, float> varying_positions;
  geometry::Mat<2, 3, float> varying_texcoords;
  geometry::Mat<3, 3, float> varying_normals;
};

class DepthShader : public our_gl::IShader {
 public:
  our_gl::gl_Position ShadeVertex(const our_gl::OurGL& gl,
                                  model::Vertex model_vertex,
                                  int vertex_index) override;

  our_gl::gl_Fragment ShadeFragment(
      const our_gl::OurGL& gl, geometry::Vec<3, float> gl_FragCoord,
      const geometry::Vec<3, float> barycentric) const override {
    return TGAColor(255, 255, 255, 255);
  }

 private:
  geometry::Mat<3, 3, float> varying_positions;
};

class AOShader : public our_gl::IShader {
 public:
  our_gl::gl_Position ShadeVertex(const our_gl::OurGL& gl,
                                  model::Vertex model_vertex,
                                  int vertex_index) override;

  our_gl::gl_Fragment ShadeFragment(
      const our_gl::OurGL& gl, geometry::Vec<3, float> gl_FragCoord,
      const geometry::Vec<3, float> barycentric) const override;

 private:
  geometry::Mat<2, 3, float> varying_texcoords;
};
