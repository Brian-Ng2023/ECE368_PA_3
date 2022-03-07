#include <stdio.h>
#include <stdlib.h>
#include "pack.h"

TreeNode * newNode() {
	TreeNode * Node = malloc(sizeof(TreeNode));
	Node->number = 0;
	Node->width = 0;
	Node->height = 0;
	Node->cut = 0;
	Node->left = NULL;
	Node->right = NULL;
	return Node;
}

//Comparison helper function for finding dimensions
static int findLargest(TreeNode * left, TreeNode * right, int input) {
	if (input == 0) { //Input 0 compares width
		return ((left->width) - (right->width));
	}
	else { //Input 1 compares height
		return ((left->height) - (right->height));
	}
}

static void buildTreeHelper(TreeNode * Node, TreeNode * arrayofNodes, int * index) {
	//This helper function starts the recursive process for building
	//the tree with a starting input of an empty root node and an index
	//for the arrayofNodes.
	
	//With an end condition that *index < 0, first start by assigning
	//the input node to the corresponding Node in the array at the 
	//input index. If the assigned Node is determined to not have a cut
	//value assigned, the index is decremented and the function is returned.
	//Otherwise, if it is a cut, this means it is a non-leaf node. The
	//index is decremented and a recursive call is made first for the right
	//node then the left node, as the tree is being built from a reverse
	//preorder.
	
	//This recursive process should mean that if a leaf node is hit i.e.
	//the cut variable is NULL, the recursive function stops itself
	//and goes back up one level in the tree. Once the left and right
	//subtrees are filled, the function also returns, which completes
	//the non-leaf node to either complete at least one group of cuts
	//in the packing or the entire binary tree. 
	
	//Because the index is a pointer, the array can be properly updated
	//after each recursive call when a Node is inserted into the tree.
	if(*index < 0) {
		return;
	}
	*Node = arrayofNodes[*index];
	if(Node->cut == 0) {
		*index -= 1;
		return;
	}
	*index -= 1;
	Node->left = newNode();
	Node->right = newNode();

	//Node->left = Node->right = NULL;

	buildTreeHelper(Node->right, arrayofNodes, index);
	buildTreeHelper(Node->left, arrayofNodes, index);
	return;			
}

TreeNode * buildTree(TreeNode * arrayofNodes, int count) {
	TreeNode * root;
	root = newNode();
	int * index = malloc(sizeof(*index));
	*index = count - 1;

	buildTreeHelper(root, arrayofNodes, index);
	free(index);
	return root;
}

void destroyTree(TreeNode * Node) {
	if (Node == NULL){
		return;
	}
	destroyTree(Node->left);
	destroyTree(Node->right);
	free(Node);
}

void printPreorder (FILE * fp2, TreeNode * Node) {
	//Preorder printing of the tree
	if (Node != NULL) {
		if(Node->cut != 0) {
			fprintf(fp2, "%c\n", Node->cut);
		}
		else {
			fprintf(fp2, "%d(%d,%d)\n", Node->number, Node->width, Node->height);
		}

		printPreorder(fp2, Node->left);
		printPreorder(fp2, Node->right);
	}
}

void printPostorder(FILE * fp3, TreeNode * Node) {
	//Postorder printing of the tree (for dimensions)
	if (Node != NULL) {
		printPostorder(fp3, Node->left);
		printPostorder(fp3, Node->right);

		if(Node->cut != 0) {
			fprintf(fp3, "%c(%d,%d)\n", Node->cut, Node->width, Node->height);
		}
		else {
			fprintf(fp3, "%d(%d,%d)\n", Node->number, Node->width, Node->height);
		}
	}
}

void findDimensions(TreeNode * Node) {
	//To find the dimensions of the cut nodes, they can be obtained by
	//the pattern:
	//H cut:
	//    Width = largest width between child nodes
	//    Height = sum of heights between child nodes
	//
	//V cut:
	//    Width = sum of width's between child nodes
	//    Height = largest height between child nodes
	//
	//This pattern will be taken recursively by traversing the postorder
	//of the tree
	
	if (Node != NULL) {
		findDimensions(Node->left);
		findDimensions(Node->right);
		
		if (Node->cut == 72) {
			if (findLargest(Node->left, Node->right, 0) >= 0) {
				Node->width = Node->left->width;
			}
			else {
				Node->width = Node->right->width;
			}
			Node->height = Node->left->height + Node->right->height;
		}
		else if (Node->cut == 86) {
			if (findLargest(Node->left, Node->right, 1) >= 0) {
				Node->height = Node->left->height;
			}
			else {
				Node->height = Node->right->height;
			}
			Node->width = Node->left->width + Node->right->width;
		}
		return;	
	}
	return;
}

void readLine(FILE * fp, TreeNode * Node) {
	int ch = 0;
	ch = fgetc(fp);
	if (ch == 72 || ch == 86) {
		//When reading each input, initialize each Node and store the data
		//into the proper Node value
		Node->number = 0;
		Node->width = 0;
		Node->height = 0;

		Node->left = NULL;
		Node->right = NULL;
	
		Node->cut = ch;
		//Pass over newline
		fgetc(fp);
	}
	//If the read element is not a cut, store it as an integer pattern
	else {
		fseek(fp, -1, SEEK_CUR);
		Node->cut = 0;
		Node->left = NULL;
		Node->right = NULL;
		
		fscanf(fp, "%d(%d,%d)\n", &Node->number, &Node->width, &Node->height);
	}

}
