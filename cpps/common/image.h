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

#include <algorithm>
#include <vector>

struct RgbaColor {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  RgbaColor() : r(0), g(0), b(0), a(0) {}

  RgbaColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
      : r(r), g(g), b(b), a(a) {}

  RgbaColor(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), a(255) {}

  RgbaColor& operator+=(const RgbaColor& other) {
    r = std::min(r + other.r, 255);
    g = std::min(g + other.g, 255);
    b = std::min(b + other.b, 255);
    return *this;
  }

  RgbaColor operator+(const RgbaColor& other) const {
    RgbaColor result = *this;
    result += other;
    return result;
  }

  RgbaColor& operator*=(float scalar) {
    r = std::min(static_cast<int>(r * scalar), 255);
    g = std::min(static_cast<int>(g * scalar), 255);
    b = std::min(static_cast<int>(b * scalar), 255);
    return *this;
  }

  RgbaColor operator*(float scalar) const {
    RgbaColor result = *this;
    result *= scalar;
    return result;
  }
};

struct GrayscaleColor {
  uint8_t value;

  GrayscaleColor() : value(0) {}

  explicit GrayscaleColor(uint8_t value) : value(value) {}
};

template <class Color>
class Image {
 public:
  Image() : width_(0), height_(0) {}
  Image(int width, int height)
      : width_(width), height_(height), data_(width * height) {}
  Image(int width, int height, std::vector<Color> data)
      : width_(width), height_(height), data_(data) {}

  int GetWidth() const { return width_; }
  int GetHeight() const { return height_; }
  const std::vector<Color>& GetData() const { return data_; }
  int size() const { return data_.size(); }

  void set(int x, int y, const Color& color) { data_[y * width_ + x] = color; }
  void Clear() {
    for (int i = 0; i != data_.size(); ++i) {
      data_[i] = Color();
    }
  }
  void FlipY() {
    std::vector<Color> temp(width_ * height_);
    for (int y = 0; y < height_; y++) {
      for (int x = 0; x < width_; x++) {
        temp[(height_ - 1 - y) * width_ + x] = data_[y * width_ + x];
      }
    }
    data_ = temp;
  }

  void Fill(const Color& color) {
    for (int i = 0; i != data_.size(); ++i) {
      data_[i] = color;
    }
  }

  Color at(int x, int y) const {
    if (x < 0 || x >= width_) {
      throw std::out_of_range("x: " + std::to_string(x) +
                              " is out of Image x range");
    }

    if (y < 0 || y >= height_) {
      throw std::out_of_range("y: " + std::to_string(y) +
                              " is out of Image y range");
    }

    return data_[y * width_ + x];
  }

 private:
  int width_;
  int height_;
  std::vector<Color> data_;
};
