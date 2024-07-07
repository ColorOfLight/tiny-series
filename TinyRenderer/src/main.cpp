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

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <random>
#include <string>
#include <vector>

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
    geometry::Viewport(0, 0, width, height, 1);

const int ao_phi_step = 10;
const int ao_theta_step = 10;

float smoothstep(float edge0, float edge1, float x) {
  // Clamp x to the [0, 1] range
  x = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
  // Evaluate the smoothstep function
  return x * x * (3 - 2 * x);
}

std::vector<geometry::Vec<3, float>> GeneratePointsOnSphere(float radius,
                                                            int phi_step,
                                                            int theta_step) {
  if (radius < 0) {
    throw std::runtime_error("Radius must be positive");
  }

  if (phi_step < 2) {
    throw std::runtime_error("Phi step must be more than 1");
  }

  if (theta_step < 2) {
    throw std::runtime_error("Theta step must be more than 1");
  }

  std::vector<geometry::Vec<3, float>> points;
  points.resize((phi_step - 1) * theta_step + 2);

  int points_idx = 0;
  for (int phi_idx = 0; phi_idx != phi_step + 1; ++phi_idx) {
    if (phi_idx == 0) {
      points[points_idx++] = geometry::Vec<3, float>({0, 0, radius});
    } else if (phi_idx == phi_step) {
      points[points_idx++] = geometry::Vec<3, float>({0, 0, -radius});
    } else {
      for (int theta_idx = 0; theta_idx != theta_step; ++theta_idx) {
        float phi = M_PI * phi_idx / phi_step;
        float theta = 2 * M_PI * theta_idx / theta_step;

        float x = radius * sin(phi) * cos(theta);
        float y = radius * sin(phi) * sin(theta);
        float z = radius * cos(phi);

        points[points_idx++] = geometry::Vec<3, float>({x, y, z});
      }
    }
  }

  return points;
}

TGAImage GenerateAOMap(const model::Model& model, our_gl::OurGL& gl,
                       DepthShader& depth_shader, AOShader& ao_shader,
                       TGAImage& null_image, TGAImage& z_buffer,
                       TGAImage& shadow_depth_buffer) {
  std::vector<std::vector<int>> final_ao_map;
  final_ao_map.resize(height * width);
  for (int i = 0; i != height; ++i) {
    final_ao_map[i].resize(width);
  }

  auto ao_points = GeneratePointsOnSphere(2, ao_phi_step, ao_theta_step);

  for (int i = 0; i != ao_points.size(); ++i) {
    // get random eye
    geometry::Vec<3, float> random_eye = ao_points[i] * (-1);

    gl.u_shadow_vpm_mat =
        geometry::Perspective(3) * geometry::ViewMatrix(random_eye, center, up);

    gl.DrawModel(model, depth_shader, null_image, shadow_depth_buffer);
    gl.DrawModel(model, ao_shader, null_image, z_buffer);

    for (int y = 0; y != height; ++y) {
      for (int x = 0; x != width; ++x) {
        if (gl.u_ao_map->at(y)[x]) {
          final_ao_map[y][x] += 1;
        }
      }
    }

    for (int y = 0; y != height; ++y) {
      for (int x = 0; x != width; ++x) {
        gl.u_ao_map->at(y)[x] = false;
      }
    }

    if (i == ao_points.size() - 1) {
      z_buffer.write_tga_file("z_buffer.tga");
      shadow_depth_buffer.write_tga_file("shadow_depth_buffer.tga");
    }

    for (int y = 0; y != height; ++y) {
      for (int x = 0; x != width; ++x) {
        z_buffer.set(x, y, TGAColor(0, 0, 0, 255, 1));
      }
    }

    std::cout << i << std::endl;
  }

  TGAImage final_ao_image(width, height, TGAImage::GRAYSCALE);

  for (int y = 0; y != height; ++y) {
    for (int x = 0; x != width; ++x) {
      int8_t color_value =
          static_cast<int8_t>(final_ao_map[y][x] * 255 / ao_points.size());
      final_ao_image.set(x, y,
                         TGAColor(color_value, color_value, color_value, 255));
    }
  }

  return final_ao_image;
}

