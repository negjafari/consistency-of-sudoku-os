#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>


#define FIFO_FILE "fifo78"



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
              board[j][i] = el;
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


int row_duplication(char char_array[], int size){
  char board[size][size];


  for(int i=0; i<size;i++){
    for(int j=0;j<size;j++){
      board[j][i] = char_array[(j*size) + i];
    }
  }


  for(int i=0; i<size;i++){
    for(int j=0;j<size;j++){
       printf("%c ", board[i][j]);
    }
    printf("\n");
  }


  //row traverse
  for (int row = 0; row < size; row++)
   {
       for (int col = 0; col < size; col++)
       {
           char num = board[row][col];
           for (int otherCol = col + 1; otherCol < size; otherCol++)
           {
               if (num == board[row][otherCol])
               {
                   return 0;
               }
           }
       }
   }

  return 1;

}


int col_duplication(char char_array2[], int size2){
  char board2[size2][size2];


  for(int i=0; i<size2;i++){
    for(int j=0;j<size2;j++){
      board2[j][i] = char_array2[(j*size2) + i];
    }
  }


  for(int i=0; i<size2;i++){
    for(int j=0;j<size2;j++){
       printf("%c ", board2[i][j]);
    }
    printf("\n");
  }



  //column traverse
  for (int row = 0; row < size2; row++)
   {
       for (int col = 0; col < size2; col++)
       {
           char num = board2[col][row];
           for (int otherCol = col + 1; otherCol < size2; otherCol++)
           {
               if (num == board2[otherCol][row])
               {
                   return 0;
               }
           }
       }
   }

  return 1;
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


        int result1 = row_duplication(nb2, s2);
        printf("result1 : %d\n",result1);
        close(f2);



        int f22 = open(FIFO_FILE, O_RDWR);
        write(f22, nb2, s2*s2);
        close(f22);

        // printf("\n");
        // for(int g=0 ; g<s2*s2 ; g++){
        //   printf("%c ",nb2[g]);
        // }
        // printf("\n");



        exit(0);
      }
      else {
        int pid3 = vfork();
        int f3;
        int s3 = (int)n;
        char nb3[s3*s3];

        if(pid3==0){
          printf("\nthird child- thirdChildID : %d\n", getpid());

          int f3 = open(FIFO_FILE, O_RDWR);
          read(f3, nb3, s3*s3);
          close(f3);

          int result2 = col_duplication(nb3, s3);
          printf("result2 : %d\n",result2);


          // printf("\n");
          // for(int l = 0 ; l<s3*s3 ; l++){
          //   printf("%c ", nb3[l]);
          // }
          // printf("\n");




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
