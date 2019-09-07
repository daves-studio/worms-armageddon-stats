Hook a function call; uses cmake to build, with an external assembly file.

# Building

using ninja, on linux using mingw:

    cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=Toolchain-cross-mingw32-linux.cmake .
    ninja -C build/

built library is `build/wkmain.dll`; copy this to `~/.wine/drive_c/GOG\ Games/Worms\ Armageddon/`

to change between release and debug, add `-DCMAKE_BUILD_TYPE=Release` or `-DCMAKE_BUILD_TYPE=Debug`.

On windows with make, it will probably look more like:

    cmake -B build .
    make -C build/
