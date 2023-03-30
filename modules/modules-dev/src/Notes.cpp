#include "Notes.hpp"

using namespace tftp;

void Notes::onReset() {
  text = "";
  dirty = true;
}

void Notes::fromJson(json_t *rootJ) {
  Module::fromJson(rootJ);
  // In <1.0, module used "text" property at root level.
  json_t *textJ = json_object_get(rootJ, "text");
  if (textJ) text = json_string_value(textJ);
  dirty = true;
}

json_t *Notes::dataToJson() {
  json_t *rootJ = json_object();
  json_object_set_new(rootJ, "text", json_stringn(text.c_str(), text.size()));
  return rootJ;
}

void Notes::dataFromJson(json_t *rootJ) {
  json_t *textJ = json_object_get(rootJ, "text");
  if (textJ) text = json_string_value(textJ);
  dirty = true;
}

void NotesTextField::step() {
  LedDisplayTextField::step();
  if (module && module->dirty) {
    setText(module->text);
    module->dirty = false;
  }
}

void NotesTextField::onChange(const ChangeEvent &e) {
  if (module) module->text = getText();
}

void NotesDisplay::setModule(Notes *module) {
  Vec trPos = Vec(1.2, 0);
  NotesTextField *textField = createWidget<NotesTextField>(trPos);
  textField->box.size = box.size;
  textField->multiline = true;
  textField->module = module;
  addChild(textField);
}

struct NotesWidget : TFTPModuleWidget {
  NotesWidget(Notes *module) {
    setModule(module);
    setPanel("modules-dev/res/Notes");
    addScrews();

    NotesDisplay *notesDisplay = createWidget<NotesDisplay>(mm2px(Vec(4.8, 10.5)));
    notesDisplay->box.size = mm2px(Vec(92, 111.5));
    notesDisplay->setModule(module);
    addChild(notesDisplay);
  }
};

Model *modelNotes = rack::createModel<Notes, NotesWidget>("Notes");
