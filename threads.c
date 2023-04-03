//funkcja zwracająca instancję pustej listy (uchwyt)
//dwukierunkowa lista, posortowana z dowolnym typem danych
//'w konstruktorze' komparator typu danych
//funkcja zwracajaca liste musi dostac wskaznik na typ komparatora


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#define SMALLER -1
#define GREATER 1
#define DESCENDING -1
#define ASCENDING 1
#define EQUAL 0
#define INTTYPE 1
#define STRINGTYPE 2
#define BUFFSIZE 10

typedef struct element
{
    struct element* next;
    struct element* previous;
    void *member;
} element;

typedef struct head
{
    struct element* next;
    struct element* last;
    int (*comparator)(void*, void*);
    int (*print) (void*);
    int order;
    int size;
} head;

typedef struct arg_struct {
    pthread_mutex_t lock;
    head* h;
} args;

void* CreateHead(void* comparator, void* print, int order)
{
    head *h = (head*)malloc(sizeof(head));
    h->comparator = comparator;
    h->print = print;
    h->order = order;
    h->next = NULL;
    h->last = NULL;
    h->size = 0;
    return h;
}

void* CreateElement(void* member)
{
    element *el = (element*)malloc(sizeof(element));
    el->member = member;
    el->next = NULL;
    el->previous = NULL;
    return el;
}


int IntComparator(void* member1, void* member2) //int
{
    int*mem1 = member1;
    int*mem2 = member2;
    if(*mem1 < *mem2)
    {
        return SMALLER;
    }
    else if (*mem1 > *mem2)
    {
        return GREATER;
    }
    else
      return EQUAL;
}

void IntPrint(head* h)
{
  if(h->next == NULL)
  {
    printf("[]\n");
    return;
  }
  element *el = h->next;
  printf("[");
  do
  {
    int *num = el->member;
    printf("%i", *num);
    if(el->next != NULL)
      printf(",");
    else
      break;
    el=el->next;
  }
  while(1);
  printf("]\n" );
}

int StringComparator(void* member1, void* member2) //int
{
    int i=0;
    char* mem1 = member1;
    char* mem2 = member2;
    while(mem1[i] != '\0' && mem2[i] != '\0')
    {
      if(mem1[i] > mem2[i])
        return GREATER;
      else if(mem1[i] < mem2[i])
        return SMALLER;
      else
        i++;
    }
    if(mem1[i] == '\0' && mem2[i] == '\0')
      return EQUAL;
    else if(mem1[i] == '\0')
      return SMALLER;
    else
      return GREATER;
}

void StringPrint(head* h)
{
  if(h->next == NULL)
  {
    printf("[]\n");
    return;
  }
  element *el = h->next;
  printf("[");
  do
  {
    char *str = el->member;
    printf("\"%s\"", str);
    if(el->next != NULL)
      printf(",");
    else
      break;
    el=el->next;
  }
  while(1);
  printf("]\n" );
}

//Removes element from list and return ptr to element or null if not found
void* RemoveElement(head *h, void *member)
{
  if(h->next == NULL)
    return NULL;

  element *e = h->next;

  if((*h->comparator)(e->member,member) == EQUAL)
  {
    if(e->next == NULL)
      h->next = NULL;
    else
    {
      element *temp = e->next;
      h->next = temp;
      temp->previous= NULL;
    }
    h->size = h->size - 1;
    void *mem = e->member;
    free(e);
    return mem;
  }

  if(e->next == NULL)
    return NULL;

  e=e->next;

  while(1)
  {
    if((*h->comparator)(e->member,member) == EQUAL)
    {
      if(e->next == NULL){
        element *temp = e->previous;
        temp->next = NULL;
      }
      else
      {
        element *temp = e->previous;
        element *temp2 = e->next;
        temp2->previous = temp;
        temp->next = temp2;
      }
      h->size = h->size-1;
      void *mem = e->member;
      free(e);
      return mem;
    }
    if(e->next != NULL)
      e=e->next;
    else
      break;
  }
return NULL;

}



