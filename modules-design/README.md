# VCV Rack Module Development

[![][screenshot-thumb]][screenshot]

This is the example module documented in [this tutorial](https://vcvrack.com/manual/PluginDevelopmentTutorial) (with a few minor changes).

For plugin development on systems an Apple M1 chip, I've found that all I need to do to compile the plugin such that it will be loaded by my VCV Rack installation is start a bash session with this:

```shell
arch -x86_64 /bin/bash
```

I've also added a convenience target for cleaning, building, and installing:

```shell
make rebuild
```

Note that all of the steps for setting up a VCV Rack development environment need to be followed before using this code, just as described in the tutorial.

## Additional Deps (for IDEs)

In order for VS Code to load all the files, I had to install the following:

* `brew install jansson glew`

Download the following sources:

* nanovg & nanosvg
* oui-blendish
* simde (simd-everywhere)
* pffft (marton78)
* speexdsp (xiph)

Then updated my C++ prefs with the following include path additions:

```
"/opt/homebrew/include/",
"/opt/vcvrack/Rack-SDK/include/",
"/opt/nanovg/src/",
"/opt/nanosvg/src/",
"/opt/homebrew/Cellar/glew/2.2.0_1/include",
"/opt/oui-blendish/",
"/opt/simde/",
"/opt/pffft",
"/opt/speexdsp/include"
```

[//]: ---Named-Links---

[screenshot-thumb]: https://raw.githubusercontent.com/oubiwann/vcvrack-tutorial/main/res/images/screenshot.jpg
[screenshot]: https://raw.githubusercontent.com/oubiwann/vcvrack-tutorial/main/res/images/screenshot.jpg
