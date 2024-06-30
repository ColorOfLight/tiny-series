#include <math.h>
#include "tgaimage.h"

using namespace std;

void draw_line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
  int x_diff = abs(x1 - x0);
  int y_diff = abs(y1 - y0);

  int larger_diff = max(x_diff, y_diff);

  for (int i = 0; i <= larger_diff; ++i)
  {
    float t = larger_diff == 0 ? 0.0f : float(i) / float(larger_diff);
    int x = x0 + (x1 - x0) * t;
    int y = y0 + (y1 - y0) * t;
    image.set(x, y, color);
  }
}