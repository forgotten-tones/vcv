#include "forgottentones.hpp"
#ifdef DESIGN
#include "SinOsc.hpp"
#endif
#ifdef DEV
#include "Notes.hpp"
#include "REPL.hpp"
#endif

Plugin *tftpPlugins;

void init(rack::Plugin *p)
{
  tftpPlugins = p;

  // Add released modules here

  // For modules currently under development:

#ifdef DEV
  p->addModel(modelNotes);
  p->addModel(modelREPL);
#endif

  // For design modules:

#ifdef DESIGN
  p->addModel(modelSinOsc);
#endif
}
