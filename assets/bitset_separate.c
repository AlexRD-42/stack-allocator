void	ft_bitset(size_t *bitmap, size_t start, size_t end)
{
	const size_t	word_start = start / WORD_BIT;
	const size_t	word_end = (end - 1) / WORD_BIT;
	const size_t	start_mask = SIZE_MAX >> (start % WORD_BIT);
	const size_t	end_mask = SIZE_MAX << (-end % WORD_BIT);
	const size_t	diff_mask = (size_t)-(word_start != word_end);

	start = word_start + 1;
	bitmap[word_start] |= start_mask & (end_mask | diff_mask);
	while (start < word_end)
		bitmap[start++] = SIZE_MAX;
	bitmap[word_end] |= end_mask & diff_mask;
}

// Bitset and bitclr are kept separate to reduce branching
void	ft_bitclr(size_t *bitmap, size_t start, size_t end)
{
	const size_t	word_start = start / WORD_BIT;
	const size_t	word_end = (end - 1) / WORD_BIT;
	const size_t	start_mask = SIZE_MAX >> (start % WORD_BIT);
	const size_t	end_mask = SIZE_MAX << (-end % WORD_BIT);
	const size_t	diff_mask = (size_t)-(word_start != word_end);

	start = word_start + 1;
	bitmap[word_start] &= ~(start_mask & (end_mask | diff_mask));
	while (start < word_end)
		bitmap[start++] = 0;
	bitmap[word_end] &= ~(end_mask & diff_mask);
}