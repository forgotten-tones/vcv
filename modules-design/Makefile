# If RACK_DIR is not defined when calling the Makefile, default to two
# directories above
RACK_DIR ?= ../..

# FLAGS will be passed to both the C and C++ compiler
FLAGS += -DTEST
CFLAGS += -fPIC
CXXFLAGS +=

# Careful about linking to shared libraries, since you can't assume much about
# the user's environment and library search path. Static libraries are fine, but
# they should be added to this plugin's build system.
LDFLAGS += -L./target/debug

# Add .cpp files to the build
SOURCES += $(wildcard src/*.cpp)

# Add files to the ZIP package when running `make dist`
# The compiled plugin and "plugin.json" are automatically added.
DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)
DISTRIBUTABLES += $(wildcard presets)


RACK_INSTALL = ~/Documents/Rack2
PLUGINS = $(RACK_INSTALL)/plugins
TFPT_DESIGN = $(PLUGINS)/ForgottenTonesDesign
TFPT = $(PLUGINS)/ForgottenTones

# Include the Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk

native-rebuild: clean rust all dist install

rebuild:
	@arch -x86_64 make native-rebuild

rebuild-log: rebuild
	@tail -f $(RACK_INSTALL)/log.txt

uninstall:
	@rm -rf $(TFPT_DESIGN) $(TFPT)

log:
	@grep Forgotten $(RACK_INSTALL)/log.txt

rust-setup:
	@cargo install --force cbindgen

rust-clean:
	@rm -f src/rust.hpp src/libforgotten_tones.a

rust: rust-clean
	@cargo build -vv
	@cargo build -vv --release
	@mkdir -p build/src
	@ranlib target/release/libforgotten_tones.a
	@cp -p target/release/libforgotten_tones.a src
	@cp -p target/release/libforgotten_tones.a ./build/src

check-symbols:
	nm src/libforgotten_tones.a 2> /dev/null |grep tftp

show-plugins:
	ls -al $(PLUGINS)

show-installed:
	ls -al $(TFPT)*
