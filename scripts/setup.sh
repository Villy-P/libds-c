#!/usr/bin/env bash
set -euo pipefail

# Use sudo only if not already root (e.g. inside Docker/CI)
SUDO=""
if [ "$(id -u)" -ne 0 ]; then
  SUDO="sudo"
fi

echo "Installing system dependencies..."

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  # build-essential  - C compiler and standard build tools
  # cmake            - build system
  # clang-format     - code formatting, enforced via pre-commit and CI
  # git              - version control
  # ninja-build      - faster alternative build backend for cmake
  # doxygen          - generates API documentation
  # pipx             - isolated installs for Python CLI tools (pre-commit)
  $SUDO apt update
  $SUDO apt install -y build-essential cmake clang-format git ninja-build doxygen pipx
 
elif [[ "$OSTYPE" == "darwin"* ]]; then
  if ! command -v brew &> /dev/null; then
    echo "Homebrew not found. Please install it from https://brew.sh and re-run this script."
    exit 1
  fi
  brew update
  brew install cmake clang-format git ninja doxygen pipx
 
else
  echo "Unsupported OS: $OSTYPE"
  echo "This script supports Linux (apt) and macOS (brew) only."
  exit 1
fi

echo "Setting up pipx..."
pipx ensurepath
# Make pipx-installed tools available immediately in this session,
# since ensurepath only takes effect in new shells
export PATH="$HOME/.local/bin:$PATH"

echo "Installing pre-commit..."
pipx install pre-commit

echo "Running project setup..."
make setup-dev

echo "Setup Complete"