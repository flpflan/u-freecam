ANDROID_NDK ?= /opt/android-ndk
RELEASE_ANDROID_API_LEVEL ?= 24
DEBUG_ANDROID_API_LEVEL ?= 33
GENERATOR ?= Ninja

BUILDDIR = build

define build
	@mkdir -p $(BUILDDIR)/$(4)
	@cmake \
		-G $(GENERATOR) \
		-S . \
		-B $(BUILDDIR)/$(4) \
  		-D CMAKE_SYSTEM_NAME=Android \
		-D CMAKE_BUILD_TYPE=$(3) \
		-D CMAKE_ANDROID_NDK=$(ANDROID_NDK) \
		-D CMAKE_SYSTEM_VERSION=$(2) \
		-D CMAKE_ANDROID_ARCH_ABI=$(1)
	@cmake --build $(BUILDDIR)/$(4) --parallel
endef

debug-all: debug-arm64 debug-x86_64

debug-arm64:
	$(call build,"arm64-v8a",$(DEBUG_ANDROID_API_LEVEL),Debug,debug)

debug-x86_64:
	$(call build,"x86_64",$(DEBUG_ANDROID_API_LEVEL),Debug,debug-x86_64)

release-all: release-arm64 release-x86_64

release-arm64:
	$(call build,"arm64-v8a",$(RELEASE_ANDROID_API_LEVEL),Release,release-arm64)

release-x86_64:
	$(call build,"x86_64",$(RELEASE_ANDROID_API_LEVEL),Release,release-x86_64)

clean:
	@echo "Cleaning up"
	@rm -r $(BUILDDIR)
