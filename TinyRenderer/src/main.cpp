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

#include "./geometry/mat.h"
#include "./geometry/utils.h"
#include "./geometry/vec.h"
#include "./model.h"
#include "./our_gl.h"
#include "./shader.h"
#include "./tgaimage.h"

const TGAColor red(255, 0, 0, 255);
const TGAColor white(255, 255, 255, 255);

const int width = 800;
const int height = 800;

const geometry::Vec<3, float> eye{1, 1, 3};
const geometry::Vec<3, float> center{0, 0, 0};
const geometry::Vec<3, float> up{0, 1, 0};
const geometry::Vec<3, float> light_dir{0, 0, -1};
const geometry::Vec<3, float> light_position{0, 0, 1};

const geometry::Mat<4, 4, float> view_mat =
    geometry::ViewMatrix(eye, center, up);
const geometry::Mat<4, 4, float> perspective_mat = geometry::Perspective(3);
const geometry::Mat<4, 4, float> viewport_mat =
    geometry::Viewport(0, 0, width, height, 255);

int main() {
  model::Model model("./assets/african_head.obj");

  TGAImage texture;
  texture.read_tga_file("./assets/african_head_diffuse.tga");
  texture.flip_vertically();

  TGAImage tangent_normal_map;
  tangent_normal_map.read_tga_file("./assets/african_head_nm_tangent.tga");
  tangent_normal_map.flip_vertically();

  TGAImage image(width, height, TGAImage::RGB);
  TGAImage null_image(width, height, TGAImage::RGB);
  TGAImage z_buffer(width, height, TGAImage::GRAYSCALE);
  TGAImage shadow_depth_buffer(width, height, TGAImage::GRAYSCALE);

  MainShader shader;
  DepthShader depth_shader;

  geometry::Mat<4, 4, float> light_view_matrix =
      geometry::ViewMatrix(light_position - light_dir, light_position, up);
  geometry::Mat<4, 4, float> light_proj_matrix = geometry::Orthographic(2);
  geometry::Mat<4, 4, float> light_vpm = light_proj_matrix * light_view_matrix;

  our_gl::OurGL gl;

  gl.g_viewport_mat = viewport_mat;

  gl.u_vpm_mat = perspective_mat * view_mat;
  gl.u_light_vpm_mat = light_vpm;
  gl.u_light_dir = light_dir;
  gl.u_view_vector = eye - center;
  gl.u_texture = texture;
  gl.u_tangent_normal_map = tangent_normal_map;
  gl.u_shadow_depth_map = &shadow_depth_buffer;

  gl.u_shadow_vpm_mat = light_vpm;

  gl.DrawModel(model, depth_shader, null_image, shadow_depth_buffer);
  gl.DrawModel(model, shader, image, z_buffer);

  image.write_tga_file("output.tga");
  z_buffer.write_tga_file("z_buffer.tga");
  shadow_depth_buffer.write_tga_file("shadow_depth_buffer.tga");

  return 0;
}
