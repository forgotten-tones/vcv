#pragma once

#include "forgottentones.hpp"

extern Model *modelBash;

namespace tftp
{
  namespace bash
  {
    extern std::string version;
    extern std::string prompt;
    extern std::string welcome;
    extern std::string bin;

    std::string banner();
    std::string entry(std::string &str);

    struct Shell
    {
      void read();
      void eval();
      void print();
      void loop();
    };

  } // namespace repl

  struct Bash : TFTPModule
  {
    // std::vector<std::string> tokens;
    std::string text;
    bool dirty = false;

    void dataFromJson(json_t *rootJ) override;
    json_t *dataToJson() override;
    void fromJson(json_t *rootJ) override;
    void onReset() override;
    std::string reset();
  };

  struct BashTextField : LedDisplayTextField
  {
    Bash *module;

    void step() override;
    void onChange(const ChangeEvent &e) override;
  };

  struct BashDisplay : LedDisplay
  {
    void setModule(Bash *module);
    void draw(const DrawArgs &args);
  };

} // namespace tftp
