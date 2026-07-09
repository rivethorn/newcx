#!/bin/bash
set -e

# === GitHub CMake Project Installer ===
REPO="rivethorn/newcx.git"
EXEC_NAME="newcx"
C_EXEC_NAME="newc"
CPP_EXEC_NAME="newcpp"

# Detect OS and architecture
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    PLATFORM="linux-x86_64"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    PLATFORM="darwin-arm64"
else
    echo "Unsupported OS: $OSTYPE"
    exit 1
fi

VERSION=$(curl -s https://api.github.com/repos/$REPO/releases/latest | grep '"tag_name":' | sed -E 's/.*"v([^"]+)".*/\1/')
URL="https://github.com/${REPO}/releases/download/v${VERSION}/${EXEC_NAME}-v${VERSION}-${PLATFORM}"

echo "Downloading ${EXEC_NAME} v${VERSION} for ${PLATFORM}..."
curl -L -o /tmp/${EXEC_NAME} "$URL"
chmod +x /tmp/${EXEC_NAME}

# Install system-wide (sudo) or locally (~/.local/bin)
if command -v sudo >/dev/null && [ "$EUID" -ne 0 ]; then
    sudo mv /tmp/${EXEC_NAME} /usr/local/bin/${EXEC_NAME}
    echo "${EXEC_NAME} installed to /usr/local/bin"
    sudo ln -s /usr/local/bin/${EXEC_NAME} /usr/local/bin/${C_EXEC_NAME}
    echo "Generated ${C_EXEC_NAME} symlink, you should use this."
    sudo ln -s /usr/local/bin/${EXEC_NAME} /usr/local/bin/${CPP_EXEC_NAME}
    echo "Generated ${CPP_EXEC_NAME} symlink, you should use this."
else
    mkdir -p ~/.local/bin
    mv /tmp/${EXEC_NAME} ~/.local/bin/${EXEC_NAME}
    echo "${EXEC_NAME} installed to ~/.local/bin"
    echo 'Add ~/.local/bin to your PATH if needed: export PATH="$HOME/.local/bin:$PATH"'
    ln -s ~/.local/bin/${EXEC_NAME} ~/.local/bin/${C_EXEC_NAME}
    echo "Generated ${C_EXEC_NAME} symlink, you should use this."
    ln -s ~/.local/bin/${EXEC_NAME} ~/.local/bin/${CPP_EXEC_NAME}
    echo "Generated ${CPP_EXEC_NAME} symlink, you should use this."
fi

echo "Installation complete! Run with: ${C_EXEC_NAME} or ${CPP_EXEC_NAME}"
