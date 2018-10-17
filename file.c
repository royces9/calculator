#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "matrix.h"
#include "stack.h"
#include "variables.h"

#include "sya.h"
#include "fileStruct.h"
#include "file.h"


err_ret runFile(char **input, vari *var, int offset) {
	//error variable
	err_ret error = 0;

	//maximum size of tree
	int maxSize = 1024; //maximum size of tree

	char **fileString = calloc(maxSize, sizeof(*fileString));
	__MALLOC_CHECK(fileString, error);

	fileTree *tree = createLeaf(&error);
	if(error) return error;

	//make tree structure
	if( (error = createTree(input[0], tree, fileString, &maxSize, offset)) ) {
		cutDownTree(tree);
		freeString(fileString, maxSize);

		return error;
	}
  
	//execute tree
	error = executeTree(tree, var, maxSize);

	//free tree
	cutDownTree(tree);

	//free array of strings
	freeString(fileString, maxSize);

	return error;
}


//create and populate tree
err_ret createTree(char *fileName, fileTree *tree, char **fileString, int *maxSize, int offset){

	//file to read from
	FILE *inputFile = fopen(fileName, "r");

	//size of char buffer that each line of the file is copied too
	char buffer[1024];

	//direction to branch in tree
	int8_t direction = 0;

	//error checking
	err_ret error = 0;

	//lengh of string
	int length = 0;

	//stack data structure convenient for creating tree
	stack *stk = new_stk(128);

	//open and check if file exists
	//return error if it doesn't exist/can't open
	if(!inputFile) {
		free(tree);
		free(fileString);
		tree = NULL;
		fileString = NULL;

		return error = -8;
	}

	for(int i = 0; i < offset; ++i){
		fgets(buffer, 1024, inputFile);
	}

  
	//creates the tree structure
	for(int i = 0; fgets(buffer, 1024, inputFile); ++i) {

		int offset = 0;

		//removing trailing spaces
		if(buffer[0] == ' ')
			for(; buffer[offset] == ' '; ++offset);

		char *bufferHold = buffer+offset;
		length = strlen(bufferHold);

		//skips a blank line, # comments out a line
		if(!strcmp(bufferHold, "\n") || (bufferHold[0] == '#'))
			continue;

		//replaces end new line with a null terminated character
		if(bufferHold[length - 1] == '\n')
			bufferHold[--length] = '\0';

		//allocates memory and copies string from file into array
		//puts that string into tree struct
		fileString[i] = malloc((length+1) * sizeof(**fileString));
		__MALLOC_CHECK(fileString[i], error);

		strcpy(*(fileString+i), bufferHold);
		tree->line = fileString[i];

		//check whether to branch left or right
		direction = checkProgramFlow(*(fileString+i));

		tree->num = i + 2;

		switch(direction) {

			//the first statement in a while/if/else branch right
		case 1: //if
		case 2: //while
		case -2: //else
			push(stk, tree);
			tree->right = createLeaf(&error);
			tree = tree->right;
			break;

			//end
			//signifies end of while/if/else 
		case -1:
			tree = pop(stk);

		default:
			tree->left = createLeaf(&error);
			tree = tree->left;
			break;
		}

		//make fileString bigger if there is no room in it for the next iteration
		if((i + 1) >= *maxSize) {
			*maxSize *= 2;
			fileString = realloc(fileString, *maxSize * sizeof(*fileString));
		}
	}  

	free_stk(stk, NULL);

	fclose(inputFile);
	return error;
}


err_ret executeTree(fileTree *tree, vari *var, int maxSize){
	//checking the direction of program flow
	int8_t direction = 0;

	//checking conditionals: if/while
	int8_t check = 0;

	//check top of check stack
	int8_t checkTop = 0;

	//stack structure for nested conditionals
	int8_t *checkStack = calloc(maxSize, sizeof(*checkStack));
  
	//error 
	err_ret error = 0;

	//create new file stack
	stack *stk = new_stk(128);
  
	//executes the tree
	//checks that the current leaf and the string it holds are not NULL
	while((tree != NULL) && (tree->line != NULL)) {
		error = 0;
		//check whether to branch left or right down tree
		direction = checkProgramFlow(tree->line);

		//if the line ends with ';', don't print the line, the line still executes
		if((tree->line[strlen(tree->line)-1] != ';') && (direction == 0)) {
			printf("> %s\n", tree->line);
		}

		switch(direction) {
		case 1: //if
			check = checkConditional(tree->line, direction, var);

			checkStack[checkTop++] = check;
    
			if(check < 0) { //if there is an error in the if
				error = check;
				break;

			} else if(check) {
				//if the condition inside the if is true
				//push tree->left, and continue execution
				//from tree->right, the line after the if
				//everything inside the if executes
				//otherwise go directly to tree->left

				push(stk, tree->left);
				tree = tree->right;

			} else {
				tree = tree->left;
			}

			break;

		case 2: //while

			check = checkConditional(tree->line, direction, var);
			checkStack[checkTop] = check;

			if(check < 0){
				error = check;
				break;

			} else if(check){
				push(stk, tree);
				tree = tree->right;
	
			} else{
				tree = tree->left;
			}


			//push tree to continue execution from the while
			//continue execution from tree->right
			//when the loop comes back, the condition is rechecked
			//when the condition becomes false, go to the line after the while block
			break;

	
		case -1: //end 
			tree = pop(stk); //tree returns to whatever is on top of the stack
			break;

      
		case -2: //else

			//checks the value from the previous if iteration
			//when check is 0, the if condition was false, so the else block
			//executes
			//when the check is non 0, the if condition executed
			//the else block is skipped

			if(checkStack[--checkTop] == 0){
				push(stk, tree->left);
				tree = tree->right;

			} else { //check is true, continue after the else
				tree = tree->left;
			}
			break;

      
		default: //for executing non conditional lines
			error = sya(tree->line, var);

			if(error >= -1) {
				//print output
				if((tree->line[strlen(tree->line)-1] != ';') && (direction == 0))
					print_mat(var->ans);

				//continue execution going left
				tree = tree->left;
			}
			break;
		}

		if(error && (error != -1))
			break;
	}

	free_stk(stk, NULL);
	free(checkStack);

	if(error)
		printf("Error on line: %d\n", tree->num);

	return error;
}


//determine whether to branch left or right
int8_t checkProgramFlow(char *input) {
	if(strstr(input, "else")) return -2;
	if(strstr(input, "end")) return -1;
	if(strstr(input, "if(")) return 1;
	if(strstr(input, "while(")) return 2;
	return 0;
}


//determine if input string is while or if
char *parseCondition(char *input, int type) {

	//take advantage of the fact that strchr
	//searchs for first occurence of a letter

	switch(type){
	case 1: //if
		input = strchr(input, 'i');
		input += 2;
		break;
		
	case 2: //while
		input = strchr(input, 'w');
		input += 5;
		break;
	}

	return input;
}


//checks conditionals in while/if
int8_t checkConditional(char *input, int type, vari *var) {
	input = parseCondition(input, type);

	err_ret error = sya(input, var);
	if(error) return error; 

	//guarantee that the output is 1 or 0
	return !!var->ans->elements[0];
}


//free double array
void freeString(char **string, int max) {
	for(int i = 0; i < max; ++i){
		free(string[i]);
		string[i] = NULL;
	}
	free(string);
}
