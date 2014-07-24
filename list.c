#include <stdio.h>

#include "list.h"

list* add_list(int key, list *head){
  list *p;
  
  if((p = (list *)malloc(sizeof(list)))==NULL){
    printf("malloc error\n");
    exit(1);
  }
  p->id=key;
  p->link=head;
  head = p;

  return head;
}


list* list_make(int x){
  int i;
  list *head;
  head=NULL;
  
  for(i=x;i>0;i--){
    head=add_list(i,head);
  }
  return head;
}

void list_dump(list* a){
  while(a!=NULL){
    printf("%d -> ",a->id);
    a=a->link;
  }
  printf("[]\n");
}
/*
void list_free(list* a){
  if(a==NULL)return;
  list_free(a->link);
  free(a);
  return;
  }*/
void list_free(list* a){
  list* b;
  while(a!=NULL){
    b=a;
    a=a->link;
    free(b);
  }
  return;
}
