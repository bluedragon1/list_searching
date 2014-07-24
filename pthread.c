#include <stdio.h>
#include <pthread.h>
#define N 2 // the number of thread
#define M 300000000 //the number of node
#include "list.h"

typedef struct hikisu{
  int id;
  list *a;
  int find;
}hikisu;

//指定した数をリスト中から見つけ出す
void example(int id, int find, list *list){
  int i;
  i=id;
  while(list!=NULL){
    if(i%N == 0){      
      if(find == list->id){
	printf("Thread:%d find %d !!\n", id, find);
	return;
      }
      i=N;
    }
    list=list->link;
    i--;
  }
  printf("Thread:%d can't find %d...\n", id, find);
  return;
}

void* run(void* arg){
  hikisu *thread_data;
  list *nowlist;
  thread_data=(hikisu *)arg;
  printf("My thread id is %d\n",thread_data->id);
  nowlist=thread_data->a;
  example(thread_data->id, thread_data->find, nowlist);
  return NULL;
}

int main(int argc, char *argv[]){
  pthread_t thread[N];
  int i,find;
  hikisu m[N];
  list *main_list;

  main_list=list_make(M);
  //list_dump(main_list);

  if(argv[1] != NULL) find=atoi(argv[1]);
  else find=1;

  for(i=0;i<N;i++){
    m[i].id=i;
    m[i].find=find;
    m[i].a=main_list;
    pthread_create(&thread[i],NULL,run,&m[i]); 
  }
  for(i=0;i<N;i++){
    pthread_join(thread[i],NULL); 
  }
  printf("Searching Finish!\n");
  list_free(main_list);
  return 0;
}
