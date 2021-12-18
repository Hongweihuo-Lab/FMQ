# FMQ

Efficient compression and indexing of highly-repeated DNA data sets

## What is it?
FMQ is a reference-based algorithm for efficiently compressing and indexing highly similar genome sequence data. Using the FMQ algorithm, a genome sequence data set is divided into reference and non-reference
sequences, with the former used to construct compression index. Besides enabling fast search, this indexing strategy also significantly reduces storage footprint since only the difference between the reference and the non-reference sequences needs to be stored. To further improve search efficiency, the sequence difference is represented with a succinct data structure, and the context information around the differential site is also compression indexed. 

## How to use it?   
### Step I. Install
   1. download (or clone) the source code from https://github.com/hongweihuo-lab/FMQ
   2. Compile the source code. 
### Step II. run FMQ 

#### 1. Index construction 
   run the shell command: "./FMQ geomone ref num q", where parameters are       
    
    (1) geomone is the genome dataset.
    (2) ref is the reference sequence.
    (3) num is the number of sequences in geomone dataset. 
    (4) q is the length of context. 

#### 2. Locate query 
   run the shell command: "./FMQ geomone ref num pattern q", where parameters are 
    
    (1) geomone is the genome dataset.        
    (2) ref is the reference sequence.     
    (3) num is the number of sequences in geomone. 
    (4) pattern is the query pattern. 
    (5) q is the length of context.    

#### 3. Extract query 
  run the shell command: " ./FMQ geomone ref num position len q", where parameters are
    
    (1) geomone is the geomone dataset. 
    (2) ref is the refernce sequence.
    (3) num is the number of sequences in geomone dataset. 
    (4) position is the start position of the extracted substring. 
    (5) len is the length of the extracted substring. 
    (6) q is the length of context. 
       
## Paper
H. Huo, X. Chen, X. Guo, and J. S. Vitter, FMQ: Efficient compression and indexing for highly repetitive DNA sequence collections, IEEE/ACM Transactions on Computational Biology and Bioinformatics (TCBB), January 21, 2020, [PDF](https://doi.org/10.1109/TCBB.2020.2968323). 
