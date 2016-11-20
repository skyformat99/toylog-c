#ifndef _TOYBITMAP_H_
#define _TOYBITMAP_H_

typedef struct 
{
    int map_len;
    int * map;
}ToyBitmap;

int  init_map_size(ToyBitmap * map, int size);
void  free_map(ToyBitmap * map);
int  set_map_value(ToyBitmap * map, int pos, int value);
int  get_map_value(ToyBitmap * map, int pos);
void *mzero(void *s, int n);

#endif /* end file */

