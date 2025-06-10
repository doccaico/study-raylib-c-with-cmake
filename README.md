## study-raylib-c-with-cmake (Windows only)

### Build Requirements
- MSVC
- cmake
- ninja

### Debug Build
```sh
$ cmake -S . -B build/debug -GNinja -DCMAKE_BUILD_TYPE=Debug

# Build lifegame
$ cmake --build build/debug --config Debug -- lifegame
# Build flappy
# $ cmake --build build/debug --config Debug -- flappy

# Run lifegame
$ build\debug\lifegame\lifegame.exe
# Run flappy
# $ pushd build\debug\flappy && flappy.exe && popd
```

### Release Build
```sh
$ cmake -S . -B build/release -GNinja -DCMAKE_BUILD_TYPE=Release

# Build lifegame
$ cmake --build build/release --config Release -- lifegame
# Build flappy
# $ cmake --build build/release --config Release -- flappy

# Run
$ build\release\lifegame\lifegame.exe
```
