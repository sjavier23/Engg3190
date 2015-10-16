	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	#include<dirent.h>
	#include<stdbool.h>
	#include<math.h>

	/* Function that converts any decimal between 0-16 to binary */
	char * toBinary(char * dec){

		int newDec = atoi(dec);
		char * binArray[17];
		char * newNum;

		binArray[0] = "0000";
		binArray[1] = "0001";
		binArray[2] = "0010";
		binArray[3] = "0011";
		binArray[4] = "0100";
		binArray[5] = "0101";
		binArray[6] = "0110";
		binArray[7] = "0111";
		binArray[8] = "1000";
		binArray[9] = "1001";
		binArray[10] = "1010";
		binArray[11] = "1011";
		binArray[12] = "1100";
		binArray[13] = "1101";
		binArray[14] = "1110";
		binArray[15] = "1111";

		return binArray[newDec];
	}

	int toMinterm(char * string){
		int sum, j, s;
		
		for(sum=0, j=0, s=strlen(string)-1; s >= 0; s--, ++j){
		    if(string[s] == '1'){
		        sum = sum + pow(2,j);
		    }
		}
		
		return sum;
	}

	/* Compares two binary strings to calculate the hamming distance. */
	int compareBinary(char * bin1, char * bin2){
		int i =0, numMatches=0;
		for(i=0; i<4;i++){
			if(bin1[i]==bin2[i]){
				numMatches++;
			}
		}
		return numMatches;
	}

	/* Generates binary strings when the hamming distance between to primes is 1. */
	char * combinePrimes(char * bin1, char * bin2){
		int i = 0;
		char * combinedPrimes=malloc(sizeof(char)*5);

		for(i=0;i<4;i++){
			if(bin1[i]==bin2[i]){
				combinedPrimes[i]=bin1[i];
			}
			else{
				combinedPrimes[i]='-';
			}
		}
		combinedPrimes[4] = '\0';
		return combinedPrimes;
	}

	/* Takes in one binary string, an array, and the size. Traverses array to find if input string already exists. */
	bool alreadyExists(char * newBin, char ** binArray, int arraySize){
		int i =0;
		if(arraySize==0){
			return false;
		}

		for(i=0; i<arraySize;i++){
			if(strcmp(newBin, binArray[i])==0){
				return true;
			}
		}
		return false;
	}

	/* Start Main */
	int main(void){

	// File I/O Variables //
	FILE * input_file, * output_file, * buffer_file;

	// Integers needed for algorithm //
	int numTokens=0, i=0, j=0, k=0;
	int numPrimes=0, numDontCares=0, firstArrayRows=0, secondArrayRows=0, thirdArrayRows=0;
	int hammingDistance = 0, numMatches = 0, numEssentials = 0;

	// Char variables needed for algorithm //
	char buffer[512], * tokenTemp, *primes, *dontCare;
	char *tokenLine[10], *primeLine[20], *dontCareLine[20];

	// String arrays //
	char * firstArray[17];
	char * secondArray[17];
	char * thirdArray[17];
	char * primeArray[17];
	char a;

	// 2d int array for prime table //
	int ** primeTable;


	input_file = fopen("input.txt", "a+");
	output_file = fopen("output.txt", "w");
	
	// Read through file line by line to prepare for parsing //
	while(fgets(buffer, sizeof(buffer), input_file)){
		fprintf(output_file, "%s", buffer);
		
		tokenTemp = strtok(buffer,".d.m.i");
		// Parse the input using new lines as tokenizer
		while(tokenTemp!=NULL){
		tokenLine[i] = strdup(tokenTemp);
		tokenTemp = strtok(NULL, "\n");
		i++;
		}
	}

	// // Store minterms //
	primes = strtok(tokenLine[3], " .m");
	while(primes!=NULL){
		primeLine[j] = strdup(primes);
		primes = strtok(NULL, " .m");
		j++;
		numPrimes++;
	}

	// // Store dontcares //
	dontCare = strtok(tokenLine[5], " .d");
	while(dontCare!=NULL){
		dontCareLine[k] = strdup(dontCare);
		dontCare = strtok(NULL, " .d");
		//printf("%s\n" , dontCareLine[k]);
		k++;
		numDontCares++;
	}

	// Add number of primes and dont cares to find size of first array.
	firstArrayRows = numPrimes+numDontCares;

	/* Allocate 2d prime table array, now that we know # minterms */
	primeTable = (int**)malloc(10*sizeof(int));
	for(i=0;i<10;i++){
		primeTable[i] = (int*)malloc(firstArrayRows*sizeof(int));
	}

	// Place primes and dont cares into step 1 2d array //
		for(i=0; i<numPrimes; i++){
			//primeTable[0][i]=atoi(primeLine[i]);
			//printf("Entry: %d\n",primeTable[0][i]);
			firstArray[i]=toBinary(primeLine[i]);
		}

		for(j=numPrimes; j<firstArrayRows;j++){
			//primeTable[0][j]=atoi(dontCareLine[j-numPrimes]);
			//printf("Entry: %d\n",primeTable[0][j]);
			firstArray[j]=toBinary(dontCareLine[j-numPrimes]);
		}

   fprintf(output_file, ".p\n");

	// Iterate through list of primes/dont cares, calculate the hamming distance, and store 
	for(i=0; i<firstArrayRows;i++){
		numMatches = 0;
		
		for(k=0; k<firstArrayRows;k++){

		// Copy pointer to string in order to separate by char (needed for hamming calculation)
		char firstString[5], secondString[5];
		strcpy(firstString, firstArray[i]);
		strcpy(secondString, firstArray[k]);

		hammingDistance = compareBinary(firstString, secondString);

			// If the hamming distance is 3, check if already exists in array. If not, push into array.
			if(hammingDistance==3){
				char *combinedPrime = combinePrimes(firstString, secondString);

					if(alreadyExists(combinedPrime,secondArray,secondArrayRows)==false){
						secondArray[secondArrayRows] = combinedPrime;
						numMatches++;
						secondArrayRows++;
					}

					else{
						numMatches++;
					}
			}
		}

		// If binary string cannot be combined with another, place in array that has all prime implicants.
		if(numMatches==0){
		//printf("%s\n" ,firstArray[i]);
		fprintf(output_file,"%s\n", firstArray[i]);
		numEssentials++;
		}
	}


	// Repeat logic for step 2 array //
	printf("\nPrime Implicants: \n");

	for(i=0; i<secondArrayRows;i++){
		numMatches = 0;
		
		for(k=0; k<secondArrayRows;k++){

		char firstString[5], secondString[5];
		strcpy(firstString, secondArray[i]);
		strcpy(secondString, secondArray[k]);

		hammingDistance = compareBinary(firstString, secondString);

			if(hammingDistance==3){
				char *combinedPrime = combinePrimes(firstString, secondString);
				if(alreadyExists(combinedPrime,thirdArray,thirdArrayRows)==false){
						thirdArray[thirdArrayRows] = combinedPrime;
						fprintf(output_file,"%s\n", combinedPrime);
						numMatches++;
						thirdArrayRows++;
					}

					else{
						numMatches++;
					}
			}
		}

		if(numMatches==0){
		printf("%s \n" ,secondArray[i]);
		fprintf(output_file,"%s\n", secondArray[i]);
		numEssentials++;
		
		}
	}


	//* Print third array *//
	for(k=0;k<thirdArrayRows;k++){
		printf("%s \n", thirdArray[k]);
	}

	fprintf(output_file, ".end\n");

	return 0;
	}
