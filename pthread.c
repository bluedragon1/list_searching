#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "list.h"

int core=1;
int profile;

static inline double get_wall_time()
{
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9; /* ナノ秒 */
}

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
  printf(" -c: the number of core\n");
  printf(" -m: list length\n");
  printf(" -f: find number\n");
  printf(" -h: usage\n");
  printf(" -p: profile mode\n");
  exit(1);
}


int main(int argc, char *argv[]){
  pthread_t *thread;
  int i, find, result, list_length;
  hikisu *m;
  list *main_list;
  clock_t t1, t2;
  double d1,d2;
  find=1;//見つける数の初期値
  list_length=1;//リストの長さの初期値
  profile=0;//profileモード


  while((result=getopt(argc, argv, "m:pc:f:h")) != -1) {
    switch (result) {
    case 'm'://リストの長さ
      list_length=atoi(optarg);
      break;
    case 'c'://スレッドの数
      core=atoi(optarg);
      break;
    case 'f'://見つける数
      find=atoi(optarg);
      break;
    case 'p'://profile
      profile=1;
      break;
    case 'h'://使い方
      usage();
      break;
    }
  }
  main_list=list_make(list_length);  
  thread=(pthread_t *)malloc(sizeof(pthread_t)*core);
  m=(hikisu *)malloc(sizeof(hikisu)*core);

  t1=clock();
  d1=get_wall_time();
  /////////////////////////////////////////////

  for(i=0;i<core;i++){
    m[i].id=i;
    m[i].find=find;
    m[i].a=main_list;
    pthread_create(&thread[i],NULL,run,&m[i]); 
  }
  for(i=0;i<core;i++){
    pthread_join(thread[i],NULL); 
  }

  ///////////////////////////////////////////
  t2=clock();
  d2=get_wall_time();
  if(profile){
    printf("\ncpu_time: %f[s]\n",(double)(t2-t1)/CLOCKS_PER_SEC);
    //printf("t1: %f[s]\n",(double)t1/CLOCKS_PER_SEC);
    //printf("t2: %f[s]\n",(double)t2/CLOCKS_PER_SEC);
    printf("wall_time: %f[s]\n\n",(double)d2-d1);
    //printf("d1: %f[s]\n",(double)d1);
    //printf("d2: %f[s]\n",(double)d2);

  }

  printf("Searching Finish!\n");
  list_free(main_list);
  free(thread);
  free(m);
  return 0;
}
