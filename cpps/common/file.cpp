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

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "./image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb_image_write.h"

// Helper function to read little-endian uint16_t
std::uint16_t readUint16LE(std::ifstream& file) {
  std::uint8_t low, high;
  file.read(reinterpret_cast<char*>(&low), sizeof(low));
  file.read(reinterpret_cast<char*>(&high), sizeof(high));
  return static_cast<std::uint16_t>(low) |
         (static_cast<std::uint16_t>(high) << 8);
}

// Helper function to write little-endian uint16_t
void writeUint16LE(std::ofstream& file, std::uint16_t value) {
  file.put(value & 0xFF);
  file.put((value >> 8) & 0xFF);
}

RgbaColor ReadRgbaColor(std::ifstream& file, int bpp) {
  RgbaColor color;

  color.b = file.get();
  color.g = file.get();
  color.r = file.get();
  color.a = bpp == 32 ? file.get() : 255;

  if (!file.good()) {
    throw std::runtime_error("An error occurred while reading the data");
  }

  return color;
}

// Helper function to read TGA header
TGAHeader readTGAHeader(std::ifstream& file) {
  TGAHeader header;

  file.read(reinterpret_cast<char*>(&header.id_length),
            sizeof(header.id_length));
  file.read(reinterpret_cast<char*>(&header.color_map_type),
            sizeof(header.color_map_type));
  file.read(reinterpret_cast<char*>(&header.data_type_code),
            sizeof(header.data_type_code));
  header.color_map_origin = readUint16LE(file);
  header.color_map_length = readUint16LE(file);
  file.read(reinterpret_cast<char*>(&header.color_map_depth),
            sizeof(header.color_map_depth));
  header.x_origin = readUint16LE(file);
  header.y_origin = readUint16LE(file);
  header.width = readUint16LE(file);
  header.height = readUint16LE(file);
  file.read(reinterpret_cast<char*>(&header.bits_per_pixel),
            sizeof(header.bits_per_pixel));
  file.read(reinterpret_cast<char*>(&header.image_descriptor),
            sizeof(header.image_descriptor));

  return header;
}

// Helper function to write TGA header
void writeTGAHeader(std::ofstream& file, const TGAHeader& header) {
  file.put(header.id_length);
  file.put(header.color_map_type);
  file.put(header.data_type_code);
  writeUint16LE(file, header.color_map_origin);
  writeUint16LE(file, header.color_map_length);
  file.put(header.color_map_depth);
  writeUint16LE(file, header.x_origin);
  writeUint16LE(file, header.y_origin);
  writeUint16LE(file, header.width);
  writeUint16LE(file, header.height);
  file.put(header.bits_per_pixel);
  file.put(header.image_descriptor);
}

Image<RgbaColor> ReadTga(const std::string& file_path) {
  std::ifstream file(file_path, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + file_path);
  }

  TGAHeader header = readTGAHeader(file);

  if (!file.good()) {
    throw std::runtime_error("Failed to read TGA header");
  }

  int width = header.width;
  int height = header.height;
  int bpp = header.bits_per_pixel;

  if (bpp != 24 && bpp != 32) {
    throw std::runtime_error(
        "Unsupported TGA format (only 24 or 32 bpp are supported)");
  }

  std::vector<RgbaColor> data(width * height);

  if (header.data_type_code == 2 || header.data_type_code == 3) {
    for (int i = 0; i < width * height; ++i) {
      data[i] = ReadRgbaColor(file, bpp);
    }
  } else if (header.data_type_code == 10) {
    // Handle RLE compressed data
    int pixel_count = width * height;
    int current_pixel = 0;
    while (current_pixel < pixel_count) {
      uint8_t chunk_header = file.get();

      if (chunk_header < 128) {
        chunk_header++;
        for (int i = 0; i < chunk_header; ++i) {
          data[current_pixel++] = ReadRgbaColor(file, bpp);
        }
      } else {
        chunk_header -= 127;
        auto color = ReadRgbaColor(file, bpp);

        for (int i = 0; i < chunk_header; ++i) {
          data[current_pixel++] = color;
        }
      }
    }
  } else {
    throw std::runtime_error("Unsupported TGA format");
  }

  Image<RgbaColor> image = Image<RgbaColor>(width, height, data);

  if (header.image_descriptor & 0x20) {
    image.FlipY();
  }

  return image;
}

