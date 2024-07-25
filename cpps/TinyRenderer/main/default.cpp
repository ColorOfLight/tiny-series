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
  model::Model model("../assets/shark.obj");

  Image<RgbaColor> diffuse_texture = ReadPng("../assets/shark.png");

  Image<RgbaColor> normal_map = ReadPng("../assets/shark_nm.png");

  int width = 800;
  int height = 800;

  geometry::Vec<3, float> light_direction = {0.0f, 0.0f, -2.0f};
  geometry::Vec<3, float> camera_position = {1.0f, 1.0f, 1.0f};

  RenderModelResult result =
      RenderModel(model, diffuse_texture, normal_map, width, height,
                  light_direction, camera_position);

  WritePng("../output.png", result.frame);
  WritePng("../z_buffer.png", result.z_buffer);
  WritePng("../shadow_map.png", result.shadow_map_buffer);
  WritePng("../ao.png", result.ao_buffer);
}