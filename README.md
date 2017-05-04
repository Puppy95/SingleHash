# SingleHash
use one hash function to produce multiple hash values and build faster hash based data structures

We have implemented SingleHash technique on CM sketches, Bloom filters and d-left hash tables. For each kind of data structure there is a class which can be initiated with three parameters, namely w, k and type. For CM sketches, w is the length of each array and k is the number of arrays; for d-left hash tables w is the length of each section and k is the number of sections; for Bloom filters w is the length of the bit array, k is the number of hash functions; we also implemented the pratition version of Bloom filter in the partition-bf.h, and for it w is the length of each section and k is the number of sections. For all theses classes, the parameter type has three values, 0, 1, and 2, which stands for the classic version, SingleHash version and Less Hash version respectively. 

Each class has a function named Insert and a function named Query, which is used to insert or query an item in the data structure.

D-left hash tables have functions named CountUsedChain and CountusedChainLength, for counting the number of linked lists and their total length respectively.

hashfunctions .h is used to provide hash functions for the data structures.

These main.cpp files are meant to test the performance of these data structures, which load data from files, vary the parameters, carry out experiments and store the results like false positive rate or speed in the output files.

Data has not been uploaded deal to the limit of size. Any data with the format of a string in each line can be used of test.
