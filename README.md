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

# Run
$ build\debug\lifegame\lifegame.exe
```

### Release Build
```sh
$ cmake -S . -B build/release -GNinja -DCMAKE_BUILD_TYPE=Release

# Build lifegame
$ cmake --build build/release --config Release -- lifegame

# Run
$ build\release\lifegame\lifegame.exe
```
