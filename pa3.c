#include <stdio.h>
#include <stdlib.h>
#include "pack.h"

int main (int argc, char ** argv) {
	if (argc != 5) {
		fprintf(stderr, "Incorrect number of arguments.\n");
		return EXIT_FAILURE;
	}

	FILE *fp = fopen(argv[1], "r");
	if(fp == NULL) {
		fprintf(stderr, "Invalid argument argv[1].\n");
		return EXIT_FAILURE;
	}
	//The input file will contain a strictly binary tree in postorder
	//containing the block and the cutline nodes describing the packing.
	//The first output will be the preorder of the given postorder binary
	//tree, which will be done by reading the input data and building
	//the preorder binary tree based on the structure of the preorder tree
	
	//If a postorder input binary tree is as
	//3(3,3)
	//1(5,4)
	//2(7,7)
	//V
	//H
	
	//Because the tree is strictly binary, the preorder is able to be 
	//determined by this pattern
	// -The last node is the root of the preorder
	// -If the preceding node is a single Character (H or V), it is
	// a non leaf node and will have 2 children
	// -If the preceding node is an integer, it is a leaf node
	
	int count = 0;
	//First determine how many nodes are in the input by counting the
	//number of newline elements
	char ch;
	while ((ch=fgetc(fp)) != EOF) {
		if (ch == '\n') {
			count++;
		}
	}
	
	TreeNode * arrayofNodes = malloc(sizeof(*arrayofNodes) * count);
	fseek(fp, 0L, SEEK_SET);
	for (int i = 0; i < count; i++) {
		readLine(fp, &arrayofNodes[i]);
	}

	//The file will be read using a function to distinguish int and char
	//values being read by the fgetc/fread functions. Each data value will
	//be divided by the \n character, and if a character being read by fgetc
	//is not equal to the value for U or V, it will be considered a leaf
	//node, and hence the data will be interpreted as integers instead
	//using fread.
	
	//A malloc will first be created to store the input TreeNode values.
	//ReadLine will read one line of an input until the \n character,
	//and store the read data into the Node.
	
	TreeNode * root;

	root = buildTree(arrayofNodes, count);
	
	FILE * fp2 = fopen(argv[2], "w");
	if(fp2 == NULL) {
		fprintf(stderr, "Invalid argument argv[2].\n");
		return EXIT_FAILURE;
	}

	printPreorder(fp2, root);


	FILE * fp3 = fopen(argv[3], "w");
	if(fp3 == NULL) {
		fprintf(stderr, "Invalid argument argv[3].\n");
		return EXIT_FAILURE;
	}
	findDimensions(root);
	printPostorder(fp3, root);

	FILE * fp4 = fopen(argv[4], "w");
	if(fp4 == NULL) {
		fprintf(stderr, "Invalid argument argv[4].\n");
		return EXIT_FAILURE;
	}
	findCoords(root);
	printCoords(fp4, root);

	destroyTree(root);
	free(arrayofNodes);
	fclose(fp);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	return EXIT_SUCCESS;
}
