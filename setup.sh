#!/bin/bash

if [ "$EUID" -ne 0 ]; then
    echo "This script must be run as root."
    echo "Try: sudo $0"
    exit 1
fi

SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
ROOTKIT_SH="$SCRIPT_DIR/rootkit.sh"

if [ ! -f "$ROOTKIT_SH" ]; then
    echo "Error: rootkit.sh not found in $SCRIPT_DIR."
    exit 1
fi

ln -sf "$ROOTKIT_SH" /usr/bin/rootkit

echo "Symbolic link created: /usr/bin/rootkit -> $ROOTKIT_SH"
echo "You can now use 'rootkit' as a command."