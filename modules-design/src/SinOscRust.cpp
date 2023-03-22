#include "plugin.hpp"
#include "rust.hpp"

struct SinOscRust : Module
{
  float phase = 0.f;
  float blinkPhase = 0.f;

  SinOscRust()
  {
    config(
      ParamId::ParamsLen, InputId::InputsLen, OutputId::OutputsLen, LightId::LightsLen);
    configParam(ParamId::PitchParam, 0.f, 1.f, 0.f, "");
    configInput(InputId::PitchInput, "");
    configOutput(OutputId::SineOutput, "");
  }

  void process(const ProcessArgs& args) override
  {
    // Compute the frequency from the pitch parameter and input
    float pitch = params[ParamId::PitchParam].getValue();
    pitch += inputs[InputId::PitchInput].getVoltage();
    pitch = clamp(pitch, -4.f, 4.f);
    // The default pitch is C4 = 261.6256f
    float freq = dsp::FREQ_C4 * std::pow(2.f, pitch);

    // Accumulate the phase
    phase += freq * args.sampleTime;
    if (phase >= 0.5f)
      phase -= 1.f;

    // Compute the sine output
    float sine = std::sin(2.f * M_PI * phase);
    // Audio signals are typically +/-5V
    // https://vcvrack.com/manual/VoltageStandards
    outputs[OutputId::SineOutput].setVoltage(5.f * sine);

    // Blink light at 1Hz
    blinkPhase += args.sampleTime;
    if (blinkPhase >= 1.f)
      blinkPhase -= 1.f;
    lights[LightId::BlinkLight].setBrightness(blinkPhase < 0.5f ? 1.f : 0.f);
  }
};

struct SinOscRustWidget : ModuleWidget
{
  SinOscRustWidget(SinOscRust* module)
  {
    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/SinOsc.svg")));

    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewSilver>(
      Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    addParam(createParamCentered<RoundBlackKnob>(
      mm2px(Vec(15.24, 48.957)), module, ParamId::PitchParam));

    addInput(createInputCentered<PJ301MPort>(
      mm2px(Vec(15.24, 75.824)), module, InputId::PitchInput));

    addOutput(createOutputCentered<PJ301MPort>(
      mm2px(Vec(15.24, 102.949)), module, OutputId::SineOutput));

    addChild(createLightCentered<MediumLight<RedLight>>(
      mm2px(Vec(15.24, 24.713)), module, OutputId::SineOutput));
  }
};

Model* modelSinOscRust = createModel<SinOscRust, SinOscRustWidget>("SinOscRust");
