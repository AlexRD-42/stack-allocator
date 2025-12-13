/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bit_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 10:36:32 by adeimlin          #+#    #+#             */
/*   Updated: 2025/12/13 19:26:05 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "ft_alloc.h"

size_t	ft_bsf(size_t word)
{
	size_t	i;

	i = 0;
    word = word & -word;
	word >>= 1;	// Review
	while (word)
	{
		word >>= 1;
		i++;
	}
	return (i);
}

// With O1 optimizes to popcount
size_t	ft_popcount(size_t word)
{
	size_t	i;

	i = 0;
	while (word)
	{
		word &= word - 1;
		i++;
	}
	return (i);
}

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
size_t	ft_bitfind(size_t *word, size_t start, size_t end, bool bit)
{
	size_t			cur;
	size_t			bitmap;
	const size_t	invert = SIZE_MAX * !bit;

	cur = start + META_BIT - (start % META_BIT);
	bitmap = (word[start / META_BIT] ^ invert) & (SIZE_MAX >> (start % META_BIT));
	if (bitmap != 0)
		return (cur - 1 - (ft_bsr(bitmap) - 1)); // the compiler needs the -1
	while (cur < end)
	{
		bitmap = word[cur / META_BIT] ^ invert;
		if (bitmap != 0)
			return (cur + (META_BIT - 1) - (ft_bsr(bitmap) - 1));
		cur += META_BIT;
	}
	return (SIZE_MAX);
}

void	ft_bitset(size_t bitmap[static META_COUNT], size_t start, size_t end)
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
		bitmap[word_start] |= start_mask & end_mask;
		return ;
	}
	bitmap[word_start] |= SIZE_MAX >> (start % META_BIT);
	i = word_start + 1;
	while (i < word_end)
		bitmap[i++] = SIZE_MAX;
	bitmap[word_end] |= end_mask;
}

void	ft_bitclr(size_t bitmap[static META_COUNT], size_t start, size_t end)
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
		bitmap[word_start] &= ~(start_mask & end_mask);
		return ;
	}
	bitmap[word_start] &= ~start_mask;
	i = word_start + 1;
	while (i < word_end)
		bitmap[i++] = 0;
	bitmap[word_end] &= ~end_mask;
}

void	ft_bitrange(size_t bitmap[static META_COUNT], size_t start, size_t end, bool set)
{
	size_t			start_mask;
	size_t			end_mask;

	start_mask = SIZE_MAX >> (start % META_BIT);
	start = start / META_BIT;
	end_mask = SIZE_MAX << ((META_BIT - (end % META_BIT)) % META_BIT);
	end = (end - 1) / META_BIT;
	if (set)
	{
		bitmap[start] |= SIZE_MAX >> (start % META_BIT);
		while (start < end)
			bitmap[start++] = SIZE_MAX;
		bitmap[end] |= end_mask;
	}
	else
	{
		bitmap[start] &= ~start_mask;
		while (start < end)
			bitmap[start++] = 0;
		bitmap[end] &= ~end_mask;	
	}
}
