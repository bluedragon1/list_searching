#include <stdio.h>
#include <stdlib.h>
typedef struct list{
  int id;
  struct list *link;
}list;

list* list_make(int x);

void list_dump(list* a);

void list_free(list* a);
