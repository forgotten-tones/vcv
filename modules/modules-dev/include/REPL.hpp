#pragma once

#include "forgottentones.hpp"

extern Model *modelREPL;

namespace tftp {
namespace repl {
extern std::string version;
extern std::string prompt;
extern std::string welcome;

std::string banner();
std::string entry(std::string &str);
}  // namespace repl

struct REPL : TFTPModule {
  // std::vector<std::string> tokens;
  std::string text;
  bool dirty = false;

  void dataFromJson(json_t *rootJ) override;
  json_t *dataToJson() override;
  void fromJson(json_t *rootJ) override;
  void onReset() override;
  std::string reset();
};

struct REPLTextField : LedDisplayTextField {
  REPL *module;

  void step() override;
  void onChange(const ChangeEvent &e) override;
};

struct REPLDisplay : LedDisplay {
  void setModule(REPL *module);
  void draw(const DrawArgs &args);
};

}  // namespace tftp
