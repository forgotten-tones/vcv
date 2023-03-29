#include "SinOsc.hpp"

void tftp::SinOsc::process(const ProcessArgs& args) {
  // Compute the frequency from the pitch parameter and input
  float pitch = params[PITCH_PARAM].getValue();
  pitch += inputs[PITCH_INPUT].getVoltage();
  pitch = clamp(pitch, -4.f, 4.f);
  // The default pitch is C4 = 261.6256f
  float freq = dsp::FREQ_C4 * std::pow(2.f, pitch);

  // Accumulate the phase
  phase += freq * args.sampleTime;
  if (phase >= 0.5f) phase -= 1.f;

  // Compute the sine output
  float sine = std::sin(2.f * M_PI * phase);
  // Audio signals are typically +/-5V
  // https://vcvrack.com/manual/VoltageStandards
  outputs[SINE_OUTPUT].setVoltage(5.f * sine);

  // Blink light at 1Hz
  blinkPhase += args.sampleTime;
  if (blinkPhase >= 1.f) blinkPhase -= 1.f;
  lights[BLINK_LIGHT].setBrightness(blinkPhase < 0.5f ? 1.f : 0.f);
};

struct SinOscWidget : ModuleWidget {
  SinOscWidget(tftp::SinOsc* module) {
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

    addParam(createParamCentered<RoundBlackKnob>(pitchKnobPos, module, tftp::SinOsc::PITCH_PARAM));
    addInput(createInputCentered<PJ301MPort>(voctInPos, module, tftp::SinOsc::PITCH_INPUT));
    addOutput(createOutputCentered<PJ301MPort>(voctOutPos, module, tftp::SinOsc::SINE_OUTPUT));
    addChild(
        createLightCentered<MediumLight<RedLight>>(litePos, module, tftp::SinOsc::BLINK_LIGHT));
  }
};

Model* modelSinOsc = createModel<tftp::SinOsc, SinOscWidget>("SinOsc");
