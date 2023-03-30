#pragma once

#include "forgottentones.hpp"

extern Model* modelNotes;

namespace tftp {

struct Notes : TFTPModule {
  std::string text;
  bool dirty = false;

  void onReset() override;
  void fromJson(json_t* rootJ) override;
  json_t* dataToJson() override;
  void dataFromJson(json_t* rootJ) override;
};

struct NotesTextField : LedDisplayTextField {
  Notes* module;

  void step() override;
  void onChange(const ChangeEvent& e) override;
};

struct NotesDisplay : LedDisplay {
  void setModule(Notes* module);
  void draw(const DrawArgs& args);
};

}  // namespace tftp
