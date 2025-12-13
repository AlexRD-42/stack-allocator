/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 15:24:01 by adeimlin          #+#    #+#             */
/*   Updated: 2025/12/13 19:27:40 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "ft_alloc.h"

static uint8_t	data[MEMORY_SIZE];
static size_t	metadata[MEMORY_SIZE / META_SIZE];
static size_t	metadata_end[MEMORY_SIZE / META_SIZE];

// Returns 0 for word == 0, and 1 based indexing
// With O1 optimizes to lzcnt or bsr
size_t	ft_bsr(size_t word)
{
	size_t	i;

	i = 0;
	while (word)
	{
		word >>= 1;
		i++;
	}
	return (i);
}

// If end is not word aligned, it will scan past end until alignment
size_t	stt_bitfind(size_t start, size_t end, bool bit)
{
	size_t			cur;
	size_t			bitmap;
	const size_t	invert = SIZE_MAX * !bit;

	cur = start + META_BIT - (start % META_BIT);
	bitmap = (metadata[start / META_BIT] ^ invert) & (SIZE_MAX >> (start % META_BIT));
	if (bitmap != 0)
		return (cur - 1 - (ft_bsr(bitmap) - 1)); // the compiler needs the -1
	while (cur < end)
	{
		bitmap = metadata[cur / META_BIT] ^ invert;
		if (bitmap != 0)
			return (cur + (META_BIT - 1) - (ft_bsr(bitmap) - 1));
		cur += META_BIT;
	}
	return (SIZE_MAX);
}

void	stt_bitset(size_t start, size_t end)
{
	size_t			i;
	const size_t	word_start = start / META_BIT;
	const size_t	word_end = (end - 1) / META_BIT;
	size_t			start_mask;
	size_t			end_mask;

	start_mask = SIZE_MAX >> (start % META_BIT);
	end_mask = SIZE_MAX << ((META_BIT - (end % META_BIT)) % META_BIT);
	if (word_end == word_start)
	{
		metadata[word_start] |= start_mask & end_mask;
		return ;
	}
	metadata[word_start] |= SIZE_MAX >> (start % META_BIT);
	i = word_start + 1;
	while (i < word_end)
		metadata[i++] = SIZE_MAX;
	metadata[word_end] |= end_mask;
}

void	stt_bitclr(size_t start, size_t end)
{
	size_t			i;
	const size_t	word_start = start / META_BIT;
	const size_t	word_end = (end - 1) / META_BIT;
	size_t			start_mask;
	size_t			end_mask;

	start_mask = SIZE_MAX >> (start % META_BIT);
	end_mask = SIZE_MAX << ((META_BIT - (end % META_BIT)) % META_BIT);
	if (word_end == word_start)
	{
		metadata[word_start] &= ~(start_mask & end_mask);
		return ;
	}
	metadata[word_start] &= ~start_mask;
	i = word_start + 1;
	while (i < word_end)
		metadata[i++] = 0;
	metadata[word_end] &= ~end_mask;
}

void	ft_free(void *ptr)
{
	ptrdiff_t	start;
	size_t		end;
	size_t		start_mask;
	size_t		end_mask;

	if (ptr == NULL)
		return ;
	start = ((uint8_t *)ptr - data);
	if (start >= MEMORY_SIZE || start < 0)
		return ;
	start_mask = (1 << (META_BIT - 1 - start % META_BIT));
	if ((metadata[start / META_BIT] & ~start_mask) == 0)
		return ;
	end = ft_bitfind(metadata_end, (size_t)start, MEMORY_SIZE, 1);
	if (end == SIZE_MAX)
		return ;
	ft_bitclr(metadata, (size_t)start, end);
	end_mask = (1 << (META_BIT - 1 - (end % META_BIT)));
	metadata_end[start / META_BIT] &= ~end_mask;
}

// Assign blocks, calls free if necessary
void	*ft_alloc(size_t bytes)
{
	size_t	p1;
	size_t	p2;

	bytes = (bytes + (bytes % BLOCK_SIZE)) / BLOCK_SIZE;
	p1 = 0;
	while (p1 != SIZE_MAX)
	{
		p2 = ft_bitfind(metadata, p1, p1 + bytes, 1); // Scans for the first set bit
		if (p2 - p1 >= bytes)
			break ;
		p1 = ft_bitfind(metadata, p1, sizeof(metadata), 0); // Scans for the first unset bit
	}
	if (p1 == SIZE_MAX)
		return (NULL);
	ft_bitset(metadata, p1, p1 + bytes);
	metadata_end[p1 / META_BIT] |= 1 << (META_BIT - 1 - p1 % META_BIT);
	return (data + p1 * META_BIT * BLOCK_SIZE);
}
