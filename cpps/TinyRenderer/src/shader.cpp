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

#include "./shader.h"

#include "./geometry/utils.h"
#include "./geometry/vec.h"

our_gl::gl_Position MainShader::ShadeVertex(const our_gl::OurGL& gl,
                                            Vertex model_vertex,
                                            int vertex_index) {
  Vec<4, float> pos_4 =
      Vec<4, float>({model_vertex.position[0], model_vertex.position[1],
                     model_vertex.position[2], 1});

  Vec<4, float> new_position = gl.u_vpm_mat * pos_4;

  varying_positions.SetColumn(vertex_index, GetNDC(pos_4));
  varying_texcoords.SetColumn(vertex_index, model_vertex.texture_coords);
  varying_normals.SetColumn(vertex_index, model_vertex.normal);

  return GetNDC(gl.g_viewport_mat * new_position);
}

our_gl::gl_Fragment MainShader::ShadeFragment(
    const our_gl::OurGL& gl, Vec<3, float> gl_FragCoord,
    const Vec<3, float> barycentric) const {
  Vec<3, float> position = varying_positions * barycentric;
  Vec<3, float> normal = varying_normals * barycentric;
  normal.Normalize();
  Vec<2, float> texture_coords = varying_texcoords * barycentric;

  // Get Darboux Basis
  Mat<3, 3, float> darboux_basis_matrix;
  darboux_basis_matrix.SetRow(0, varying_positions.GetColumnVector(1) -
                                     varying_positions.GetColumnVector(0));
  darboux_basis_matrix.SetRow(1, varying_positions.GetColumnVector(2) -
                                     varying_positions.GetColumnVector(0));
  darboux_basis_matrix.SetRow(2, normal);

  Mat<3, 3, float> darboux_matrix_inverse = Inverse(darboux_basis_matrix);

  Vec<3, float> darboux_i =
      (darboux_matrix_inverse *
       Vec<3, float>({varying_texcoords[0][1] - varying_texcoords[0][0],
                      varying_texcoords[0][2] - varying_texcoords[0][0], 0}))
          .GetNormalized();
  Vec<3, float> darboux_j =
      (darboux_matrix_inverse *
       Vec<3, float>({varying_texcoords[1][1] - varying_texcoords[1][0],
                      varying_texcoords[1][2] - varying_texcoords[1][0], 0}))
          .GetNormalized();

  RgbaColor tangent_normal_color =
      our_gl::FindNearestTextureColor(texture_coords, gl.u_tangent_normal_map);
  Vec<3, float> tangent_normal =
      our_gl::ConvertColorToVec(tangent_normal_color);

  Vec<3, float> real_normal = darboux_i * tangent_normal[0] +
                              darboux_j * tangent_normal[1] +
                              normal * tangent_normal[2];
  real_normal.Normalize();

  Vec<3, float> light_dir = gl.u_light_dir;

  RgbaColor texture_color =
      our_gl::FindNearestTextureColor(texture_coords, gl.u_texture);

  RgbaColor phong_color = our_gl::GetPhongColor(real_normal, gl.u_view_vector,
                                                light_dir, texture_color);

  // Get shadow
  Mat<4, 4, float> shadow_depth_viewport = Viewport(0.f, 0.f, 1.f, 1.f, 255.f);

  Vec<4, float> screen_coord_from_light =
      (shadow_depth_viewport * gl.u_light_vpm_mat *
       Vec<4, float>({position[0], position[1], position[2], 1}));

  GrayscaleColor shadow_depth = our_gl::FindNearestTextureColor(
      Vec<2, float>({screen_coord_from_light[0] / screen_coord_from_light[3],
                     screen_coord_from_light[1] / screen_coord_from_light[3]}),
      *gl.u_shadow_depth_map);

  if (screen_coord_from_light[2] + 0.05 * 255 <
      static_cast<float>(shadow_depth.value)) {
    return phong_color * 0.1;
  }

  return phong_color;
}

our_gl::gl_Position DepthShader::ShadeVertex(const our_gl::OurGL& gl,
                                             Vertex model_vertex,
                                             int vertex_index) {
  Vec<4, float> pos_4 =
      Vec<4, float>({model_vertex.position[0], model_vertex.position[1],
                     model_vertex.position[2], 1});

  Vec<4, float> new_position = gl.u_shadow_vpm_mat * pos_4;

  return GetNDC(gl.g_viewport_mat * new_position);
}

our_gl::gl_Position ZShader::ShadeVertex(const our_gl::OurGL& gl,
                                         Vertex model_vertex,
                                         int vertex_index) {
  Vec<4, float> pos_4 =
      Vec<4, float>({model_vertex.position[0], model_vertex.position[1],
                     model_vertex.position[2], 1});

  Vec<4, float> new_position = gl.u_vpm_mat * pos_4;

  return GetNDC(gl.g_viewport_mat * new_position);
}
