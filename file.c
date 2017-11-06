#include <stdio.h>
#include <string.h>

#include "stack.h"
#include "file.h"
#include "fileStruct.h"
#include "sya.h"

int runFile(char **input, vari *var, double *ans){
  FILE *inputFile;
  char buffer[1024];
  int error = 0, length = 0, maxSize = 1024, i = 0, direction = 0;
  char **fileString = malloc(maxSize * sizeof(*fileString));
  fileTree tree;
  fileTree *head = tree;
  fileStack stk, readStk;
  
  inputFile = fopen(input[0], "r");
  if(!inputFile){
    return error = -7;
  }
  printf("\n");

  while(fgets(buffer, 1024, inputFile)){
    length = strlen(buffer);

    if(!strcmp(buffer, "\n") || (buffer[0] == '#')){ //skips a blank line, # comments out a line
      continue;
    }

    if(buffer[length - 1] == '\n'){ //replaces end new line with a null terminated character
      buffer[--length] = '\0'; //update the length of the new string
    }

    char *(fileString+i) = malloc(length * sizeof(**fileString));
    strcpy(filestring+i, buffer);

    direction = checkProgramFlow(fileString+i);
    if(direction > 0){
      head->right = createLeaf();
      head->right.line = fileString + i;
      head = head->right;

      fPush(stk, head);
      fPush(readStk, head);
    }
    else if(direction = -1){
      head = fPop(stk);
    }
    else{
      head->left = createLeaf();
      head->left.line = fileString + i;
      head = head->left;
    }

    if(++i >= maxSize){
      maxSize = 2 * length;
      fileString = realloc(fileString, maxSize* sizeof(*fileString));
    }
  }  

  //read tree stuff goes here?
  int check = 1;
  for(int j = 0; check; ++j){
    if(buffer[length - 1] != ';'){ //if the line ends with ';', don't print the line, still executes
      printf("> %s\n", buffer);
    }

    direction = checkProgramFlow(fileString + j);
    if(direction > 0){
    }
    else if(direction == -1){
    }
    else{
    }



    error = sya(buffer, ans, var);
    if(error){
      fclose(inputFile);
      return error;
    }

    if(buffer[length - 1] != ';'){
      printf(">    %lf\n\n", *ans);
    }
  }
  fclose(inputFile);
  return 0;
}

int checkProgramFlow(char *input){
  if(strstr(input, "end")) return -1;
  if(strstr(input, "if(")) return 1;
  if(strstr(input, "else")) return 2;
  if(strstr(input, "while(")) return 3;
}
