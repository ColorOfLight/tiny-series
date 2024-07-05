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

#include "./model.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace model {
struct VertexIndex {
  int position_index;
  int texture_coords_index;
  int normal_index;
};

Model::Model(const std::string& file_name)
    : positions_(std::vector<geometry_new::Vec<3, float>>()),
      normals_(std::vector<geometry_new::Vec<3, float>>()),
      texture_coords_(std::vector<geometry_new::Vec<2, float>>()),
      faces_(std::vector<std::vector<model::Vertex>>()) {
  std::ifstream file(file_name);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + file_name);
  }

  std::vector<std::vector<VertexIndex>> face_vertex_indices;

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    if (prefix == "v") {
      geometry_new::Vec<3, float> position;
      iss >> position[0] >> position[1] >> position[2];
      positions_.push_back(position);
    } else if (prefix == "vn") {
      geometry_new::Vec<3, float> normal;
      iss >> normal[0] >> normal[1] >> normal[2];
      normals_.push_back(normal);
    } else if (prefix == "vt") {
      geometry_new::Vec<2, float> texture_coords;
      iss >> texture_coords[0] >> texture_coords[1];
      texture_coords_.push_back(texture_coords);
    } else if (prefix == "f") {
      std::vector<VertexIndex> vertexIndexes;
      std::string vertex_str;
      while (iss >> vertex_str) {
        std::istringstream vertex_iss(vertex_str);
        std::string vertex;
        std::getline(vertex_iss, vertex, '/');
        int position_index = std::stoi(vertex);
        std::getline(vertex_iss, vertex, '/');
        int texture_coords_index = std::stoi(vertex);
        std::getline(vertex_iss, vertex, '/');
        int normal_index = std::stoi(vertex);

        VertexIndex vertexIndex = {position_index, texture_coords_index,
                                   normal_index};
        vertexIndexes.push_back(vertexIndex);
      }

      face_vertex_indices.push_back(vertexIndexes);
    }
  }

  for (const std::vector<VertexIndex>& face_vertex_index :
       face_vertex_indices) {
    std::vector<Vertex> vertices;
    for (const VertexIndex& vertex_index : face_vertex_index) {
      if (vertex_index.position_index > positions_.size()) {
        throw std::runtime_error("Invalid position index: " +
                                 std::to_string(vertex_index.position_index));
      }

      if (vertex_index.normal_index > normals_.size()) {
        throw std::runtime_error("Invalid normal index: " +
                                 std::to_string(vertex_index.normal_index));
      }

      if (vertex_index.texture_coords_index > texture_coords_.size()) {
        throw std::runtime_error(
            "Invalid texture coords index: " +
            std::to_string(vertex_index.texture_coords_index));
      }

      const geometry_new::Vec<3, float>& position =
          positions_[vertex_index.position_index - 1];
      const geometry_new::Vec<3, float>& normal =
          normals_[vertex_index.normal_index - 1];
      const geometry_new::Vec<2, float>& texture_coords =
          texture_coords_[vertex_index.texture_coords_index - 1];
      vertices.push_back(Vertex{position, normal, texture_coords});
    }
    faces_.push_back(vertices);
  }
}

Model::~Model() {}

const std::vector<model::Vertex>& Model::get(int index) const {
  return faces_[index];
}

std::ostream& operator<<(std::ostream& os, const model::Vertex& vertex) {
  os << "Vertex(" << vertex.position << ", " << vertex.normal << ", "
     << vertex.texture_coords << ")";
  return os;
}
}  // namespace model
