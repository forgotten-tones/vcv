#include "SinOsc.hpp"

using namespace tftp;

void SinOsc::blink(const ProcessArgs& args, float hz) {
  blinkPhase += (args.sampleTime * hz);
  if (blinkPhase >= hz) blinkPhase -= hz;
  lights[static_cast<int>(SinOsc::LightId::blink)].setBrightness(blinkPhase < 0.5f ? 1.f : 0.f);
}

float SinOsc::freq() {
  // The default pitch is C4 = 261.6256f
  return dsp::FREQ_C4 * std::pow(2.f, pitch());
}

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
  // Audio signals are typically +/-5V
  // https://vcvrack.com/manual/VoltageStandards
  outputs[static_cast<int>(SinOsc::OutputId::sin)].setVoltage(5.f * sin);
}

void SinOsc::update_phase(const ProcessArgs& args) {
  phase += freq() * args.sampleTime;
  if (phase >= 0.5f) phase -= 1.f;
}

struct SinOscWidget : ModuleWidget {
  SinOscWidget(SinOsc* module) {
    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/SinOsc.svg")));

    // TODO: Move this into createScrews func
    Vec tlScrewPos = Vec(RACK_GRID_WIDTH, 0);
    Vec trScrewPos = Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0);
    Vec blScrewPos = Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH);
    Vec brScrewPos = Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH);

    addChild(createWidget<ScrewSilver>(tlScrewPos));
    addChild(createWidget<ScrewSilver>(trScrewPos));
    addChild(createWidget<ScrewSilver>(blScrewPos));
    addChild(createWidget<ScrewSilver>(brScrewPos));

    Vec pitchKnobPos = mm2px(Vec(15.24, 48.957));
    Vec voctInPos = mm2px(Vec(15.24, 75.824));
    Vec voctOutPos = mm2px(Vec(15.24, 102.949));
    Vec litePos = mm2px(Vec(15.24, 24.713));

    addParam(createParamCentered<RoundBlackKnob>(pitchKnobPos, module,
                                                 static_cast<int>(SinOsc::ParamId::pitch)));
    addInput(createInputCentered<PJ301MPort>(voctInPos, module,
                                             static_cast<int>(SinOsc::InputId::pitch)));
    addOutput(createOutputCentered<PJ301MPort>(voctOutPos, module,
                                               static_cast<int>(SinOsc::OutputId::sin)));
    addChild(createLightCentered<MediumLight<RedLight>>(litePos, module,
                                                        static_cast<int>(SinOsc::LightId::blink)));
  }
};

Model* modelSinOsc = createModel<SinOsc, SinOscWidget>("SinOsc");
