#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

#define UINT_MAX 4294967295;

Node* newNode(char val, int frequency){ 
    Node* n = (struct Node*)malloc(sizeof(struct Node));
    n->left = n->right = NULL;
    n->val = val;
	n->isUsed = 0;
    n->frequency = frequency;
    return n;
}

void initParent(Node* parent, Node* left, Node* right){
	parent->frequency = left->frequency + right->frequency;
	parent->left = left;
	parent->right = right;
	parent->isUsed = 0;
	if(right->val < left->val) parent->val = right->val;
	else parent->val = left->val;
}

void resizeArrayIfNeeded(NodeArray * node_array){
	if(node_array->nChars == node_array->capacity){
		node_array->capacity++;
		node_array->nodes = realloc(node_array->nodes, node_array->capacity * (sizeof(struct Node**)));
	}
}

int getMinIndex(NodeArray*node_array){
	int index = -1, lowestChar = 256;
	unsigned freq = UINT_MAX;
	for(int i = 0; i < node_array->nChars; i++){
		if(((node_array->nodes[i]->frequency < freq) || (node_array->nodes[i]->frequency == freq) && (node_array->nodes[i]->val < lowestChar)) && (!node_array->nodes[i]->isUsed)){
			index = i;
			lowestChar = node_array->nodes[i]->val;
			freq = node_array->nodes[i]->frequency;
		}
	} 
	return index;
}

void insertParent(NodeArray * node_array, Node * parent){
	resizeArrayIfNeeded(node_array);
	int replaceIndex = node_array->nChars;
	for(int i = 0; i < node_array->nChars-1; i++){
		if(node_array->nodes[i]->frequency == parent->frequency){
			replaceIndex = i;
			break;
		}
	}
	for(int i = node_array->nChars-2; i >= replaceIndex; i--){
		if(i >= replaceIndex){
			node_array->nodes[i+1] = node_array->nodes[i];
		}
	}
	node_array->nodes[replaceIndex] = parent;
}

NodeArray* buildHuffmanTree(NodeArray*node_array){
	int min1Index, min2Index, nextIndex = 0;;
	NodeArray * HufTree = (NodeArray *) malloc(sizeof(struct NodeArray));
    HufTree->nChars = 0;
    HufTree->capacity = 2;
    HufTree->nodes = (struct Node**)malloc(HufTree->capacity * (sizeof(struct Node)));
	for(int i = 0; i < node_array->nChars; i++){
		resizeArrayIfNeeded(HufTree);
		HufTree->nodes[i] = node_array->nodes[i];
		HufTree->nChars++;
	}
	while(1){
		resizeArrayIfNeeded(HufTree);
		min1Index = getMinIndex(HufTree);
		if(min1Index == -1){break;}
		HufTree->nodes[min1Index]->isUsed = 1;
		min2Index = getMinIndex(HufTree);
		if(min2Index == -1){break;}
		HufTree->nodes[min2Index]->isUsed = 1;
		Node* par = (struct Node*)malloc(sizeof(struct Node));
		initParent(par, HufTree->nodes[min2Index], HufTree->nodes[min1Index]);
		HufTree->nodes[HufTree->nChars] = par;
		HufTree->nChars++;

	}
	return HufTree;
}

NodeArray * createHistogramFromFile(char * input_file, int * ascii_freq){	
	FILE * fin = fopen(input_file, "r");
	int c, count = 0, repet = 0;
	int max = 1, curfreq = 1, tmax, cmax = 0;;
	while ((c = fgetc(fin)) != EOF){
		if(ascii_freq[c]==0){count++;}
		ascii_freq[c]++;
		if(ascii_freq[c]>cmax){cmax = ascii_freq[c];}
	}
	fclose(fin);
	NodeArray * NA = initializeNodeArray(ascii_freq, count);
	repet = 0;
	NA->nChars = count;
	for(int i = 1; i <= cmax; i++){
		int j = 0;
		for(j = 0; j < 256; j++){
			if(ascii_freq[j] == i){
				NA->nodes[repet++] = newNode(j, ascii_freq[j]);
			}
		}
	}
	return NA;
}

NodeArray * initializeNodeArray(int * ascii_freq, int nChars){
	NodeArray * NA = (NodeArray *) malloc(sizeof(struct NodeArray));
    NA->nChars = 0;
    NA->capacity = nChars;
    NA->nodes = (struct Node**)malloc(nChars * (sizeof(struct Node)));
    return NA;
}

void clearNodeArray(NodeArray * node_array){
	for(int i = 0; i < node_array->nChars; i++){
		if(node_array->nodes[i] != NULL){
			free(node_array->nodes[i]);
			node_array->nodes[i] = NULL;
		}
	}
	free(node_array->nodes);
	node_array->nodes = NULL;
	free(node_array);
	node_array=NULL;
}

void printNodeArray(NodeArray * node_array){
	for (int i=0; i < node_array->nChars; i++){
		printf("%c, %u\n",node_array->nodes[i]->val,node_array->nodes[i]->frequency);
	}
}

void printTree(Node* parent, int * moves, int index){
	if (parent->left == NULL && parent->right == NULL){
		printf("%03d = %c\t",parent->val,parent->val);
		for(int i = 0; i < index; i++){
			printf("%d",moves[i]);
		}
		printf("\n");
	}
	if (parent->right) {
		moves[index] = 1;
		printTree(parent->right, moves, index + 1);
	}
	if (parent->left) {
		moves[index] = 0;
		printTree(parent->left, moves, index + 1);
	}
}

void printHuffmanTree(NodeArray * huffman_tree){
	int moves[256];
	Node * parent = huffman_tree->nodes[huffman_tree->nChars-1];
	printTree(parent, moves, 0);
}


