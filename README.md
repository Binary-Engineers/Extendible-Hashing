# Extendible Hashing 

## Overview
With data volumes rapidly increasing, traditional data storage and retrieval methods are becoming inefficient. Sequential data structures can lead to slow lookups, taking O(log n) or even O(n) time, depending on data sorting. **Hashing** offers an optimized solution by enabling constant-time operations (O(1)) for data retrieval, insertion, and deletion, making it ideal for large datasets and high-performance requirements.

This project implements **extendible hashing**, a dynamic hashing technique that adapts as the dataset grows. This method allows the hash table to resize automatically, efficiently handling collisions and supporting fast, constant-time lookups.

## Features
- **Extendible Hashing Implementation**: Uses a hashing function to allocate records into specific buckets, supporting constant-time retrieval.
- **Dynamic Resizing**: Manages data growth by expanding the hash table as necessary.
- **Collision Handling**: Implements methods to handle hash collisions, ensuring data integrity and lookup efficiency.

## Usage
Once all functions are implemented, run `ExtendibleHashing.cpp` to test various hash operations such as insertion, deletion, and lookup. Printing functions are included to visualize the directory structure and data buckets, helping to verify the accuracy of your solution.

## Considerations
- **Hashing Functionality**: The provided hashing function calculates bucket allocations and manages bitwise operations.
- **Collision Resolution**: Ensures that records are stored without overwriting, with dynamic resizing of buckets when needed.
