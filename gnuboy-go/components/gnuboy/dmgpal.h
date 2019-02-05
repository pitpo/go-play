#ifndef __DMGPAL_H__
#define __DMGPAL_H__

struct palette 
{
    int hash[20];
    int bg[4];
    int obj0[4];
    int obj1[4];
};

extern int dmg_pal[][4];
extern int dmgcolor;

void dmg_pal_cycle();
void dmg_pal_set();

#endif