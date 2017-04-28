#include "Game.h"
#include "Objects/Object.h"

void CGame::init_quick_tables()
{
    int i;
    int part_size;
    List<CObject> l;
    CObject* o;

    part_size = dy / QUICK_PARTS;

    /* Background: */
    l.Instance(background);
    l.Rewind();
    while (l.Iterate(o))
    {
        i = o->get_y() / part_size;
        if (i < 0)
            i = 0;
        if (i >= QUICK_PARTS - 1)
            i = QUICK_PARTS - 1;
        quick_background[i].Add(o);
    }

    /* Middleground: */
    l.Instance(middleground);
    l.Rewind();
    while (l.Iterate(o))
    {
        i = o->get_y() / part_size;
        if (i < 0)
            i = 0;
        if (i >= QUICK_PARTS - 1)
            i = QUICK_PARTS - 1;
        quick_middleground[i].Add(o);
    }

    /* Foreground: */
    l.Instance(foreground);
    l.Rewind();
    while (l.Iterate(o))
    {
        i = o->get_y() / part_size;
        if (i < 0)
            i = 0;
        if (i >= QUICK_PARTS - 1)
            i = QUICK_PARTS - 1;
        quick_foreground[i].Add(o);
    }
}

void CGame::delete_quick_tables(void)
{
    int i;

    for (i = 0; i < QUICK_PARTS; i++)
    {
        while (!quick_background[i].EmptyP())
            quick_background[i].ExtractIni();
        while (!quick_middleground[i].EmptyP())
            quick_middleground[i].ExtractIni();
        while (!quick_foreground[i].EmptyP())
            quick_foreground[i].ExtractIni();
    }
}

void CGame::get_quick_min_max(CObject* o, long yoffs, int* min, int* max)
{
    get_quick_min_max(o->get_y() + yoffs, o->get_y() + yoffs + o->get_dy(), min, max);
}

void CGame::get_quick_min_max(long ymin, long ymax, int* min, int* max)
{
    int i;
    int part_size = dy / QUICK_PARTS;

    i = ((ymin - BIGGEST_OBJECT) / part_size) - 1;
    if (i < 0)
        i = 0;
    if (i >= QUICK_PARTS - 1)
        i = QUICK_PARTS - 1;
    *min = i;

    i = (ymax / part_size);
    if (i < 0)
        i = 0;
    if (i >= QUICK_PARTS - 1)
        i = QUICK_PARTS - 1;
    *max = i;
}
