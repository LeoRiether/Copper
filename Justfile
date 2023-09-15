make:
    premake5 gmake2
    bear -- make

run *args:
    just format
    premake5 gmake2
    bear -- make
    ./penguin_debug {{args}}

release *args:
    just format
    premake5 gmake2
    bear -- make config=release
    ./penguin_release {{args}}

grind:
    just make
    valgrind ./penguin_debug

format:
    find ./ -iname *.h -o -iname *.cpp | xargs clang-format -i

