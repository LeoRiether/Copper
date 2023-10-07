from PIL import Image
import os
from colorama import Fore, Style

EXTENSIONS = [".jpg", ".jpeg", ".png", ".webp"]
IGNORE = ['unicorn_atlas.png', 'spaghetti_atlas.png']

def parse_args():
    from argparse import ArgumentParser

    parser = ArgumentParser(prog="Image Cropper")
    parser.add_argument(
        "-f",
        "--file",
        help="Path to file that should be cropped. If not specified, crops the entire assets folder",
        required=False,
    )

    return parser.parse_args()


def crop_image(path):
    with Image.open(path) as image:
        bounding_box = image.getbbox()
        width, height = image.size
        if bounding_box != (0, 0, width, height):
            print(f"{Fore.GREEN}Cropping{Style.RESET_ALL} {path}")
            *name, ext = path.split(".")
            new_path = str.join(".", name) + ".untrimmed." + ext
            os.rename(path, new_path)

            image.crop(bounding_box).save(path)


def main():
    args = parse_args()

    if args.file:
        crop_image(args.file)
    else:
        # Walk files in assets/
        for root, _, files in os.walk("assets/"):
            print(f"{Fore.YELLOW}Dir{Style.RESET_ALL} {root}")
            for file in files:
                is_image = any(file.endswith(ext) for ext in EXTENSIONS)
                not_ignored = all(file != entry for entry in IGNORE)
                if ".untrimmed" not in file and is_image and not_ignored:
                    crop_image(os.path.join(root, file))


if __name__ == "__main__":
    main()
