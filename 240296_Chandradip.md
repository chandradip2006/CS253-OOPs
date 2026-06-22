CS253 Assignment 1: Memory-Efficient Versioned File Indexer Using C++
Author: Chandradip Karmakar
Roll Number: 240296


# Overview: #

This program is a memory-efficient versioned file indexer designed to process large text files incrementally. It builds a word-level index mapping unique, case-insensitive alphanumeric words to their frequencies. By utilizing a fixed-size buffer, the system guarantees that the entire file is never loaded into memory, ensuring memory usage scales only with the number of unique words, not the file size.The program maintains separate indices for different file versions and supports various analytical queries on them 

# Architecture & Design: #

The solution employs a robust Object-Oriented Design featuring the following core components:

1. Buffered File Reader: Handles reading the input files incrementally using a strictly enforced fixed-size buffer (between 256 KB and 1024 KB) .
2. Tokenizer: Parses the raw buffer data into contiguous alphanumeric words, correctly handling tokens that are split across buffer boundaries.
3. Versioned Indexer: Maintains the frequency map (word-level index) for each distinct version provided during execution.
4. Query Processor: Processes analytical queries against the indices.

# C++ Features: #
1. Inheritance: Utilizes at least one abstract base class and two derived classes (e.g., for different query types).
2. Polymorphism: Employs virtual functions and dynamic dispatch to resolve query executions at runtime.
3. Function Overloading: Implements overloaded functions with varying parameter lists.
4. Exception Handling: Uses try, catch, and throw blocks to gracefully handle runtime errors (e.g., invalid buffer sizes or missing files).
5. Templates: Integrates at least one user-defined template (class or function).

# Compilation: #
    Compilation command:        g++ O2 240296_Chandradip.cpp -o analyzer

# Usage & Execution Examples: #
1. Word Query (Single File) Returns the frequency of a given word in a specified version.
    Command:            ./analyzer --file dataset_v1.txt --version v1 --buffer 512 --query word --word error 
2. Top-K Query (Single File) Displays the top-K most frequent words in a specified version, sorted in descending order .
    Command:            ./analyzer --file dataset_v1.txt --version v1 --buffer 512 --query top --top 10 
3. Difference Query (Two Files) Computes the difference in frequency of a specific word between two distinct versions .
    Command:            ./analyzer --file1 dataset_v1.txt --version1 v1 --file2 dataset_v2.txt --version2 v2 --buffer 512 --query diff --word error 

# Inputs & Outputs:#

-----Word count query-----
eg:1     ./analyzer --file test_logs.txt --version v1 --buffer 256 --query word --word error  
        Version: v1
        Count: 605079
        Buffer Size (KB): 256
        Execution Time (s): 1.31127

eg:2     ./analyzer --file test_logs.txt --version v1 --buffer 256 --query word --word DEVOPs
        Version: v1
        Count: 1209558
        Buffer Size (KB): 256
        Execution Time (s): 1.29655

eg:3     ./analyzer --file test_logs.txt --version v1 --buffer 256 --query word --word DEVOPS
        Version: v1
        Count: 1209558
        Buffer Size (KB): 256
        Execution Time (s): 1.48475

-----Top-10 frequent words query-----
 ./analyzer --file test_logs.txt --version v1 --buffer 256 --query top --top 10
Top-10 words in version v1:
devops 1209558
debug 605150
error 605079
info 604266
warning 604149
orderservice 484437
paymentservice 484078
authservice 483842
searchservice 483162
userservice 483125
Buffer Size (KB): 256
Execution Time (s): 1.30456

-----Difference query-----
eg:1     ./analyzer --file1 test_logs.txt --version1 v1 --file2 verbose_logs.txt --version2 v2 --buffer 256 --query diff --word error
        Difference (v2 - v1): -495377
        Buffer Size (KB): 256
        Execution Time (s): 2.7623

eg:2     ./analyzer --file1 test_logs.txt --version1 v1 --file2 verbose_logs.txt --version2 v2 --buffer 256 --query diff --word request
        Difference (v2 - v1): -247648
        Buffer Size (KB): 256
        Execution Time (s): 2.71906
    
