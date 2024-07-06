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

class Shader : public our_gl::IShader {
 public:
  our_gl::gl_Position ShadeVertex(const our_gl::OurGL& gl,
                                  model::Vertex model_vertex,
                                  int vertex_index) override {
    geometry::Vec<4, float> pos_4 = geometry::Vec<4, float>(
        {model_vertex.position[0], model_vertex.position[1],
         model_vertex.position[2], 1});

    geometry::Vec<4, float> new_position = gl.u_vpm_mat * pos_4;

    varying_positions.SetColumn(vertex_index, geometry::GetNDC(pos_4));
    varying_texcoords.SetColumn(vertex_index, model_vertex.texture_coords);
    varying_normals.SetColumn(vertex_index, model_vertex.normal);

    return geometry::GetNDC(gl.g_viewport_mat * new_position);
  }
  our_gl::gl_Fragment ShadeFragment(
      const our_gl::OurGL& gl,
      const geometry::Vec<3, float> barycentric) const override {
    geometry::Vec<3, float> position = varying_positions * barycentric;
    geometry::Vec<3, float> normal = varying_normals * barycentric;
    normal.Normalize();
    geometry::Vec<2, float> texture_coords = varying_texcoords * barycentric;

    // Get Darboux Basis
    geometry::Mat<3, 3, float> darboux_basis_matrix;
    darboux_basis_matrix.SetRow(0, varying_positions.GetColumnVector(1) -
                                       varying_positions.GetColumnVector(0));
    darboux_basis_matrix.SetRow(1, varying_positions.GetColumnVector(2) -
                                       varying_positions.GetColumnVector(0));
    darboux_basis_matrix.SetRow(2, normal);

    geometry::Mat<3, 3, float> darboux_matrix_inverse =
        geometry::Inverse(darboux_basis_matrix);

    geometry::Vec<3, float> darboux_i =
        (darboux_matrix_inverse *
         geometry::Vec<3, float>(
             {varying_texcoords[0][1] - varying_texcoords[0][0],
              varying_texcoords[0][2] - varying_texcoords[0][0], 0}))
            .GetNormalized();
    geometry::Vec<3, float> darboux_j =
        (darboux_matrix_inverse *
         geometry::Vec<3, float>(
             {varying_texcoords[1][1] - varying_texcoords[1][0],
              varying_texcoords[1][2] - varying_texcoords[1][0], 0}))
            .GetNormalized();

    TGAColor tangent_normal_color = our_gl::FindNearestTextureColor(
        texture_coords, gl.u_tangent_normal_map);
    geometry::Vec<3, float> tangent_normal =
        our_gl::ConvertColorToVec(tangent_normal_color);

    geometry::Vec<3, float> real_normal = darboux_i * tangent_normal[0] +
                                          darboux_j * tangent_normal[1] +
                                          normal * tangent_normal[2];
    real_normal.Normalize();

    geometry::Vec<3, float> light_dir = gl.u_light_dir;

    TGAColor texture_color =
        our_gl::FindNearestTextureColor(texture_coords, gl.u_texture);

    TGAColor phong_color = our_gl::GetPhongColor(real_normal, gl.u_view_vector,
                                                 light_dir, texture_color);

    // Get shadow
    geometry::Mat<4, 4, float> shadow_depth_viewport =
        geometry::Viewport(0.f, 0.f, 1.f, 1.f, 255.f);

    geometry::Vec<4, float> screen_coord_from_light =
        (shadow_depth_viewport * gl.u_light_vpm_mat *
         geometry::Vec<4, float>({position[0], position[1], position[2], 1}));

    TGAColor shadow_depth = our_gl::FindNearestTextureColor(
        geometry::Vec<2, float>(
            {screen_coord_from_light[0] / screen_coord_from_light[3],
             screen_coord_from_light[1] / screen_coord_from_light[3]}),
        *gl.u_shadow_depth_map);

    if (screen_coord_from_light[2] + 0.01 * 255 < shadow_depth.bgra[0]) {
      return phong_color * 0.1;
    }

    return phong_color;
  }

 private:
  geometry::Mat<3, 3, float> varying_positions;
  geometry::Mat<2, 3, float> varying_texcoords;
  geometry::Mat<3, 3, float> varying_normals;
};

class DepthShader : public our_gl::IShader {
 public:
  our_gl::gl_Position ShadeVertex(const our_gl::OurGL& gl,
                                  model::Vertex model_vertex,
                                  int vertex_index) override {
    geometry::Vec<4, float> pos_4 = geometry::Vec<4, float>(
        {model_vertex.position[0], model_vertex.position[1],
         model_vertex.position[2], 1});

    geometry::Vec<4, float> new_position = gl.u_shadow_vpm_mat * pos_4;

    return geometry::GetNDC(gl.g_viewport_mat * new_position);
  }

  our_gl::gl_Fragment ShadeFragment(
      const our_gl::OurGL& gl,
      const geometry::Vec<3, float> barycentric) const override {
    return TGAColor(255, 255, 255, 255);
  }

 private:
  geometry::Mat<3, 3, float> varying_positions;
};

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

  Shader shader;
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
