#!/bin/bash
here="`dirname \"$0\"`"
cd "$here"
cd ..
./vendor/bin/premake/premake5_mac gmake2 --hot-reload
