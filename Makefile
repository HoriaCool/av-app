# paths #
CLIENT_SRC_PATH   = src/client
SERVER_SRC_PATH   = src/server
CLIENT_BUILD_PATH = build/client
SERVER_BUILD_PATH = build/server

# executables #
CLIENT_BIN_NAME = client.exe
SERVER_BIN_NAME = server.exe

# command line arguments #
# Easy to modify from shell
# E.g. make run ARGS="arg1 arg2 arg3"
ARGS = # set default

# make rule #
MAKE = make --no-print-directory -f GNUMakefile


.PHONY: default_target
default_target: release

.PHONY: release
release:
	@$(MAKE) SRC_PATH="$(CLIENT_SRC_PATH)" \
		 BUILD_PATH="$(CLIENT_BUILD_PATH)" \
		 BIN_NAME="$(CLIENT_BIN_NAME)"
	@echo ""
	@$(MAKE) SRC_PATH="$(SERVER_SRC_PATH)" \
		 BUILD_PATH="$(SERVER_BUILD_PATH)" \
		 BIN_NAME="$(SERVER_BIN_NAME)"

.PHONY: clean
clean:
	@$(MAKE) clean BIN_NAME="$(CLIENT_BIN_NAME)"
	@echo ""
	@$(MAKE) clean BIN_NAME="$(SERVER_BIN_NAME)"

.PHONY: run_client
run_client:
	@$(MAKE) run BIN_NAME="$(CLIENT_BIN_NAME)" ARGS="$(ARGS)"

.PHONY: run_client_vg
run_client_vg:
	@$(MAKE) run_vg BIN_NAME="$(CLIENT_BIN_NAME)" ARGS="$(ARGS)"

.PHONY: run_server
run_server:
	@$(MAKE) run BIN_NAME="$(SERVER_BIN_NAME)" ARGS="$(ARGS)"

.PHONY: run_server_vg
run_server_vg:
	@$(MAKE) run_vg BIN_NAME="$(SERVER_BIN_NAME)" ARGS="$(ARGS)"