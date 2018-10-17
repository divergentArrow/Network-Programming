#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //to use toLower
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h> //to make sure i dont get any errors for open()
int syscallCount(char* textfile, char* string){
	//look at system calls 
	//printf("file:%s string:%s\n",textfile,string);
	char *c;
	int stringLength=strlen(string); //length of string you are trying to find
	int file;
	int count=0; //count the number of occurences
	int wordLength=0; //length of current word
	c=(char*) calloc(150,sizeof(char));
	file=open(textfile,O_RDONLY,0);
	if(file<0){
		perror("./p05");
		return -1;
	}
	int position=0;
	int size=1;
	int i=0;
	
	while(size>0){
		size=read(file,c,1);//reads in one char at a time	
		if(tolower(c[i])== tolower(string[i])){
			wordLength++; //increase word length if character matches
			if(wordLength==stringLength){
				if(stringLength==1){
					count++;
					wordLength=0;
				}
				else{
					count++; //increase count if the characters and length matches substring you are trying to find
					wordLength=0; //reset word length cause you found a matching word
					lseek(file,position-wordLength,SEEK_SET);
				}
				
			}
		}
		else if(tolower(c[i])!=tolower(string[i])){ //if character doesnt match start search from next character
				wordLength=0; //reset count since word you were ooking at doesnt amtch so go to next one
			lseek(file,position-wordLength,SEEK_SET);//does not use stdio library so it is valid
		}
		if(c[i]=='\0'){
			break;
		}
		position++;//increase file position
	}
	
	close(file); //close the file
	free(c);
	return count;
}


int stringCounter(char* fileName, char* substring){ 
	int counter=0; //counts the number of times the substring occurs
	FILE *file;
	file = fopen(fileName,"r");
	if( file == NULL ) { //if file doesn't exist
   	//printf("Error: %s\n", strerror(errno)); //can use either to print out error
      perror("./p05");
      return(-1);
   }
   if(file){ //file can be opened and it exists
    int subStr = strlen(substring); //length of the substring we are trying to find. 
	 int wordLength=0; //the substring length of the word we are looking at right now
	 int i=0; //used for indexing across the substring
	 int letter; //letter read in from file
	 int filePositionCount=0;
	 while(1){
	 filePositionCount++; //get the postion of the file so that you can remember where to start after you increment the counter
	 letter=fgetc(file);
	 if(feof(file)){ 
			break; //reached end of file
	 }
		if(tolower(letter)==tolower(substring[i])){
			i++;
			wordLength++;
		  if(wordLength==subStr){ //if all the characters of the word matches up with the substring increment counter
			if(subStr==1){
			//fseek(file,filePositionCount-wordLength+1,SEEK_SET);
				counter++;  
				wordLength=0; //reset back to 0 to start counting for next word
				i=0; //reset index to go through different indexes of the string
			}
			else{
				fseek(file,filePositionCount-wordLength+1,SEEK_SET); //go back to the next position from where you started your search thorugh the word for EX: aaa finds aa then goes to the 2nd postion and searches from there to get aa
				counter++;  
				wordLength=0; //reset back to 0 to start counting for next word
				i=0; //reset index to go through different indexes of the string
			}
		  }
		}
		else if(letter!=substring[i]){ //if the characters dont match
			fseek(file,filePositionCount-wordLength-1,SEEK_SET);//If it doesnt go to next position from where you started looking to see if the word 		       																		matches the substring in the file. 
			wordLength=0; //reset word length since you need to look to see if the new word is the same length;
			i=0; //reset indexing
		}
		else{ //break out of for loop worst case
			break;
		}
	}
  
	fclose(file);//close file at the end
} //end of if(file)
else{ //if file does not exist or it does but not found
	perror("./p05");
   return(-1);
}
	return counter; //returns the counter and will return 0 if not found.
}

int main(int argc, char *argv[]){ 
 	char* sys="--systemcalls";
 	char* ex="./p05";
 	
 	if(strcmp(argv[0],ex)!=0)//if its not 0 then no executable
 	{
 		perror("error: no executable\n");
 		return -1;
 	}
 	if(!(strcmp(argv[1],sys))){ //if its the same it returns 0 so !0==1 
 		if(argc<=3){
 			printf("3 or less areguements is not enough\n");
 			return -1;//not enough arguements
 		}
		int j=3;
		for(j=3;j<argc;j++){ 
			int z=syscallCount(argv[2],argv[j]); //make a different method using read() instead of fgetc
			printf("%d\n",z);
		} 		
	}
	else if(argc<=2){
 		printf("2 or less areguements is not enough\n");
 		return -1;//not enough arguements
 	}
 	else{
		int k=2;
 		for(k=2;k<argc;k++){ //going to traverse thorugh arguements assuming first one is calling it then second one is file and ones after are substrings
			int t=stringCounter(argv[1],argv[k]); 
			printf("%d\n",t); //prints output which will be 0 if substring is not in file
		}//end for
	}//end else
}
