#pragma once

#include "forgottentones.hpp"

extern Model *modelBash;

namespace tftp {
namespace bash {
extern std::string version;
extern std::string prompt;
extern std::string welcome;

std::string banner();
std::string entry(std::string &str);
std::string exec(const char *cmd);

struct Shell {
  void read();
  void eval();
  void print();
  void loop();
};

}  // namespace bash

struct Bash : TFTPModule {
  // std::vector<std::string> tokens;
  std::string text = "";
  bool dirty = false;

  void dataFromJson(json_t *rootJ) override;
  json_t *dataToJson() override;
  void fromJson(json_t *rootJ) override;
  void onReset() override;
  std::string reset();
  std::string reset_with_prompt();
};

struct BashTextField : LedDisplayTextField {
  Bash *module;

  void step() override;
  void onChange(const ChangeEvent &e) override;
};

struct BashDisplay : LedDisplay {
  void setModule(Bash *module);
  void draw(const DrawArgs &args);
};

}  // namespace tftp
