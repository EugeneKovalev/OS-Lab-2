#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmemory.h"

int _free(VA blockAddress);
int _read(VA blockAddress, void* bufferAddress, size_t bufferSize);
void print();

void defrag(struct block* previousBlock);

int main(int argc, char* argv[])
{
	VA  firstVirtualBlock = (VA)malloc(sizeof(VA)),
		secondVirtualBlock = (VA)malloc(sizeof(VA)),
		thirdVirtualBlock = (VA)malloc(sizeof(VA)),
		fourthVirtualBlock = (VA)malloc(sizeof(VA)),
		fifthVirtualBlock = (VA)malloc(sizeof(VA));

	void *firstVirtualBuffer = "Alpha",
		*secondVirtualBuffer = "Bravo",
		*thirdVirtualBuffer = "Charlie",
		*fourthVirtualBuffer = "Delta",
		*fifthVirtualBuffer = "Echo";

	_init(5, 10);
	_write(firstVirtualBlock, firstVirtualBuffer, strlen((char*)firstVirtualBuffer));
	_write(secondVirtualBlock, secondVirtualBuffer, strlen((char*)secondVirtualBuffer));
	_write(thirdVirtualBlock, thirdVirtualBuffer, strlen((char*)thirdVirtualBuffer));
	_write(fourthVirtualBlock, fourthVirtualBuffer, strlen((char*)fourthVirtualBuffer));
	_write(fifthVirtualBlock, fifthVirtualBuffer, strlen((char*)fifthVirtualBuffer));


	print();
	_write(thirdVirtualBlock, "Foxtrot", strlen("Foxtrot"));
	print();
	_free(thirdVirtualBlock);
	print();
	system("pause");
	return 0;
}

void print()
{
	VA firstVirtualBlock = "1", secondVirtualBlock = "2", thirdVirtualBlock = "3", fourthVirtualBlock = "4", fi = "5";
	struct block* cur;
	size_t freeMemory = 0;
	int i = 0;
	char d[40];


	cur = firstBlock;
	while (cur != NULL)
	{
		for (i = 0; i < 40; i++)
			d[i] = '\0';
		_read(cur->virtualAddress, d, 20);
		// puts(d); 
		printf("%s\n", d);
		freeMemory += cur->size;
		cur = cur->next;
	}
	freeMemory = amountOfBytes - freeMemory;
	printf("EMPTY : %d \n", freeMemory);
	printf("=========================================\n");
}