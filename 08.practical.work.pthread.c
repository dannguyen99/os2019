#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUFFER_SIZE 10


typedef struct {
  char type;
  int amount;
  char unit;
} item;

item buffer[BUFFER_SIZE];
int first = 0;
int last = 0;

void printLog(){
  printf("first = %d\t last = %d\n", first, last);
  return;
}

void produce(item *i){
  while ((first + 1) % BUFFER_SIZE == last){
    //do nothing -- no free buffer item
    printf("No free buffer item!\n");
  }
  memcpy(&buffer[first], i, sizeof(item));
  first = (first + 1) % BUFFER_SIZE;
  printLog();
}

item* consume(){
  item* i = malloc(sizeof(item));
  while (first == last){
    // do nothing -- nothing to consume
    printf("Nothing to consume!\n");
  }
  memcpy(i, &buffer[last], sizeof(item));
  last = (last + 1) % BUFFER_SIZE;
  printLog();
  return i;
}

item* initItem(char type, int amount, char unit){
  item* i = malloc(sizeof(item));
  i->type = type;
  i->amount = amount;
  i->unit = unit;
  return i;
}

//produce routines
void *produceThread(void *param){
  printf("This is produce thread\n");
  produce(initItem('1',3,'0'));
  produce(initItem('0',9,'1'));
  produce(initItem('0',3,'0'));
}

//consume routines
void *consumeThread(void *param){
  printf("This is consume thread\n");
  consume();
  consume();
}

int main(){
  pthread_t tid1, tid2;
  printf("Initial log:\nfirst = %d\tlast = %d\n\n", first, last);

  //create two new threads
  if(pthread_create(&tid1, NULL, produceThread, NULL) != 0){
    perror("pthread_created() error");
  }
  //second thread
  if(pthread_create(&tid2, NULL, consumeThread, NULL) != 0){
    perror("pthread_created() error");
  }

  // wait for two threads finish
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  return 0;
}
