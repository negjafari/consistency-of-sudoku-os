#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>


#define FIFO_FILE "file3"



char ceaser_cipher_decoder(int ch) {
  ch = toupper(ch);

  int shift = 2;

  if (ch >= 'A' && ch<= 'Z') {
      char c = ch - 'A';
      c += shift;
      c = c % 26;
      ch = c + 'A';
  }

  return ch;
}


char **create_board(char *filename, int n){

  char **board;
  board = malloc(sizeof(int*) * n);

  for(int a = 0; a < n; a++) {
      board[a] = malloc(sizeof(int*) * n);
  }

  FILE *fp;
  int ch;
  int s=0;

  fp = fopen(filename, "r");

  if(fp) {
    int i=0, j=0;
    do {
        ch = fgetc (fp);
        if ( isgraph(ch) ){
          if(s>4) {
            char el = ceaser_cipher_decoder(ch);
            if(el!='#' && el!='*'){
              board[i][j] = el;
              i++;
            }
            else if(el=='#'){
              j++;
              i=0;
            }
          }
          s++;
        }
      } while (ch != EOF);
      fclose(fp);

  }

  else {
    printf("failed to open file");
  }

  return board;
}


int main(int argc , char *argv[]) {

  FILE *fp;
  char *filename;
  int n, a, b;


  if(argc < 2){
    printf("missing file name\n");
    return(1);
  }
  else {
    filename = argv[1];
  }

  printf("parent opens file : %s - parentID : %d\n", filename, getpid());

  fp = fopen(filename, "r");


  // detemine size of board
    if(fp) {
      int i=0, j=0,s=0;
      do {
          char ch = fgetc (fp);
          if ( isgraph(ch) ){
             if(s==0){
               n = (char)ch - '0';
             }
             else if(s==2){
               a = (char)ch - '0';
             }
             else if(s==4){
               b = (char)ch - '0';
             }
             s++;
          }
        } while (s<5);
        fclose(fp);

    }

    else {
      printf("failed to open file");
    }


    //first child -> create board
    int pid1 = vfork();
    if (pid1==0){
      int f1;
      char **b1;
      int s1 = (int)n;
      char nb1[s1*s1];

      printf("\nfirst child creating board - firstChildID : %d\n", getpid());
      b1 = create_board(filename, (int)n);


        for(int i=0; i<s1; i++) {
          for(int j=0; j<s1; j++) {
            printf("%c ", b1[i][j]);
          }
          printf("\n");
        }


        int k = 0;
        for(int i=0; i<(int)n; i++) {
          for(int j=0; j<(int)n; j++) {
            char ch = b1[i][j];
            nb1[k] = ch;
            k++;
          }
        }


      f1 = open(FIFO_FILE, O_CREAT|O_RDWR);
      write(f1, nb1, s1*s1);


      close(f1);

      exit(0);

    }
    else {

      int pid2 = vfork();
      int f2;
      int s2 = (int)n;
      char nb2[s2*s2];

      if(pid2==0){
        //check row
        printf("\nsecond child - secondChildID : %d\n" ,getpid());

        int f2 = open(FIFO_FILE, O_RDWR);
        read(f2, nb2, s2*s2);

        for(int i=0; i<s2*s2; i++) {
            printf("%c ", nb2[i]);
        }


        close(f2);
        exit(0);
      }
      else {
        int pid3 = vfork();

        if(pid3==0){
          printf("\nthird child- thirdChildID : %d\n", getpid());
          exit(0);
        }
        else {
          int pid4 = vfork();
          if(pid4==0){
            printf("\nforth child- forthChildID : %d\n", getpid());
            exit(0);
          }

          else {
            printf("\nin parent- parentID : %d\n", getpid());
            exit(0);
          }
        }
      }
    }

    return 0;

}
