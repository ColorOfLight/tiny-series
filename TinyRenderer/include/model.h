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
#include <vector>

#include "./geometry/vec.h"

namespace model {

struct Vertex {
  const geometry::Vec<3, float>& position;
  const geometry::Vec<3, float>& normal;
  const geometry::Vec<2, float>& texture_coords;
};

std::ostream& operator<<(std::ostream& os, const model::Vertex& vertex);

class Model {
 public:
  explicit Model(const std::string& file_name);
  ~Model();

  inline int size() const { return faces_.size(); }
  const std::vector<Vertex>& get(int index) const;

 private:
  std::vector<std::vector<Vertex>> faces_;
  std::vector<geometry::Vec<3, float>> positions_;
  std::vector<geometry::Vec<3, float>> normals_;
  std::vector<geometry::Vec<2, float>> texture_coords_;
};
}  // namespace model
