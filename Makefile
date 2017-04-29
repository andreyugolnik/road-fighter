VER_MAJOR=1
VER_MINOR=0
VER_RELEASE=1269
BUILD_DIR_RELEASE=.build_release
BUILD_DIR_DEBUG=.build_debug
BUNDLE_NAME=roadfighter

UNAME=$(shell uname -s)
ifeq ($(UNAME), Darwin)
	BUNDLE_NAME=roadfighter.app
endif

all:    release

help:
	@echo "Usage:"
	@echo "    make <release | debug>    - make release or debug application"
	@echo "    make <clean>              - cleanup directory"

release:
	$(shell if [ ! -d $(BUILD_DIR_RELEASE) ]; then mkdir $(BUILD_DIR_RELEASE); fi)
	cd $(BUILD_DIR_RELEASE) ; cmake -DCMAKE_BUILD_TYPE=Release -DAPP_VERSION_MAJOR:STRING=$(VER_MAJOR) -DAPP_VERSION_MINOR:STRING=$(VER_MINOR) -DAPP_VERSION_RELEASE:STRING=$(VER_RELEASE) .. ; make ; cd ..
	cp -r $(BUILD_DIR_RELEASE)/$(BUNDLE_NAME) .

debug:
	$(shell if [ ! -d $(BUILD_DIR_DEBUG) ]; then mkdir $(BUILD_DIR_DEBUG); fi)
	cd $(BUILD_DIR_DEBUG) ; cmake -DCMAKE_BUILD_TYPE=Debug -DAPP_VERSION_MAJOR:STRING=$(VER_MAJOR) -DAPP_VERSION_MINOR:STRING=$(VER_MINOR) -DAPP_VERSION_RELEASE:STRING=$(VER_RELEASE) .. ; make ; cd ..
	cp -r $(BUILD_DIR_DEBUG)/$(BUNDLE_NAME) .

clean:
	rm -fr $(BUILD_DIR_RELEASE) $(BUILD_DIR_DEBUG) $(BUNDLE_NAME)
