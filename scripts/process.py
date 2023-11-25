import os
import shutil  # wtf kind of name is that?
from PIL import Image
from colorama import Fore, Style
from collections import namedtuple

############################
#        Rule Types        #
############################
Resize = namedtuple("Resize", ["path", "width", "height"], defaults=(None, None, None))
Trim = namedtuple("Trim", ["path"])
Concat = namedtuple("Concat", ["path", "inputs"])

RULES = [
    Trim("img/Barris.png"),
    Resize("img/Barris.png", width=212),
    Trim("img/Escavadeira.png"),
    Resize("img/Escavadeira.png", width=372),
    Trim("img/Viga_B.png"),
    Resize("img/Viga_B.png", height=360),
    Concat(
        "img/RobotCanCore.png",
        [
            "img/RobotCan/RobotCanWalkCore/spritesheet.png",
            "img/RobotCan/RobotCanFire1Core/spritesheet.png",
            "img/RobotCan/RobotCanFire2Core/spritesheet.png",
            "img/RobotCan/RobotCanHideCore/spritesheet.png",
        ],
    ),
    Concat(
        "img/RobotCanBase.png",
        [
            "img/RobotCan/RobotCanWalkBase/spritesheet.png",
            "img/RobotCan/RobotCanFire1Base/spritesheet.png",
            "img/RobotCan/RobotCanFire2Base/spritesheet.png",
            "img/RobotCan/RobotCanHideBase/spritesheet.png",
        ],
    ),
]


def backup(path):
    bakpath = path + ".bak"
    if os.path.exists(bakpath):
        # turn path back to original
        shutil.copyfile(bakpath, path)
    elif os.path.exists(path):
        # make backup
        shutil.copyfile(path, bakpath)


def resize(path, target_width, target_height):
    print(
        f"{Fore.CYAN}Resizing{Fore.RESET} {path} to "
        f"{Fore.CYAN}{target_width if target_width is not None else target_height}{Fore.RESET} pixels "
        f"{'wide' if target_width is not None else 'tall'}"
    )

    with Image.open(path) as image:
        width, height = image.size
        new_size = None
        if target_width is not None:
            ratio = target_width / width
            new_size = target_width, int(height * ratio)
        else:
            ratio = target_height / height
            new_size = int(width * ratio), target_height
        image.resize(new_size, Image.Resampling.LANCZOS).save(path)


def trim(path):
    print(f"{Fore.YELLOW}Trimming{Fore.RESET} {path}")
    with Image.open(path) as image:
        bounding_box = image.getbbox()
        image.crop(bounding_box).save(path)


def concat(path, inputs):
    print(f"{Fore.MAGENTA}Concatenating{Fore.RESET} {path} from")
    for inp in inputs:
        print(f"  {Style.DIM}{inp}{Style.RESET_ALL}")

    inputs = (os.path.join("assets/", inp) for inp in inputs)
    imgs = [Image.open(inp) for inp in inputs]
    total_width = max(img.width for img in imgs)
    total_height = sum(img.height for img in imgs)
    result = Image.new("RGBA", (total_width, total_height))

    current_height = 0
    for img in imgs:
        result.paste(img, (0, current_height))
        current_height += img.height

    result.save(path)


def main():
    paths = {os.path.join("assets/", rule.path) for rule in RULES}
    for path in paths:
        backup(path)

    for rule in RULES:
        path = os.path.join("assets/", rule.path)

        if isinstance(rule, Resize):
            resize(path, rule.width, rule.height)
        elif isinstance(rule, Trim):
            trim(path)
        elif isinstance(rule, Concat):
            concat(path, rule.inputs)
        else:
            print(f"Rule not registered: {rule}")


if __name__ == "__main__":
    main()
