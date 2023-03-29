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

struct NotesTextField : LedDisplayTextField {
  Notes *module;
  NVGcolor bgColor = nvgRGBA(0.0, 0.0, 0.0, 0.0);
  NVGcolor color = nvgRGB(0x00, 0xff, 0x00);

  void step() override {
    LedDisplayTextField::step();
    if (module && module->dirty) {
      setText(module->text);
      module->dirty = false;
    }
  }

  void onChange(const ChangeEvent &e) override {
    if (module) module->text = getText();
  }
};

struct NotesDisplay : LedDisplay {
  NVGcolor bgColor = nvgRGBA(0.0, 0.0, 0.0, 0.0);
  NVGcolor color = nvgRGB(0x00, 0xff, 0x00);
  void setModule(Notes *module) {
    NotesTextField *textField = createWidget<NotesTextField>(Vec(1.2, 0));
    textField->box.size = box.size;
    textField->multiline = true;
    textField->module = module;
    addChild(textField);
  }
};

struct NotesWidget : TFTPModuleWidget {
  NotesDisplay *notesDisplay;

  NotesWidget(Notes *module) {
    setModule(module);
    setPanel("Notes");
    addScrews();

    notesDisplay = createWidget<NotesDisplay>(mm2px(Vec(4.8, 10.5)));
    notesDisplay->box.size = mm2px(Vec(92, 111.5));
    notesDisplay->setModule(module);
    addChild(notesDisplay);
  }

  void step() override {
    Notes *wModule = dynamic_cast<Notes *>(module);
    if (wModule->dirty) {
      notesDisplay->color = wModule->color;
      notesDisplay->bgColor = wModule->bgColor;
      wModule->dirty = false;
    }
    app::ModuleWidget::step();
  }
};

Model *modelNotes = rack::createModel<Notes, NotesWidget>("Notes");
