/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 21:55:18 by adeimlin          #+#    #+#             */
/*   Updated: 2025/12/16 16:16:01 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "ft_alloc.h"

// To do: optimize
void	*ft_memchr(const void *vptr, uint8_t byte, size_t length)
{
	const size_t	ones = SIZE_MAX / 0xFF;
	const size_t	high = ones * 0x80;
	const uint8_t	*ptr = (const uint8_t *) vptr;

	while (length > 0)
	{
		if (*ptr == byte)
			return (ptr);
		length--;
		ptr++;
	}
	return (NULL);
}

// Needs optimization
int64_t	ft_memcmp(const void *vptr1, const void *vptr2, size_t length)
{
	const uint8_t	*ptr1 = (const uint8_t *) vptr1;
	const uint8_t	*ptr2 = (const uint8_t *) vptr2;

	if (vptr1 == vptr2)
		return (0);
	while (length > 0)
	{
		if (*ptr1 != *ptr2)
			return (*ptr1 - *ptr2);
		ptr1++;
		ptr2++;
		length--;
	}
	return (0);
}
