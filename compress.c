//File for functions focused on compression/encoding
#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

//recursive implementation of searching through the tree
void generateHufCode(int c, Node* parent, int * moves, int index, FILE * fout){ // yes, ther is a more efficient way to do this (direct path) but I was tired and this works.
	if (parent->left == NULL && parent->right == NULL){ // check if leaf node
		if(c == parent->val){ // check if node that is being searched for
			for(int i = 0; i < index; i++){ // index is the size of the moves array
				bit_buffer <<= 1;         
				if (moves[i]) bit_buffer |= 1; // the moves array acts as a log to store if we have gone left or right
				bufcount++;              
				if (bufcount == 8) { // check if the buffer is 8 and needs to be printed to the file
					fwrite(&bit_buffer, sizeof(bit_buffer), 1, fout); //write the byte
					bit_buffer = 0; // reset counters
					bufcount = 0;
				}
			}
		}
	}
	if (parent->right) { // go right on the huffman tree
		moves[index] = 1; //log move
		generateHufCode(c, parent->right, moves, index + 1, fout); // recursive call
	}
	if (parent->left) { // go left on the huffman tree
		moves[index] = 0; //log move
		generateHufCode(c, parent->left, moves, index + 1, fout); // recursive call
	}
}

void writeBufLeftovers(FILE * fout){ //if we have an incomplete byte this will check and write it.
	if((8-bufcount) && (bufcount != 0)){
		bit_buffer <<= 8-bufcount;                   
		fwrite(&bit_buffer, sizeof(bit_buffer), 1, fout); 
		bit_buffer = 0;
		bufcount = 0;
	}
}

void createCompressedFile(char* input_file, char * output_file, NodeArray * node_array, NodeArray * huffman_tree){
	FILE *fout = fopen(output_file, "w");
	FILE *fin = fopen(input_file, "r");
	fwrite(&node_array->nChars, sizeof(unsigned), 1, fout); // write number of unique chars
	for(int i = 0; i < node_array->nChars; i++){
		putc(node_array->nodes[i]->val, fout);
		fwrite(&node_array->nodes[i]->frequency, sizeof(unsigned), 1, fout); //write each char and number combo
	}
	bit_buffer = 0;
	bufcount = 0;
	int c;
	while((c = fgetc(fin)) != EOF){
		int moves[256] = {0};
		generateHufCode(c, huffman_tree->nodes[huffman_tree->nChars-1], moves, 0, fout); // assemble huffman code and write to file
	}
	writeBufLeftovers(fout); // write the left oferver of the huffman buffer
	fclose(fout);
	fclose(fin);

}