int main() {
  model::Model model("./assets/diablo3_pose.obj");

  TGAImage texture;
  // texture.read_tga_file("./assets/diablo3_pose_diffuse.tga");
  texture.read_tga_file("./final_ao.tga");
  texture.flip_vertically();

  TGAImage tangent_normal_map;
  tangent_normal_map.read_tga_file("./assets/diablo3_pose_nm_tangent.tga");
  tangent_normal_map.flip_vertically();

  TGAImage image(width, height, TGAImage::RGB);
  TGAImage null_image(width, height, TGAImage::RGB);
  TGAImage z_buffer(width, height, TGAImage::GRAYSCALE);
  TGAImage shadow_depth_buffer(width, height, TGAImage::GRAYSCALE);

  TGAImage ssao_frame_buffer(width, height, TGAImage::GRAYSCALE);

  std::vector<std::vector<bool>> ao_map{false};

  ao_map.resize(height);
  for (int i = 0; i != height; ++i) {
    ao_map[i].resize(width);
  }

  MainShader shader;
  DepthShader depth_shader;
  AOShader ao_shader;
  ZShader z_shader;

  geometry::Mat<4, 4, float> light_view_matrix =
      geometry::ViewMatrix(light_position - light_dir, light_position, up);
  geometry::Mat<4, 4, float> light_proj_matrix = geometry::Orthographic(2);
  geometry::Mat<4, 4, float> light_vpm = light_proj_matrix * light_view_matrix;

  our_gl::OurGL gl;

  gl.g_viewport_mat = viewport_mat;
  gl.g_width = width;
  gl.g_height = height;

  gl.u_vpm_mat = perspective_mat * view_mat;
  gl.u_light_vpm_mat = light_vpm;
  gl.u_light_dir = light_dir;
  gl.u_view_vector = eye - center;
  gl.u_texture = texture;
  gl.u_tangent_normal_map = tangent_normal_map;
  gl.u_shadow_depth_map = &shadow_depth_buffer;

  gl.u_shadow_vpm_mat = light_vpm;

  gl.u_ao_map = &ao_map;
  gl.u_ao_map_width = width;
  gl.u_ao_map_height = height;

  // TGAImage final_ao_image =
  //     GenerateAOMap(model, gl, depth_shader, ao_shader, null_image, z_buffer,
  //                   shadow_depth_buffer);

  // final_ao_image.write_tga_file("final_ao.tga");

  // gl.DrawModel(model, depth_shader, null_image, shadow_depth_buffer);
  gl.DrawModel(model, z_shader, null_image, z_buffer);

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      int z_value = static_cast<int>(z_buffer.get(x, y)[0]);

      if (z_value <= 0) continue;

      float total = 0;

      for (int dx = -1; dx <= 1; dx++) {
        if (x + dx < 0 || x + dx >= width) continue;

        for (int dy = -1; dy <= 1; dy++) {
          if (y + dy < 0 || y + dy >= height) continue;
          if (dx == 0 && dy == 0) continue;

          float distance = sqrt(dx * dx + dy * dy);
          int delta_z = z_buffer.get(x + dx, y + dy)[0] - z_value;

          float exposed_angle = std::max(
              0.f, atan2f(static_cast<float>(delta_z) / 255.f, distance));

          total += geometry::kPi / 2 - exposed_angle;
        }
      }

      total /= (geometry::kPi / 2) * 8;
      total = pow(total, 100);
      total = smoothstep(0.05, 0.95, total);

      int color_value = static_cast<int>(total * 255);
      ssao_frame_buffer.set(
          x, y, TGAColor(color_value, color_value, color_value, 255));
    }
  }

  // image.write_tga_file("output.tga");
  // z_buffer.write_tga_file("z_buffer.tga");
  // shadow_depth_buffer.write_tga_file("shadow_depth_buffer.tga");
  ssao_frame_buffer.write_tga_file("ssao.tga");

  return 0;
}
