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

#include "./image.h"
#include "./our_gl.h"

class MainShader : public IShader {
 public:
  gl_Position ShadeVertex(const OurGL& gl, Vertex model_vertex,
                          int vertex_index) override;
  gl_Fragment ShadeFragment(const OurGL& gl, Vec<3, float> gl_FragCoord,
                            const Vec<3, float> barycentric) const override;

 private:
  Mat<3, 3, float> varying_positions;
  Mat<2, 3, float> varying_texcoords;
  Mat<3, 3, float> varying_normals;
};

class DepthShader : public IShader {
 public:
  gl_Position ShadeVertex(const OurGL& gl, Vertex model_vertex,
                          int vertex_index) override;

  gl_Fragment ShadeFragment(const OurGL& gl, Vec<3, float> gl_FragCoord,
                            const Vec<3, float> barycentric) const override {
    return RgbaColor(255, 255, 255);
  }

 private:
  Mat<3, 3, float> varying_positions;
};

class ZShader : public IShader {
 public:
  gl_Position ShadeVertex(const OurGL& gl, Vertex model_vertex,
                          int vertex_index) override;

  inline gl_Fragment ShadeFragment(
      const OurGL& gl, Vec<3, float> gl_FragCoord,
      const Vec<3, float> barycentric) const override {
    return RgbaColor(0, 0, 0, 0);
  }

 private:
  Mat<3, 3, float> varying_positions;
};
