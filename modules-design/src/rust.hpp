#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

enum InputId {
  PitchInput,
  InputsLen,
};

enum LightId {
  BlinkLight,
  LightsLen,
};

enum OutputId {
  SineOutput,
  OutputsLen,
};

enum ParamId {
  PitchParam,
  ParamsLen,
};

extern "C" {

void root(ParamId p, InputId i, OutputId o, LightId l);

double tftp_freq(double current, double increment);

double tftp_brightness(double phase);

double tftp_sin(double phase);

} // extern "C"
