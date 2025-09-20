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

for cfg in glob.glob("./definitions/*.yaml"):
    with open(cfg, encoding="utf-8") as f:
        raw_config = yaml.safe_load(f)

    modules = []
    config = {"modules": []}
    for module, content in raw_config.items():
        for klass in content.get("classes", []):
            for p in klass.get("static_properties", []):
                p["static"] = True
            klass["properties"] = klass.get("properties", [])
            klass["properties"] += klass.get("static_properties", [])
        modules.append({"module": module} | content)

    config = {"modules": modules}
    output = template.render(config)

    base_name = os.path.splitext(os.path.basename(cfg))[0]
    with open(f"build/{base_name}.hpp", "w", encoding="utf-8") as f:
        f.write(output)

    print(f"Generated build/{base_name}.hpp")
