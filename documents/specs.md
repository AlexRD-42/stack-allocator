
Goal: Make a fast allocator that has:
- alloc, realloc, calloc and free operations
- metadata as small as possible

# Metadata
If the metadata only has binary used and unused states, then its impossible
for free to know how many blocks to free from just a pointer. Possible solutions are:
1) Have the size as a parameter for free:
	- Fastest solution, but makes it a lot more cumbersome to use and error prone;
2) Return a vector struct with start, current and capacity
	- The free will use the start and capacity to determine allocation; The memory cost is outsourced to the caller per allocation
3) Use a block to signify end of allocation:
	- Simplest, but uses a memory block per allocation and harms cache locality
4) Have a separate metadata variable that indicates end of allocation
	- Overall best solution, but uses twice as much metadata

With option two, we'd use 2 bits of metadata per memory block
	1 byte block, 2/08 = 25.00% of metadata
	4 byte block, 2/32 = 06.25% of metadata
	8 byte block, 2/64 = 3.125% of metadata

Since keeping alignment is intentional, and there is no point of allocating something lower than a word,
at worst we'd be using 6% in 32bit architecture and 3% in 64bit. That's acceptable.
But ideally block size is way higher, like 64 bytes

# Fragmentation
How to deal with fragmentation?
- Create a neural network to choose the best fit

// Megablock: How many blocks can be represented by a size_t in metadata
// Or BLOCK_PAGE_SIZE
// For example: block_size 8 and size_t = 8 bytes, 64 bits per size_t for 8 bytes of block = 512 bytes

// Memory pool:
// Static write pointer so that allocations run linearly
// and then only go back if 
You could have multiple pools that as time goes on, combine 



// Memory tag: tag the allocation with a specific ID, 
// so that later you can call free on a group. Or better yet, assign it to a specific pool
// This would work more or less like a pool with dynamic size

A struct allocator, with an operation to copy all pointers that you received into a new area