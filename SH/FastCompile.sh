#!/bin/sh

# -- If a special path is needed to ecompile set it here
# -- Path is considered to be run from the root if started by starthere.sh
ECOMPILE_PATH="./scripts/ecompile"
# -----------
ECOMPILE_CONF_PATH="./scripts/ecompile-linux.cfg"

$ECOMPILE_PATH -b -r -C "${ECOMPILE_CONF_PATH}" 2>&1 > log/ecompile-fast.log

exit 0
