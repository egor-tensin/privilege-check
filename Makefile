include prelude.mk

TOOLSET ?= mingw
PLATFORM ?= auto
CONFIGURATION ?= Debug
CMAKE_FLAGS ?=

this_dir  := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
src_dir   := $(this_dir)
build_dir := $(this_dir)build
cmake_dir := $(build_dir)/cmake
DESTDIR   ?= $(build_dir)/install

$(eval $(call noexpand,TOOLSET))
$(eval $(call noexpand,PLATFORM))
$(eval $(call noexpand,CONFIGURATION))
$(eval $(call noexpand,CMAKE_FLAGS))
$(eval $(call noexpand,DESTDIR))

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
		--install '$(call escape,$(DESTDIR))' \
		-- \
		'$(call escape,$(src_dir))' \
		'$(call escape,$(cmake_dir))' \
		$(CMAKE_FLAGS)

.PHONY: install
install: build
