// C program to illustrate factorial calculation
// using fork() in C for Linux
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h>


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


char char_to_num(char ch){
  char num = ch - 'A';
  return num;
}


void print_board(char** board) {
  int n = 9;
  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++) {
      printf("%c ", board[i][j]);
    }
    printf("\n");
  }

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

      //printf("\nsize : %d\n", s);
  }

  else {
    printf("failed to open file");
  }

  return board;
}



int main(int argc , char *argv[] )
{

  // const int LIMIT = 5;
  // int queue = [LIMIT];
  // int front,rear;
  FILE *fp;
  char *filename;
  char **board;
  int n, a, b;
  int pid1, pid2, pid3, pid4;



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
  pid1 = fork();
  if (pid1==0){
    printf("\nfirst child creating board - firstChildID : %d\n", getpid());
    board = create_board(filename, (int)n);
    for(int i=0; i<(int)n; i++) {
      for(int j=0; j<(int)n; j++) {
        printf("%c ", board[i][j]);
      }
      printf("\n");
    }
    exit(0);
  }
  else {
    sleep(1);
    pid2 = fork();

    if(pid2==0){
      printf("\nsecond child- secondChildID : %d\n", getpid());
    }
    else {
      sleep(1);
      pid3 = fork();

      if(pid3==0){
        printf("\nthird child- thirdChildID : %d\n", getpid());
      }

      else {
        sleep(1);
        pid4 = fork();

        if(pid4==0){
          printf("\nforth child- forthChildID : %d\n", getpid());
        }
        else{
          sleep(1);
          printf("\nin parent- parentID : %d\n", getpid());
        }


      }
    }
  }

   return 0;

}
