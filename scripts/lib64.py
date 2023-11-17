content = ""
with open("copper.make", "r") as f:
    content = f.read()

content = content.replace("-L/usr/lib64", "")
content = content.replace(
    "$(CXX)",
    "$(CXX) -static-libgcc -static-libstdc++",
)

with open("copper.make", "w") as f:
    f.write(content)
