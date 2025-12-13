/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:02:43 by adeimlin          #+#    #+#             */
/*   Updated: 2025/12/12 16:26:18 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "ft_alloc.h"

// 0b10010001
// 0b01101110
size_t	ft_membsr(size_t *word, size_t start, size_t request, size_t count)
{
	size_t	i;
	size_t	run;
	size_t	offset;
	size_t	bitmap;

	run = 0;
	i = start / META_BIT;
	offset = start % META_BIT;
	bitmap = word[i] & (SIZE_MAX >> offset);
	if (bitmap != 0)
		return (META_BIT - 1 - ft_bsr(bitmap) + i * META_BIT);
	run += META_BIT - offset;
	i++;
	while (run < request && i < count)
	{
		bitmap = word[i];
		if (bitmap != 0)
			return (META_BIT - 1 - ft_bsr(bitmap) + i * META_BIT);
		run += META_BIT;
		i++;
	}
	return (start + run);
}

size_t	ft_membsr_not(size_t *word, size_t start, size_t request, size_t count)
{
	size_t	i;
	size_t	run;
	size_t	offset;
	size_t	bitmap;

	run = 0;
	i = start / META_BIT;
	offset = start % META_BIT;
	bitmap = ~word[i] & (SIZE_MAX >> offset);
	if (bitmap != 0)
		return (META_BIT - 1 - ft_bsr(bitmap) + i * META_BIT);
	run += META_BIT - offset;
	i++;
	while (run < request && i < count)
	{
		bitmap = ~word[i];
		if (bitmap != 0)
			return (META_BIT - 1 - ft_bsr(bitmap) + i * META_BIT);
		run += META_BIT;
		i++;
	}
	return (start + run);
}

size_t	ft_first_fit(size_t *word, size_t request, size_t count)
{
	const size_t	end = count * META_BIT;
	size_t			p1;
	size_t			p2;

	p1 = 0;
	while (p1 < end)
	{
		p2 = ft_membsr(word, p1, request, count);		// Scans for the first set bit
		if (p2 - p1 >= request)
			return (p1);
		p1 = ft_membsr_not(word, p2, request, count);	// Scans for the first unset bit
	}
	return (SIZE_MAX);
}

int	main(int argc, char **argv, char **envp)
{
	size_t	map[2] = {0xFFF00FF0F000FFFF, 0xFFF0000FFFFFFFFF};
	size_t	first_zero;
	size_t	shift_count;
	size_t	first_one;
	size_t	number_of_zeros;

	first_zero = ft_first_fit(map, 14, 2);
	return (0);
}
