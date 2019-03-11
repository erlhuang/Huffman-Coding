//encode.c
//oliver mulcahy (opm@ucsc.edu)

# include <stdlib.h>
# include <getopt.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include "queue.h"
# include "stack.h"
# include "bv.h"

int main(int argc, char *argv[])
{
	uint32_t magic = 0xdeadd00d;
	uint64_t fileSize = 0;
	uint16_t leafCount = 0;
	int depth = 20;
	bool verbose = false;
	bool printT = false;
	char *oFile = "STDOUT_FILENO";
	char *iFile;
	int response;
	while((response = getopt(argc, argv, "i:o:vp:d:")) != -1)
	{
		switch(response)
		{
			case 'i':
			{
				iFile = optarg;
				break;
			}
			case 'o':
			{
				oFile = optarg;
				break;
			}
			case 'v':
			{
				verbose = true;
				break;
			}
			case 'p':
			{
				printT = true;
				break;
			}
			case 'd':
			{
				depth = atoi(optarg);
				break;
			}
			default:
			{
				break;
			}
		}
	}

	//open file
	FILE* inputFile = fopen(iFile, "r");
	if (inputFile == NULL)
	{
		perror("Error opening input file);
		exit(errno);
		return 0;
	}

	//create export file
	int outFile = open(oFile, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if (outFile == -1)
	{
		perror("Error opening output file");
		exit(errno);
		return 0;
	}

	//create histogram
	uint32_t *histogram = malloc(256 * sizeof(uint32_t));
	if (histogram == NULL){ printf("Error creating histogram"; return 0; }

	//populate histogram
	int c;
	while ((c = fgetc(inputFile)) != EOF)
	{
		histogram[c]++;
		fileSize++;
	}
	//increase 0 and 255 so we have something of a tree
	histogram[0]++;
	histogram[255]++;

	//counting elements more than 0 so I can create queue with lowest size possible
	uint32_t elements = 0;
	for (int i = 0; i < 256; i++)
	{
		if (histogram[i] > 0)
		{
			elements++;
		}
	}

	queue *superQueue = newQueue(elements);
	//pull the entries where the count is greater than 0 and create treenodes
	for (int i = 0; i < 256; i++)
	{
		if (histogram[i] > 0)
		{
			treeNode node = *newNode(i, 1, histogram[i]);
			enqueue(superQueue, node);
			leafCount++;
		}
	}

	//pull 2 nodes of queue, combine, put back into queue
	treeNode *root = NULL;
	while (!queueEmpty(superQueue))
	{
		treeNode x;
		dequeue(superQueue, &x);
		if (!queueEmpty(superQueue))
		{
			treeNode y;
			treeNode *z;
			dequeue(superQueue, &y);
			z = join(&x, &y);
			enqueue(superQueue, *z);
		}
		else
		{
			root = &x;
			break;
		}
	}

	//performing a post-order traversal
	code superCode = newCode();
	code codeTable = newCode();
	//this is where the seg fault happens
	buildCode(root, superCode, &codeTable);

	//writing magic number to oFile
	write(outFile, &magic, sizeof(magic));

	//writing length of original file
	write(outFile, &fileSize, sizeof(fileSize));

	//writing size of tree
	//this was made by darrell long
	leafCount = leafCount > 0 ? 3 * leafCount - 1 : 0;
	write(outFile, &leafCount, sizeof(leafCount));

	//write tree to file
	dumpTree(root, outFile);

	//copy bits of code for ofile
	//we must reset the read operation first
	inputFile = fopen(iFile, "r");
	if (inputFile == NULL)
	{
		perror("error");
		exit(errno);
		return 0;
	}

	//now we can write the bits to the file
	bitV* bv = newVec(10000);
	c = 0;
	//set bits
	while ((c = fgetc(inputFile)) != EOF)
	{
		setBit(bv, c);
	}
	//write bits
	for (int i = 0; i < 256; i++)
	{
		uint32_t t = valBit(bv, i);
		write(outFile, &t, sizeof(uint32_t));
	}

	//print other info
	if (printT)
	{
		printTree(root, depth);
	}
	if (verbose)
	{
		printf("Original %lu bits: tree (%d)", fileSize * 8, leafCount);
	}
	//close files and clean up
	close(outFile);
	fclose(inputFile);
	delTree(root);
	return 0;
}
