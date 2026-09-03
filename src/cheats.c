#include "includes.h"

// GameGenie ex: 0AC-E6B-F7E
// addr = 0x46FC
// new_val = 0xA0
// old_val = 0xE5

// GameShark ex: 01FF16D0
// addr = 0xD016
// new_val = 0xFF
// bank = 0x01   (for multi bank 0 to 7: 0x90 to 0x97)

typedef enum cht_type {
    invalid_type, game_genie, game_shark, cheat_type_max
} cheat_type;

typedef struct cht_game_genie {
    u16 addr;
    u8 new_val;
    u8 old_val;
} cht_game_genie;

typedef struct cht_game_shark {
    u16 addr;
    u8 new_val;
    u8 bank;
} cht_game_shark;

typedef union cht_code {
    cht_game_genie cht_gg;
    cht_game_shark cht_gs;
    u32 cht_raw;
} cht_code;

typedef struct cht_pld {
    enum cht_type type;
    cht_code code;
} cht_pld;

typedef struct cheat {
    char name[64];
    u8 cht_size;
    cht_pld codes[];
} cheat;

/* cheat1_desc = "Wild Encounters are Always Shiny (Mixed Code)"
   cheat1_code = "91AA00DD+222-A1C-D4E"
   cheat1_enable = false
 */
const cheat cht1_pokemon_crystal =
  {
    .name = "Wild Encounters are Always Shiny (Mixed Code)",
    .cht_size = 2,
    .codes = {
      {.type = game_shark, .code.cht_gs = {.addr = 0xDD00, .new_val = 0xAA, .bank = 0x91}},
      {.type = game_genie, .code.cht_gg = {.addr = 0x4C22, .new_val = 0x22, .old_val = 0xEC}}
    }
};

/*
 * cheat0_desc = "Infinite Money"
 * cheat0_code = "010F4ED8+01424FD8+013F50D8"
 * cheat0_enable = false
 */
const cheat cht2_pokemon_crystal =
  {
    .name = "Infinite Money",
    .cht_size = 3,
    .codes = {
      {.type = game_shark, .code.cht_gs = {.addr = 0xD84E, .new_val = 0x0F, .bank = 0x01}},
      {.type = game_shark, .code.cht_gs = {.addr = 0xD84F, .new_val = 0x42, .bank = 0x01}},
      {.type = game_shark, .code.cht_gs = {.addr = 0xD850, .new_val = 0x3F, .bank = 0x01}},
    }
};

// database example
const cheat* const cheats_pokemon_crystal[] = {
    &cht1_pokemon_crystal,
    &cht2_pokemon_crystal
};
