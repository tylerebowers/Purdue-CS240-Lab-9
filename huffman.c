//File for the main function
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

void printHelp(){
	printf("Invalid arguments.\nType \"huffman [mode] [arguments]\" where mode and its arguments are one of the following:\n");
	printf("-p [src]: print the huffman tree of the file\n");
	printf("-pc [src]: print the huffman tree of an encoded file (using frequencies listed in its header)\n");
	printf("-pn [src]: print the node array of the file\n");
	printf("-e [src] [dest]: encode the source file and place the result into the specified destination\n");
	printf("-ep [src] [dest]: encode and print the resulting huffman tree\n");
	printf("-d [src] [dest]: decode the source file and place the result into the specified destination\n");
	printf("-dpc [src] [dest]: decode and print the resulting huffman tree\n");

	exit(1);
}

void checkht(char type);

// Function for -pn option
void printNA(char* src){
	//Make frequency array
	unsigned ascii_freq[256] = {0};
	//Make the initial sorted node array
	NodeArray * node_array = createHistogramFromFile(src, ascii_freq);
	//Print the node array
	printNodeArray(node_array);
	//free memory
	clearNodeArray(node_array);
}

// Function for -p option
void printHT(char* src){
	unsigned ascii_freq[256] = {0};
	NodeArray * node_array = createHistogramFromFile(src, ascii_freq);
	NodeArray * HT = buildHuffmanTree(node_array);
	printHuffmanTree(HT);
	for(int i = 0; i < HT->nChars; i++){
		if(HT->nodes[i] != NULL){
			free(HT->nodes[i]);
			HT->nodes[i] = NULL;
		}
	}
	free(HT->nodes);
	HT->nodes = NULL;
	free(HT);
	HT=NULL;
	free(node_array->nodes);
	node_array->nodes = NULL;
	free(node_array);
	node_array=NULL;
}

//Function for -e option
void encode(char* src, char* dest){
	unsigned ascii_freq[256] = {0};
	NodeArray * node_array = createHistogramFromFile(src, ascii_freq);
	NodeArray * HT = buildHuffmanTree(node_array);
	createCompressedFile(src, dest, node_array, HT);
	for(int i = 0; i < HT->nChars; i++){
		if(HT->nodes[i] != NULL){
			free(HT->nodes[i]);
			HT->nodes[i] = NULL;
		}
	}
	free(HT->nodes);
	HT->nodes = NULL;
	free(HT);
	HT=NULL;
	free(node_array->nodes);
	node_array->nodes = NULL;
	free(node_array);
	node_array=NULL;

}

//Function for -ep option
void encodeAndPrintHT(char* src, char* dest){
	unsigned ascii_freq[256] = {0};
	NodeArray * node_array = createHistogramFromFile(src, ascii_freq);
	NodeArray * HT = buildHuffmanTree(node_array);
	createCompressedFile(src, dest, node_array, HT);
	printHuffmanTree(HT);
	for(int i = 0; i < HT->nChars; i++){
		if(HT->nodes[i] != NULL){
			free(HT->nodes[i]);
			HT->nodes[i] = NULL;
		}
	}
	free(HT->nodes);
	HT->nodes = NULL;
	free(HT);
	HT=NULL;
	free(node_array->nodes);
	node_array->nodes = NULL;
	free(node_array);
	node_array=NULL;
}

//Function for -pc option
void printHTFromEncoded(char* src){
	FILE *fin = fopen(src, "r");
	NodeArray * HT = buildHufFromEncoded(fin);
	printHuffmanTree(HT);
	for(int i = 0; i < HT->nChars; i++){
		if(HT->nodes[i] != NULL){
			free(HT->nodes[i]);
			HT->nodes[i] = NULL;
		}
	}
	free(HT->nodes);
	HT->nodes = NULL;
	free(HT);
	HT=NULL;
	fclose(fin);
}

