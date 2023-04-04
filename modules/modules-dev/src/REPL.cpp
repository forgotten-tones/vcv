#include <vector>
#include <string>
#include <cstdint>
#include <ostream>

#include "sexpresso.hpp"
#include <plog/Log.h>

#include "REPL.hpp"
#include "str.hpp"

namespace tftp
{

  namespace repl
  {
    std::string version = "0.0.0";
    std::string prompt = "ft-lisp> ";
    std::string welcome = "Welcome to the Forgotten Tones VCV Rack REPL";

    std::string banner() { return welcome + ", v" + version + "\n\n" + prompt; }
    std::string entry(std::string &cmd)
    {
      return cmd.substr(prompt.length(), cmd.length() - prompt.length());
    }
  } // namespace repl
} // namespace tftp

using namespace tftp;

void REPL::reset()
{
  PLOG_INFO << "Calling reset ...";
  // TODO: change to "screen" ... and add one for "history"
  text = tftp::repl::banner();
  dirty = true;
}

void REPL::onReset() { this->reset(); }

void REPL::fromJson(json_t *rootJ)
{
  PLOG_INFO << "loading data from JSON ...";
  Module::fromJson(rootJ);
  json_t *textJ = json_object_get(rootJ, "text");
  if (textJ)
    text = json_string_value(textJ);
  dirty = true;
}

json_t *REPL::dataToJson()
{
  PLOG_INFO << "storing data as JSON ...";
  json_t *rootJ = json_object();
  json_object_set_new(rootJ, "text", json_stringn(text.c_str(), text.size()));
  return rootJ;
}

void REPL::dataFromJson(json_t *rootJ)
{
  PLOG_INFO << "extracting data from JSON ...";
  json_t *textJ = json_object_get(rootJ, "text");
  if (textJ)
    text = json_string_value(textJ);
  dirty = true;
}

void REPLTextField::step()
{
  LedDisplayTextField::step();
  if (module && module->dirty)
  {
    PLOG_INFO << "Performing text update in 'step' ...";
    setText(module->text);
    module->dirty = false;
  }
}

void REPLTextField::onChange(const ChangeEvent &e)
{
  if (module)
  {
    std::string newText = getText() + "\n";
    PLOG_DEBUG << "New text: %s", newText.c_str();
    std::vector<std::string> tokens = tftp::str::tokenize(newText, '\n');
    PLOG_DEBUG << "Logging tokens (lines) ...";
    for (auto line : tokens)
      PLOG_VERBOSE << "\tline: %s", line.c_str();

    std::string last = tftp::str::lastLine(newText);
    PLOG_DEBUG << "Got text change event (last): %s", last.c_str();
    std::string cmd = tftp::str::penultimateLine(newText);
    PLOG_DEBUG << "Got text change event (cmd): %s", cmd.c_str();
    if (str::startsWith(last, repl::prompt))
    {
      std::string entry = repl::entry(last);
      PLOG_DEBUG << "Got entry: %s", entry.c_str();
      newText += "You typed: " + entry + "\n" + repl::prompt;
      auto sexp = sexpresso::parse(entry);
      PLOG_DEBUG << "Parsed operation: %s", sexp.getChild(1).toString().c_str();
      module->dirty = true;
    }

    module->text = newText;
  }
}

void REPLDisplay::draw(const DrawArgs &args) {}

void REPLDisplay::setModule(REPL *module)
{
  Vec trPos = Vec(1.2, 0);
  REPLTextField *textField = createWidget<REPLTextField>(trPos);
  textField->box.size = box.size;
  textField->multiline = true;
  textField->module = module;
  textField->color = nvgRGB(0, 255, 0);
  addChild(textField);
}

struct REPLWidget : TFTPModuleWidget
{
  REPLWidget(REPL *module)
  {
    setModule(module);
    setPanel("modules-dev/res/REPL");
    addScrews();

    REPLDisplay *replDisplay = createWidget<REPLDisplay>(mm2px(Vec(4.8, 10.5)));
    replDisplay->box.size = mm2px(Vec(142, 116.2));
    replDisplay->setModule(module);
    addChild(replDisplay);
    // module->reset();
  }
};

Model *modelREPL = rack::createModel<REPL, REPLWidget>("REPL");
