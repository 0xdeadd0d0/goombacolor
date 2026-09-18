#include "includes.h"
#include "cheats.h"
#include "goombacht.h"

/* Test apply simple multiline GS cheat code */
void vblank_apply_ram_patch(void)
{
    u32 i;
    u8 * ram_addr = NULL;

    for (i = 0; i < cheat1.cht_size; i++)
    {
        if (cheat1.codes[i].type == GAME_SHARK)
        {
            ram_addr = &XGB_RAM[cheat1.codes[i].code.cht_gs.addr - 0xC000];
            *ram_addr = cheat1.codes[i].code.cht_gs.new_val;
        }
    }
}
