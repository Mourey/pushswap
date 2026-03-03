/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_complex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	get_max_bits(int n)
{
	int	bits;

	bits = 0;
	while ((n >> bits) != 0)
		bits++;
	return (bits);
}

static void	radix_pass(t_context *ctx, int bit)
{
	int	size;
	int	i;

	size = ctx->a.size;
	i = 0;
	while (i < size)
	{
		if ((ctx->a.data[0] >> bit) & 1)
			do_ra(ctx);
		else
			do_pb(ctx);
		i++;
	}
	while (ctx->b.size > 0)
		do_pa(ctx);
}

void	sort_complex(t_context *ctx)
{
	int	max_bits;
	int	bit;

	if (ctx->a.size <= 1 || is_sorted(&ctx->a))
		return ;
	if (ctx->a.size <= 5)
	{
		if (ctx->a.size == 2)
			sort_two(ctx);
		else if (ctx->a.size == 3)
			sort_three(ctx);
		else
			sort_five(ctx);
		return ;
	}
	max_bits = get_max_bits(ctx->a.size - 1);
	bit = 0;
	while (bit < max_bits)
	{
		radix_pass(ctx, bit);
		bit++;
	}
}
