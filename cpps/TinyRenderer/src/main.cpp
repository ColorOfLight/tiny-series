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

#include "./file.h"
#include "./geometry/vec.h"
#include "./render.h"

int main() {
  model::Model model("../assets/diablo3_pose.obj");

  Image<RgbaColor> diffuse_texture =
      ReadTga("../assets/diablo3_pose_diffuse.tga");
  Image<RgbaColor> normal_map =
      ReadTga("../assets/diablo3_pose_nm_tangent.tga");

  int width = 800;
  int height = 800;

  geometry::Vec<3, float> light_direction = {0.0f, 0.0f, -1.0f};
  geometry::Vec<3, float> camera_position = {1.0f, 1.0f, 1.0f};

  RenderModelResult result =
      RenderModel(model, diffuse_texture, normal_map, width, height,
                  light_direction, camera_position);

  WriteTga("../output.tga", result.frame);
  WriteTga("../z_buffer.tga", result.z_buffer);
  WriteTga("../shadow_map.tga", result.shadow_map_buffer);
  WriteTga("../ao.tga", result.ao_buffer);
}