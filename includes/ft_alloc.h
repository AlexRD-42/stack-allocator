/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_alloc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:01:57 by adeimlin          #+#    #+#             */
/*   Updated: 2025/12/13 19:24:13 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ALLOC_H
# define FT_ALLOC_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include <limits.h>

// Assert memory size, block size and meta size are divisible
# ifndef MEMORY_SIZE
#  define MEMORY_SIZE 65536u
# endif

# ifndef BLOCK_SIZE
#  define BLOCK_SIZE __SIZEOF_POINTER__
# endif

enum e_sizes
{
	BLOCK_BIT = BLOCK_SIZE * CHAR_BIT,
	BLOCK_COUNT = MEMORY_SIZE / BLOCK_SIZE,
	META_SIZE = sizeof(size_t),
	META_BIT = META_SIZE * CHAR_BIT,
	META_COUNT = BLOCK_COUNT / META_BIT
};

size_t	ft_bsf(size_t word);
size_t	ft_bsr(size_t word);
size_t	ft_bsr(size_t word);
size_t	ft_bitfind(size_t *word, size_t start, size_t end, bool bit);
void	ft_bitset(size_t bitmap[static META_COUNT], size_t start, size_t end);
void	ft_bitclr(size_t bitmap[static META_COUNT], size_t start, size_t end);
void	ft_bitrange(size_t bitmap[static META_COUNT], size_t start, size_t end, bool bit);

size_t	ft_popcount(size_t word);

typedef struct s_bitptr
{
	void		*ptr;
	uint32_t	index;
	uint32_t	offset;
}	t_bitptr;

typedef struct s_bitpos
{
	uint32_t	index;		// Array index
	uint32_t	offset;		// Bit offset (max 64)
}	t_bitpos;

typedef struct s_vec
{
	void	*start;
	void	*ptr;
	void	*end;
}	t_vec;

typedef struct s_vec2
{
	void		*data;
	uint32_t	offset;
	uint32_t	capacity;
}	t_vec2;

#endif
