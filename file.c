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
  fileTree tree, *head = &tree;

  fileStack stk;
  stk.top = 0;
  
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
      fPush(&stk, head);
      head->right = createLeaf();
      head = head->right;
      break;

    case -1: //end
      head = fPop(&stk);
      head->left = createLeaf();
      head = head->left;
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

  //read tree stuff goes here?
  int check = 0;
  fileStack execStack;
  execStack.top = 0;
  head = &tree;
  for(;head->line != NULL;){
    direction = checkProgramFlow(head->line);

    //if the line ends with ';', don't print the line, still executes
    if((head->line[strlen(head->line)-1] != ';') && direction == 0){
      printf("> %s\n", head->line);
    }

    switch(direction){
    case 1: //if
      check = checkConditional(head->line, direction, var, ans);
      if(check < 0){
	return check;
      }

      if(check){
	if(head->left->line != NULL){
	  fPush(&execStack, head->left);
	}
	head = head->right;
      }
      else{
	head = head->left;
	check = 0;
      }
      break;

    case 2: //while
      check = checkConditional(head->line, direction, var, ans);
      printf("check %d\n", check);
      if(check < 0){
	return check;
      }

      if(check){
	if(head->left->line !=NULL){
	  fPush(&execStack, head);
	}
	head = head->right;
      }
      else{
	head = head->left;
      }
      break;
	
    case -1: //end
      printf("direction: end\n");
      head = fPop(&execStack);
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
      if(error){
	fclose(inputFile);
	return error;
      }
      head = head->left;
      printf("leftline %s\n", head->line);
      break;
    }
    //printf("Post direction.\n");

    if((head->left == NULL) && (head->right == NULL)){
      break;
    }
  }
  cutDownTree(&tree);

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

char *parseCondition(char *input, int type){
  switch(type){
  case 1:
    input = strchr(input, 'i');
    input += 2;
    return input;
  case 2:
    input = strchr(input, 'w');
    input += 5;
    return input;
  }
}

int checkConditional(char *input, int type, vari *var, double *ans){
  input = parseCondition(input, type);
  int error;
  error = sya(input, ans, var);
  if(error){
    return error;
  }
  return *ans;
}

