# UCM Makefile for C++ projects
# Giovanni Gonzalez Araujo

WORKDIR = $(shell pwd)

build_config = '{\n\
"version": "0.2.0", \n\
"configurations": [\n\
{\n\
"type": "lldb",\n\
"request": "launch",\n\
"name": "Debug Current",\n\
"program": "$(WORKDIR)/$(1)",\n\
"args": [],\n\
"cwd": "$(WORKDIR)"\n\
}\n\
]\n\
}'

CONFIG_FOLDER = $(WORKDIR)/.theia
CONFIG_FILE = $(WORKDIR)/.theia/launch.json

all:
	g++ main.cpp -g -I. -std=c++11 -o app
	@rm -rf $(CONFIG_FOLDER)
	@mkdir -p $(CONFIG_FOLDER)
	@echo $(call build_config,app) > $(CONFIG_FILE)

.PHONY: test
test:
	@rm -rf $(CONFIG_FOLDER)
	@mkdir -p $(CONFIG_FOLDER)
	@echo $(call build_config,test) > $(CONFIG_FILE)
	g++ -g -I. test.cpp -o test
	clear && echo "Running all tests:" && ./test --output=color
	

pull:
ifeq ($(shell git rev-parse --is-inside-work-tree 2>/dev/null),true)
	@git reset --hard
	@git clean -fdx
	@git pull --ff-only
else
	@echo "No git repository found in the current directory"
endif

clean:
	rm -f app
	rm -f test
