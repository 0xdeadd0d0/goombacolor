#include "includes.h"
#include "cheats.h"
#include "goombacht.h"

/* cheat1_desc = "Wild Encounters are Always Shiny (Mixed Code)"
   cheat1_code = "91AA00DD+222-A1C-D4E"
   cheat1_enable = false
 */
const cheat cht1_pokemon_crystal =
  {
    .name = "Wild Encounters are Always Shiny (Mixed Code)",
    .cht_size = 2,
    .codes = {
      {.type = GAME_SHARK, .code.cht_gs = {.addr = 0xDD00, .new_val = 0xAA, .bank = 0x91}},
      {.type = GAME_GENIE, .code.cht_gg = {.addr = 0x4C22, .new_val = 0x22, .old_val = 0xEC}}
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
      {.type = GAME_SHARK, .code.cht_gs = {.addr = 0xD84E, .new_val = 0x0F, .bank = 0x01}},
      {.type = GAME_SHARK, .code.cht_gs = {.addr = 0xD84F, .new_val = 0x42, .bank = 0x01}},
      {.type = GAME_SHARK, .code.cht_gs = {.addr = 0xD850, .new_val = 0x3F, .bank = 0x01}},
    }
};

// database example
const cheat* const cheats_pokemon_crystal[] = {
    &cht1_pokemon_crystal,
    &cht2_pokemon_crystal
};
