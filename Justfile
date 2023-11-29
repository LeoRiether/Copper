bear:
    make clean
    premake5 gmake2
    bear -- make

run *args:
    just format
    premake5 gmake2
    make
    ./copper_debug {{args}}

release *args:
    just format
    premake5 gmake2
    make config=release
    ./copper_release {{args}}

grind:
    premake5 gmake2
    make
    valgrind ./copper_debug

format:
    find src -iname *.h -o -iname *.cpp | xargs clang-format -i
    find include -iname *.h -o -iname *.cpp | xargs clang-format -i

publish-docs:
    mkdocs gh-deploy

publish-linux:
    rm -rf bin/
    rm -rf copper-linux
    premake5 gmake2
    make config=release

    mkdir -p copper-linux
    mv copper_release copper-linux/copper
    cp -r assets/ copper-linux
    cp consts.ini copper-linux
    7z a copper-linux.zip copper-linux

publish-win:
    rm -r bin/
    rm -rf copper-windows
    premake5 --file=./premake5-windows.lua gmake2 
    python3 ./scripts/lib64.py
    make config=release

    mkdir -p copper-windows
    mv copper_release.exe copper-windows/copper.exe
    cp mingw-sdl/SDL2/bin/SDL2.dll copper-windows
    cp mingw-sdl/SDL2/bin/sdl2-config copper-windows
    cp mingw-sdl/image/bin/SDL2_image.dll copper-windows
    cp mingw-sdl/mixer/bin/SDL2_mixer.dll copper-windows
    cp mingw-sdl/ttf/bin/SDL2_ttf.dll copper-windows
    cp mingw-sdl/libwinpthread-1.dll copper-windows
    cp -r assets/ copper-windows/
    cp consts.ini copper-windows/
    7z a copper-windows.zip copper-windows

publish: publish-linux publish-win

