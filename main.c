// C program to illustrate factorial calculation
// using fork() in C for Linux
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_FILE "FIFOfile"


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

// void print_board(char** board) {
//   int n = 9;
//   for(int i=0; i<n; i++) {
//     for(int j=0; j<n; j++) {
//       printf("%c ", board[i][j]);
//     }
//     printf("\n");
//   }
//
// }

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

  FILE *fp;
  char *filename;
  //char **board;
  int n, a, b;
  //int pid1, pid2, pid3, pid4;



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
    int fd;
    char **board;
    printf("\nfirst child creating board - firstChildID : %d\n", getpid());
    board = create_board(filename, (int)n);



      for(int i=0; i<9; i++) {
        for(int j=0; j<9; j++) {
          printf("%c ", board[i][j]);
        }
        printf("\n");
      }


    int size = (int)n * (int)n;



    fd = open(FIFO_FILE, O_CREAT|O_RDWR);
    write(fd, board, size*sizeof(char));

    close(fd);

    exit(0);

  }
  else {
    //sleep(1);
    int pid2 = vfork();
    int fd2;
    char **board2;
    board2 = malloc(sizeof(int*) * 9);

    for(int a = 0; a < 9; a++) {
        board2[a] = malloc(sizeof(int*) * 9);
    }
    //int size = (int)n * (int)n;
    if(pid2==0){
      printf("\nsecond child - secondChildID : %d\n" ,getpid());

      int size = (int)n * (int)n;
      //int size = 81;

      //fd2 = open(FIFO_FILE, O_RDWR);
      //read(fd2, board2, size*sizeof(char));







      exit(0);
    }
    else {
      //sleep(1);
      int pid3 = vfork();

      if(pid3==0){
        printf("\nthird child- thirdChildID : %d\n", getpid());
      }

      else {
        //sleep(1);
        int pid4 = vfork();

        if(pid4==0){
          printf("\nforth child- forthChildID : %d\n", getpid());
          exit(0);
        }
        else{
          //sleep(1);
          printf("\nin parent- parentID : %d\n", getpid());
          exit(0);
        }


      }
    }
  }

   return 0;

}
