import os


def parse_args():
    from argparse import ArgumentParser

    parser = ArgumentParser(prog="Image Cropper")
    parser.add_argument(
        "classpath",
        help="Name or path of the class, supports stuff like 'physics/Engine'",
    )

    return parser.parse_args()


def main():
    args = parse_args()
    classpath = args.classpath
    classname = classpath.split("/")[-1]

    if os.path.exists(f"include/{classpath}.h"):
        print(f"\033[91merror!\033[0m file include/{classpath}.h already exists") # ]]
        return
    if os.path.exists(f"src/{classpath}.cpp"):
        print(f"\033[91merror!\033[0m file src/{classpath}.cpp already exists") # ]]
        return

    if len(classpath.split('/')) > 1:
        os.makedirs(os.path.join(*classpath.split('/')[:-1]), exist_ok=True)

    includes = ""
    inherit = ""

    if classpath.startswith("component/"):
        includes += '#include "Component.h"'
        inherit = ": public Component "

    with open(f"./include/{classpath}.h", "w") as fh:
        fh.write(
            f"""#pragma once

{includes}

class {classname} {inherit}{{
   public:

   private:

}};

"""
        )

    with open(f"./src/{classpath}.cpp", "w") as fcpp:
        fcpp.write(
            f"""#include "{classpath}.h"

#define MODULE "{classname}"

"""
        )

    print("\033[92mok!\033[0m")


if __name__ == "__main__":
    main()
