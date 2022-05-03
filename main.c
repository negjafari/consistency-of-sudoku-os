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


void print_board(char board[9][9]) {
  int n = 9;
  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++) {
      printf("%c ", board[i][j]);
    }
    printf("\n");
  }

}





int main(int argc , char *argv[] )
{

  FILE *fp;
  char *filename;
  int ch;
  char * buffer = 0;
  long length;
  int s=0;
  int n, a, b;
  char board[9][9];


  if(argc < 2){
    printf("missing file name\n");
    return(1);
  }
  else {
    filename = argv[1];
    printf("filename : %s\n", filename);

  }

  fp = fopen(filename, "r");

  if(fp) {
    int i=0, j=0;
    do {
        ch = fgetc (fp);
        if ( isgraph(ch) ){
           // int i = putchar (ch);
           if(s==0){
             n = (char)ch - '0';
             printf("\n n : %d\n", n);
           }
           else if(s==2){
             a = (char)ch - '0';
             printf("\n a : %d\n", a);
           }
           else if(s==4){
             b = (char)ch - '0';
             printf("\n b : %d\n", b);
           }
           else {
             char el = ceaser_cipher_decoder(ch);
             if(el!='#' && el!='*'){

               // printf("%c:i %d/j %d-", el, i,j);
               board[i][j] = el;
               i++;
               // printf("%c:%d - ", el ,ele);
             }
             else if(el=='#'){
               j++;
               i=0;
             }
           }


           s++;
        }
      } while (ch != EOF);

      printf("\nsize : %d\n", s);
      print_board(board);


  }



  else {
    printf("failed to open file");
  }

   return 0;

}
