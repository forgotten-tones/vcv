#pragma once

#include "rack.hpp"

using namespace rack;

namespace tftp {

struct TFTPModule : Module {
  //   void process(const ProcessArgs& args) override;
};

struct TFTPModuleWidget : ModuleWidget {
  void addScrews();
  void setPanel(std::string);
};

}  // namespace tftp
