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

#pragma once

#include <string>

#include "./image.h"

struct TGAHeader {
  std::uint8_t id_length = 0;
  std::uint8_t color_map_type = 0;
  std::uint8_t data_type_code = 0;
  std::uint16_t color_map_origin = 0;
  std::uint16_t color_map_length = 0;
  std::uint8_t color_map_depth = 0;
  std::uint16_t x_origin = 0;
  std::uint16_t y_origin = 0;
  std::uint16_t width = 0;
  std::uint16_t height = 0;
  std::uint8_t bits_per_pixel = 0;
  std::uint8_t image_descriptor = 0;
};

Image<RgbaColor> ReadTga(const std::string &file_path);

void WriteTga(const std::string &file_path, const Image<RgbaColor> &image);

void WriteTga(const std::string &file_path, const Image<GrayscaleColor> &image);