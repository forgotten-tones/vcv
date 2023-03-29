#include "module.hpp"
#include "plugin.hpp"

using namespace tftp;

void TFTPModuleWidget::addScrews() {
  Vec tlScrewPos = Vec(RACK_GRID_WIDTH, 0);
  Vec trScrewPos = Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0);
  Vec blScrewPos = Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH);
  Vec brScrewPos = Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH);

  addChild(createWidget<ScrewSilver>(tlScrewPos));
  addChild(createWidget<ScrewSilver>(trScrewPos));
  addChild(createWidget<ScrewSilver>(blScrewPos));
  addChild(createWidget<ScrewSilver>(brScrewPos));
}

void TFTPModuleWidget::setPanel(std::string name) {
  std::string svg = "res/" + name;
  svg += ".svg";
  ModuleWidget::setPanel(createPanel(asset::plugin(pluginInstance, svg)));
}
