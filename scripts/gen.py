# -- stdlib --
import os
import glob
import yaml

# -- third party --
from jinja2 import Environment, FileSystemLoader

# -- code --

env = Environment(loader=FileSystemLoader("."))
template = env.get_template("klass.hpp.j2")


os.makedirs("build", exist_ok=True)

for cfg in glob.glob("./*.yaml"):
    with open(cfg, encoding="utf-8") as f:
        config = yaml.safe_load(f)

    output = template.render(config)

    base_name = os.path.splitext(os.path.basename(cfg))[0]
    with open(f"build/{base_name}.hpp", "w", encoding="utf-8") as f:
        f.write(output)

    print(f"Generated build/{base_name}.hpp")
