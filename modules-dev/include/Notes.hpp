#pragma once

#include "forgottentones.hpp"

extern Model* modelNotes;

namespace tftp {

struct Notes : TFTPModule {
  // A param is a knob, switch, button, slider, etc.; red #ff0000
  enum class ParamId { pitch, len };
  // An input; green #00ff00
  enum class InputId { pitch, len };
  // An output; blue #0000ff
  enum class OutputId { sin, len };
  // A light; magenta #ff00ff
  enum class LightId { blink, len };
  // There is also support for custom components; yellow #ffff00

  std::string text;
  bool dirty = false;
  NVGcolor bgColor = nvgRGBA(0.0, 0.0, 0.0, 0.0);
  NVGcolor color = nvgRGB(0x00, 0xff, 0x00);

  Notes() {
    config(static_cast<int>(ParamId::len), static_cast<int>(InputId::len),
           static_cast<int>(OutputId::len), static_cast<int>(LightId::len));
    configParam(static_cast<int>(ParamId::pitch), 0.f, 1.f, 0.f, "");
    configInput(static_cast<int>(InputId::pitch), "");
    configOutput(static_cast<int>(OutputId::sin), "");
  }

  void onReset() override;
  void process(const ProcessArgs& args) override;
  void fromJson(json_t* rootJ) override;
  rootJ json_t* dataToJson() override;
  void dataFromJson(json_t* rootJ) override;
};

}  // namespace tftp
