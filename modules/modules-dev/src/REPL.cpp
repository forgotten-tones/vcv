#include "REPL.hpp"
#include <plog/Log.h>
#include <cstdint>
#include <ostream>
#include <string>
#include <vector>
#include "sexpresso.hpp"
#include "str.hpp"

namespace tftp {

namespace repl {
std::string version = "0.0.0";
std::string prompt = "ft-lisp> ";
std::string welcome = "Welcome to the Forgotten Tones VCV Rack REPL";

std::string banner() { return welcome + ", v" + version + "\n\n" + prompt; }
std::string entry(std::string &cmd) {
  return cmd.substr(prompt.length(), cmd.length() - prompt.length());
}
}  // namespace repl
}  // namespace tftp

using namespace tftp;

std::string REPL::reset() {
  PLOG_INFO << "Calling reset ...";
  // TODO: change "text" to "screen" ... and add one for "history"
  dirty = true;
  return tftp::repl::banner();
}

void REPL::onReset() { text = this->reset(); }

void REPL::fromJson(json_t *rootJ) {
  PLOG_INFO << "loading data from JSON ...";
  Module::fromJson(rootJ);
  json_t *textJ = json_object_get(rootJ, "text");
  if (textJ) text = json_string_value(textJ);
  dirty = true;
}

json_t *REPL::dataToJson() {
  PLOG_INFO << "storing data as JSON ...";
  json_t *rootJ = json_object();
  json_object_set_new(rootJ, "text", json_stringn(text.c_str(), text.size()));
  return rootJ;
}

void REPL::dataFromJson(json_t *rootJ) {
  PLOG_INFO << "extracting data from JSON ...";
  json_t *textJ = json_object_get(rootJ, "text");
  if (textJ) text = json_string_value(textJ);
  dirty = true;
}

void REPLTextField::step() {
  LedDisplayTextField::step();
  if (module && module->dirty) {
    PLOG_INFO << "Performing text update in 'step' ...";
    setText(module->text);
    module->dirty = false;
  }
}

void REPLTextField::onChange(const ChangeEvent &e) {
  bool changed = false;
  if (module) {
    std::string newText = getText() + "\n";
    PLOG_DEBUG.printf("New text: %s", newText.c_str());
    std::vector<std::string> tokens = tftp::str::tokenize(newText, '\n');
    PLOG_DEBUG << "Logging tokens (lines) ...";
    for (auto line : tokens) PLOG_VERBOSE.printf("\tline: %s", line.c_str());

    std::string last = tftp::str::lastLine(newText);
    PLOG_DEBUG.printf("Got text change event (last): %s", last.c_str());
    std::string cmd = tftp::str::penultimateLine(newText);
    PLOG_DEBUG.printf("Got text change event (cmd): %s", cmd.c_str());
    if (str::startsWith(last, repl::prompt)) {
      std::string entry = repl::entry(last);
      PLOG_DEBUG.printf("Got entry: %s", entry.c_str());
      newText += "You typed: " + entry + "\n" + repl::prompt;
      if (!entry.empty()) {
        auto sexp = sexpresso::parse(entry);
        std::string op = sexp.getChild(0).toString();
        PLOG_DEBUG.printf("Parsed operation: %s", op.c_str());
        // XXX temporary:
        if (op == "clear") {
          PLOG_DEBUG << "Clearing terminal ...";
          newText = module->reset();
          changed = true;
        }
      }
      module->dirty = true;
    }

    module->text = newText;
  }
}

void REPLDisplay::draw(const DrawArgs &args) {}

void REPLDisplay::setModule(REPL *module) {
  Vec trPos = Vec(1.2, 0);
  REPLTextField *textField = createWidget<REPLTextField>(trPos);
  textField->box.size = box.size;
  textField->multiline = true;
  textField->module = module;
  textField->color = nvgRGB(0, 255, 0);
  addChild(textField);
}

struct REPLWidget : TFTPModuleWidget {
  REPLWidget(REPL *module) {
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
