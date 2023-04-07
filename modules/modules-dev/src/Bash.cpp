#include "Bash.hpp"
#include <plog/Log.h>
#include <array>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "sexpresso.hpp"
#include "str.hpp"
#include "terminal.hpp"

namespace tftp {

namespace bash {
std::string version = "0.0.0";
std::string prompt = "ready> ";  // Prompt adapted from RSTS/E, an OS that ran on PDP-11/70s
std::string welcome =
    "Forgotten Tones VCV Rack system shell\n\n"
    ".d8888b.888..888.d8888b...%%%%...%%..%%..%%%%%%..%%......%%.....\n"
    "88K.....888..88888K......%%......%%..%%..%%......%%......%%.....\n"
    "\"Y8888b.888..888\"Y8888b...%%%%...%%%%%%..%%%%....%%......%%.....\n"
    ".....X88Y88b.888.....X88.....%%..%%..%%..%%......%%......%%.....\n"
    ".88888P'.\"Y88888 88888P'..%%%%...%%..%%..%%%%%%..%%%%%%..%%%%%%.\n"
    ".............888................................................\n"
    "        Y8b d88P\n"
    "         \"Y88P\"  v" +
    version + "\n";

std::string banner() { return welcome; }
std::string entry(std::string &cmd) {
  std::string e = (cmd.substr(prompt.length(), cmd.length() - prompt.length()));
  tftp::str::rtrim(e);
  return e;
}

std::string exec(const char *cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
    PLOG_ERROR << "popen() failed!";
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

void Shell::read() {}

void Shell::eval() {}

void Shell::print() {}

void Shell::loop() {}
}  // namespace bash
}  // namespace tftp

using namespace tftp;

std::string Bash::reset() {
  PLOG_INFO << "Calling reset ...";
  // TODO: change "text" to "screen" ... and add one for "history"
  dirty = true;
  return tftp::bash::banner();
}

std::string Bash::reset_with_prompt() { return Bash::reset() + "\n" + bash::prompt; }

void Bash::onReset() { text = this->reset_with_prompt(); }

void Bash::fromJson(json_t *rootJ) {
  PLOG_INFO << "loading data from JSON ...";
  Module::fromJson(rootJ);
  json_t *textJ = json_object_get(rootJ, "text");
  if (textJ) text = json_string_value(textJ);
  if (text.size() == 0) {
    text = this->reset_with_prompt();
  }
  dirty = true;
}

json_t *Bash::dataToJson() {
  PLOG_VERBOSE << "storing data as JSON ...";
  if (text.size() == 0) {
    text = this->reset_with_prompt();
  }
  json_t *rootJ = json_object();
  json_object_set_new(rootJ, "text", json_stringn(text.c_str(), text.size()));
  return rootJ;
}

void Bash::dataFromJson(json_t *rootJ) {
  PLOG_INFO << "extracting data from JSON ...";
  json_t *textJ = json_object_get(rootJ, "text");
  if (textJ) text = json_string_value(textJ);
  dirty = true;
}

void BashTextField::step() {
  LedDisplayTextField::step();
  if (module && module->dirty) {
    PLOG_INFO << "Performing text update in 'step' ...";
    setText(module->text);
    module->dirty = false;
  }
}

void BashTextField::onChange(const ChangeEvent &e) {
  if (module) {
    std::string newText = getText() + "\n";
    PLOG_VERBOSE.printf("New text: %s", newText.c_str());
    std::vector<std::string> tokens = tftp::str::tokenize(newText, '\n');
    PLOG_DEBUG << "Logging tokens (lines) ...";
    for (auto line : tokens) PLOG_VERBOSE.printf("\tline: %s", line.c_str());

    std::string last = tftp::str::lastLine(newText);
    PLOG_DEBUG.printf("Got text change event (last): %s", last.c_str());
    std::string cmd = tftp::str::penultimateLine(newText);
    PLOG_DEBUG.printf("Got text change event (cmd): %s", cmd.c_str());
    if (str::startsWith(last, bash::prompt)) {
      std::string entry = bash::entry(last);
      PLOG_DEBUG.printf("Got entry: %s", entry.c_str());
      if (entry == "clear") {
        newText = "";
      } else if (entry == "reset") {
        newText = module->reset();
      } else {
        newText += bash::exec(entry.c_str());
      }
      module->dirty = true;
    }
    newText += "\n" + tftp::bash::prompt;
    module->text = newText;
  }
}

void BashDisplay::draw(const DrawArgs &args) {}

void BashDisplay::setModule(Bash *module) {
  Vec trPos = Vec(1.2, 0);
  BashTextField *textField = createWidget<BashTextField>(trPos);
  textField->box.size = box.size;
  textField->multiline = true;
  textField->module = module;
  textField->color = nvgRGB(0, 255, 0);
  addChild(textField);
}

struct BashWidget : TFTPModuleWidget {
  BashWidget(Bash *module) {
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