void InsertElement(head *h, void *member)
{

    element *el = CreateElement(member);
    h->size = h->size + 1;
    //List has no elements
    if(h->next == NULL){
      h->next = el;
      el->previous = NULL;
      h->last = el;
      return;
    }
    element *e = h->next;

    if((*h->comparator)(e->member,member) == h->order)
    {
      h->next = el;
      e->previous = el;
      el->next = e;
      el->previous = NULL;
      return;
    }

    if(e->next == NULL)
    {
      e->next = el;
      el->previous = e;
      h->last = el;
      return;
    }

    e = e->next; //


    while(1)
    {
      if((*h->comparator)(e->member,member) == h->order){
        element *temp = e->previous;
        temp->next = el;
        e->previous = el;
        el->next = e;
        el->previous = temp;
        return;
      }
      if(e->next != NULL)
        e=e->next;
      else
        break;
    }

    e->next = el;
    el->previous = e;
    h->last = el;
}

void Reverse(head *h){
  // element *s = h->last;
  // element * f = s->previous->previous;
  // int *v = f -> member;
  // printf("%d\n", *v);
  element *el = h->last;
  h->next = el;

  element *temp;
  if(el->previous == NULL)
  {
   h->last = el;
   return;
  }
  temp = el;
  element *temp2 = temp;
  temp = temp->previous;
  el->previous = NULL;
  while(1){
    el->next = temp;
    if(temp->previous == NULL){
          break;
    }
    temp2 = temp;
    temp = temp->previous;
    temp2->previous = el;
    el=el->next;


  }
  temp->next = NULL;
  temp->previous = el;
  h->last = temp;


  h->order = h->order == ASCENDING ? DESCENDING: ASCENDING;

}

#define K 4
#define N 16
#define TIME_S 1

void Add(args* a){
  for(int i=0; i<K; i++){
    sleep(TIME_S);
    char *ptr = (char*)malloc(4*sizeof(char));
    ptr[0] = 'l';
    ptr[1] = 'o';
    ptr[2] = 'l';
    ptr[3] = '\0';
    pthread_mutex_lock(&(a->lock));
    InsertElement(a->h, ptr);
    pthread_mutex_unlock(&(a->lock));
  }
}
void Delete(args* a){
  for(int i=0; i<K; i++){
    sleep(TIME_S);
    char *ptr = (char*)malloc(4*sizeof(char));
    ptr[0] = 'l';
    ptr[1] = 'o';
    ptr[2] = 'l';
    ptr[3] = '\0';
    pthread_mutex_lock(&(a->lock));
    char* p = RemoveElement(a->h, ptr);
    pthread_mutex_unlock(&(a->lock));
    if(p)
      free(p);
    free(ptr);
  }
}
void TestMenu(){

  args a;
  a.h = CreateHead(&StringComparator, &StringPrint, ASCENDING);
  pthread_mutex_init(&(a.lock), NULL);
  pthread_t workers[N];
  for(int i=0; i<N/2; i++){
    pthread_create(&workers[i], NULL, (void*)Add, &a);
  }
  for(int i=0; i<N/2; i++){
    pthread_join(workers[i], NULL);
  }
//  a.h->print(a.h);
  for(int i=N/2; i<N; i++){
    pthread_create(&workers[i], NULL, (void*)Delete, &a);
  }
  for(int i=N/2; i<N; i++){
    pthread_join(workers[i], NULL);
  }
//  a.h->print(a.h);
}

void TestMenu2(){
  head* h = CreateHead(&StringComparator, &StringPrint, ASCENDING);

  for(int i=0; i<N/2; i++){
    for(int i=0; i<K; i++){
      sleep(TIME_S);
      char *ptr = (char*)malloc(4*sizeof(char));
      ptr[0] = 'l';
      ptr[1] = 'o';
      ptr[2] = 'l';
      ptr[3] = '\0';
      InsertElement(h, ptr);
    }
  }
//  h->print(h);
  for(int i=N/2; i<N; i++){
    for(int i=0; i<K; i++){
      sleep(TIME_S);
      char *ptr = (char*)malloc(4*sizeof(char));
      ptr[0] = 'l';
      ptr[1] = 'o';
      ptr[2] = 'l';
      ptr[3] = '\0';
      char* p = RemoveElement(h, ptr);
      if(p)
        free(p);
      free(ptr);
    }
  }
//  h->print(h);

}

int main()
{
  time_t begin = time(NULL);
  TestMenu();
  time_t end = time(NULL);
//  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n%ld\n", end-begin);

  begin = time(NULL);
  TestMenu2();
  end = time(NULL);
  printf("\n%ld\n", end-begin);
  return 0;
}
