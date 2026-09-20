#include "includes.h"
#include "cheats.h"
#include "goombacht.h"

#define MAX_CHEAT 16

EWRAM_BSS const cheat *cheat_list[MAX_CHEAT];
EWRAM_BSS const cheat * const *cheat_db;

void init_cheat_database(void)
{
    cheat_db = cheat_codes;
    enable_cheat(cheat_db[1]);
}

bool enable_cheat(const cheat *new_cheat)
{
    bool status = false;
    int i;
    for (i = 0; i < MAX_CHEAT; i++)
    {
        if (cheat_list[i] == NULL)
        {
            cheat_list[i] = new_cheat;
            status = true;
            break;
        }
    }
    return status;
}

bool disable_cheat(const cheat *rm_cheat)
{
    bool status = false;
    int to_fill = MAX_CHEAT;
    int i;

    for (i = 0; i < MAX_CHEAT; i++)
    {
        if (cheat_list[i] == NULL)
        {
            break;
        }
        else if (cheat_list[i] == rm_cheat)
        {
            cheat_list[i] = NULL;
            to_fill = i;
            status = true;
        }
    }
    if (status && (to_fill < (i - 1)))
    {
        cheat_list[to_fill] = cheat_list[i];
        cheat_list[i] = NULL;
    }
    return status;
}

/* Test apply simple multiline GS cheat code */
void vblank_apply_ram_patch(void)
{
    int i, j;
    u8 * ram_addr = NULL;

    for (i = 0; i < MAX_CHEAT; i++)
    {
        if (cheat_list[i] == NULL)
        {
            break;
        }
        for (j = 0; j < cheat_list[i]->cht_size; j++)
        {
            if (cheat_list[i]->codes[j].type == GAME_SHARK)
            {
                ram_addr = &XGB_RAM[cheat_list[i]->codes[j].code.cht_gs.addr - 0xC000];
                *ram_addr = cheat_list[i]->codes[j].code.cht_gs.new_val;
            }
        }
    }
}
