#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileStruct.h"
#include "stack.h"
#include "file.h"
#include "sya.h"

int runFile(char **input, vari *var, double *ans){
  FILE *inputFile;
  char buffer[1024];
  int error = 0, length = 0, maxSize = 1024, i = 0, direction = 0;
  char **fileString = malloc(maxSize * sizeof(*fileString));
  fileTree tree;
  fileTree *head = &tree;
  fileStack stk, readStk, end;
  
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

    fileString[i] = malloc(length * sizeof(**fileString));
    strcpy(*(fileString+i), buffer);

    head->line = fileString[i];

    direction = checkProgramFlow(*(fileString+i));

    switch(direction){
    case 1: //if
    case 2: //while
    case -2: //else
      head->right = createLeaf();
      fPush(&stk, head);
      head = head->right;

      //fPush(&readStk, head);
      break;

    case -1: //end
      head = fPop(&stk);
      head->left = createLeaf();
      head = head->left;
      //fPush(&end, head);
      break;

    default:
      head->left = createLeaf();
      head = head->left;
      break;
    }

    if(++i >= maxSize){
      maxSize = 2 * length;
      fileString = realloc(fileString, maxSize * sizeof(*fileString));
    }
  }  
  printf("Done making tree.\n");

  //read tree stuff goes here?
  int condCount = 0;
  int check;
  fileStack execStack;
  head = &tree;
  for(int j = 0;; ++j){
    printf("Before checkProgramFlow.\n");
    direction = checkProgramFlow(*(fileString + j));
    printf("After checkProgramFlow.\n");

    //if the line ends with ';', don't print the line, still executes
    if((head->line[strlen(head->line)-1] != ';') || !direction){
      printf("> %s\n", head->line);
    }

    switch(direction){
    case 1: //if
      check = checkConditional(head->line, direction, var, ans);
	if(check){
	  fPush(&execStack, head->left);
	  head = head->right;
	}
	else{
	  head = head->left;
	  check = 0;
	}
	break;

    case 2: //while
      check = checkConditional(head->line, direction, var, ans);
	if(check){
	  fPush(&execStack, head);
	  head = head->right;
	}
	else{
	  head = head->left;
	}
	break;
	
    case -1: //end
      head = fPop(&execStack);
      head = head->left;
      break;
      
    case -2: //else
      if(check == 0){
	fPush(&execStack, head);
	head = head->right;
      }
      else{
	head = head->left;
      }
      break;

    default:
      error = sya(head->line, ans, var);
      head = head->left;
      if(error){
	fclose(inputFile);
	return error;
      }
      break;
    }
    printf("Post direction.\n");

    if(buffer[length - 1] != ';'){
      printf(">    %lf\n\n", *ans);
    }

    if((head->left == NULL) && (head->right == NULL)){
      break;
    }
  }
  printf("Done executing lines.\n");
  fclose(inputFile);
  return 0;
}

int checkProgramFlow(char *input){
  if(strstr(input, "else")) return -2;
  if(strstr(input, "end")) return -1;
  if(strstr(input, "if(")) return 1;
  if(strstr(input, "while(")) return 2;
  return 0;
}

void parseCondition(char *input, int type){
  switch(type){
  case 1:
    input = strchr(input, 'i');
    input += 2;
    return;
  case 2:
    return;
  case 3:
    input = strchr(input, 'w');
    input += 5;
    return;
  }
}

int checkConditional(char *input, int type, vari *var, double *ans){
  parseCondition(input, type);
  int error;
  error = sya(input, ans, var);
  if(error){
    return error;
  }
  return *ans;
}

