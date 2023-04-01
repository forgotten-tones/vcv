#include "str.hpp"
#include "REPL.hpp"

namespace tftp
{

  namespace repl
  {
    std::string version = "0.0.0";
    std::string prompt = "ft-lisp> ";
    std::string welcome = "Welcome to the Forgotten Tones VCV Rack REPL";

    std::string banner()
    {
      return welcome + ", v" + version + "\n\n" + prompt;
    }

    std::string entry(std::string &cmd)
    {
      return cmd.substr(prompt.length(), cmd.length() - prompt.length());
    }
  }
}

using namespace tftp;

void REPL::onReset()
{
  DEBUG("Calling reset ...");
  // TODO: change to "screen" ... and add one for "history"
  text = tftp::repl::banner();
  dirty = true;
}

void REPL::fromJson(json_t *rootJ)
{
  DEBUG("loading data from JSON ...");
  Module::fromJson(rootJ);
  json_t *textJ = json_object_get(rootJ, "text");
  if (textJ)
    text = json_string_value(textJ);
  dirty = true;
}

json_t *REPL::dataToJson()
{
  DEBUG("storing data as JSON ...");
  json_t *rootJ = json_object();
  json_object_set_new(rootJ, "text", json_stringn(text.c_str(), text.size()));
  return rootJ;
}

void REPL::dataFromJson(json_t *rootJ)
{
  DEBUG("extracting data from JSON ...");
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
    DEBUG("Performing text update in 'step' ...");
    setText(module->text);
    module->dirty = false;
  }
}

void REPLTextField::onChange(const ChangeEvent &e)
{
  if (module)
  {
    std::string newText = getText() + "\n";
    DEBUG("New text: %s", newText.c_str());
    std::vector<std::string> tokens = tftp::str::tokenize(newText, '\n');
    DEBUG("Logging tokens (lines) ...");
    for (auto line : tokens)
      DEBUG("\tline: %s", line.c_str());

    std::string last = tftp::str::lastLine(newText);
    DEBUG("Got text change event (last): %s", last.c_str());
    std::string cmd = tftp::str::penultimateLine(newText);
    DEBUG("Got text change event (cmd): %s", cmd.c_str());
    if (str::startsWith(last, repl::prompt))
    {
      std::string entry = repl::entry(last);
      DEBUG("Got entry: %s", entry.c_str());
      newText += "You typed: " + entry + "\n" + repl::prompt;
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
    replDisplay->box.size = mm2px(Vec(92, 116.2));
    replDisplay->setModule(module);
    addChild(replDisplay);
  }
};

Model *modelREPL = rack::createModel<REPL, REPLWidget>("REPL");
