#include <emscripten/bind.h>

#include "./geometry/vec.h"
#include "file.h"
#include "image.h"
#include "model.h"
#include "render.h"

void render() {
  model::Model model("model.obj");
  Image<RgbaColor> diffuse_texture = ReadPng("diffuse.png");
  Image<RgbaColor> normal_map = ReadPng("normal.png");

  int width = 800;
  int height = 800;

  geometry::Vec<3, float> light_direction = {0.0f, 0.0f, -2.0f};
  geometry::Vec<3, float> camera_position = {1.0f, 1.0f, 1.0f};

  RenderModelResult result =
      RenderModel(model, diffuse_texture, normal_map, width, height,
                  light_direction, camera_position);

  WritePng("output.png", result.frame);
  WritePng("z_buffer.png", result.z_buffer);
  WritePng("shadow_map.png", result.shadow_map_buffer);
  WritePng("ao.png", result.ao_buffer);
}

EMSCRIPTEN_BINDINGS(module) { emscripten::function("render", &render); }
