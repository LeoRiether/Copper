import os
import shutil  # wtf kind of name is that?
from PIL import Image
from colorama import Fore
from collections import namedtuple

############################
#        Rule Types        #
############################
Resize = namedtuple("Resize", ["path", "target_width"])
Trim = namedtuple("Trim", ["path"])

RULES = [
    Trim("img/Barris.png"),
    Resize("img/Barris.png", 212),
    Trim("img/Escavadeira.png"),
    Resize("img/Escavadeira.png", 372),
]


def backup(path):
    bakpath = path + ".bak"
    if os.path.exists(bakpath):
        # turn path back to original
        shutil.copyfile(bakpath, path)
    else:
        # make backup
        shutil.copyfile(path, bakpath)


def resize(path, target_width):
    print(
        f"{Fore.CYAN}Resizing{Fore.RESET} {path} to "
        f"{Fore.CYAN}{target_width}{Fore.RESET} pixels wide"
    )

    with Image.open(path) as image:
        width, height = image.size
        ratio = target_width / width
        new_size = (target_width, int(height * ratio))
        image.resize(new_size, Image.Resampling.LANCZOS).save(path)


def trim(path):
    print(f"{Fore.YELLOW}Trimming{Fore.RESET} {path}")
    with Image.open(path) as image:
        bounding_box = image.getbbox()
        image.crop(bounding_box).save(path)


def main():
    paths = {os.path.join("assets/", rule.path) for rule in RULES}
    for path in paths:
        backup(path)

    for rule in RULES:
        path = os.path.join("assets/", rule.path)

        if isinstance(rule, Resize):
            resize(path, rule.target_width)
        elif isinstance(rule, Trim):
            trim(path)
        else:
            print(f"Rule not registered: {rule}")


if __name__ == "__main__":
    main()
