make:
    premake5 gmake2
    bear -- make

run *args:
    premake5 gmake2
    bear -- make
    ./penguin_debug {{args}}

release *args:
    premake5 gmake2
    bear -- make config=release
    ./penguin_release {{args}}

grind:
    just make
    valgrind ./penguin_debug

format:
    find ./ -iname *.h -o -iname *.cpp | xargs clang-format -i


