Implementation of the greedy algorithm Huffman Coding in C.
https://en.wikipedia.org/wiki/Huffman_coding
Was trying to optimize encoding portion which resulted in a bug which I'm in the process of fixing... 
Simple overview:
 Encoding:
  -Utilizes a priority queue (implemented with binary heap) to count the frequency of each character in the given file. 
    Huffman tree will be built off this priority queue.
  - After tree is built we write to the compressed file the path of the Huffman tree, which when
  decoded will decompress the original file. 
  -We start the compressed file with 0xdeadd00d to let our decoder know that it is a huffman compressed file. The
    next 8 bytes is the size of the original file, and the 2 bytes after that gives the size of the tree. 
 
  Decoding:
  -First checks to see if 0xdeadd00d is the first 32 bits of the file. If it is, we proceed with the decompression algorithm, else we 
  exit with error message.
  - If output file is not specified we print to stdout
  - Find the original size of file, size of tree, and decode the encrypted Huffman tree. Once the tree is built,
  the rest of the bits until the end of the encrypted file contains the encoding of characters in the Huffman tree.
  Once we land on a leaf node, we write it to our output file. 
