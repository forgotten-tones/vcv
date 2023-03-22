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

} // extern "C"
