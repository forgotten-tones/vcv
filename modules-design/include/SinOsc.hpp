#pragma once

#include "plugin.hpp"

extern Model* modelSinOsc;

namespace tftp {

struct SinOsc : TFTPModule {
  // A param is a knob, switch, button, slider, etc.; red #ff0000
  enum class ParamId { pitch, len };
  // An input; green #00ff00
  enum class InputId { pitch, len };
  // An output; blue #0000ff
  enum class OutputId { sin, len };
  // A light; magenta #ff00ff
  enum class LightId { blink, len };
  // There is also support for custom components; yellow #ffff00

  float phase = 0.f;
  float blinkPhase = 0.f;

  SinOsc() {
    config(static_cast<int>(ParamId::len), static_cast<int>(InputId::len),
           static_cast<int>(OutputId::len), static_cast<int>(LightId::len));
    configParam(static_cast<int>(ParamId::pitch), 0.f, 1.f, 0.f, "");
    configInput(static_cast<int>(InputId::pitch), "");
    configOutput(static_cast<int>(OutputId::sin), "");
  }

  void blink(const ProcessArgs& args, float hz);
  float freq();
  float pitch();
  void process(const ProcessArgs& args) override;
  void update_phase(const ProcessArgs& args);
  void update_output(const ProcessArgs& args);
};

}  // namespace tftp
