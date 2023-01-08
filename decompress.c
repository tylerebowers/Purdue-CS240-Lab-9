#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

unsigned totfreq = 0;

int readIntFromFile(FILE * in_file){
	unsigned num;
	fread(&num, sizeof(unsigned), 1, in_file);
	return num;
}


unsigned char readCharFromFile(FILE * in_file){
	unsigned char let;
	fread(&let, sizeof(char), 1, in_file);
	return let;
}


NodeArray * createHistogramFromFreq(int * ascii_freq, int unique, int cmax){		
	int nIndex = 0;
	
	NodeArray * NA = initializeNodeArray(ascii_freq, unique); // new node array
	NA->nChars = unique;

	for(int i = 1; i <= cmax; i++){ // this adds them in order
		int j = 0;
		for(j = 0; j < 256; j++){
			if(ascii_freq[j] == i){
				NA->nodes[nIndex++] = newNode(j, ascii_freq[j]); // makea new node for the element 
			}
		}
	}
	return NA;
}


NodeArray * buildHufFromEncoded(FILE* in_file){
	int cmax = 0, uniqueNum = readIntFromFile(in_file); // this reads the number of unique chars
	unsigned ascii_freq[256] = {0};
	for(int i = 0; i < uniqueNum; i ++){
		int c = readCharFromFile(in_file); // this reads each char and qty from the file untill it has reached the nuber of chars
		int qty = readIntFromFile(in_file);
		totfreq += qty;
		ascii_freq[c] = qty;
		if(ascii_freq[c]>cmax){cmax = ascii_freq[c];} // this is the maximum qty ( it is used for sorting)
	}
	NodeArray * NA = createHistogramFromFreq(ascii_freq, uniqueNum, cmax); //this makes the histogram from frequency
	NodeArray * HT = buildHuffmanTree(NA); // make the huffman tree
	free(NA->nodes);
	NA->nodes = NULL;
	free(NA);
	NA=NULL;
	return HT;
}

NodeArray * uncompress(char * input_file, char* output_file){
	FILE *fin = fopen(input_file, "r");
	FILE *fout = fopen(output_file, "w");
	NodeArray * HT = buildHufFromEncoded(fin);
	int b, numchars = 0, gpos = 0, bufcap = 16;
	unsigned * buffer = (unsigned*)malloc(bufcap*sizeof(unsigned)); // the buffer should be malloced since we dont know how big it shoud be
	while((b = fgetc(fin)) != EOF){
		if(gpos >= bufcap){bufcap += 16;buffer = realloc(buffer, bufcap*sizeof(unsigned));} // check if we need to make it bigger
		for(int i = 7; i >= 0; i--){
			buffer[gpos++] = 1 & (b >> i);
		}
	}
	Node * curNode = HT->nodes[HT->nChars-1];
	for(int i = 0; i <= gpos; i ++){ // get the letter by using the whole huffman code
		if(curNode->left == NULL && curNode->right == NULL){
			fputc(curNode->val, fout);
			curNode = HT->nodes[HT->nChars-1];
			numchars++;
			i -= 1;
		} else if(buffer[i]){
			curNode = curNode->right;
		} else{
			curNode = curNode->left;
		}
		if(totfreq == numchars){break;}
	}
	fclose(fin);
	fclose(fout);
	free(buffer);
	buffer = NULL;
	return HT;
}
