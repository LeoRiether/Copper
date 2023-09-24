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
    find ./ -iname *.h -o -iname *.cpp | xargs clang-format -i

publish-docs:
    mkdocs gh-deploy

