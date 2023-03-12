//funkcja zwracająca instancję pustej listy (uchwyt)
//dwukierunkowa lista, posortowana z dowolnym typem danych
//'w konstruktorze' komparator typu danych
//funkcja zwracajaca liste musi dostac wskaznik na typ komparatora


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    return e;
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
      return e;
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

void TestMenu(){
  char q=0;
  void* fun1;
  void* fun2;
  int type;

  printf("Wybierz typ listy \'s' - string \'i' - int\n" );
  while(q!='s' && q!='i')
    q = getchar();

  if(q=='s')
  {
    type = STRINGTYPE;
    fun1 = &StringComparator;
    fun2 = &StringPrint;
  }
  else
  {
    type = INTTYPE;
    fun1 = &IntComparator;
    fun2 = &IntPrint;
  }

  printf("Wybierz kolejność \'a' - ascending \'d' - descending\n" );
  while(q!='a' && q!='d')
    q = getchar();

  int ord = q == 'a' ? ASCENDING: DESCENDING;
  head* h = CreateHead(fun1, fun2, ord);

  printf("Wcisnij \'i' aby dodać, \'d' aby usunąć, 'p' aby wypisać, 'c' - clear\n 'r' - aby odwrócić kolejność 'q' - aby wyjść \n");
  while(1){
    q = getchar();
    if(q =='i'){
      void *p;
      if(type == INTTYPE)
      {
        p = (int*)malloc(sizeof(int));
        scanf("%d", (int*)p);
      }
      else
      {
        int buff = BUFFSIZE;
        p = (char*)malloc(buff*sizeof(char));
        scanf("%s", (char*)p);
       }
       InsertElement(h, p);
    }
    else if(q=='d'){
      void *p;
      if(type == INTTYPE)
      {
        p = (int*)malloc(sizeof(int));
        scanf("%d", (int*)p);
      }
      else
      {
        int buff = BUFFSIZE;
        p = (char*)malloc(buff*sizeof(char));
        scanf("%s", (char*)p);
       }
       void* deleted = RemoveElement(h, p);
       if(deleted != NULL)
        free(deleted);

    }
    else if(q=='p'){
        h->print(h);
    }
    else if(q=='c'){
      system("clear");
      printf("Wcisnij \'i' aby dodać, \'d' aby usunąć, 'p' aby wypisać, 'c' - clear\n 'r' - aby odwrócić kolejność 'q' - aby wyjść \n");
    }
    else if(q=='r'){
      Reverse(h);
    }
    else if(q=='q'){
      break;
    }
  }
}

int main()
{
  TestMenu();
    return 0;
}


printf("%s", h->print(h))
