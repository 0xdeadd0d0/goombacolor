#!/usr/bin/env python3

import configparser
import logging
import pathlib
import string
from enum import Enum
from jinja2 import Template


def ror(n, d, int_bits=32):
    return (n >> d)|(n << (int_bits - d)) & 0xFFFFFFFF

def convert_gg2goombacht(cheat, cheat_type):
    cheat_type = cheat_type
    rom_addr = f"{int((cheat[5], 16)^0xF)&0xF}{cheat[2]}{cheat[3]}{cheat[4]}"
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

    if len(cheat_codes) > 5:
        cheat_codes = cheat_codes.replace("-", "")
        cheat_codes = cheat_codes.replace("x", "0")
        cheat_codes = cheat_codes.replace("X", "0")
        cheat_codes = cheat_codes.replace("?", "0")
        cheat_codes = cheat_codes.replace("Y", "0")
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
                    logger.debug("invalid code format...")
                    return []
            gen_cheat_codes.append(
                {
                    "cheat_type": cheat_type, "cheat_code_raw": cheat.upper()
                }
            )
    return gen_cheat_codes

logging.basicConfig(filename=f'{__name__}.log', level=logging.INFO)
logger = logging.getLogger(__name__)
# Initialize the built-in parser
config = configparser.ConfigParser()

cht_db_dir = pathlib.Path("./libretro-database/cht/Nintendo - Game Boy Color")
f_names = list(cht_db_dir.rglob("*.cht"))
data_base_entry = { "data_base_entry": []}
g_nb_cheats = 0
file_num = 0

logger.info(f"Total files found: {len(f_names)}")

for f_name in f_names:

    try:
        with open(f_name, "r", encoding="utf-8") as f:
            config.read_string("[DEFAULT]\n" + f.read())
    except:
        logger.debug("An exception occurred")
        continue


    # Access data directly like a dictionary / JSON object
    total_cheats = int(config.get("DEFAULT", "cheats"))
    data_base = { "game_name": f_name, "cheats": []}
    data_base["rom_crc32"] = "0xDEADD0D0"
    data_base["rom_name"] = pathlib.PurePosixPath(f_name).name[:64]

    logger.info(f"Total cheats found: {total_cheats}")

    for i in range(total_cheats):
        try:
            # Read individual cheat elements safely
            cheat_desc = config.get("DEFAULT", f"cheat{i}_desc")
            cheat_desc = "".join([c for c in cheat_desc if c.isalnum()])
            cheat_codes = config.get("DEFAULT", f"cheat{i}_code")
            cheat_codes = convert_cheat_codes(cheat_codes)
        except:
            logger.debug("An exception occurred")
            continue
        if not len(cheat_desc) > 0:
            cheat_desc = f"\"cheat{i}\""
        if len(cheat_codes) == 0:
            continue
        logger.debug(f"desc: {cheat_desc}\ncode:{cheat_codes}")
        data_base["cheats"].append(
            {
                "varname": f"cheat{g_nb_cheats}", "cheat_desc": cheat_desc[:63], "cheat_size": len(cheat_codes), "cheat_codes": cheat_codes
            }
        )
        g_nb_cheats = g_nb_cheats + 1

    if len(data_base["cheats"]) > 0:
        data_base["nb_cheats"] = f"{len(data_base["cheats"])}"
        data_base["data_base_entry_varname"] = f"cheat_db_entry{file_num}"
        data_base_entry["data_base_entry"].append(data_base)
    logger.info(f"Total cheats valid: {len(data_base["cheats"])}")
    file_num = file_num + 1

# Load and build template
with open("goombacht.h.j2") as f:
    template = Template(f.read())

# Export generated file
with open("goombacht.h", "w") as f:
    f.write(template.render(data_base_entry))

exit(0)
