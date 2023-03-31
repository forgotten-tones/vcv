#include "SinOsc.hpp"

using namespace tftp;

void SinOsc::blink(const ProcessArgs& args, float hz) {
  blinkPhase += (args.sampleTime * hz);
  if (blinkPhase >= hz) blinkPhase -= hz;
  lights[static_cast<int>(SinOsc::LightId::blink)].setBrightness(blinkPhase < 0.5f ? 1.f : 0.f);
}

float SinOsc::freq() { return dsp::FREQ_C4 * std::pow(2.f, pitch()); }

float SinOsc::pitch() {
  float pitch = params[static_cast<int>(SinOsc::ParamId::pitch)].getValue();
  pitch += inputs[static_cast<int>(SinOsc::InputId::pitch)].getVoltage();
  return clamp(pitch, -4.f, 4.f);
}

void SinOsc::process(const ProcessArgs& args) {
  update_output(args);
  blink(args, 2.f);
};

void SinOsc::update_output(const ProcessArgs& args) {
  update_phase(args);
  float sin = std::sin(2.f * M_PI * phase);
  outputs[static_cast<int>(SinOsc::OutputId::sin)].setVoltage(5.f * sin);
}

void SinOsc::update_phase(const ProcessArgs& args) {
  phase += freq() * args.sampleTime;
  if (phase >= 0.5f) phase -= 1.f;
}

struct SinOscWidget : TFTPModuleWidget {
  SinOscWidget(SinOsc* module) {
    setModule(module);
    setPanel("modules-design/res/SinOsc");
    addScrews();

    Vec pitchKnobPos = mm2px(Vec(15.24, 48.957));
    Vec voctInPos = mm2px(Vec(15.24, 75.824));
    Vec voctOutPos = mm2px(Vec(15.24, 102.949));
    Vec litePos = mm2px(Vec(15.24, 24.713));

    int knobPitchId = static_cast<int>(SinOsc::ParamId::pitch);
    int cvPitchId = static_cast<int>(SinOsc::InputId::pitch);
    int cvOutId = static_cast<int>(SinOsc::OutputId::sin);
    int liteId = static_cast<int>(SinOsc::LightId::blink);
    addParam(createParamCentered<RoundBlackKnob>(pitchKnobPos, module, knobPitchId));
    addInput(createInputCentered<PJ301MPort>(voctInPos, module, cvPitchId));
    addOutput(createOutputCentered<PJ301MPort>(voctOutPos, module, cvOutId));
    addChild(createLightCentered<MediumLight<RedLight>>(litePos, module, liteId));
  }
};

Model* modelSinOsc = rack::createModel<SinOsc, SinOscWidget>("SinOsc");
