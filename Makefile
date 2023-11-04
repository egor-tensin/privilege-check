include prelude.mk

this_dir    := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
src_dir     := $(this_dir)
build_dir   := $(this_dir)build
cmake_dir   := $(build_dir)/cmake
install_dir := $(build_dir)/install

TOOLSET        ?= mingw
PLATFORM       ?= auto
CONFIGURATION  ?= Debug
CMAKE_FLAGS    ?=
INSTALL_PREFIX ?= $(install_dir)

$(eval $(call noexpand,TOOLSET))
$(eval $(call noexpand,PLATFORM))
$(eval $(call noexpand,CONFIGURATION))
$(eval $(call noexpand,CMAKE_FLAGS))
$(eval $(call noexpand,INSTALL_PREFIX))

.PHONY: all
all: build

.PHONY: clean
clean:
	rm -rf -- '$(call escape,$(build_dir))'

.PHONY: build
build:
	cd cmake && python3 -m project.build \
		--toolset '$(call escape,$(TOOLSET))' \
		--platform '$(call escape,$(PLATFORM))' \
		--configuration '$(call escape,$(CONFIGURATION))' \
		--install '$(call escape,$(INSTALL_PREFIX))' \
		-- \
		'$(call escape,$(src_dir))' \
		'$(call escape,$(cmake_dir))' \
		$(CMAKE_FLAGS)

.PHONY: install
install: build
