#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

#include "sya.h"
#include "fileStruct.h"
#include "file.h"


err_ret runFile(char **input, struct vari *var, int offset) {
	//error variable
	err_ret error = 0;

	fileTree *tree = new_leaf();
	if(!tree)
		return -6;

	//make tree structure
	if( !(error = createTree(input[0], tree, offset)) )
		error = executeTree(tree, var);

	//free tree
	cutDownTree(tree);

	return error;
}


//create and populate tree
err_ret createTree(char *fileName, fileTree *tree, int skip){
	//file to read from
	FILE *inputFile = fopen(fileName, "r");
	if(!inputFile)
		return -8;

	//error checking
	err_ret error = 0;

	//size of char buffer that each line of the file is copied too
	char *buffer = malloc(BUFF_SIZE * sizeof(*buffer));
	if(!buffer)
		return -6;


	//stack data structure convenient for creating tree
	struct stack *stk = new_stk(128);

	for(int i = 0; i < skip; ++i)
		fgets(buffer, BUFF_SIZE, inputFile); 
 
	//creates the tree structure
	for(int i = 0; fgets(buffer, BUFF_SIZE, inputFile) && !error; ++i) {

		int offset = 0;

		//removing trailing spaces
		for(; buffer[offset] == ' '; ++offset);

		char *bufferHold = buffer + offset;
		int len = strlen(bufferHold);

		//skips a blank line, # comments out a line
		if(!strcmp(bufferHold, "\n") || (bufferHold[0] == '#'))
			continue;

		//replaces end new line with a null terminated character
		if(bufferHold[len - 1] == '\n')
			bufferHold[--len] = '\0';

		//put line into tree struct
		tree->line = malloc(sizeof(*tree->line) * (len + 1));
		if(!tree->line) {
			error = -6;
			break;
		}

		strcpy(tree->line, bufferHold);

		//set line number
		tree->num = i + 2;

		switch(checkProgramFlow(tree->line)) {

			//the first statement in a while/if/else branch right
		case 1: //if
		case 2: //while
		case -2: //else
			push(stk, tree);
			if( (tree->right = new_leaf()) ) {
				tree = tree->right;
			} else {
				error = -6;
			}
			break;

			//end
			//signifies end of while/if/else 
		case -1:
			tree = pop(stk);

			//fall through
		default:
			if( (tree->left = new_leaf()) ) {
				tree = tree->left;
			} else {
				error = -6;
			}

			break;
		}
	}  

	free_stk(stk, NULL);
	free(buffer);

	fclose(inputFile);

	return error;
}


err_ret executeTree(fileTree *tree, struct vari *var){
	//stack structure for nested conditionals
	int8_t *checkStack = calloc(BUFF_SIZE / 4, sizeof(*checkStack));
	if(!checkStack)
		return -6;

	//top of check stack
	int8_t top = 0;

	//error 
	err_ret error = 0;

	//create new file stack
	struct stack *stk = new_stk(128);
	if(!stk) {
		free(checkStack);
		return -6;
	}
  
	//executes the tree
	//checks that the current leaf and the string it holds are not NULL
	for(;tree && tree->line;) {
		//check whether to branch left or right down tree
		int8_t dir = checkProgramFlow(tree->line);

		switch(dir) {
		case 1: //if
			checkStack[top] = checkConditional(tree->line, dir, var);

			if(checkStack[top]) {
				//error in if
				if(checkStack[top] < 0) {
					error = checkStack[top];
					break;
				}

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
			
			++top;
			
			break;

		case 2: //while
			checkStack[top] = checkConditional(tree->line, dir, var);

			if(checkStack[top]) {
				if(checkStack[top] < 0) {
					error = checkStack[top];
					break;
				}

				push(stk, tree);
				tree = tree->right;
	
			} else {
				tree = tree->left;
			}


			//push tree to continue execution from the while
			//continue execution from tree->right
			//when the loop comes back, the condition is rechecked
			//when the condition becomes false, go to the line after the while block
			break;

	
		case -1: //end
			//tree returns to whatever is on top of the stack
			if( !(tree = pop(stk)) )
				error = -5;

			break;

      
		case -2: //else

			//checks the value from the previous if iteration
			//when check is 0, the if condition was false, so the else block
			//executes
			//when the check is non 0, the if condition executed
			//the else block is skipped

			if(!checkStack[--top]) {
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
				if((tree->line[strlen(tree->line)-1] != ';') && !dir) {
					printf("> %s\n", tree->line);
					print_mat(var->ans);
				}

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

	switch(type) {
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
int8_t checkConditional(char *input, int type, struct vari *var) {
	input = parseCondition(input, type);

	err_ret error = sya(input, var);

	return error ? error : !!var->ans->elements[0];
}
