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

void RenderWireframe(const model::Model& model, const TGAColor& color,
                     TGAImage& image) {
  int width = image.width();
  int height = image.height();

  for (int i = 0; i < model.size(); i++) {
    const auto& face = model.get(i);
    for (int j = 0; j < 3; j++) {
      const geometry::Vec3f& v0 = face[j].position;
      const geometry::Vec3f& v1 = face[(j + 1) % 3].position;
      int x0 = (v0.x + 1.) * width / 2.;
      int y0 = (v0.y + 1.) * height / 2.;
      int x1 = (v1.x + 1.) * width / 2.;
      int y1 = (v1.y + 1.) * height / 2.;
      DrawLine(x0, y0, x1, y1, image, color);
    }
  }
}

void RenderRandomColors(const model::Model& model, TGAImage& image) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(0, 255);

  int width = image.width();
  int height = image.height();

  for (int i = 0; i < model.size(); i++) {
    const auto& face = model.get(i);
    const geometry::Vec3f& v0 = face[0].position;
    const geometry::Vec3f& v1 = face[1].position;
    const geometry::Vec3f& v2 = face[2].position;

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

void RenderFlatShading(const model::Model& model,
                       const geometry::Vec3f& light_dir,
                       const TGAColor& base_color, TGAImage& image) {
  int width = image.width();
  int height = image.height();

  // height x width z-buffer
  std::vector<std::vector<float>> z_buffer(
      height,
      std::vector<float>(width, -std::numeric_limits<float>::infinity()));

  for (int i = 0; i < model.size(); i++) {
    auto& face = model.get(i);
    const geometry::Vec3f& p0 = face[0].position;
    const geometry::Vec3f& p1 = face[1].position;
    const geometry::Vec3f& p2 = face[2].position;

    const geometry::Vec3f& n0 = face[0].normal;
    const geometry::Vec3f& n1 = face[1].normal;
    const geometry::Vec3f& n2 = face[2].normal;

    geometry::Vec3f p0_screen = geometry::Vec3f(
        (p0.x + 1.) * width / 2., (p0.y + 1.) * height / 2., p0.z);
    geometry::Vec3f p1_screen = geometry::Vec3f(
        (p1.x + 1.) * width / 2., (p1.y + 1.) * height / 2., p1.z);
    geometry::Vec3f p2_screen = geometry::Vec3f(
        (p2.x + 1.) * width / 2., (p2.y + 1.) * height / 2., p2.z);

    DrawTriangle(p0_screen, p1_screen, p2_screen, n0, n1, n2, base_color,
                 light_dir, image, z_buffer);
  }
}
