#pragma once

#include "plugin.hpp"

extern Model* modelSinOsc;

namespace tftp {

struct SinOsc : Module {
  // A param is a knob, switch, button, slider, etc.; red #ff0000
  enum ParamId { PITCH_PARAM, PARAMS_LEN };
  // An input; green #00ff00
  enum InputId { PITCH_INPUT, INPUTS_LEN };
  // An output; blue #0000ff
  enum OutputId { SINE_OUTPUT, OUTPUTS_LEN };
  // A light; magenta #ff00ff
  enum LightId { BLINK_LIGHT, LIGHTS_LEN };
  // There is also support for custom components; yellow #ffff00

  float phase = 0.f;
  float blinkPhase = 0.f;

  SinOsc() {
    config(SinOsc::PARAMS_LEN, SinOsc::INPUTS_LEN, SinOsc::OUTPUTS_LEN, SinOsc::LIGHTS_LEN);
    configParam(SinOsc::PITCH_PARAM, 0.f, 1.f, 0.f, "");
    configInput(SinOsc::PITCH_INPUT, "");
    configOutput(SinOsc::SINE_OUTPUT, "");
  }

  void process(const ProcessArgs& args) override;
};

}  // namespace tftp
