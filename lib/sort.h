#ifndef _SORT_H
#define _SORT_H 1

int pivot (void *array, int l, int u, size_t size,
     int (*cmp) (const void *, const void *));

void sort (void *, size_t, size_t, int (*cmp) (const void *, const void *));


#endif /*sort.h*/
