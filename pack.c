#include <stdio.h>
#include <stdlib.h>
#include "pack.h"

TreeNode * newNode() {
	TreeNode * Node = malloc(sizeof(TreeNode));
	Node->number = 0;
	Node->width = 0;
	Node->height = 0;
	Node->cut = 0;
	Node->xcord = 0;
	Node->ycord = 0;
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

void findCoords(TreeNode * Node) {
	//To find the coordinates of each leaf node, they need to be in
	//reference to the cut lines.
	//
	//One thing to consider is determining which leaf node will
	//start with the coordinate (0,0). Given 0,0 references the bottom left
	//corner, one can observe that the block with that coordinate is
	//in the right subtree of an H cut and a left subtree of a V cut.
	//This means one can actually traverse nodes based on cuts to reach the
	//0,0 coordinate and the build the rest of the coordinates off this
	//reference point.
	//
	//All blocks are organized respective to the cutlines as such:
	//H cuts:
	//    Left node is above
	//    Right node is below
	//V cuts:
	//    Left Node is left
	//    Right Node is right
	//
	//A pattern to notice can be the coordinates of a block are
	//the width of the left V node plus the height of right H node
	//starting from the base 0,0 coordinates.
	//
	//However, there is another pattern to realize in a more recursive
	//manner:
	//Given the width and height of a cut node
	//    X coordinate of the right child of a V node is the 
	//    width of the left child
	//    
	//    X coordinate of the left child of a V node is zero at base
	//
	//    Y coordinates are the same for both children of a V node
	//
	//    X coordinates are the same for both children of an H node
	//
	//    Y coordinate of the left child of an H node is the height
	//    of the right child
	//
	//    Y coordinate of the right child of an H node is zero at base
	//
	//Implementing this recursive pattern implies every coordinate is 
	//built based on the child coordinates
	if (Node != NULL) {	
		
		if(Node->cut == 72) {
			Node->left->ycord += Node->right->height;
			if(Node->ycord != 0) {//Update starting coords if nonzero
				Node->left->ycord += Node->ycord;
				Node->right->ycord += Node->ycord;
			}
			else if(Node->xcord != 0) {
				Node->right->xcord += Node->xcord;
				Node->left->xcord += Node->xcord;
			}
		}
		else if (Node->cut == 86) {
			Node->right->xcord += Node->left->width;
			if(Node->xcord != 0) {
				Node->right->xcord += Node->xcord;
				Node->left->xcord += Node->xcord;
			}
			else if(Node->ycord != 0) {
				Node->left->ycord += Node->ycord;
				Node->right->ycord += Node->ycord;
			}
		}
		
		findCoords(Node->left);
		findCoords(Node->right);
		
	}
}

void printCoords(FILE * fp4, TreeNode * Node) {
	if (Node != NULL) {
		printCoords(fp4, Node->left);
		printCoords(fp4, Node->right);

		if(Node->cut == 0) {
			fprintf(fp4, "%d((%d,%d)(%d,%d))\n", Node->number, Node->width, Node->height, Node->xcord, Node->ycord);
		}
	}
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
		Node->xcord = 0;
		Node->ycord = 0;
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
		Node->xcord = 0;
		Node->ycord = 0;
		Node->left = NULL;
		Node->right = NULL;
		
		fscanf(fp, "%d(%d,%d)\n", &Node->number, &Node->width, &Node->height);
	}

}
