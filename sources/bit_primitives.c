/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bit_primitives.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 10:36:32 by adeimlin          #+#    #+#             */
/*   Updated: 2025/12/16 11:06:59 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

// With O1 optimizes to lzcnt or bsr
size_t	ft_bsr(size_t word)
{
	size_t	i;

	if (word == 0)
		return (sizeof(word) * CHAR_BIT);
	i = SIZE_MAX;
	while (word)
	{
		word >>= 1;
		i++;
	}
	return (i);
}

size_t	ft_lzcnt(size_t word)
{
	size_t	i;

	i = sizeof(word) * CHAR_BIT;
	while (word)
	{
		word >>= 1;
		i--;
	}
	return (i);
}

size_t	ft_bsf(size_t word)
{
	size_t	i;

	word = word & -word;
	if (word == 0)
		return (sizeof(word) * CHAR_BIT);
	i = SIZE_MAX;
	while (word)
	{
		word >>= 1;
		i++;
	}
	return (i);
}

// With O1 optimizes to popcount
// Kernighan loop for bit counts
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
