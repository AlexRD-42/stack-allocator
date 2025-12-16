/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   static_allocator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 15:24:01 by adeimlin          #+#    #+#             */
/*   Updated: 2025/12/16 16:39:50 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "ft_alloc.h"

# ifndef MEMORY_SIZE
#  define MEMORY_SIZE 1024
# endif

# ifndef BLOCK_SIZE
#  define BLOCK_SIZE __SIZEOF_POINTER__
# endif

static uint8_t	data[MEMORY_SIZE];
static size_t	metadata[MEMORY_SIZE / (BLOCK_SIZE * WORD_BIT)];
static size_t	metadata_end[MEMORY_SIZE / (BLOCK_SIZE * WORD_BIT)];

// For pointers within range, it scans metadata_end until its terminator byte
// UB for invalid PTR
void	ft_free(void *ptr)
{
	size_t	offset;
	size_t	end;
	size_t	bit_mask;

	offset = (uintptr_t)ptr - (uintptr_t)data;
	if (ptr == NULL || offset >= MEMORY_SIZE)
		return ;
	offset /= BLOCK_SIZE;
	end = ft_bitfind(metadata_end, offset, MEMORY_SIZE / BLOCK_SIZE, 1);
	if (end > MEMORY_SIZE / BLOCK_SIZE)
		return ;
	ft_bitset(metadata, offset, end + 1, 0);
	bit_mask = ~((size_t)1 << (WORD_BIT - 1 - (end % WORD_BIT)));
	metadata_end[end / WORD_BIT] &= bit_mask;
}

// Size of metadata is 1 bit per memory block
void	*ft_alloc(size_t bytes)
{
	size_t			p1;
	size_t			p2;
	size_t			blocks;

	blocks = ((bytes + (BLOCK_SIZE - 1)) / BLOCK_SIZE);
	if (bytes == 0 || bytes >= MEMORY_SIZE || blocks > MEMORY_SIZE / BLOCK_SIZE)
		return (NULL);
	p1 = ft_bitfind(metadata, 0, MEMORY_SIZE / BLOCK_SIZE, 0);
	while (p1 <= MEMORY_SIZE / BLOCK_SIZE - blocks)
	{
		p2 = ft_bitfind(metadata, p1, p1 + blocks, 1); // Scans for the first set bit
		if (p2 - p1 >= blocks)	// Bit pos delta, when p2 == SIZE_MAX also breaks
			break ;
		p1 = ft_bitfind(metadata, p2, MEMORY_SIZE / BLOCK_SIZE, 0); // Scans for the first unset bit
	}
	if (p1 == SIZE_MAX || p1 + blocks > MEMORY_SIZE / BLOCK_SIZE)
		return (NULL);
	ft_bitset(metadata, p1, p1 + blocks, 1);
	p2 = p1 + blocks - 1;
	metadata_end[p2 / WORD_BIT] |= (size_t)1 << (WORD_BIT - 1 - p2 % WORD_BIT);
	return (data + p1 * BLOCK_SIZE);
}
