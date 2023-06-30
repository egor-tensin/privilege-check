include prelude.mk

TOOLSET ?= mingw
CONFIGURATION ?= Debug
CMAKE_FLAGS ?=

this_dir  := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
src_dir   := $(this_dir)
ifdef CI
build_dir := $(this_dir)../build
else
build_dir := $(this_dir).build
endif
cmake_dir := $(build_dir)/cmake
DESTDIR   ?= $(build_dir)/install

$(eval $(call noexpand,TOOLSET))
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
ifdef CI
	cd cmake && python3 -m project.ci.cmake --install -- $(CMAKE_FLAGS)
else
	cd cmake && python3 -m project.cmake.build \
		--toolset '$(call escape,$(TOOLSET))' \
		--configuration '$(call escape,$(CONFIGURATION))' \
		--build '$(call escape,$(cmake_dir))' \
		--install '$(call escape,$(DESTDIR))' \
		-- \
		'$(call escape,$(src_dir))' \
		$(CMAKE_FLAGS)
endif

.PHONY: install
install: build
