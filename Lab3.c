/*
 * Oscar Choy
 * ID# 53555169
 * 2/19/2017
 * Lab 3
 
 * This is a program meant to simulate allocation and deallocation of memory, using an array as the memory space
 * being allocated and deallocated, similar to a heap. This array will be 128 bytes large, hence the "heap" capacity
 * wil be of this size. Using the output console, the program will prompt the user to imput a command to
 * be used with this array of "memory". The available commands are "allocate", "free", "printheader", 
 * "printheap", "writeheap", "blocklist", and "quit". Each "block" will have a header of two bytes, and 
 * a payload of however many bytes was specified at the time of allocation.
 *
 * The "allocate" command takes one argument: the number of bytes that will form the payload of the 
 * allocated block. 
 * It's format is: allocate 10
 *
 * The "free" command frees a specified block of memory. It takes one argument: the block number associated
 * with the allocated block of memory the user wishes to free. 
 * Its format is: free 10
 *
 * The "blocklist" command prints out information about all the blocks in the "heap". It takes no arguments. It
 * prints out the following about each block: size, if it allocated or not, the start address, and the end address.
 * All the addresses will be printes out in hexadecimal format.
 * Its format is: blocklist
 *
 * The "writeheap" command writes a specified character into a number of bytes in a specified block in the heap.
 * It takes 3 arguments: which block to write in, which character will be written, and how many times it will be
 * written, in that exact order. It will only write in the payload, and never in the header. 
 * Its format is: writeheap 3 A 24
 * 
 * The "printheap" command prints out the contents of a portion of the heap. It takes 2 arguments: the block to be
 * printed out from, and how many bytes of that block's payload are to printed out.
 * Its format is: printheap 3 10
 *
 * The "printheader" command prints out the header of a block in the heap. It takes one argument: the block whose
 * header the user wishes to print out. The header is two bytes long and will be printed in hexadecimal.
 * It's format is: preintheader 2
 *
 * The "quit" command quits the program.
 *
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int countTokens(char* arrStr);
char getHexa(int hexNum);
void printHexa(int size);
int allocate(char * ptr, char * endPtr, int aSize, int bNum);
int freeBlock(char * ptr, char * endPtr, int blockNum);
void printHead(char * ptr, char * endPtr, int blockNum);
void printFromHeap(char * ptr, char * endPtr, int blockNum, int numBytes);
void writeToBlock(char * ptr, char * endPtr, char eChar, int blockNum, int numWrites);
void showBlockList(char * ptr, char * endPtr);

int main() {
  
  char input[500];
  char oInput[500];
  int isLive = 0, heapSize = 128, blockCount = 0; 
  int argCount, numParams, intVal1, intVal2, currSize, confirm;
  int i = 2;
  char command[500];
  char junk[500];
  char entChar, heapVal;
  
  char *heap = (char*)malloc(heapSize * sizeof(char));
  heap[0] = 0;
  heap[1] = -2;
  while (i < 128) {
    heap[i] = 0;
    i++;
  }
  char *start = heap;
  char *end = heap + 128;

  while(isLive == 0) {  
    printf("> ");
    gets(input);

    if (strcmp(input, "quit") == 0) {
      isLive = 1;
    } else if (strcmp(input, "blocklist") == 0) {
      showBlockList(start, end);
    } else {
      strcpy(oInput, input);
      argCount = countTokens(oInput);
	  
      if (argCount == 2) {
        numParams = sscanf(input, "%s %d", command, &intVal1);
	if (numParams != 2) {
	  printf("Invalid command. Try again.\n");
	} else {
	  if (strcmp(command, "allocate") == 0) {
	    if (intVal1 > 126) {
	      printf("Payload too big. Cannot allocate.\n");
	    } else if (intVal1 <= 0) {
	      printf("Invalid payload value. Cannot allocate.\n");
	    } else {
	      confirm = allocate(start, end, intVal1, blockCount);
          if (confirm == 1) {
	        blockCount += 1;
		    printf("%d\n", blockCount);
	      } else {
	        printf("Cannot allocate. Not enough available memory.\n");
	      }
	      start = heap;
	    }
	  } else if (strcmp(command, "free") == 0) {
	    if ((intVal1 > blockCount) || (intVal1 <= 0)) {
	      printf("Cannot free. Invalid block to free.\n");
	    } else {
	      confirm = freeBlock(start, end, intVal1);
	      if (confirm > 0) {
		printf("%d\n", confirm);
	      } else {
		printf("Cannot free. Block is not allocated in heap.\n");
	      }
	      start = heap;
	    }
	  } else if (strcmp(command, "printheader") == 0) {
	    if ((intVal1 > blockCount) || (intVal1 <= 0)) {
	      printf("Invalid block. Cannot print header.\n");
	    } else {
              printHead(start, end, intVal1);
	    }
	    start = heap;
	  } else {
	    printf("Invalid command. Try again.\n");
	  }
	}
      } else if (argCount == 3) {
	numParams = sscanf(input, "%s %d %d", command, &intVal1, &intVal2);
	if (numParams == 3) {
	  if (strcmp(command, "printheap") == 0) {
	    if (intVal1 <= 0) {
	      printf("Invalid block to print from\n");
	    } else if ((intVal2 > 126) || (intVal2 < 0)) {
	      printf("Invalid number of blocks to print out\n.");
	    } else {
	      printFromHeap(start, end, intVal1, intVal2);
	      start = heap;
	    }
	  } else {
	    printf("Invalid command. Please try again.\n");
	  }
	} else {
          printf("Invalid command. Please try again.\n");
	} 
      } else if (argCount == 4) {
	numParams = sscanf(input, "%s %d %c %d", command, &intVal1, &entChar, &intVal2);
	if (numParams == 4) {
	  if (strcmp(command, "writeheap") == 0) {
	    if (intVal1 <= 0) {
	      printf("Invalid block number to write to.\n");
	    } else if (intVal2 < 0) {
	      printf("Invalid number of characters to write in\n");
	    } else if ((entChar > 127) || (entChar < 0)) {
	      printf("Character not valid.\n");
	    } else {
	      writeToBlock(start, end, entChar, intVal1, intVal2);
	      start = heap;
	    }
          } else {
	    printf("Invalid command. Please try again.\n");
	  }
	} else {
	  printf("Invalid command. Please try again.\n");
        }
      } else {
	printf("Invalid command. Please try again.\n");
      }
    } 
  }
  free(heap);
  return 0;
}

void printHexa(int size) {
  int hDigits = size / 16;
  int lDigits = size % 16;
  char sizeHex [3];
  char highVal;
  char lowVal;
  if (hDigits > 9) {
    highVal = getHexa(hDigits);
    if (lDigits > 9) {
      lowVal = getHexa(lDigits);
      sprintf(sizeHex, "%c%c", highVal, lowVal);
    } else {
      sprintf(sizeHex, "%c%d", highVal, lDigits);
    }
  } else {
    if (lDigits > 9) {
      lowVal = getHexa(lDigits);
      sprintf(sizeHex, "%d%c", hDigits, lowVal);
    } else {
      sprintf(sizeHex, "%d%d", hDigits, lDigits);
    }
  }
  printf("%s", sizeHex);
}

int countTokens(char* arrStr) {

  int tokCount = 0;
  char* token;
  const char s[] = "\n";
  
  token = strtok(arrStr, s);
  token = strtok(token, " ");
  
  while (token != NULL) {
    tokCount += 1;
    token = strtok(NULL, " ");
  }
  return tokCount;
}

char getHexa(int hexNum) {
  if (hexNum == 10) {
    return 'a';
  } else if (hexNum == 11) {
    return 'b';
  } else if (hexNum == 12) {
    return 'c';
  } else if (hexNum == 13) {
    return 'd';
  } else if (hexNum == 14) {
    return 'e';
  } else if (hexNum == 15) {
    return 'f';
  }
}

int allocate(char * ptr, char * endPtr, int aSize, int bNum) {
  printf("\n");
  int currSize, allocd = 0;
  char allocated, cSize;
  while((ptr < endPtr) && (allocd == 0)) {
    currSize = (int)(((unsigned char)(*(ptr + 1) & -2) >> 1) + 1);
    allocated = (*(ptr + 1) & 1);
    if ((allocated == 0) && (currSize >= (aSize + 2))) {
	  *ptr = (bNum + 1);
	  if (((currSize - (aSize + 2)) > 2) || ((currSize - (aSize + 2)) == 0)) {
        cSize = ((aSize + 1) << 1);
        cSize ^= 1;
        *(ptr + 1) = cSize;
        currSize -= (aSize + 2);
        if (currSize != 0) {
          ptr += (aSize + 2);
		  *ptr = 0;
		  cSize = (currSize - 1) << 1;
          *(ptr + 1) = cSize;
	    }
	  } else {
		cSize = (currSize - 1) << 1;
		cSize ^= 1;
		*(ptr + 1) = cSize;
		ptr += currSize;
	  }
      allocd = 1;
    } else {
      ptr += currSize;
    }
  }
  return allocd;
}

int freeBlock(char * ptr, char * endPtr, int blockNum) {
  printf("\n");
  int currSize, allocated, currBNum = 0, blockFreed = 0, found = 0;
  char cSize;
  while ((ptr < endPtr) && (found == 0)) {
    currBNum = *ptr;
    currSize = (int)(((unsigned char)(*(ptr + 1) & -2) >> 1) + 1);
    allocated = (*(ptr + 1) & 1);
    if ((currBNum == blockNum) && (allocated == 1)){
      *ptr = 0;
      cSize = *(ptr + 1);
      cSize ^= 1;
      *(ptr + 1) = cSize;
      blockFreed = blockNum;
      found = 1;
    } else {
      ptr += currSize;
    }
  }
  return blockFreed;
}

void printHead(char * ptr, char * endPtr, int blockNum) {
  printf("\n");
  int currSize, currBNum, found = 0;
  while ((ptr < endPtr) && (found == 0)) {
    currBNum = *ptr;
    if (currBNum == blockNum) {
	  currSize = (int)(((unsigned char)(*(ptr + 1) & -2) >> 1) + 1);
      printHexa(currBNum);
      printHexa(currSize);
      printf("\n");
      found = 1;			
    } else {
      currSize = (int)(((unsigned char)(*(ptr + 1) & -2) >> 1) + 1);
      ptr += currSize;
    }
  }
  if (found == 0) {
    printf("Could not print block header. Block not found\n");
  }
}

void printFromHeap(char * ptr, char * endPtr, int blockNum, int numBytes) {
  printf("\n");
  int currSize, currBNum, found = 0;
  if (numBytes == 0) {
    printf("\n");
  } else {
    int counter = numBytes;
    int i = 0, complete = 0;
    while ((ptr < endPtr) && (found == 0)) {
      currBNum = *ptr;
      if (currBNum == blockNum) {
        while ((complete == 0) && (ptr < endPtr)) {
	  currSize = (int)(((unsigned char)(*(ptr + 1) & -2) >> 1) + 1);
	  ptr += 2;
          if (currSize > counter) {
            while (i < counter) {
	      printf("%c", *(ptr + i));
	      i += 1;
	    }
          } else {
            while (i < (currSize - 2)) {
	      printf("%c", *(ptr + i));
	      i += 1;
	    }
          }
          i = 0;
	  counter -= (currSize - 2);
	  if (counter <= 0) {
	    complete = 1;
	  } else {
	    ptr += (currSize - 2);
	  }
        }
        printf("\n");
        found = 1;
      } else {
        currSize = (int)(((unsigned char)(*(ptr + 1) & -2) >> 1) + 1);
        ptr += currSize;
      }
    }
    if (found == 0) {
      printf("Could not print. Block entered was not found\n");
    }
  }
}

void writeToBlock(char * ptr, char * endPtr, char eChar, int blockNum, int numWrites) {
  int currSize, currBNum, found = 0;
  int i = 0;
  while ((ptr < endPtr) && (found == 0)) {
    currBNum = *ptr;
    currSize = (int)(((unsigned char)(*(ptr + 1) & -2) >> 1) + 1);
    if (currBNum == blockNum) {
      if ((currSize - 2) < numWrites) {
        printf("\nWrite too big\n");
      } else {
        ptr += 2;
	while (i < numWrites) {
	  *(ptr + i) = eChar;
	  i += 1;
	}
      }
      found = 1;
    } else {
      ptr += currSize;
    }
  }
  if (found == 0) {
    printf("Could not write to that block. Block not allocated\n");
  }
}

void showBlockList(char * ptr, char * endPtr) {
  printf("\n");
  int currSize, currAddress = 0, allocated = 0;
  printf("Size ");
  printf("Allocated ");
  printf("Start                ");
  printf("End                 ");
  printf("\n");
  while (ptr < endPtr) {
    currSize = (int)(((unsigned char)(*(ptr + 1) & -2) >> 1) + 1);
    allocated = (*(ptr + 1) & 1);
    printf("%-4d ", currSize);
    if (allocated == 0) {
      printf("no        ");
    } else {
      printf("yes       ");
    }		
    printf("%-20p ", ptr);
    ptr += (currSize - 1);
    printf("%-20p ", ptr); 
    printf("\n");
    ptr += 1;
  }
}
