#ifndef __PACK_H__
#define __PACK_H__

typedef struct TreeNode {
	int number;
	int width;
	int height;
	int cut;
	int xcord;
	int ycord;
	struct TreeNode * left;
	struct TreeNode * right;
} TreeNode;

TreeNode * newNode();

TreeNode * buildTree(TreeNode * arrayofNodes, int count);

void findDimensions(TreeNode * Node);

void findCoords(TreeNode * Node);

void destroyTree(TreeNode * Node);

void printPreorder(FILE * fp2, TreeNode * Node);

void printPostorder(FILE * fp3, TreeNode * Node);

void printCoords(FILE * fp4, TreeNode * Node);

void readLine(FILE * fp, TreeNode * Node);

#endif