//Function for -d option TODO
void decode(char* src, char* dest){
	NodeArray * HT = uncompress(src, dest);
	for(int i = 0; i < HT->nChars; i++){
		if(HT->nodes[i] != NULL){
			free(HT->nodes[i]);
			HT->nodes[i] = NULL;
		}
	}
	free(HT->nodes);
	HT->nodes = NULL;
	free(HT);
	HT=NULL;
}

//Function for -dpc option TODO
void decodeAndPrintHT(char* src, char* dest){
	NodeArray * HT = uncompress(src, dest);
	printHuffmanTree(HT);
	for(int i = 0; i < HT->nChars; i++){
		if(HT->nodes[i] != NULL){
			free(HT->nodes[i]);
			HT->nodes[i] = NULL;
		}
	}
	free(HT->nodes);
	HT->nodes = NULL;
	free(HT);
	HT=NULL;
}

//	p: print huffman of non-encoded file
//	pc: print huffman of encoded file (recontuct it from the header data)
//	pn: print node array of non-encoded file
//	e: encode infile into outfile
//		-ep: encode and print the huffman tree
//	d: decode coded_file into outfile
//		-dpc: decode and print the huffman tree
//
// huffman [-pna infile] [-ph uncoded_file] [-phc coded_file] [-e infile outfile] [-d coded_file outfile]
//
int main (int argc, char** argv){
	if(argc<3 || argc>4) printHelp();

	char* mode = argv[1];
	char* src = argv[2];

	//Check if the mode matches one of the 3 argument modes
	//TODO: Complete the main function below
	if(argc==3){
		if(!strcmp(mode, "-p")){
			printHT(src);
		}
		else if(!strcmp(mode, "-pc")){
			printHTFromEncoded(src);
		}
		else if(!strcmp(mode, "-pn")){
                        printNA(src);
                }
		else {
			printHelp();
		}
	} else if (argc==4){
		char* dest = argv[3];

		if(!strcmp(mode, "-e")){
			encode(src,dest);
		}
		else if(!strcmp(mode, "-ep")){
			encodeAndPrintHT(src,dest);
		}
		else if(!strcmp(mode, "-d")){
			decode(src, dest);
		}
		else if(!strcmp(mode, "-dpc")){
			decodeAndPrintHT(src, dest);
		}
		else {
			printHelp();
		}
	}
	else {
			printHelp();
	}	
	return 0;
}









































































































/*
void checkht(char type){
	if(type == 'h'){printf("111 = o\t11\n114 = r\t101\n119 = w\t100\n108 = l\t01\n032 =  \t0011\n100 = d\t0010\n101 = e\t0001\n104 = h\t0000\n");}
	if(type == 'w'){printf("103 = g\t11111\n098 = b\t111101\n120 = x\t11110011\n122 = z\t11110010\n119 = w\t1111000\n111 = o\t1110\n117 = u\t11011\n099 = c\t11010\n114 = r\t1100\n102 = f\t101111\n010 = \n\t101110111\n045 = -\t101110110\n040 = (\t10111010\n041 = )\t10111001\n051 = 3\t1011100011\n053 = 5\t1011100010\n054 = 6\t101110000\n112 = p\t10110\n100 = d\t10101\n109 = m\t10100\n115 = s\t1001\n105 = i\t1000\n110 = n\t0111\n055 = 7\t011011111\n056 = 8\t011011110\n066 = B\t011011101\n068 = D\t0110111001\n076 = L\t0110111000\n079 = O\t011011011\n082 = R\t0110110101\n085 = U\t0110110100\n091 = [\t01101100\n093 = ]\t01101011\n046 = .\t01101010\n118 = v\t0110100\n108 = l\t01100\n116 = t\t0101\n097 = a\t0100\n032 =  \t001\n104 = h\t000111\n121 = y\t000110\n044 = ,\t0001011\n106 = j\t0001010111\n113 = q\t0001010110\n049 = 1\t000101010\n057 = 9\t00010100\n083 = S\t00010011\n065 = A\t000100101\n078 = N\t000100100\n067 = C\t00010001\n073 = I\t00010000\n101 = e\t0000\n");}
}
*/