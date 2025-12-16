/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm_static_allocator.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 12:02:20 by adeimlin          #+#    #+#             */
/*   Updated: 2025/12/15 14:45:14 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>

# ifndef MEMORY_SIZE
#  define MEMORY_SIZE 65536u
# endif

# ifndef BLOCK_SIZE
#  define BLOCK_SIZE __SIZEOF_POINTER__
# endif

size_t	ft_bitfind(const size_t *word, size_t start, size_t end, bool bit);
void	ft_bitset(size_t *bitmap, size_t start, size_t end);
void	ft_bitclr(size_t *bitmap, size_t start, size_t end);

enum e_sizes
{
	WORD_SIZE = sizeof(size_t),
	WORD_BIT = WORD_SIZE * CHAR_BIT,
};

// For pointers within range, it scans metadata_end until its terminator byte
// UB for invalid PTR
void	stt_free(void *ptr, uint8_t *data, size_t *metadata, size_t *metadata_end)
{
	ptrdiff_t		start;
	size_t			end;
	size_t			bit_mask;

	if (ptr == NULL)
		return ;
	start = ((uint8_t *)ptr - data);
	if (start < 0 || (size_t)start >= MEMORY_SIZE)
		return ;
	start /= BLOCK_SIZE;
	end = ft_bitfind(metadata_end, (size_t)start, MEMORY_SIZE / BLOCK_SIZE, 1);
	if (end > MEMORY_SIZE / BLOCK_SIZE)
		return ;
	ft_bitclr(metadata, (size_t)start, end);
	bit_mask = ~((size_t)1 << (WORD_BIT - 1 - (end % WORD_BIT)));
	metadata_end[end / WORD_BIT] &= bit_mask;
}

// Size of metadata is 1 bit per memory block
void	*stt_alloc(size_t bytes, uint8_t *data, size_t *metadata, size_t *metadata_end)
{
	size_t			p1;
	size_t			p2;
	size_t			blocks;

	if (bytes == 0 || bytes >= MEMORY_SIZE)
		return (NULL);
	blocks = ((bytes + (BLOCK_SIZE - 1)) / BLOCK_SIZE);
	p1 = ft_bitfind(metadata, 0, MEMORY_SIZE / BLOCK_SIZE, 0);
	while (p1 != SIZE_MAX)
	{
		p2 = ft_bitfind(metadata, p1, p1 + blocks, 1); // Scans for the first set bit
		if (p2 - p1 >= blocks)	// Bit pos delta, when p2 == SIZE_MAX also breaks
			break ;
		p1 = ft_bitfind(metadata, p2, MEMORY_SIZE / BLOCK_SIZE, 0); // Scans for the first unset bit
	}
	if ((p1 | p2) == SIZE_MAX)
		return (NULL);
	p2 = p1 + blocks;
	ft_bitset(metadata, p1, p1 + blocks);
	metadata_end[p2 / WORD_BIT] |= (size_t)1 << (WORD_BIT - 1 - p2 % WORD_BIT);
	return (data + p1 * BLOCK_SIZE);
}

static
void	*stt_core(void *ptr, size_t bytes, bool action)
{
	static uint8_t	data[MEMORY_SIZE];
	static size_t	metadata[MEMORY_SIZE / (BLOCK_SIZE * WORD_BIT)];
	static size_t	metadata_end[MEMORY_SIZE / (BLOCK_SIZE * WORD_BIT)];

	if (action)
		return (stt_alloc(bytes, data, metadata, metadata_end));
	else
		stt_free(ptr, data, metadata, metadata_end);
	return (NULL);
}

void	ft_free(void *ptr)
{
	stt_core(ptr, 0, 0);
}

void	ft_alloc(size_t bytes)
{
	stt_core(NULL, bytes, 1);
}