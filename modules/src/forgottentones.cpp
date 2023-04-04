#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Appenders/ColorConsoleAppender.h>

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
  static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
  plog::init(plog::verbose, &consoleAppender);

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
