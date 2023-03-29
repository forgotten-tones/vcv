# If RACK_DIR is not defined when calling the Makefile, default to two
# directories above
RACK_DIR ?= ../..

# FLAGS will be passed to both the C and C++ compiler
FLAGS += -I./include
CFLAGS +=
CXXFLAGS +=

# Careful about linking to shared libraries, since you can't assume much about
# the user's environment and library search path. Static libraries are fine, but
# they should be added to this plugin's build system.
LDFLAGS +=

# Add .cpp files to the build
SOURCES += $(wildcard src/*.cpp)

# Add files to the ZIP package when running `make dist`
# The compiled plugin and "plugin.json" are automatically added.
DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)
DISTRIBUTABLES += $(wildcard presets)

RACK_APP = ~/Applications/Audio/VCV\ Rack\ 2\ Pro
RACK_INSTALL = ~/Documents/Rack2
PLUGINS = $(RACK_INSTALL)/plugins
TFPT = $(PLUGINS)/ForgottenTones

JSON_MODS = "$(shell jq -c .modules json/plugin.json | tr -s ' ')"

ifdef DESIGN
FLAGS += -DDESIGN=1
FLAGS += -Imodules-design/include
SOURCES += $(wildcard modules-design/src/*.cpp)
JSON_MODS += "$(shell jq -c . json/modules-design.json | tr -s ' ')"
endif

ifdef DEV
FLAGS += -DDEV=1
FLAGS += -Imodules-dev/include
SOURCES += $(wildcard modules-dev/src/*.cpp)
JSON_MODS += "$(shell jq -c . json/modules-dev.json | tr -s ' ')"
endif

# Include the Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk

$(RACK_INSTALL):
	@mkdir -p $(RACK_INSTALL)

json-modules:
	@echo "$(JSON_MODS)"

native-rebuild: clean all dist install

rebuild:
	@arch -x86_64 make native-rebuild DESIGN=$(DESIGN) DEV=$(DEV)

design:
	@$(MAKE) rebuild DESIGN=1

design-open: design open

dev:
	@$(MAKE) rebuild DEV=1

release: rebuild

all-plugins:
	@$(MAKE) rebuild DEV=1 DESIGN=1

all-plugins-open: all-plugins open

rebuild-log: rebuild
	@tail -f $(RACK_INSTALL)/log.txt

uninstall:
	@rm -rf $(TFPT)

log:
	@grep Forgotten $(RACK_INSTALL)/log.txt

show-plugins:
	ls -al $(PLUGINS)

show-installed:
	ls -al $(TFPT)*

open:
	open -a $(RACK_APP)

rebuild-open: rebuild open
