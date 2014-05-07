#include "stree.c"

#include "mapread.c"
#include "dptable.c"
#include <errno.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/types.h>




int validateArgs(const int argc, const char **argv) {
	if (argc != 4) {
		printf("\nERROR: Please check your input.\n");
		printf("Format: <test executable> <input fasta> <reads fasta> <input alphabet>\n\n");
		return -1;
	}

	printf("\nPlease verify the following information:\n\n");
	printf("Input File:\t%s\n", argv[1]);
	printf("Input Read:\t%s\n", argv[2]);
	printf("Input Alph:\t%s\n", argv[3]);

	printf("\nIf any of this is wrong, press Ctrl+C to panic.\nOtherwise press ENTER to continue...");
	getchar();
	return (0);
}

int setUp(const char ** argv) {

	char inputname[64];
	char readsname[64];
	char alphaname[64];

	struct stat input_st;
	struct stat reads_st;
	struct stat alpha_st;

	stat(argv[1], &input_st);
	stat(argv[2], &reads_st);
	stat(argv[3], &alpha_st);

	unsigned int input_size = input_st.st_size;
	unsigned int reads_size = reads_st.st_size;
	unsigned int alpha_size = alpha_st.st_size;

	strcpy(inputname, argv[1]);
	strcpy(readsname, argv[2]);
	strcpy(alphaname, argv[3]);

	FILE *inputfile;
	FILE *readsfile;
	FILE *alphafile;

	if ((inputfile = fopen(inputname, "r")) == NULL) {
		if (errno == ENOENT) {
			printf("\nERROR: No sequence file found.\n");
			return -1;
		}
		else {
			printf("\nERROR: Unhandled sequence read error.\n");
			return -1;
		}
	}

	if ((readsfile = fopen(readsname, "r")) == NULL) {
		if (errno == ENOENT) {
			printf("\nERROR: No reads sequence file found.\n");
			return -1;
		}
		else {
			printf("\nERROR: Unhandled reads sequence read error.\n");
			return -1;
		}
	}

	if ((alphafile = fopen(alphaname, "r")) == NULL) {
		if (errno == ENOENT) {
			printf("\nERROR: No sequence file found.\n");
			return -1;
		}
		else {
			printf("\nERROR: Unhandled sequence read error.\n");
			return -1;
		}
	}

	ibuff = (char*)malloc(input_size);
	if (ibuff == NULL) {
		printf("\nERROR: Cannot allocate memory for sequence.\n");
		return -1;
	}

	// rbuff allocation is in the read section	

	abuff = (char*)malloc(alpha_size);
	if (abuff == NULL) {
		printf("\nERROR: Cannot allocate memory for alphabet.\n");
		return -1;
	}

	unsigned int i = 0;

//	int ibytes = input_size;
//	int rbytes = reads_size;
//	int abytes = alpha_size;
	char inchar = '\0';

	// read in name of sequence
	 do {
		inchar = fgetc(inputfile);
		if (inchar == '>') {
			inchar = fgetc(inputfile);
			while (inchar != '\n'  && inchar == '|') {
				if (i < 15) {
					iname[i] = inchar;
					++i;
				}
				inchar = fgetc(inputfile);
			}
			iname[i] = '\0';
		}
	} while (inchar != '\n' && inchar != EOF);

	// read in the sequence and ++inputLen
	do {
		inchar = fgetc(inputfile);
		while (inchar != EOF) {
			if (inchar != ' ' && inchar != '\n') {
				ibuff[inputLen] = inchar;
				++inputLen;
			}
			inchar = fgetc(inputfile);
		}
	} while (inchar != EOF);
	ibuff[inputLen] = '$';
	inputLen++;
	ibuff[inputLen] = '\0';

	// get total number of lines in read
	int numlines = 0;
	int longRead = 0;
	int curRead  = 0;

	do {
		inchar = fgetc(readsfile);
		++curRead;
		if (inchar == '\n') {
			++numlines;
			if (curRead > longRead) {	// also get max line length
				longRead = curRead;
				curRead = 0;
			}
		}
	} while (inchar != EOF);

	fseek(readsfile, 0, SEEK_SET);	// reset to beginning

	// allocate reads indexing array
	rbuff = (char*)malloc(reads_size);
	if (rbuff == NULL) {
		printf("\nERROR: Cannot allocate memory for reads sequence.\n");
		return (-1);
	}
	
	// read in reads
	readsList = (char**)malloc(numlines * sizeof(char*));
	if (readsList == NULL) {
		printf("\nERROR: Cannot allocate memory for readsList.\n");
		return (-1);
	}
	
	int readIndex = 0;
	int currReadLen = 0;
	int maxReadLen = 0;
	
	// read in reads, names, and make a readsList
	do {
		inchar = fgetc(readsfile);
		if (inchar == '>') {		// read in a name
			inchar = fgetc(readsfile);
			readsList[readIndex] = &rbuff[readsLen];
			++readIndex;
			while(inchar != '\n' && inchar != EOF) {
				rbuff[readsLen] = inchar;
				++readsLen;
				inchar = fgetc(readsfile);
			}
			//rbuff[readsLen] = '\0'; 
			//++readsLen;
		}
		else if(inchar != EOF){
			readsList[readIndex] = &rbuff[readsLen];
			++readIndex;
			++numReads;
			while (inchar != '\n' && inchar != EOF) {
//				if (inchar == 'N')		for banana only
//					rbuff[readsLen] = 'A';
//				else
					rbuff[readsLen] = inchar;
				++readsLen;
				++currReadLen;
				inchar = fgetc(readsfile);
			}
		}
		if (currReadLen > maxReadLen) {
			maxReadIndex = readIndex;
			maxReadLen = currReadLen;
			currReadLen = 0;
		}
		if (inchar == '\n') {
			rbuff[readsLen] = '\0';
			++readsLen;
		}
	} while (inchar != EOF);
	rbuff[readsLen] = '\0';

	// read in alphabet
	do {
		inchar = fgetc(alphafile);
		if (inchar != ' ' && inchar != '\n' && inchar != EOF) {
			abuff[alphabetLen] = inchar;
			++alphabetLen;
		}
	} while (inchar != EOF);
	abuff[alphabetLen] = '\0';

	fclose(inputfile);
	fclose(readsfile);
	fclose(alphafile);

	return (0);
}

int main(const int argc, const char *argv[]){
	if(validateArgs(argc, argv)<0){
		return -1;
	}
	setUp(argv);
	Node *root = suffixTree();
	
	prepareST(root);

	mapReads(root);



	return 0;
}
