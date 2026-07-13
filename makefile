# Ensures commands run in one shell and prevent issues with file names
.ONESHELL:
.PHONY: setup setup-prod build build-prod run run-build-if clean rebuild production help precommit-install setup-dev clang-format clang-format-check

# Sets up cross-platform support
ifeq ($(OS),Windows_NT)
	EXE := .exe
	CLEAN := rmdir /S /Q
	EXEC_PREFIX :=
else
	EXE :=
	CLEAN := rm -rf
	EXEC_PREFIX := ./
endif

# Sets up basic build variables
BUILD_DIR := build
BIN_DIR := bin
JOBS ?= 4
EXECUTABLE := libds-c-test
CLANG_TIDY := clang-tidy
CPPCHECK   := cppcheck

# Run once when first initializing project
setup:
	cmake -S . -B $(BUILD_DIR) -G Ninja \
	-DCMAKE_C_COMPILER=clang \
	-DCMAKE_CXX_COMPILER=clang++ \
	-DCMAKE_BUILD_TYPE=Debug

# Sets up program to be build for production
setup-prod:
	cmake -S . -B $(BUILD_DIR) -G Ninja -DCMAKE_BUILD_TYPE=Release

# Runs the CMake file
build:
	cmake --build $(BUILD_DIR) --config Debug -- -j$(JOBS)

# Runs the CMake file in production mode
build-prod:
	cmake --build $(BUILD_DIR) --config Release -- -j$(JOBS)

lint:
	cmake --build $(BUILD_DIR) --target clang-tidy

cppcheck:
	cmake --build $(BUILD_DIR) --target cppcheck

analyze: lint cppcheck

build-docs:
	cmake --build $(BUILD_DIR) --target docs

# Runs the program: Use this when you make changed
run: build
	@echo Running main executable...
	cd "$(BUILD_DIR)/$(BIN_DIR)" && ./$(EXECUTABLE)$(EXE) $(args)

# Cleans up the build directory
clean:
	$(CLEAN) $(BUILD_DIR)
	
rebuild: clean setup run
production: clean setup-prod build-prod

precommit-install:
	pre-commit install

setup-dev: setup precommit-install

clang-format:
	cmake --build $(BUILD_DIR) --target clang-format

clang-format-check:
	cmake --build $(BUILD_DIR) --target clang-format-check

help:
	@echo "Available targets:"
	@echo "  setup       - Configure Debug build"
	@echo "  setup-prod  - Configure Release build"
	@echo "  build       - Build Debug build"
	@echo "  build-prod  - Build Release build"
	@echo "  run         - Run Debug main executable"
	@echo "  clean       - Remove build directory"
	@echo "  rebuild     - Clean, setup Debug, build and run"
	@echo "  lint        - Run clang-tidy"
	@echo "  cppcheck    - Run cppcheck"
	@echo "  analyze     - Run both analyzers"
	@echo "  build-docs  - Build documentation using Doxygen"
	@echo "  precommit-install - Install pre-commit hooks"
	@echo "  setup-dev   - Setup development environment (setup + precommit-install)"
	@echo "  clang-format - Format code using clang-format"
	@echo "  clang-format-check - Check code formatting using clang-format"