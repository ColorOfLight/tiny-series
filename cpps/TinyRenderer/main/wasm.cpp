#include <emscripten/bind.h>

#include "./geometry/vec.h"
#include "file.h"
#include "image.h"
#include "model.h"
#include "render.h"

void render(emscripten::val light_position_val,
            emscripten::val camera_position_val, int width, int height) {
  model::Model model("model.obj");
  Image<RgbaColor> diffuse_texture = ReadPng("diffuse.png");
  Image<RgbaColor> normal_map = ReadPng("normal.png");

  std::vector<float> light_position_vector =
      emscripten::vecFromJSArray<float>(light_position_val);
  std::vector<float> camera_position_vector =
      emscripten::vecFromJSArray<float>(camera_position_val);

  geometry::Vec<3, float> light_position = {light_position_vector[0],
                                            light_position_vector[1],
                                            light_position_vector[2]};
  geometry::Vec<3, float> camera_position = {camera_position_vector[0],
                                             camera_position_vector[1],
                                             camera_position_vector[2]};

  RenderModelResult result =
      RenderModel(model, diffuse_texture, normal_map, width, height,
                  light_position, camera_position);

  WritePng("output.png", result.frame);
  WritePng("z_buffer.png", result.z_buffer);
  WritePng("shadow_map.png", result.shadow_map_buffer);
  WritePng("ao.png", result.ao_buffer);
}

EMSCRIPTEN_BINDINGS(module) { emscripten::function("render", &render); }
