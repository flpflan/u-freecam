ANDROID_NDK ?= $ANDROID_NDK
ANDROID_API_LEVEL ?= 24
CMAKE_BACKEND ?= Ninja

BUILDDIR = build

define build
	@mkdir -p $(BUILDDIR)/$(3)
	@cmake \
		-G $(CMAKE_BACKEND) \
		-S . \
		-B $(BUILDDIR)/$(3) \
		-D CMAKE_BUILD_TYPE=$(2) \
		-D CMAKE_SYSTEM_NAME=Android \
		-D CMAKE_SYSTEM_VERSION=33 \
		-D CMAKE_ANDROID_ARCH_ABI=$(1) \
		-D CMAKE_ANDROID_NDK=$(ANDROID_NDK)
	@cmake --build $(BUILDDIR)/$(3)
endef

debug-all: debug-arm64 debug-x86_64

debug-arm64:
	$(call build,"arm64-v8a",Debug,debug)

debug-x86_64:
	$(call build,"x86_64",Debug,debug-x86_64)

release-all: release-arm64 release-x86_64

release-arm64:
	$(call build,"arm64-v8a",Release,release-arm64)

release-x86_64:
	$(call build,"x86_64",Release,release-x86_64)

clean:
	@echo "Cleaning up"
	@rm -r $(BUILDDIR)
