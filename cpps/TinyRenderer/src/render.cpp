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

#include "./render.h"

#include "./geometry/mat.h"
#include "./geometry/utils.h"
#include "./geometry/vec.h"
#include "./our_gl.h"
#include "./shader.h"

Image<GrayscaleColor> GenerateSsaoImage(const model::Model& model,
                                        our_gl::OurGL& gl,
                                        Image<GrayscaleColor>& z_buffer);

RenderModelResult RenderModel(const model::Model& model,
                              const Image<RgbaColor>& diffuse_texture,
                              const Image<RgbaColor>& normal_map, int width,
                              int height,
                              const geometry::Vec<3, float>& light_position,
                              const geometry::Vec<3, float>& camera_position) {
  Image<RgbaColor> frame(width, height);
  Image<GrayscaleColor> z_buffer(width, height);
  Image<GrayscaleColor> shadow_map_buffer(width, height);

  MainShader shader;
  DepthShader depth_shader;

  geometry::Vec<3, float> center{0, 0, 0};

  geometry::Vec<3, float> up{0, 1, 0};

  geometry::Vec<3, float> camera_up =
      !geometry::IsParallel((camera_position - center), up)
          ? up
          : geometry::Vec<3, float>({0, 0, 1});

  geometry::Vec<3, float> light_up =
      !geometry::IsParallel((light_position - center), up)
          ? up
          : geometry::Vec<3, float>({0, 0, 1});

  geometry::Vec<3, float> light_direction =
      (center - light_position).Normalize();

  geometry::Mat<4, 4, float> viewport_matrix =
      geometry::Viewport(0, 0, width, height, 1);
  geometry::Mat<4, 4, float> perspective_matrix = geometry::Perspective(3);
  const geometry::Mat<4, 4, float> view_matrix =
      geometry::ViewMatrix(camera_position, center, camera_up);

  geometry::Mat<4, 4, float> light_view_matrix =
      geometry::ViewMatrix(light_position, center, light_up);
  geometry::Mat<4, 4, float> light_proj_matrix =
      geometry::Orthographic(4, 4, 4);
  geometry::Mat<4, 4, float> light_vpm = light_proj_matrix * light_view_matrix;

  our_gl::OurGL gl;

  gl.g_viewport_mat = viewport_matrix;
  gl.g_width = width;
  gl.g_height = height;

  gl.u_vpm_mat = perspective_matrix * view_matrix;
  gl.u_light_vpm_mat = light_vpm;
  gl.u_light_dir = light_direction;
  gl.u_view_vector = camera_position - center;
  gl.u_texture = diffuse_texture;
  gl.u_tangent_normal_map = normal_map;
  gl.u_shadow_depth_map = &shadow_map_buffer;

  gl.u_shadow_vpm_mat = light_vpm;

  Image<RgbaColor> null_image(gl.g_width, gl.g_height);

  gl.DrawModel(model, depth_shader, null_image, shadow_map_buffer);

  Image<GrayscaleColor> ao_image = GenerateSsaoImage(model, gl, z_buffer);
  gl.DrawModel(model, shader, frame, z_buffer);

  return {frame, z_buffer, shadow_map_buffer, ao_image};
}

// SSAO with simple angle calculation
Image<GrayscaleColor> GenerateSsaoImage(const model::Model& model,
                                        our_gl::OurGL& gl,
                                        Image<GrayscaleColor>& z_buffer) {
  Image<GrayscaleColor> ssao_image(gl.g_width, gl.g_height);

  Image<RgbaColor> null_image(gl.g_width, gl.g_height);
  ZShader z_shader;

  gl.DrawModel(model, z_shader, null_image, z_buffer);

  for (int x = 0; x < gl.g_width; x++) {
    for (int y = 0; y < gl.g_height; y++) {
      int z_value = static_cast<int>(z_buffer.at(x, y).value);

      if (z_value <= 0) continue;

      float total = 0;

      for (int dx = -1; dx <= 1; dx++) {
        if (x + dx < 0 || x + dx >= gl.g_width) continue;

        for (int dy = -1; dy <= 1; dy++) {
          if (y + dy < 0 || y + dy >= gl.g_height) continue;
          if (dx == 0 && dy == 0) continue;

          float distance = sqrt(dx * dx + dy * dy);
          int delta_z =
              static_cast<int>(z_buffer.at(x + dx, y + dy).value) - z_value;

          float exposed_angle = std::max(
              0.f, atan2f(static_cast<float>(delta_z) / 255.f, distance));

          total += geometry::kPi / 2 - exposed_angle;
        }
      }

      total /= (geometry::kPi / 2) * 8;
      total = pow(total, 100);
      total = geometry::smoothstep(0.05, 0.95, total);

      int color_value = static_cast<int>(total * 255);
      ssao_image.set(x, y, GrayscaleColor{static_cast<uint8_t>(color_value)});
    }
  }

  return ssao_image;
}