#!/bin/sh

SCRIPT="build.sh"
BUILD_DIR="build"

if [ -d "$BUILD_DIR" ]; then
  echo "[$SCRIPT]: '$BUILD_DIR' already exists."
else
  echo "[$SCRIPT]: Creating '$BUILD_DIR' directory."
  mkdir $BUILD_DIR
fi

cd $BUILD_DIR

echo "[$SCRIPT]: Running 'cmake'."
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1

COPY_FILE="compile_commands.json"
echo "[$SCRIPT]: Copying the '$COPY_FILE'."
mv $COPY_FILE ../$COPY_FILE

echo "[$SCRIPT]: Running 'make'."
make

echo "[$SCRIPT]: Done."

