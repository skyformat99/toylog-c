#ifndef _TOYBITMAP_H_
#define _TOYBITMAP_H_

typedef struct 
{
    int map_len;
    char * map;
}ToyBitmap;

int  init_map_size(ToyBitmap * map, int size);
int  set_map_value(ToyBitmap * map, int pos, char value);
char get_map_value(ToyBitmap * map, int pos);

#endif /* end file */

