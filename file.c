#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileStruct.h"
#include "stack.h"
#include "file.h"
#include "sya.h"

int runFile(char **input, vari *var) {
  int error = 0; //int to put errors into
  int maxSize = 1024; //maximum size of tree

  char **fileString = calloc(maxSize, sizeof(*fileString));
  __MALLOC_CHECK(fileString, error);

  fileTree *tree = createLeaf();

  //make tree structure
  error = createTree(input[0], tree, fileString, &maxSize);
  if(error) return error;

  //execute tree
  error = executeTree(tree, var, maxSize);

  //free tree
  cutDownTree(tree);

  //free array of strings
  freeString(fileString, maxSize);

  return error;
}


//create new fileStack
fileStack newFileStack() {
  fileStack out;
  out.top = 0;
  out.occ = 0;
  memset(out.stk, 0, sizeof(out.stk));
  return out;
}


//create and populate tree
int createTree(char *fileName, fileTree *tree, char **fileString, int *maxSize){
  FILE *inputFile = fopen(fileName, "r"); //file to read from
  char buffer[1024]; //size of char buffer that each line of the file is copied too
  int direction = 0; //direction to branch in tree
  int error = 0; //error checking
  int length = 0; //lengh of string

  fileStack stk = newFileStack();

  fileTree *head = tree;

  //open and check if file exists
  //return error if it doesn't exist/can't open
  if(!inputFile) {
    free(tree);
    free(fileString);
    return error = -8;
  }
  printf("\n");

  //creates the tree structure
  for(int i = 0; fgets(buffer, 1024, inputFile); ++i) {
    int offset = 0;
    //removing trailing spaces
    if(buffer[0] == ' ') {
      for(offset = 0; buffer[offset] == ' '; ++offset);
      //empty for loop
    }

    char *bufferHold = buffer;
    bufferHold += offset;
    length = strlen(bufferHold);

    //skips a blank line, # comments out a line
    if(!strcmp(bufferHold, "\n") || (bufferHold[0] == '#')) {
      continue;
    }

    //replaces end new line with a null terminated character
    if(bufferHold[length - 1] == '\n') {
      bufferHold[--length] = '\0'; //update the length of the new string
    }

    //allocates memory and copies string from file into array
    //puts that string into tree struct
    fileString[i] = malloc((length+1) * sizeof(**fileString));
    __MALLOC_CHECK(fileString[i], error);
    strcpy(*(fileString+i), bufferHold);
    head->line = fileString[i];

    //check whether to branch left or right
    direction = checkProgramFlow(*(fileString+i));

    switch(direction) {

    //the first statement in a while/if/else branch right
    case 1: //if
    case 2: //while
    case -2: //else
      fPush(&stk, head);
      head->right = createLeaf();
      head = head->right;
      break;

      //end
      //signifies end of while/if/else 
    case -1:
      head = fPop(&stk);

    default:
      head->left = createLeaf();
      head = head->left;
      break;
    }

    //make fileString bigger if there is no room in it for the next iteration
    if((i+1) >= *maxSize) {
      *maxSize *= 2;
      fileString = realloc(fileString, *maxSize * sizeof(*fileString));
    }
  }  
  fclose(inputFile);
}


int executeTree(fileTree *tree, vari *var, int maxSize){
  fileTree *head = tree;
  int direction = 0; //checking the direction of program flow
  int check = 0; //checking conditionals: if/while
  int error = 0; //error 
  fileStack stk = newFileStack(); //create new file stack
  vari temp = *var;
  
  //executes the tree
  //checks that the current leaf and the string it holds are not 0
  while((head != NULL) && (head->line != NULL)) {

    //check whether to branch left or right down tree
    direction = checkProgramFlow(head->line);

    //if the line ends with ';', don't print the line, the line still executes
    if((head->line[strlen(head->line)-1] != ';') && (direction == 0)) {
      printf("> %s\n", head->line);
    }

    switch(direction) {
    case 1: //if
      check = checkConditional(head->line, direction, var);
      if(check < 0) { //if there is an error in the if
	return check;
      }

      //if the condition inside the if is true
      //push head->left, and continue execution from head->right, the line after the if
      //everything inside the if executes
      //otherwise go directly to head->left
      if(check) {
	fPush(&stk, head->left);
	head = head->right;
      } else {
	head = head->left;
      }
      break;

    case 2: //while
      check = checkConditional(head->line, direction, var);
      if(check < 0) {
	return check;
      }

      //push head to continue execution from the while
      //continue execution from head->right
      //when the loop comes back, the condition is rechecked
      //when the condition becomes false, go to the line after the while block
      if(check) {
	fPush(&stk, head);
	head = head->right;
      } else {
	head = head->left;
      }
      break;
	
    case -1: //end 
      head = fPop(&stk); //head returns to whatever is on top of the stack
      break;
      
    case -2: //else
      //checks the value from the previous if iteration
      //when check is 0, the if condition was false, so the else block
      //executes
      //when the check is non 0, the if condition executed
      //the else block is skipped
      if(check == 0) {
	fPush(&stk, head->left);
	head = head->right;
      } else { //check is true, continue after the else
	head = head->left;
      }
      break;

      //every other line is executed normally
      //for executing non conditional lines
    default:
      error = sya(head->line, &temp);
      if(error) {
	return error;
      }

      //print output
      if((head->line[strlen(head->line)-1] != ';') && direction == 0) {
	printMatrix(temp.ans);
      }

      //continue execution going left
      head = head->left;
      break;
    }
  }
}


//determine whether to branch left or right
int checkProgramFlow(char *input) {
  if(strstr(input, "else")) return -2;
  if(strstr(input, "end")) return -1;
  if(strstr(input, "if(")) return 1;
  if(strstr(input, "while(")) return 2;
  return 0;
}


//determine if input string is while or if
char *parseCondition(char *input, int type) {
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
int checkConditional(char *input, int type, vari *var) {
  input = parseCondition(input, type);
  vari temp = *var;
  int error = sya(input, &temp);
  if(error) {
    //error can return negative values
    return error; 
  }

  //guarantee that *ans only returns 0 or 1
  return !!temp.ans.elements[0];
}


//free double array
void freeString(char **string, int max) {
  for(int i = 0; i < max; ++i){
    free(string[i]);
  }
  free(string);
}
