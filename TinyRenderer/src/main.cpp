#include <filesystem>
#include <iostream>
#include <string>

#include "line.h"
#include "model.h"
#include "tgaimage.h"

const TGAColor red(255, 0, 0, 255);
const TGAColor white(255, 255, 255, 255);

const int width = 400;
const int height = 400;

int main() {
  TGAImage image(width, height, TGAImage::RGB);

  std::string model_path;
  std::cout << "Enter the model path: ";
  std::cin >> model_path;

  Model *model = new Model(model_path.c_str());

  for (int i = 0; i < model->nfaces(); i++) {
    std::vector<int> face = model->face(i);
    for (int j = 0; j < 3; j++) {
      Vec3f v0 = model->vert(face[j]);
      Vec3f v1 = model->vert(face[(j + 1) % 3]);
      int x0 = (v0.x + 1.) * width / 2.;
      int y0 = (v0.y + 1.) * height / 2.;
      int x1 = (v1.x + 1.) * width / 2.;
      int y1 = (v1.y + 1.) * height / 2.;
      draw_line(x0, y0, x1, y1, image, white);
    }
  }

  image.write_tga_file("output.tga");

  delete model;

  return 0;
}
