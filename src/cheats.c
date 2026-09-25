#include "includes.h"
#include "cheats.h"
#include "goombacht.h"

#define MAX_CHEAT 16
#define RAM_PATCH_CHUNK_SIZE 8

typedef struct __attribute__((packed)) ram_patchs {
    u8 *phy_addr[RAM_PATCH_CHUNK_SIZE];
    u8 values[RAM_PATCH_CHUNK_SIZE];
} ram_patchs;

EWRAM_BSS const cheat *cheat_list[MAX_CHEAT];
EWRAM_BSS const cheat_db_entry * cheat_db;
ram_patchs ram_patchs_chunk;

void init_cheat_database(void)
{
    cheat_db = &cheat_db_entry913;
    /* Test infinite Money + all Badges (PKMN Crystal) */
    add_cheat(cheat_db->cheat_codes[0]);
    add_cheat(cheat_db->cheat_codes[1]);
    enable_cheats();
}

bool add_cheat(const cheat *new_cheat)
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

bool remove_cheat(const cheat *rm_cheat)
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
        cheat_list[to_fill] = cheat_list[i-1];
        cheat_list[i-1] = NULL;
    }
    return status;
}

static void prepare_ram_patch(const cht_code *cheat_code, int chunk_slot_idx)
{
    int i;
    /* Pad the struct with the first cheat code */
    if (chunk_slot_idx == 0)
    {
        for (i = 0; i < RAM_PATCH_CHUNK_SIZE; i++)
        {
            ram_patchs_chunk.phy_addr[i] = &XGB_RAM[cheat_code->cht_gs.addr - 0xC000];
            ram_patchs_chunk.values[i] = cheat_code->cht_gs.new_val;
        }
    }
    else
    {
        ram_patchs_chunk.phy_addr[chunk_slot_idx] = &XGB_RAM[cheat_code->cht_gs.addr - 0xC000];
        ram_patchs_chunk.values[chunk_slot_idx] = cheat_code->cht_gs.new_val;
    }
}

/* Test apply simple multiline GS cheat code
 * TODO: find the best place to apply patchs
 * TODO: find the best way to apply the patchs
 * TODO: add ram bank support for RAM patch
 * TODO: add rom patch support (GAME_GENIE)
 */
void enable_cheats(void)
{
    int i, j;
    int nb_ram_patch = 0;

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
                prepare_ram_patch(&cheat_list[i]->codes[j].code, nb_ram_patch++);
                if (nb_ram_patch >= RAM_PATCH_CHUNK_SIZE)
                {
                    return;
                }
            }
            /* GAME_GENIE not supported for now */
        }
    }
}

void disable_cheats(void)
{
    /* Only GAME_SHARK supported, only need to stop gs cheats code */
    memset(&ram_patchs_chunk, 0, sizeof(ram_patchs));
}

/* TODO: check disassembly, make it in asm if not looking good */
void vblank_apply_ram_patch(void)
{
    int i;
    if (ram_patchs_chunk.phy_addr[0] != NULL)
    {
        for (i = 0; i < RAM_PATCH_CHUNK_SIZE; i++)
        {
            *ram_patchs_chunk.phy_addr[i] = ram_patchs_chunk.values[i];
        }
    }
}
