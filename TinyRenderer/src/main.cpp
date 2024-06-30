#include "tgaimage.h"
#include "line.h"

const TGAColor red(255, 0, 0, 255);
const TGAColor white(255, 255, 255, 255);

int main()
{
  TGAImage image(100, 100, TGAImage::RGB);

  draw_line(13, 20, 80, 40, image, white);
  draw_line(20, 13, 40, 80, image, red);
  draw_line(80, 40, 13, 20, image, red);

  image.write_tga_file("output.tga");

  return 0;
}
