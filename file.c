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
  int error = 0, length = 0, maxSize = 1024, direction = 0, check = 0;
  char **fileString = calloc(maxSize, sizeof(*fileString));
  __MALLOC_CHECK(fileString, error);

  fileTree *tree = createLeaf(), *head = tree;
  fileStack execStack = newFileStack(), stk = newFileStack();

  //open and check if file exists
  inputFile = fopen(input[0], "r");
  if(!inputFile){
    free(tree);
    free(fileString);
    return error = -8;
  }
  printf("\n");

  //creates the tree structure
  for(int i = 0; fgets(buffer, 1024, inputFile); ++i){
    int offset = 0;
    //removing trailing spaces
    if(buffer[0] == ' '){
      for(offset = 0; buffer[offset] == ' '; ++offset);
    }
    char *bufferHold = buffer;
    bufferHold += offset;
    length = strlen(bufferHold);

    if(!strcmp(bufferHold, "\n") || (bufferHold[0] == '#')){ //skips a blank line, # comments out a line
      continue;
    }

    if(bufferHold[length - 1] == '\n'){ //replaces end new line with a null terminated character
      bufferHold[--length] = '\0'; //update the length of the new string
    }
    fileString[i] = malloc((length+1) * sizeof(**fileString));
    __MALLOC_CHECK(fileString[i], error);
    strcpy(*(fileString+i), bufferHold);
    head->line = fileString[i];

    //check whether to branch left or right
    direction = checkProgramFlow(*(fileString+i));
    switch(direction){ //the first statement in a while/if/else branch right
    case 1: //if
    case 2: //while
    case -2: //else
      fPush(&stk, head);
      head->right = createLeaf();
      head = head->right;
      break;

    case -1: //end signifies end of while/if/else 
      head = fPop(&stk);

    default:
      head->left = createLeaf();
      head = head->left;
      break;
    }

    //make fileString bigger if there is no room in it for the next iteration
    if((i+1) >= maxSize){
      maxSize *= 2;
      fileString = realloc(fileString, maxSize * sizeof(*fileString));
    }
  }  

  //reset the head as the first node
  head = tree;

  //executes the tree
  while((head != NULL) && (head->line != NULL)){
    direction = checkProgramFlow(head->line);

    //if the line ends with ';', don't print the line, still executes
    if((head->line[strlen(head->line)-1] != ';') && (direction == 0)){
      printf("> %s\n", head->line);
    }

    switch(direction){
    case 1: //if
      check = checkConditional(head->line, direction, var, ans);
      if(check < 0){ //if there is an error in the if
	cutDownTree(tree);
	freeString(fileString, maxSize);
	fclose(inputFile);
	return check;
      }

      if(check){ //push head->left to continue execution from the line after the if, and everything inside executes
	fPush(&execStack, head->left);
	head = head->right;
      }
      else{
	head = head->left;
      }
      break;

    case 2: //while
      check = checkConditional(head->line, direction, var, ans);
      if(check < 0){
	cutDownTree(tree);
	freeString(fileString, maxSize);
	fclose(inputFile);
	return check;
      }

      if(check){ //push head to continue execute from the line that contains the while, the condition is rechecked
	fPush(&execStack, head);
	head = head->right;
      }
      else{
	head = head->left;
      }
      break;
	
    case -1: //end 
      head = fPop(&execStack); //head returns to whatever is on top of the stack
      break;
      
    case -2: //else
      if(check == 0){ //checks the value from the previous if iteration
	fPush(&execStack, head->left);
	head = head->right;
      }
      else{ //check is true, continue after the else
	head = head->left;
      }
      break;

    default: //for executing non conditional lines
      error = sya(head->line, ans, var);
      if(error){
	cutDownTree(tree);
	freeString(fileString, maxSize);
	fclose(inputFile);
	return error;
      }

      if((head->line[strlen(head->line)-1] != ';') && direction == 0){ //print our output
	printf(">     %lf\n", *ans);
      }

      head = head->left;
      break;
    }
  }

  //free tree
  cutDownTree(tree);

  //free array of strings
  freeString(fileString, maxSize);

  fclose(inputFile);
  return 0;
}

//create new fileStack
fileStack newFileStack(){
  fileStack out;
  out.top = 0;
  out.occ = 0;
  memset(out.stk, 0, sizeof(out.stk));
  return out;
}

//determine whether to branch left or right
int checkProgramFlow(char *input){
  if(strstr(input, "else")) return -2;
  if(strstr(input, "end")) return -1;
  if(strstr(input, "if(")) return 1;
  if(strstr(input, "while(")) return 2;
  return 0;
}

//determine if it's while or if
char *parseCondition(char *input, int type){
  switch(type){
  case 1: //if
    input = strchr(input, 'i'); //take advantage of the fact that it searches for first occurence
    input += 2;
    return input;
  case 2: //while
    input = strchr(input, 'w');
    input += 5;
    return input;
  }
}

//checks conditionals in while/if
int checkConditional(char *input, int type, vari *var, double *ans){
  input = parseCondition(input, type);
  int error = sya(input, ans, var);
  if(error){
    return error;
  }
  return *ans;
}

//free double array
void freeString(char **string, int max){
  for(int i = 0; i < max; ++i){
    free(string[i]);
  }
  free(string);
}
