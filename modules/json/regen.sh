#!/bin/bash

export RELEASED_MODULES=$(jq .modules json/metadata.json)
export DESIGN_MODULES=$(cat json/modules-design.json)
export DEV_MODULES=$(cat json/modules-dev.json)
export ALL_MODULES=$(echo "$RELEASED_MODULES $DESIGN_MODULES $DEV_MODULES" | jq -c -s add)

if [ ! -z "$DEV" ] && [ ! -z "$DESIGN" ]; then
    echo $(jq --argjson mods "$ALL_MODULES" '.modules = $mods' json/metadata.json) | jq . > plugin.json
elif [ ! -z "$DEV" ]; then
    echo $(jq --argjson mods "$DEV_MODULES" '.modules = $mods' json/metadata.json) | jq . > plugin.json
elif [ ! -z "$DESIGN" ]; then
    echo $(jq --argjson mods "$DESIGN_MODULES" '.modules = $mods' json/metadata.json) | jq . > plugin.json
fi
