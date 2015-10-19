typedef char* VA;

struct block {
	VA virtualAddress;
	size_t phys_address;
	size_t size;
	block *next;
};

VA allocatedMemory;
size_t amountOfBytes;
block *firstBlock = NULL;

int processExistingBlockAddressCondition(VA*, size_t);
int processNewBlockAddressCondition(VA*, size_t);

int _init(int numberOfPages, int pageSize)
{
	if (numberOfPages < 1 || pageSize < 1) return -1;
	amountOfBytes = numberOfPages * pageSize;
	allocatedMemory = (VA)malloc(amountOfBytes);
	if (allocatedMemory == NULL) return 1;
	else return 0;
}

int _malloc(VA *blockAddress, size_t bufferSize)
{
	if (bufferSize < 1 || blockAddress == NULL) return -1;
	else if (bufferSize > amountOfBytes) return -2;

	block *newBlock, *observingBlock, *previousBlock = NULL;

	if (firstBlock == NULL)
	{
		firstBlock = (block*)malloc(sizeof(block));
		firstBlock->virtualAddress = *blockAddress;
		firstBlock->phys_address = 0;
		firstBlock->size = bufferSize;
		firstBlock->next = NULL;
		return 0;
	}
	else
	{
		observingBlock = firstBlock;
		while (observingBlock != NULL)
		{
			if (observingBlock->virtualAddress == *blockAddress)
				return processExistingBlockAddressCondition(blockAddress, bufferSize);
			else observingBlock = observingBlock->next;
		}
	}

	return processNewBlockAddressCondition(blockAddress, bufferSize);
}

int processExistingBlockAddressCondition(VA *blockAddress, size_t bufferSize)
{
	block *newBlock, *observingBlock, *previousBlock = NULL;
	observingBlock = firstBlock;
	while (observingBlock->virtualAddress != *blockAddress)
	{
		previousBlock = observingBlock;
		observingBlock = observingBlock->next;
	}
	newBlock = (struct block*)malloc(sizeof(struct block));
	newBlock->next = observingBlock->next;
	previousBlock->next = newBlock;
	newBlock->virtualAddress = (VA)(observingBlock->virtualAddress);
	newBlock->phys_address = observingBlock->phys_address;
	newBlock->size = bufferSize;
	free(observingBlock);
	return 0;
}

int processNewBlockAddressCondition(VA *blockAddress, size_t bufferSize)
{
	block *newBlock, *observingBlock, *previousBlock = NULL;
	observingBlock = firstBlock;

	while (observingBlock->next != NULL)
	{
		observingBlock = observingBlock->next;
	}
	if (bufferSize > amountOfBytes - observingBlock->size - observingBlock->phys_address) return -2;

	newBlock = NULL;
	newBlock = (block*)malloc(sizeof(block));
	newBlock->next = NULL;
	newBlock->virtualAddress = *blockAddress;
	newBlock->phys_address = observingBlock->phys_address + observingBlock->size;
	newBlock->size = bufferSize;
	observingBlock->next = newBlock;
	return 0;
}


int _write(VA blockAddress, void *bufferAddress, size_t bufferSize)
{
	_malloc(&blockAddress, bufferSize);
	block *currentBlock = NULL;
	currentBlock = firstBlock;
	while (currentBlock != NULL)
	{
		if (currentBlock->virtualAddress == blockAddress) break;
		currentBlock = currentBlock->next;
	}

	if (currentBlock == NULL) return -1;
	if (bufferSize > currentBlock->size) return -2;

	memcpy(allocatedMemory + (int)currentBlock->phys_address, bufferAddress, bufferSize);
	return 0;
}

int _free(VA blockAddress)
{
	block* previousBlock = NULL;
	block* cur = firstBlock;
	VA c;

	if (blockAddress == NULL)
		return -1;

	while (cur->virtualAddress != blockAddress)
	{
		previousBlock = cur;
		cur = cur->next;
	}

	if (cur == NULL)
	{
		return -1;
	}

	if (previousBlock)
	{
		previousBlock->next = cur->next;
		c = cur->virtualAddress;
		free(cur);
		return 0;
	}
	else
	{
		firstBlock = NULL;
		free(cur);
	}

	return 0;
}

int _read(VA blockAddress, void* bufferAddress, size_t bufferSize)
{
	int bufElem = 0;
	struct block* cur = firstBlock;
	while (cur != NULL)
	{
		if (cur->virtualAddress == blockAddress)
			break;
		cur = cur->next;
	}
	if (cur == NULL)
	{
		return -1;
	}
	if (bufferSize < cur->size)
	{
		return -2;
	}
	memcpy(bufferAddress, allocatedMemory + (int)cur->phys_address, cur->size);
	return 0;
}