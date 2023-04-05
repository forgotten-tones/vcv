#include "Bash.hpp"
#include <plog/Log.h>
#include <cstdint>
#include <ostream>
#include <string>
#include <vector>
#include "sexpresso.hpp"
#include "shell.hpp"
#include "str.hpp"

namespace tftp
{

  namespace bash
  {
    std::string version = "0.0.0";
    std::string welcome = "Welcome to the Forgotten Tones VCV Rack Bash shell";
    std::string prompt = "bash> ";
    std::string bin = "/usr/local/bin/bash";

    std::string banner() { return welcome + ", v" + version + "\n\n" + prompt; }
    std::string entry(std::string &cmd)
    {
      return cmd.substr(prompt.length(), cmd.length() - prompt.length());
    }

    void Shell::read()
    {
    }

    void Shell::eval()
    {
    }

    void Shell::print()
    {
    }

    void Shell::loop()
    {
    }
  } // namespace bash
} // namespace tftp

using namespace tftp;

std::string Bash::reset()
{
  PLOG_INFO << "Calling reset ...";
  // TODO: change "text" to "screen" ... and add one for "history"
  dirty = true;
  return tftp::bash::banner();
}

void Bash::onReset() { text = this->reset(); }

void Bash::fromJson(json_t *rootJ)
{
  PLOG_INFO << "loading data from JSON ...";
  Module::fromJson(rootJ);
  json_t *textJ = json_object_get(rootJ, "text");
  if (textJ)
    text = json_string_value(textJ);
  dirty = true;
}

json_t *Bash::dataToJson()
{
  PLOG_INFO << "storing data as JSON ...";
  json_t *rootJ = json_object();
  json_object_set_new(rootJ, "text", json_stringn(text.c_str(), text.size()));
  return rootJ;
}

void Bash::dataFromJson(json_t *rootJ)
{
  PLOG_INFO << "extracting data from JSON ...";
  json_t *textJ = json_object_get(rootJ, "text");
  if (textJ)
    text = json_string_value(textJ);
  dirty = true;
}

void BashTextField::step()
{
  LedDisplayTextField::step();
  if (module && module->dirty)
  {
    PLOG_INFO << "Performing text update in 'step' ...";
    setText(module->text);
    module->dirty = false;
  }
}

void BashTextField::onChange(const ChangeEvent &e)
{
  if (module)
  {
    std::string newText = getText() + "\n";
    PLOG_DEBUG.printf("New text: %s", newText.c_str());
    std::vector<std::string> tokens = tftp::str::tokenize(newText, '\n');
    PLOG_DEBUG << "Logging tokens (lines) ...";
    for (auto line : tokens)
      PLOG_VERBOSE.printf("\tline: %s", line.c_str());

    std::string last = tftp::str::lastLine(newText);
    PLOG_DEBUG.printf("Got text change event (last): %s", last.c_str());
    std::string cmd = tftp::str::penultimateLine(newText);
    PLOG_DEBUG.printf("Got text change event (cmd): %s", cmd.c_str());
    if (str::startsWith(last, bash::prompt))
    {
      std::string entry = bash::entry(last);
      PLOG_DEBUG.printf("Got entry: %s", entry.c_str());
      newText += "You typed: " + entry + "\n" + bash::prompt;
      if (!entry.empty())
      {
        // auto sexp = sexpresso::parse(entry);
        // std::string op = sexp.getChild(0).toString();
        // PLOG_DEBUG.printf("Parsed operation: %s", op.c_str());
        // XXX temporary:
        // if (op == "clear")
        // {
        //   PLOG_DEBUG << "Clearing terminal ...";
        //   newText = module->reset();
        //   changed = true;
        // }
      }
      module->dirty = true;
    }

    module->text = newText;
  }
}

void BashDisplay::draw(const DrawArgs &args) {}

void BashDisplay::setModule(Bash *module)
{
  Vec trPos = Vec(1.2, 0);
  BashTextField *textField = createWidget<BashTextField>(trPos);
  textField->box.size = box.size;
  textField->multiline = true;
  textField->module = module;
  textField->color = nvgRGB(0, 255, 0);
  addChild(textField);
}

struct BashWidget : TFTPModuleWidget
{
  BashWidget(Bash *module)
  {
    setModule(module);
    setPanel("modules-dev/res/Terminal");
    addScrews();

    BashDisplay *bashDisplay = createWidget<BashDisplay>(mm2px(Vec(4.8, 10.5)));
    bashDisplay->box.size = mm2px(Vec(142, 116.2));
    bashDisplay->setModule(module);
    addChild(bashDisplay);
  }
};

Model *modelBash = rack::createModel<Bash, BashWidget>("Bash");
