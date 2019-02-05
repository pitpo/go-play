#include "dmgpal.h"
#include "mem.h"
#include "lcd.h"
#include "stdio.h"

// TODO: save currently used palette in each game's savestate

#if 0
// Testing/Debug palette
 int def_pal[4][4] = {{0xffffff, 0x808080, 0x404040, 0x000000},
					  {0xff0000, 0x800000, 0x400000, 0x000000},
					  {0x00ff00, 0x008000, 0x004000, 0x000000},
					  {0x0000ff, 0x000080, 0x000040, 0x000000} };
#else
// keep in mind these are actually BGR values, not RGB
#define GB_DEFAULT_PALETTE { 0xd5f3ef, 0x7ab6a3, 0x3b6137, 0x161c04 }
#define GB_GREEN_PALETTE { 0xffffff, 0x00ff52, 0x0042ff, 0x000000 }
#define GB_ORANGE_PALETTE { 0xffffff, 0x00ffff, 0x0000ff, 0x000000 }
#define GB_LIGHT_BROWN_PALETTE { 0xffffff, 0x63adff, 0x003184, 0x000000 }
#define GB_INVERTED_MONOCHROME_PALETTE { 0x000000, 0x848400, 0x00deff, 0xffffff }
#define GB_MONOCHROME_PALETTE { 0xffffff, 0xa5a5a5, 0x525252, 0x000000 }
#define GB_PASTEL_MIX_PALETTE { 0xa5ffff, 0x9494ff, 0xff9494, 0x000000 }
#define GB_DARK_BROWN_PALETTE { 0xc5e6ff, 0x849cce, 0x296b84, 0x08315a }
#define GB_DARK_GREEN_BG_PALETTE { 0xffffff, 0x31ff7b, 0xc56300, 0x000000 }
#define GB_BROWN_OBJ_PALETTE { 0xffffff, 0x8484ff, 0x3a3a94, 0x000000 }
#define GB_DARK_BLUE_PALETTE { 0xffffff, 0xde8c8c, 0x8c5252, 0x000000 }
#define GB_BLUE_PALETTE { 0xffffff, 0xffa563, 0xff0000, 0x000000 }
#define GB_GREEN_OBJ_PALETTE { 0xffffff, 0x31ff7b, 0x008400, 0x000000 }
#define GB_YELLOW_PALETTE { 0xffffff, 0x00ffff, 0x004a7b, 0x000000 }

int dmg_pal[4][4] = {GB_DEFAULT_PALETTE,
	 				 GB_DEFAULT_PALETTE,
					 GB_DEFAULT_PALETTE,
					 GB_DEFAULT_PALETTE };
#endif

const struct palette predefs[] = {
    // manual selection palettes (some are used in specific games)
    {
        { -1 },
        GB_DEFAULT_PALETTE, GB_DEFAULT_PALETTE, GB_DEFAULT_PALETTE
    },
    {
        { -1 },
        GB_GREEN_PALETTE, GB_GREEN_PALETTE, GB_GREEN_PALETTE
    },
    { /* TETRIS */
        { 0x4f67, -1 },
        GB_ORANGE_PALETTE, GB_ORANGE_PALETTE, GB_ORANGE_PALETTE
    },
    {
        { -1 },
        GB_LIGHT_BROWN_PALETTE, GB_LIGHT_BROWN_PALETTE, GB_LIGHT_BROWN_PALETTE
    },
    {
        { -1 },
        GB_INVERTED_MONOCHROME_PALETTE, GB_INVERTED_MONOCHROME_PALETTE, GB_INVERTED_MONOCHROME_PALETTE
    },
    {
        { -1 },
        GB_MONOCHROME_PALETTE, GB_MONOCHROME_PALETTE, GB_MONOCHROME_PALETTE
    },
    {
        { -1 },
        GB_PASTEL_MIX_PALETTE, GB_PASTEL_MIX_PALETTE, GB_PASTEL_MIX_PALETTE
    },
    {
        { -1 },
        GB_DARK_BROWN_PALETTE, GB_LIGHT_BROWN_PALETTE, GB_LIGHT_BROWN_PALETTE
    },
    {
        { -1 },
        GB_DARK_GREEN_BG_PALETTE, GB_BROWN_OBJ_PALETTE, GB_BROWN_OBJ_PALETTE
    },
    {
        { -1 },
        GB_DARK_BLUE_PALETTE, GB_BROWN_OBJ_PALETTE, GB_LIGHT_BROWN_PALETTE
    },
    {
        { -1 },
        GB_BROWN_OBJ_PALETTE, GB_GREEN_OBJ_PALETTE, GB_BLUE_PALETTE
    },
    {
        { -1 },
        GB_BLUE_PALETTE, GB_BROWN_OBJ_PALETTE, GB_GREEN_OBJ_PALETTE
    },
    {
        { -1 },
        GB_YELLOW_PALETTE, GB_BLUE_PALETTE, GB_GREEN_OBJ_PALETTE
    },
    // game specific palettes
    // TODO: add palettes for all games that had them defined in gbc bios
    {{0}}
};

enum pal{BG,WIN,OBJ0,OBJ1};

static int cur_pal = 0;
static int dedicated_pal = 0;
int dmgcolor = 1;

void dmg_pal_set_layer(enum pal layer, const int* pal) 
{
    int i;
    for (i = 0; i < 4; i++) 
    {
        dmg_pal[layer][i] = pal[i];
    }
}

int calc_name_hash() 
{
    int d = 256;
    int h = 0;
    int p = 65521; /* last 8-bit prime number */
    int i;

    for (i = 0; rom.name[i] != 0; i++) 
    {
        h = (d*h + (int)rom.name[i])%p;
    }
    return h;
}

void dmg_pal_cycle() 
{
#if 0
    // leave testing/debug palette intact
#else
    cur_pal = (cur_pal+1) % 14;
    printf("cur palette: %d\n", cur_pal);

    if (cur_pal == 13) 
    {
        if (dedicated_pal)
        {
            dmg_pal_set();
            return;
        }
        else
        {
            cur_pal = (cur_pal+1) % 14;
        }
    }

    dmg_pal_set_layer(BG, predefs[cur_pal].bg);
    dmg_pal_set_layer(WIN, predefs[cur_pal].bg);
    dmg_pal_set_layer(OBJ0, predefs[cur_pal].obj0);
    dmg_pal_set_layer(OBJ1, predefs[cur_pal].obj1);
    pal_dirty();
#endif
}

void dmg_pal_set() 
{
#if 0
    // leave testing/debug palette intact
#else
    int i, j;
    int hash = calc_name_hash();
    printf("running palette scan...");

    for (i = 0; *((int*)(predefs + i)) != 0; i++) 
    {
        for (j = 0; predefs[i].hash[j] != -1; j++) 
        {
            if (hash == predefs[i].hash[j]) 
            {
                printf("hash matched\n");
                dmg_pal_set_layer(BG, predefs[i].bg);
                dmg_pal_set_layer(WIN, predefs[i].bg);
                dmg_pal_set_layer(OBJ0, predefs[i].obj0);
                dmg_pal_set_layer(OBJ1, predefs[i].obj1);
                cur_pal = 13;
                dedicated_pal = 1;
                pal_dirty();
                return;
            }
        }
    }
#endif
}
