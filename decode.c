//Written by Eric Lin Huang
# include <getopt.h> //getopt
# include <stdio.h> //printf
# include <unistd.h>// from here
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>  //to here is for read() open() write() etc
# include <errno.h> //errors
# include <stdlib.h> //calloc
# include "decode.h"
# include "huffman.h"
# include "stack.h"

# define CHECK_BIT(x, k) ((x & (0x1 << (k % 8))) != 0) //creds to darrel long

int main(int argc, char *argv[])
{
      bool verbose = false, printT = false, noOutput = true;
      char *oFile;
      char *iFile;
      uint32_t magicNum, x, size = 0;
      int8_t symbol;
      uint64_t sizeOFile, bytesWritten = 0;
      uint16_t treeSize;
      int response, fd, fd1, fd2;
      struct stat buf;
      while((response = getopt(argc, argv, "i:o:vp")) != -1) //takes command line arguments
      {
          switch(response)
          {
              case 'i': //input file
              {
                  iFile = optarg;
                  break;
              }
              case 'o': //output file, default is stdout
              {
                  noOutput = false;
                  oFile = optarg;
                  break;
              }
              case 'v': //verbose
              {
                  verbose = true;
                  break;
              }
              case 'p': //print tree
              {
                  printT = true;
                  break;
              }
              default:
              {
                  break;
              }
          }
      }
      fd = open(iFile, O_RDONLY); //open input file
      if (fd == -1)
      {
          perror("error"); //if open fails print error message
          exit(errno);
          return 0;
      }
      if(!noOutput)
      {
          fd1 = open(oFile, O_RDONLY); //test if output file exists
          if (fstat(fd1, &buf) == 0) //if output file exists exit
          {
              perror(oFile);
              exit(errno);
              return 0;
          }
          fd2 = open(oFile, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);  //create output file
      }
      read(fd, &magicNum, sizeof(magicNum)); //check magic number
      if(magicNum != 0xdeadd00d)
      {
          printf("Not a compressed file\n"); //if not there print error
          return 0;
      }
      read(fd, &sizeOFile, sizeof(uint64_t)); //read next 8 bytes, sizeOFile
      read(fd, &treeSize, sizeof(uint16_t)); //read in next 2 bytes, size of tree
      uint8_t *savedTree = calloc(treeSize, sizeof(uint8_t));
      for(uint16_t i = 0; i < treeSize; i++)
      {
          read(fd, &(savedTree[i]), sizeof(uint8_t));
      }
      treeNode *loadedTree = loadTree(savedTree, treeSize);
      treeNode *pointTree = loadedTree;
      if(verbose) //if -v print stats
      {
          printf("Original %lu bits: tree (%d)\n", sizeOFile*8, treeSize);
      }
      if(printT) //if -p print tree
      {
          printTree(loadedTree, 0);
      }
      int8_t *buffer = (int8_t *)calloc(sizeOFile, sizeof(int8_t));
      int8_t *store = (int8_t *)calloc(sizeOFile, sizeof(int8_t));//array to store symbols
      read(fd, buffer, sizeOFile); //read in rest of file, stored in buffer[]
      int index = 0;
      while(bytesWritten < sizeOFile) //loop until eveythings written
      {
          x = nextBit(buffer[index]);
          size++;
          if (size % 8 == 0)
          {
              index += 1; //move to next index
          }
          symbol =  (int8_t)stepTree(loadedTree, &pointTree, x);
          if(symbol != -1)
          {
              store[bytesWritten] = symbol; //symbols stored in array
              bytesWritten += 1;
          }
      }
      if(noOutput)
      {
          write(STDOUT_FILENO, store, sizeOFile); //print out entire array
      }
      else
      {
          write(fd2, store, sizeOFile);
      }
      close(fd); //free etc
      //close(fd2);
      free(store);
      free(buffer);
      free(savedTree);
      delTree(loadedTree);
      return 0;
}

/*
Gets the nextBit of a byte and remembers the position of last checked
bit */
uint32_t nextBit(uint8_t bytes)
{
    static int bp = 0; //uses static to keep track of bit
    uint32_t ret = CHECK_BIT(bytes, bp);
    bp += 1; //goes to next bit
    bp = bp % 8;
    return ret;
}
