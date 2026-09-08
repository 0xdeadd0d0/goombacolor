#!/usr/bin/env python3

import configparser
from enum import Enum
from jinja2 import Template


class Color(Enum):
    INVALID_TYPE = 0
    GAME_GENIE = 1
    GAME_SHARK = 2
    CHEAT_TYPE_MAX = 3


def convert_cheat_codes(cheat_codes):
    gen_cheat_codes = []

    for cheat in cheat_codes:
        if "-" in cheat:
            cheat = cheat.replace("-", "")[1:]
            cheat_type = "GAME_SHARK"
        else:
            cheat_type = "GAME_GENIE"
        gen_cheat_codes.append(
            {
                "cheat_type": cheat_type, "cheat_code_raw": cheat
            }
        )
    return gen_cheat_codes

# Initialize the built-in parser
config = configparser.ConfigParser()

f_name="Pokemon_Crystal_Version.cht"

with open(f_name, "r", encoding="utf-8") as f:
    config.read_string("[DEFAULT]\n" + f.read())

# Access data directly like a dictionary / JSON object
total_cheats = int(config.get("DEFAULT", "cheats"))
data_base = { "game_name": f_name, "cheats": []}

print(f"Total cheats found: {total_cheats}")

for i in range(total_cheats):
    # Read individual cheat elements safely
    cheat_desc = config.get("DEFAULT", f"cheat{i}_desc")
    cheat_codes = config.get("DEFAULT", f"cheat{i}_code")
    if not len(cheat_desc) > 2:
        cheat_desc = f"cheat{i}"
    elif len(cheat_desc) > 66:
        cheat_desc = "".join((cheat_desc[:65], "\""))
    if len(cheat_codes) > 2:
        cheat_codes = cheat_codes[1:-1].split("+")
    else:
        total_cheats = total_cheats - 1
        continue
    print(f"desc: {cheat_desc}\ncode:{cheat_codes}")
    data_base["cheats"].append(
        {
            "varname": f"cheat{i}", "cheat_desc": cheat_desc, "cheat_size": len(cheat_codes), "cheat_codes": convert_cheat_codes(cheat_codes)
        }
    )

print(f"Total cheats valid: {total_cheats}")

# Charger et compiler le template
with open("goombacht.h.j2") as f:
    template = Template(f.read())

# Exporter le fichier source C final
with open("goombacht.h", "w") as f:
    f.write(template.render(data_base))

exit(0)
