#!/usr/bin/env bash
set -euo pipefail

# === GitHub CMake Project Installer ===
REPO="rivethorn/newcx"
EXEC_NAME="newcx"
C_EXEC_NAME="newc"
CPP_EXEC_NAME="newcpp"

# Detect OS and architecture
OS="$(uname -s)"
ARCH="$(uname -m)"

case "${OS}:${ARCH}" in
Linux:x86_64)
    PLATFORM="linux-x86_64"
    ;;
Darwin:arm64)
    PLATFORM="darwin-arm64"
    ;;
Darwin:x86_64)
    PLATFORM="darwin-x86_64"
    ;;
*)
    echo "Unsupported platform: ${OS} ${ARCH}" >&2
    exit 1
    ;;
esac

if ! command -v curl >/dev/null; then
    echo "curl is required to install ${EXEC_NAME}" >&2
    exit 1
fi

TAG="$(
    curl -fsSL "https://api.github.com/repos/${REPO}/releases/latest" |
        sed -nE 's/.*"tag_name":[[:space:]]*"([^"]+)".*/\1/p' |
        head -n 1
)"

if [[ -z "${TAG}" ]]; then
    echo "Unable to determine latest ${EXEC_NAME} release tag" >&2
    exit 1
fi

VERSION="${TAG#v}"
ASSET="${EXEC_NAME}-v${VERSION}-${PLATFORM}"
URL="https://github.com/${REPO}/releases/download/${TAG}/${ASSET}"
TMP_FILE="$(mktemp "/tmp/${EXEC_NAME}.XXXXXX")"
trap 'rm -f "${TMP_FILE}"' EXIT

echo "Downloading ${EXEC_NAME} v${VERSION} for ${PLATFORM}..."
if ! curl -fL --retry 3 -o "${TMP_FILE}" "${URL}"; then
    echo "Failed to download release asset: ${URL}" >&2
    echo "Check that this platform has a published ${ASSET} asset." >&2
    exit 1
fi

chmod +x "${TMP_FILE}"

install_system() {
    sudo mkdir -p /usr/local/bin
    sudo mv "${TMP_FILE}" "/usr/local/bin/${EXEC_NAME}"
    echo "${EXEC_NAME} installed to /usr/local/bin"
    sudo ln -sfn "/usr/local/bin/${EXEC_NAME}" "/usr/local/bin/${C_EXEC_NAME}"
    echo "Generated ${C_EXEC_NAME} symlink, you should use this."
    sudo ln -sfn "/usr/local/bin/${EXEC_NAME}" "/usr/local/bin/${CPP_EXEC_NAME}"
    echo "Generated ${CPP_EXEC_NAME} symlink, you should use this."
}

install_local() {
    mkdir -p "${HOME}/.local/bin"
    mv "${TMP_FILE}" "${HOME}/.local/bin/${EXEC_NAME}"
    echo "${EXEC_NAME} installed to ${HOME}/.local/bin"
    if [[ ":${PATH}:" != *":${HOME}/.local/bin:"* ]]; then
        echo 'Add ~/.local/bin to your PATH if needed: export PATH="$HOME/.local/bin:$PATH"'
    fi
    ln -sfn "${HOME}/.local/bin/${EXEC_NAME}" "${HOME}/.local/bin/${C_EXEC_NAME}"
    echo "Generated ${C_EXEC_NAME} symlink, you should use this."
    ln -sfn "${HOME}/.local/bin/${EXEC_NAME}" "${HOME}/.local/bin/${CPP_EXEC_NAME}"
    echo "Generated ${CPP_EXEC_NAME} symlink, you should use this."
}

if [[ -r /dev/tty && -w /dev/tty ]]; then
    printf "Install to /usr/local/bin? This may require sudo. [y/N] " >/dev/tty
    read -r INSTALL_SYSTEM </dev/tty
else
    INSTALL_SYSTEM="n"
fi

if [[ "${INSTALL_SYSTEM}" =~ ^([Yy]|[Yy][Ee][Ss])$ ]]; then
    if [[ "$EUID" -eq 0 ]]; then
        mkdir -p /usr/local/bin
        mv "${TMP_FILE}" "/usr/local/bin/${EXEC_NAME}"
        echo "${EXEC_NAME} installed to /usr/local/bin"
        ln -sfn "/usr/local/bin/${EXEC_NAME}" "/usr/local/bin/${C_EXEC_NAME}"
        echo "Generated ${C_EXEC_NAME} symlink, you should use this."
        ln -sfn "/usr/local/bin/${EXEC_NAME}" "/usr/local/bin/${CPP_EXEC_NAME}"
        echo "Generated ${CPP_EXEC_NAME} symlink, you should use this."
    elif command -v sudo >/dev/null; then
        install_system
    else
        echo "sudo is unavailable; falling back to local install." >&2
        install_local
    fi
else
    install_local
fi

echo "Installation complete! Run with: ${C_EXEC_NAME} or ${CPP_EXEC_NAME}"
