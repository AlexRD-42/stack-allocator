/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bitfind_correct.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 11:53:27 by adeimlin          #+#    #+#             */
/*   Updated: 2025/12/13 18:56:16 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Correct bitfind
size_t	ft_bitfind(size_t *word, size_t start, size_t end)
{
	size_t			cur;
	size_t			bitmap;
	const size_t	word_end = end - (end % META_BIT);

	if (start >= end)
		return (SIZE_MAX);
	bitmap = word[start / META_BIT] & (SIZE_MAX >> (start % META_BIT));
	if ((start / META_BIT) == ((end - 1) / META_BIT) && (end % META_BIT))
		bitmap &= ~(SIZE_MAX >> (end % META_BIT));
	cur = start + META_BIT - (start % META_BIT);
	if (bitmap != 0)
		return (cur - 1 - ft_bsr(bitmap));
	while (cur < word_end)
	{
		bitmap = word[cur / META_BIT];
		if (bitmap != 0)
			return (cur + META_BIT - 1 - ft_bsr(bitmap));
		cur += META_BIT;
	}
	bitmap = word[cur / META_BIT] & (~(SIZE_MAX >> (end % META_BIT)));
	if (bitmap != 0)
		return (cur + META_BIT - 1 - ft_bsr(bitmap));
	return (SIZE_MAX);
}

size_t	ft_bsr_correct(size_t word, size_t start, size_t end, bool bit)
{
	size_t	i;
	size_t	mask;

	mask = SIZE_MAX >> (start % META_BIT);
	if (start - (start % META_BIT) + META_BIT > end)
		mask &= ~(SIZE_MAX >> (end % META_BIT));
	if (bit == 0)
		word = ~word & mask;
	else
		word = word & mask;
	i = 0;
	word >>= 1;	// Review
	while (word)
	{
		word >>= 1;
		i++;
	}
	return (i);
}

void	ft_bitset(size_t bitmap[static META_COUNT], size_t start, size_t end, bool set)
{
	size_t			word_start;
	size_t			start_mask;
	size_t			end_mask;
	const size_t	word_end = (end - 1) / META_BIT;
	const size_t	fill = SIZE_MAX * set;

	word_start = start / META_BIT;
	start_mask = SIZE_MAX >> (start % META_BIT);
	end_mask = SIZE_MAX << ((META_BIT - (end % META_BIT)) % META_BIT);
	if (word_start == word_end)
	{
		end_mask &= start_mask;
		bitmap[word_start] = (bitmap[word_start] & ~end_mask) | (fill & end_mask);
		return ;
	}
	bitmap[word_start] = (bitmap[word_start] & ~start_mask) | (fill & start_mask);
	if (set)
		while (++word_start < word_end)
			bitmap[word_start] = SIZE_MAX;
	if (!set)
		while (++word_start < word_end)
			bitmap[word_start] = 0;
	bitmap[word_end] = (bitmap[word_end] & ~end_mask) | (fill & end_mask);
}
