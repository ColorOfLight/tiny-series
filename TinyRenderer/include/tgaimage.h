/*
 * Copyright Dmitry V. Sokolov
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software. Permission is granted to anyone to use this software
 * for any purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 * that you wrote the original software.
 * If you use this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Modified by Seongho Park in 2024
 */

// source:
// https://github.com/ssloy/tinyrenderer/blob/master/tgaimage.h

#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#pragma pack(push, 1)
struct TGAHeader {
  std::uint8_t idlength = 0;
  std::uint8_t colormaptype = 0;
  std::uint8_t datatypecode = 0;
  std::uint16_t colormaporigin = 0;
  std::uint16_t colormaplength = 0;
  std::uint8_t colormapdepth = 0;
  std::uint16_t x_origin = 0;
  std::uint16_t y_origin = 0;
  std::uint16_t width = 0;
  std::uint16_t height = 0;
  std::uint8_t bitsperpixel = 0;
  std::uint8_t imagedescriptor = 0;
};
#pragma pack(pop)

struct TGAColor {
  TGAColor() = default;
  TGAColor(int b, int g, int r, int a);
  TGAColor(int b, int g, int r, int a, int bpp);
  TGAColor(const TGAColor &c)
      : bgra{c.bgra[0], c.bgra[1], c.bgra[2], c.bgra[3]} {};
  std::uint8_t bgra[4] = {0, 0, 0, 0};
  std::uint8_t bytespp = 4;
  std::uint8_t &operator[](const int i) { return bgra[i]; }
  TGAColor operator*(const float intensity) const;
};

struct TGAImage {
  enum Format { GRAYSCALE = 1, RGB = 3, RGBA = 4 };

  TGAImage() = default;
  TGAImage(const int w, const int h, const int bpp);
  bool read_tga_file(const std::string filename);
  bool write_tga_file(const std::string filename, const bool vflip = true,
                      const bool rle = true) const;
  void flip_horizontally();
  void flip_vertically();
  TGAColor get(const int x, const int y) const;
  void set(const int x, const int y, const TGAColor &c);
  int width() const;
  int height() const;

 private:
  bool load_rle_data(std::ifstream &in);
  bool unload_rle_data(std::ofstream &out) const;

  int w = 0;
  int h = 0;
  std::uint8_t bpp = 0;
  std::vector<std::uint8_t> data = {};
};
