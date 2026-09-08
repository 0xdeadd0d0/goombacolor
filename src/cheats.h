#ifndef CHEATS_H_
#define CHEATS_H_

// GameGenie ex: 0AC-E6B-F7E
// addr = 0x46FC
// new_val = 0xA0
// old_val = 0xE5

// GameShark ex: 01FF16D0
// addr = 0xD016
// new_val = 0xFF
// bank = 0x01   (for multi bank 0 to 7: 0x90 to 0x97)

typedef enum cht_type {
    INVALID_TYPE, GAME_GENIE, GAME_SHARK, CHEAT_TYPE_MAX
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

#endif // CHEATS_H_
