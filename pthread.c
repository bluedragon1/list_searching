#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
//#define N 2 // the number of thread
//#define M 300000000 //the number of node
#include "list.h"

int core=1;

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
    if(i%core == 0){      
      if(find == list->id){
	printf("Thread:%d find %d !!\n", id, find);
	return;
      }
      i=core;
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

void usage(void){
  printf("options:\n");
  printf(" -p: the number of core\n");
  printf(" -m: list length\n");
  printf(" -f: find number\n");
  printf(" -h: usage\n");
  exit(1);
}


int main(int argc, char *argv[]){
  pthread_t *thread;
  int i, find, result, list_length;
  hikisu *m;
  list *main_list;

  find=1;//見つける数の初期値
  list_length=1;//リストの長さの初期値


  while((result=getopt(argc, argv, "m:p:f:h")) != -1) {
    switch (result) {
    case 'm'://リストの長さ
      list_length=atoi(optarg);
      break;
    case 'p'://スレッドの数
      core=atoi(optarg);
      break;
    case 'f'://見つける数
      find=atoi(optarg);
      break;
    case 'h'://使い方
      usage();
      break;
    }
  }
  main_list=list_make(list_length);  
  thread=(pthread_t *)malloc(sizeof(pthread_t)*core);
  m=(hikisu *)malloc(sizeof(hikisu)*core);

  for(i=0;i<core;i++){
    m[i].id=i;
    m[i].find=find;
    m[i].a=main_list;
    pthread_create(&thread[i],NULL,run,&m[i]); 
  }
  for(i=0;i<core;i++){
    pthread_join(thread[i],NULL); 
  }
  printf("Searching Finish!\n");
  list_free(main_list);
  free(thread);
  free(m);
  return 0;
}
