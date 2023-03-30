#include "forgottentones.hpp"
#ifdef DESIGN
#include "SinOsc.hpp"
#endif
#ifdef DEV
#include "Notes.hpp"
#endif

Plugin* tftpPlugins;

void init(rack::Plugin* p) {
  tftpPlugins = p;

  // Add released modules here

  // For modules currently under development:

#ifdef DEV
  p->addModel(modelNotes);
#endif

  // For design modules:

#ifdef DESIGN
  p->addModel(modelSinOsc);
#endif
}