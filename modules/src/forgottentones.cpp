#include "forgottentones.hpp"
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <tftp_logger.hpp>
#ifdef DESIGN
#include "SinOsc.hpp"
#endif
#ifdef DEV
#include "Notes.hpp"
#include "REPL.hpp"
#endif

Plugin *tftpPlugins;

void init(rack::Plugin *p) {
  static plog::ConsoleAppender<tftp::log::TxtFormatter> consoleAppender;
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
