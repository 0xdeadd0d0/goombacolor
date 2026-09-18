#!/usr/bin/env python3

import configparser
from enum import Enum
from jinja2 import Template


def ror(n, d, int_bits=32):
    return (n >> d)|(n << (int_bits - d)) & 0xFFFFFFFF

def convert_gg2goombacht(cheat, cheat_type):
    cheat_type = cheat_type
    rom_addr = f"{int(cheat[5], 16)^0xF}{cheat[2]}{cheat[3]}{cheat[4]}"
    new_val = f"{cheat[0]}{cheat[1]}"
    reverted_new_val = int(f"{cheat[1]}{cheat[0]}", 16)
    old_val = int(f"{cheat[6]}{cheat[8]}", 16)
    old_val = ror(old_val, 2, 8)
    old_val = old_val | (old_val >> 24)
    old_val = old_val ^ 0xBA
    old_val = old_val & 0xFF
    return f"{old_val:x}{new_val}{rom_addr}"

def convert_cheat_codes(cheat_codes):
    gen_cheat_codes = []

    if len(cheat_codes) > 2:
        cheat_codes = cheat_codes.replace("-", "")
        cheat_codes = cheat_codes[1:-1].split("+")
        for cheat in cheat_codes:
            match len(cheat):
                case 6:
                    cheat_type = "GAME_GENIE_6"
                    cheat = f"{cheat}00"
                    cheat = convert_gg2goombacht(cheat, cheat_type)
                case 9:
                    cheat_type = "GAME_GENIE"
                    cheat = convert_gg2goombacht(cheat, cheat_type)
                case 8:
                    cheat_type = "GAME_SHARK"
                    cheat = f"{cheat[0]}{cheat[1]}{cheat[2]}{cheat[3]}{cheat[6]}{cheat[7]}{cheat[4]}{cheat[5]}"
                case _:
                    print("invalid code format...")
                    return []
            gen_cheat_codes.append(
                {
                    "cheat_type": cheat_type, "cheat_code_raw": cheat.upper()
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
    cheat_codes = convert_cheat_codes(cheat_codes)
    if len(cheat_codes) == 0:
        continue
    print(f"desc: {cheat_desc}\ncode:{cheat_codes}")
    data_base["cheats"].append(
        {
            "varname": f"cheat{i}", "cheat_desc": cheat_desc, "cheat_size": len(cheat_codes), "cheat_codes": cheat_codes
        }
    )

print(f"Total cheats valid: {len(data_base["cheats"])}")

# Charger et compiler le template
with open("goombacht.h.j2") as f:
    template = Template(f.read())

# Exporter le fichier source C final
with open("goombacht.h", "w") as f:
    f.write(template.render(data_base))

exit(0)
