//Przetwarzanie pot3okowe przez pl
// -> p1 -> p2 -> p3 ->
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void createFile(FILE *hPlik, char text[]){
  fprintf(hPlik, "%s", text);
}

void readFile(FILE *hPlik, FILE *hPlikNew){
  char letter = fgetc(hPlik);
  fputc(letter >= 'a' ? letter + 'A' - 'a' : letter - 'A' + 'a', hPlikNew);

  while(letter != EOF)
  {
    fputc(letter, hPlikNew);
    letter = fgetc(hPlik);
  }

}
void printFile(FILE *hPlik){
  char letter = fgetc(hPlik);
  while(letter != EOF)
  {
    printf("%c", letter);
    letter = fgetc(hPlik);
  }
}

int main(int argc, char *args[]){

  FILE *hPlik = fopen("file.txt", "w+");
  printf("Zglasza sie proces tworzący plik: %d\n", getpid());
  createFile(hPlik, args[1]);
  fclose(hPlik);
  hPlik = fopen("file.txt", "r+");
  int pid =  fork();
  if(pid == 0) {
    sleep(3);
    printf("Zglasza sie proces zmieniający plik: %d\n", getpid());
    FILE *hPlik2 = fopen("file2.txt", "w+");
    readFile(hPlik, hPlik2);
    fclose(hPlik2);
    hPlik2 = fopen("file2.txt", "r+");
    int pid2 = fork();
    if(pid2 == 0) {
      sleep(3);
      printf("Zglasza sie proces zamykający plik: %d\n", getpid());
      fclose(hPlik);
      fclose(hPlik2);
      return 0;
    }
    waitpid(pid2, NULL, 0);
    printFile(hPlik2);
    fclose(hPlik2);
    fclose(hPlik);
    return 0;
  }
  waitpid(pid, NULL, 0);
  fclose(hPlik);
  return 0;
 }
