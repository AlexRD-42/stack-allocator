#include <stddef.h>
#include "ft_alloc.h"

size_t  ft_bitfind(const size_t *word, size_t start, size_t end, bool bit)
{
	size_t			i;
	size_t			offset;
	const size_t	start_mask = SIZE_MAX >> (start % WORD_BIT);
	const size_t	invert = (size_t)-(!bit);
	const size_t	word_end = (end - 1) / WORD_BIT;

	i = start / WORD_BIT;
	offset = ft_lzcnt((word[i] ^ invert) & start_mask);
	while (offset == WORD_BIT && i < word_end)
	{
		i++;
		offset = ft_lzcnt(word[i] ^ invert);
	}
	offset += i * WORD_BIT;
	if (offset >= end)
		return (SIZE_MAX);
	return (offset);
}

void	ft_bitset(size_t *bitmap, size_t start, size_t end, bool bit)
{
	const size_t	fill = (size_t)-(bit != 0);
	const size_t	start_mask = SIZE_MAX >> (start % WORD_BIT);
	const size_t	end_mask = SIZE_MAX << (-end % WORD_BIT);
	size_t			mask;
	size_t			diff_mask;

	start = start / WORD_BIT;
	end = (end - 1) / WORD_BIT;
	diff_mask = (size_t)-(start != end);
	mask = start_mask & (end_mask | diff_mask);
	bitmap[start] ^= (bitmap[start] ^ fill) & mask;
	start = start + 1;
	while (start < end)
		bitmap[start++] = fill;
	mask = end_mask & diff_mask;
	bitmap[end] ^= (bitmap[end] ^ fill) & mask;
}

// MSB first
bool	bit_read(size_t word, size_t index)
{
	return (word & ((size_t)1 << (WORD_BIT - 1 - index % WORD_BIT)));
}

// MSB first
size_t	bit_write(size_t word, size_t index, bool bit)
{
	size_t	mask;

	mask = (size_t)1 << (WORD_BIT - 1 - (index % WORD_BIT));
	if (bit)
		return (word | mask);
	else
		return (word & ~mask);
}