Image<RgbaColor> ReadJpg(const std::string& file_path) {
  int width, height, channels;
  unsigned char* data =
      stbi_load(file_path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

  if (!data) {
    throw std::runtime_error("Failed to load image: " + file_path);
  }

  std::vector<RgbaColor> image_data(width * height);
  for (int i = 0; i < width * height; ++i) {
    image_data[i] = RgbaColor{data[4 * i + 0], data[4 * i + 1], data[4 * i + 2],
                              data[4 * i + 3]};
  }

  stbi_image_free(data);

  Image<RgbaColor> image = Image<RgbaColor>(width, height, image_data);

  // Make the left bottom point as the origin
  image.FlipY();

  return image;
}

void WriteTga(const std::string& file_path, const Image<RgbaColor>& image) {
  std::cout << "Writing TGA file: " << file_path << std::endl;

  std::ofstream file(file_path, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file for writing: " + file_path);
  }

  TGAHeader header = {};
  header.bits_per_pixel = 32;
  header.width = image.GetWidth();
  header.height = image.GetHeight();
  header.data_type_code = 2;
  header.image_descriptor = 0x00;  // Bottom-left origin

  writeTGAHeader(file, header);

  if (!file.good()) {
    throw std::runtime_error("Failed to write TGA header");
  }

  for (const auto& color : image.GetData()) {
    file.put(color.b);
    file.put(color.g);
    file.put(color.r);
    file.put(color.a);
  }

  if (!file.good()) {
    throw std::runtime_error("Failed to write TGA data");
  }
}

void WriteTga(const std::string& file_path,
              const Image<GrayscaleColor>& image) {
  std::ofstream file(file_path, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file for writing: " + file_path);
  }

  TGAHeader header = {};
  header.bits_per_pixel = 8;
  header.width = image.GetWidth();
  header.height = image.GetHeight();
  header.data_type_code = 3;
  header.image_descriptor = 0x00;  // Bottom-left origin

  writeTGAHeader(file, header);

  if (!file.good()) {
    throw std::runtime_error("Failed to write TGA header");
  }

  for (const auto& color : image.GetData()) {
    file.put(color.value);
  }

  if (!file.good()) {
    throw std::runtime_error("Failed to write TGA data");
  }
}

Image<RgbaColor> ReadPng(const std::string& file_path) {
  int width, height, channels;
  unsigned char* data =
      stbi_load(file_path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

  if (!data) {
    throw std::runtime_error("Failed to load image: " + file_path);
  }

  std::vector<RgbaColor> image_data(width * height);
  for (int i = 0; i < width * height; ++i) {
    image_data[i] = RgbaColor{data[4 * i + 0], data[4 * i + 1], data[4 * i + 2],
                              data[4 * i + 3]};
  }

  stbi_image_free(data);

  Image<RgbaColor> image = Image<RgbaColor>(width, height, image_data);

  // Make the left bottom point as the origin
  image.FlipY();

  return image;
}

void WritePng(const std::string& file_path, const Image<RgbaColor>& image) {
  int width = image.GetWidth();
  int height = image.GetHeight();
  const std::vector<RgbaColor>& data = image.GetData();

  std::vector<unsigned char> image_data(width * height * 4);
  for (int i = 0; i < width * height; ++i) {
    // Flip Y data
    image_data[4 * (height - 1 - i / width) * width + 4 * (i % width) + 0] =
        data[i].r;
    image_data[4 * (height - 1 - i / width) * width + 4 * (i % width) + 1] =
        data[i].g;
    image_data[4 * (height - 1 - i / width) * width + 4 * (i % width) + 2] =
        data[i].b;
    image_data[4 * (height - 1 - i / width) * width + 4 * (i % width) + 3] =
        data[i].a;
  }

  if (!stbi_write_png(file_path.c_str(), width, height, 4, image_data.data(),
                      width * 4)) {
    throw std::runtime_error("Failed to write PNG file: " + file_path);
  }
}

void WritePng(const std::string& file_path,
              const Image<GrayscaleColor>& image) {
  int width = image.GetWidth();
  int height = image.GetHeight();
  const std::vector<GrayscaleColor>& data = image.GetData();

  std::vector<unsigned char> image_data(width * height);
  for (int i = 0; i < width * height; ++i) {
    // Flip Y-data
    image_data[(height - 1 - i / width) * width + i % width] = data[i].value;
  }

  if (!stbi_write_png(file_path.c_str(), width, height, 1, image_data.data(),
                      width)) {
    throw std::runtime_error("Failed to write PNG file: " + file_path);
  }
}
