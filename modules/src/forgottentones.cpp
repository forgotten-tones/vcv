#include "forgottentones.hpp"
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Log.h>
#include <tftp_logger.hpp>
#ifdef DESIGN
#include "SinOsc.hpp"
#endif
#ifdef DEV
#include "Bash.hpp"
#include "Notes.hpp"
#include "REPL.hpp"
#define LOG_LEVEL() plog::debug
#endif
#ifndef DEV
#define LOG_LEVEL() plog::error
#endif

Plugin *tftpPlugins;

void init(rack::Plugin *p) {
  static plog::ConsoleAppender<tftp::log::TxtFormatter> consoleAppender;
  plog::init(LOG_LEVEL(), &consoleAppender);

  PLOG_FATAL << "This is a fatal log message test";
  PLOG_ERROR << "This is an error log message test";
  PLOG_WARNING << "This is a warning log message test";
  PLOG_INFO << "This is an info log message test";
  PLOG_DEBUG << "This is a debug log message test";
  PLOG_VERBOSE << "This is a trace log message test";

  tftpPlugins = p;

  // Add released modules here

  // For modules currently under development:

#ifdef DEV
  p->addModel(modelNotes);
  p->addModel(modelREPL);
  p->addModel(modelBash);
#endif

  // For design modules:

#ifdef DESIGN
  p->addModel(modelSinOsc);
#endif
}
