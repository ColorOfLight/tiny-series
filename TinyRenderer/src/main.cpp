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

#include <filesystem>
#include <iostream>
#include <string>

#include "./model.h"
#include "./our_gl.h"
#include "./tgaimage.h"

const TGAColor red(255, 0, 0, 255);
const TGAColor white(255, 255, 255, 255);

const int width = 800;
const int height = 800;

const geometry::Vec3f eye(1, 1, 3);
const geometry::Vec3f center(0, 0, 0);
const geometry::Vec3f up(0, 1, 0);
const geometry::Vec3f light_dir(0, 0, -1);

const geometry::Mat4x4f view_mat = geometry::ViewMatrix(eye, center, up);
const geometry::Mat4x4f perspective_mat = geometry::Perspective(3);
const geometry::Mat4x4f viewport_mat =
    geometry::Viewport(0, 0, width, height, 255);

int main() {
  model::Model model("./assets/african_head.obj");

  TGAImage texture;
  texture.read_tga_file("./assets/african_head_diffuse.tga");
  texture.flip_vertically();

  TGAImage image(width, height, TGAImage::RGB);
  TGAImage z_buffer(width, height, TGAImage::GRAYSCALE);

  our_gl::FirstShader shader;
  shader.u_vpm_mat = perspective_mat * view_mat;
  shader.g_viewport_mat = viewport_mat;
  shader.u_light_dir = light_dir;
  shader.u_texture = texture;
  shader.u_view_vector = eye - center;

  for (int i = 0; i != model.size(); ++i) {
    auto face = model.get(i);

    std::vector<our_gl::Vertex> vertices = std::vector<our_gl::Vertex>(3);
    for (int v_idx = 0; v_idx != 3; ++v_idx) {
      vertices[v_idx] = shader.ShadeVertex(face[v_idx]);
    }

    our_gl::DrawTriangle(vertices, shader, image, z_buffer);
  }

  // RenderFlatShading(model, light_dir, texture, image);

  image.write_tga_file("output.tga");
  z_buffer.write_tga_file("z_buffer.tga");

  return 0;
}
