#include "plugin.hpp"
#include "rack.hpp"

struct TFTPNotes : Module
{
  std::string text;
  bool dirty = false;
  NVGcolor bgColor = nvgRGBA(0.0, 0.0, 0.0, 0.0);
  NVGcolor color = nvgRGB(0x00, 0xff, 0x00);

  void onReset() override
  {
    text = "";
    dirty = true;
  }

  void fromJson(json_t* rootJ) override
  {
    Module::fromJson(rootJ);
    // In <1.0, module used "text" property at root level.
    json_t* textJ = json_object_get(rootJ, "text");
    if (textJ)
      text = json_string_value(textJ);
    dirty = true;
  }

  json_t* dataToJson() override
  {
    json_t* rootJ = json_object();
    json_object_set_new(rootJ, "text", json_stringn(text.c_str(), text.size()));
    return rootJ;
  }

  void dataFromJson(json_t* rootJ) override
  {
    json_t* textJ = json_object_get(rootJ, "text");
    if (textJ)
      text = json_string_value(textJ);
    dirty = true;
  }
};

struct TFTPNotesTextField : LedDisplayTextField
{
  TFTPNotes* module;
  NVGcolor bgColor = nvgRGBA(0.0, 0.0, 0.0, 0.0);
  NVGcolor color = nvgRGB(0x00, 0xff, 0x00);

  void step() override
  {
    LedDisplayTextField::step();
    if (module && module->dirty) {
      setText(module->text);
      module->dirty = false;
    }
  }

  void onChange(const ChangeEvent& e) override
  {
    if (module)
      module->text = getText();
  }
};

struct TFTPNotesDisplay : LedDisplay
{
  NVGcolor bgColor = nvgRGBA(0.0, 0.0, 0.0, 0.0);
  NVGcolor color = nvgRGB(0x00, 0xff, 0x00);
  void setModule(TFTPNotes* module)
  {
    TFTPNotesTextField* textField = createWidget<TFTPNotesTextField>(Vec(1.2, 0));
    textField->box.size = box.size;
    textField->multiline = true;
    textField->module = module;
    addChild(textField);
  }
};

struct TFTPNotesWidget : ModuleWidget
{
  TFTPNotesDisplay* notesDisplay;

  TFTPNotesWidget(TFTPNotes* module)
  {
    setModule(module);
    setPanel(createPanel(asset::plugin(pluginInstance, "res/TFTPNotes.svg")));

    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(
      Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewSilver>(
      Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    notesDisplay = createWidget<TFTPNotesDisplay>(mm2px(Vec(4.8, 10.5)));
    notesDisplay->box.size = mm2px(Vec(92, 111.5));
    notesDisplay->setModule(module);
    addChild(notesDisplay);
  }

  void step() override
  {
    TFTPNotes* wModule = dynamic_cast<TFTPNotes*>(module);
    if (wModule->dirty) {
      notesDisplay->color = wModule->color;
      notesDisplay->bgColor = wModule->bgColor;
      wModule->dirty = false;
    }
    app::ModuleWidget::step();
  }
};

Model* modelTFTPNotes = createModel<TFTPNotes, TFTPNotesWidget>("TFTPNotes");
