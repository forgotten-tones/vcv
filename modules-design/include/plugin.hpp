#pragma once

#include "rack.hpp"

// TODO: once we've got a module.hpp for general custom setup, we'll
// include that header file which will have a namespace, and we can
// drop the declaration here:
namespace tftp {};

using namespace rack;
using namespace tftp;

// Declare the Plugin, defined in plugin.cpp
extern Plugin* pluginInstance;
