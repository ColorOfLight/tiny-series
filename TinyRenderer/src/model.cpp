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
// https://github.com/ssloy/tinyrenderer/blob/f6fecb7ad493264ecd15e230411bfb1cca539a12/model.cpp

#include "./model.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Model::Model(const char *filename) : verts_(), faces_() {
  std::ifstream in;
  in.open(filename, std::ifstream::in);
  if (in.fail()) return;
  std::string line;
  while (!in.eof()) {
    std::getline(in, line);
    std::istringstream iss(line.c_str());
    char trash;
    if (!line.compare(0, 2, "v ")) {
      iss >> trash;
      Vec3f v;
      for (int i = 0; i < 3; i++) iss >> v.raw[i];
      verts_.push_back(v);
    } else if (!line.compare(0, 2, "f ")) {
      std::vector<int> f;
      int itrash, idx;
      iss >> trash;
      while (iss >> idx >> trash >> itrash >> trash >> itrash) {
        idx--;  // in wavefront obj all indices start at 1, not zero
        f.push_back(idx);
      }
      faces_.push_back(f);
    }
  }
  std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;
}

Model::~Model() {}

int Model::nverts() const { return static_cast<int>(verts_.size()); }

int Model::nfaces() const { return static_cast<int>(faces_.size()); }

std::vector<int> Model::face(int idx) { return faces_[idx]; }

std::vector<int> Model::GetConstFace(int idx) const { return faces_[idx]; }

Vec3f Model::vert(int i) { return verts_[i]; }

Vec3f Model::GetConstVert(int i) const { return verts_[i]; }
