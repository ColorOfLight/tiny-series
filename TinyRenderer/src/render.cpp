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

#include <random>
#include <vector>

#include "./drawing.h"

void RenderWireframe(const Model& model, const TGAColor& color,
                     TGAImage& image) {
  int width = image.width();
  int height = image.height();

  for (int i = 0; i < model.nfaces(); i++) {
    std::vector<int> face = model.GetConstFace(i);
    for (int j = 0; j < 3; j++) {
      geometry::Vec3f v0 = model.GetConstVert(face[j]);
      geometry::Vec3f v1 = model.GetConstVert(face[(j + 1) % 3]);
      int x0 = (v0.x + 1.) * width / 2.;
      int y0 = (v0.y + 1.) * height / 2.;
      int x1 = (v1.x + 1.) * width / 2.;
      int y1 = (v1.y + 1.) * height / 2.;
      DrawLine(x0, y0, x1, y1, image, color);
    }
  }
}

void RenderRandomColors(const Model& model, TGAImage& image) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(0, 255);

  int width = image.width();
  int height = image.height();

  for (int i = 0; i < model.nfaces(); i++) {
    std::vector<int> face = model.GetConstFace(i);
    geometry::Vec3f v0 = model.GetConstVert(face[0]);
    geometry::Vec3f v1 = model.GetConstVert(face[1]);
    geometry::Vec3f v2 = model.GetConstVert(face[2]);

    int x0 = static_cast<int>((v0.x + 1.) * width / 2.);
    int y0 = static_cast<int>((v0.y + 1.) * height / 2.);
    int x1 = static_cast<int>((v1.x + 1.) * width / 2.);
    int y1 = static_cast<int>((v1.y + 1.) * height / 2.);
    int x2 = static_cast<int>((v2.x + 1.) * width / 2.);
    int y2 = static_cast<int>((v2.y + 1.) * height / 2.);

    TGAColor color(dis(gen), dis(gen), dis(gen), 255);

    DrawTriangle(x0, y0, x1, y1, x2, y2, image, color);
  }
}

void RenderFlatShading(const Model& model, const geometry::Vec3f& light_dir,
                       const TGAColor& base_color, TGAImage& image) {
  int width = image.width();
  int height = image.height();

  // height x width z-buffer
  std::vector<std::vector<float>> z_buffer(
      height,
      std::vector<float>(width, -std::numeric_limits<float>::infinity()));

  for (int i = 0; i < model.nfaces(); i++) {
    std::vector<int> face = model.GetConstFace(i);
    geometry::Vec3f v0 = model.GetConstVert(face[0]);
    geometry::Vec3f v1 = model.GetConstVert(face[1]);
    geometry::Vec3f v2 = model.GetConstVert(face[2]);

    geometry::Vec3f normal = (v1 - v0) ^ (v2 - v0);

    float intensity =
        (-1.) * (normal.normalize() * geometry::Vec3f(light_dir).normalize());

    if (intensity <= 0) {
      // Skip back faces
      continue;
    }

    geometry::Vec3f v0_screen = geometry::Vec3f(
        (v0.x + 1.) * width / 2., (v0.y + 1.) * height / 2., v0.z);
    geometry::Vec3f v1_screen = geometry::Vec3f(
        (v1.x + 1.) * width / 2., (v1.y + 1.) * height / 2., v0.z);
    geometry::Vec3f v2_screen = geometry::Vec3f(
        (v2.x + 1.) * width / 2., (v2.y + 1.) * height / 2., v0.z);

    DrawTriangle(v0_screen, v1_screen, v2_screen, base_color * intensity, image,
                 z_buffer);
  }
}
