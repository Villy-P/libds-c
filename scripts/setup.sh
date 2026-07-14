#!/usr/bin/env bash
set -euo pipefail

sudo apt update
sudo apt install -y build-essential cmake git ninja-build doxygen pipx

pipx ensurepath
pipx install pre-commit

echo "Setup Complete"