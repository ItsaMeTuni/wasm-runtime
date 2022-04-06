#!/bin/env bash
cd $(cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

cmake -H. -BDebug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
ln -s Debug/compile_commands.json .
