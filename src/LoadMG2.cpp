#include "Assets.h"
#include "FileHandling.h"
#include "Game.h"
#include "Objects/SemaphoreObject.h"

#include <cstring>

bool CGame::load_map(const char* mapname)
{
    FILE* fp = f1open(assets::makePath(mapname), "r", GAMEDATA);
    if (fp == nullptr)
    {
        return false;
    }

    /* Load map: */
    int i_tmp;
    List<TILE_SOURCE> l;

    /* Tile sources: */

    int n = 0;
    char tmp[256];
    if (2 != fscanf(fp, "%s %d", tmp, &n))
    {
        return false;
    }

    for (int i = 0; i < n; i++)
    {
        TILE_SOURCE* source = new TILE_SOURCE();
        source->load(fp);
        tile_sources.Add(source);
    }

    /* Tiles: */
    for (int j = 0; j < 256; j++)
    {
        if (2 != fscanf(fp, "%s %d", tmp, &n))
        {
            return false;
        }

        tiles[j].Delete();
        for (int i = 0; i < n; i++)
        {
            if (1 != fscanf(fp, "%s", tmp))
            {
                return false;
            }

            int i1, i2, i3, i4, mask_type, collision_mask_type;
            if (6 != fscanf(fp, "%d %d %d %d %d %d", &i1, &i2, &i3, &i4, &mask_type, &collision_mask_type))
            {
                return false;
            }

            l.Instance(tile_sources);
            l.Rewind();

            CTile* t = nullptr;
            bool found = false;
            TILE_SOURCE* source = nullptr;
            while (found == false && l.Iterate(source))
            {
                if (source->cmp(tmp))
                {
                    found = true;
                    t = new CTile(i1, i2, i3, i4, source->sfc, collision_mask_type == 2);
                }
            }

            if (t != nullptr)
            {
                tiles[j].Add(t);
            }
        }
    }

    /* Objects: */
    if (2 != fscanf(fp, "%s %d", tmp, &n))
    {
        return false;
    }

    int current_object = 0;
    int semaphore_object = 0;
    int semaphore_tiles[5][2];
    for (int i = 0; i < n; i++)
    {
        int tile_bank, tile_num;
        int nbitmaps, nlinks, nparts;
        /* "tmp" contains the name of the object */
        if (2 != fscanf(fp, "%s %s", tmp, tmp))
            return false;
        if (strcmp(tmp, "\"semaphore\"") == 0)
            semaphore_object = current_object;

        if (1 != fscanf(fp, "%d", &nbitmaps))
            return false;
        for (int j = 0; j < nbitmaps; j++)
        {
            if (3 != fscanf(fp, "%d %d %d", &tile_bank, &tile_num, &nlinks))
            {
                return false;
            }

            if (strcmp(tmp, "\"semaphore\"") == 0)
            {
                semaphore_tiles[j][0] = tile_bank;
                semaphore_tiles[j][1] = tile_num;
            }

            for (int k = 0; k < nlinks; k++)
            {
                if (2 != fscanf(fp, "%d %d", &i_tmp, &i_tmp))
                {
                    return false;
                }
            }
        }

        if (1 != fscanf(fp, "%d", &nparts))
        {
            return false;
        }

        for (int j = 0; j < nparts; j++)
        {
            if (5 != fscanf(fp, "%d %d %d %d %d", &i_tmp, &i_tmp, &i_tmp, &i_tmp, &i_tmp))
            {
                return false;
            }
        }

        if (1 != fscanf(fp, "%d", &i_tmp))
            return false;
        if (1 != fscanf(fp, "%d", &i_tmp))
            return false;
        if (2 != fscanf(fp, "%d %d", &i_tmp, &i_tmp))
            return false;

        for (int i = 0; i < 23; i++)
        {
            for (int j = 0; j < nparts; j++)
                if (1 != fscanf(fp, "%s", tmp))
                    return false;
        }

        if (5 != fscanf(fp, "%d %d %d %d %d", &i_tmp, &i_tmp, &i_tmp, &i_tmp, &i_tmp))
            return false;
        if (4 != fscanf(fp, "%d %d %d %d", &i_tmp, &i_tmp, &i_tmp, &i_tmp))
            return false;
        if (1 != fscanf(fp, "%d", &i_tmp))
            return false;
        if (i_tmp != 0)
            return false; /* No states should be defined!!! */
        if (1 != fscanf(fp, "%d", &i_tmp))
            return false;
        if (i_tmp != 0)
            return false; /* No condition states should be defined!!! */

        if (4 != fscanf(fp, "%d %d %d %d", &i_tmp, &i_tmp, &i_tmp, &i_tmp))
            return false;
        if (1 != fscanf(fp, "%s ", tmp))
            return false;
        if (strcmp(tmp, "DAMAGE") == 0)
        {
            fgets(tmp, 80, fp);
        }
        if (2 != fscanf(fp, "%s %d", tmp, &i_tmp))
            return false;
        if (i_tmp != 0)
            return false; /* No variables should be defined!!! */
        if (10 != fscanf(fp, "%d %d %d %d %d %d %d %d %d %d", &i_tmp, &i_tmp, &i_tmp, &i_tmp, &i_tmp, &i_tmp, &i_tmp, &i_tmp, &i_tmp, &i_tmp))
            return false;

        current_object++;
    }

    if (2 != fscanf(fp, "%s %d", tmp, &i_tmp))
        return false;
    if (i_tmp != 0)
        return false; /* No game variables should be defined!!! */

    if (2 != fscanf(fp, "%s %d", tmp, &i_tmp))
        return false;
    if (i_tmp != 1)
        return false; /* Only one map should be defined!!! */

    if (2 != fscanf(fp, "%s %s", tmp, tmp))
        return false;
    if (2 != fscanf(fp, "%s %d", tmp, &i_tmp))
        return false;
    if (i_tmp != 0)
        return false; /* No map variables should be defined!!! */

    if (2 != fscanf(fp, "%s %d", tmp, &i_tmp))
        return false;
    if (i_tmp != 1)
        return false; /* Only one room should be defined!!! */
    if (2 != fscanf(fp, "%s %d", tmp, &i_tmp))
        return false;
    if (i_tmp != 0)
        return false; /* No room variables should be defined!!! */

    if (3 != fscanf(fp, "%s %d %d", tmp, &i_tmp, &i_tmp))
        return false;

    /* Map: */
    {
        int x, y, tile_bank, tile_num;

        if (3 != fscanf(fp, "%s %d %d", tmp, &dx, &dy))
            return false;
        dx *= 16;
        dy *= 16;

        if (1 != fscanf(fp, "%d", &i_tmp))
            return false;

        /* Background tiles: */
        if (1 != fscanf(fp, "%d", &n))
            return false;
        for (int i = 0; i < n; i++)
        {
            if (4 != fscanf(fp, "%d %d %d %d", &x, &y, &tile_bank, &tile_num))
                return false;
            background.Add(new CObject(x, y, tiles[tile_bank][tile_num], CONSTITUTION_NONE, this));
        }

        /* Background objects: */
        if (1 != fscanf(fp, "%d", &n))
            return false;
        for (int i = 0; i < n; i++)
        {
            if (3 != fscanf(fp, "%d %d %d", &x, &y, &i_tmp))
                return false;
            if (1 != fscanf(fp, "%d", &i_tmp))
                return false;
            if (i_tmp != 0)
                return false; /* No object variables should be defined!!! */

            if (i_tmp == semaphore_object)
            {
                /* a semaphore: */
                CObject* o = new CSemaphoreObject(x, y, tiles[semaphore_tiles[0][0]][semaphore_tiles[0][1]],
                                         tiles[semaphore_tiles[1][0]][semaphore_tiles[1][1]],
                                         tiles[semaphore_tiles[2][0]][semaphore_tiles[2][1]],
                                         tiles[semaphore_tiles[3][0]][semaphore_tiles[3][1]],
                                         tiles[semaphore_tiles[4][0]][semaphore_tiles[4][1]], this);
                objects.Add(o);
                start_delay = SEMAPHORE_TIME * 7;
            }
        }

        /* Middleground tiles: */
        if (1 != fscanf(fp, "%d", &n))
            return false;
        for (int i = 0; i < n; i++)
        {
            if (4 != fscanf(fp, "%d %d %d %d", &x, &y, &tile_bank, &tile_num))
                return false;
            middleground.Add(new CObject(x, y, tiles[tile_bank][tile_num], CONSTITUTION_NONE, this));
        }

        /* Middleground objects: */
        if (1 != fscanf(fp, "%d", &n))
            return false;
        for (int i = 0; i < n; i++)
        {
            if (3 != fscanf(fp, "%d %d %d", &x, &y, &i_tmp))
                return false;
            if (1 != fscanf(fp, "%d", &i_tmp))
                return false;
            if (i_tmp != 0)
                return false; /* No object variables should be defined!!! */

            if (i_tmp == semaphore_object)
            {
                /* a semaphore: */
                CObject* o = new CSemaphoreObject(x, y, tiles[semaphore_tiles[0][0]][semaphore_tiles[0][1]],
                                         tiles[semaphore_tiles[1][0]][semaphore_tiles[1][1]],
                                         tiles[semaphore_tiles[2][0]][semaphore_tiles[2][1]],
                                         tiles[semaphore_tiles[3][0]][semaphore_tiles[3][1]],
                                         tiles[semaphore_tiles[4][0]][semaphore_tiles[4][1]], this);
                objects.Add(o);
                start_delay = SEMAPHORE_TIME * 7;
            }
        }

        /* Foreground tiles: */
        if (1 != fscanf(fp, "%d", &n))
            return false;
        for (int i = 0; i < n; i++)
        {
            if (4 != fscanf(fp, "%d %d %d %d", &x, &y, &tile_bank, &tile_num))
                return false;
            foreground.Add(new CObject(x, y, tiles[tile_bank][tile_num], CONSTITUTION_NONE, this));
        }

        /* Foreground objects: */
        if (1 != fscanf(fp, "%d", &n))
            return false;
        for (int i = 0; i < n; i++)
        {
            if (3 != fscanf(fp, "%d %d %d", &x, &y, &i_tmp))
                return false;
            if (1 != fscanf(fp, "%d", &i_tmp))
                return false;
            if (i_tmp != 0)
                return false; /* No object variables should be defined!!! */

            if (i_tmp == semaphore_object)
            {
                /* a semaphore: */
                CObject* o = new CSemaphoreObject(x, y, tiles[semaphore_tiles[0][0]][semaphore_tiles[0][1]],
                                         tiles[semaphore_tiles[1][0]][semaphore_tiles[1][1]],
                                         tiles[semaphore_tiles[2][0]][semaphore_tiles[2][1]],
                                         tiles[semaphore_tiles[3][0]][semaphore_tiles[3][1]],
                                         tiles[semaphore_tiles[4][0]][semaphore_tiles[4][1]], this);
                objects.Add(o);
                start_delay = SEMAPHORE_TIME * 7;
            }
        }
    }

    fclose(fp);
    return true;
}
