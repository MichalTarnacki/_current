//Przetwarzanie pot3okowe przez pl
// -> p1 -> p2 -> p3 ->
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void createFile(FILE *hPlik, char text[]){
  fprintf(hPlik, "%s", text);
}

void readFile(FILE *hPlik){
  FILE *hPlikNew = fopen("file2.txt", "w+");
  fclose(hPlik);
  hPlik = fopen("file.txt", "r+");
  char letter = fgetc(hPlik);
  fputc(letter >= 'a' ? letter + 'A' - 'a' : letter - 'A' + 'a', hPlikNew);
  letter = fgetc(hPlik);
  printf("Nowy tekst =\n");
  while(letter != EOF)
  {
    fputc(letter, hPlikNew);
    printf("%c", letter);
    letter = fgetc(hPlik);
  }
  printf("\n");

  fclose(hPlikNew);

}

int main(int argc, char *args[]){

  FILE *hPlik = fopen("file.txt", "w+");
  int pid =  fork();

  if(pid == 0) {
    int pid2 = fork();
    if(pid2 == 0) {
      sleep(3);
      printf("Zglasza sie proces tworzący plik: %d\n", getpid());
      createFile(hPlik, args[1]);
    }
    else{
        waitpid(pid2, NULL, 0);
        sleep(3);
        printf("Zglasza sie proces zmieniający plik: %d\n", getpid());

        readFile(hPlik);
    }
  }
  else {
      waitpid(pid, NULL, 0);
      sleep(3);
      printf("Zglasza sie proces zamykający plik: %d\n", getpid());
      fclose(hPlik);
    }


  return 0;
 }
