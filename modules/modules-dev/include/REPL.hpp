#pragma once

#include "forgottentones.hpp"

extern Model *modelREPL;

namespace tftp
{
  namespace repl
  {
    extern std::string version;
    extern std::string prompt;
    extern std::string welcome;

    std::string banner();
    std::string entry(std::string &str);
  }

  struct REPL : TFTPModule
  {
    // std::vector<std::string> tokens;
    std::string text;
    bool dirty = false;

    void onReset() override;
    void fromJson(json_t *rootJ) override;
    json_t *dataToJson() override;
    void dataFromJson(json_t *rootJ) override;
  };

  struct REPLTextField : LedDisplayTextField
  {
    REPL *module;

    void step() override;
    void onChange(const ChangeEvent &e) override;
  };

  struct REPLDisplay : LedDisplay
  {
    void setModule(REPL *module);
    void draw(const DrawArgs &args);
  };

} // namespace tftp
