/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_medium.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int		ft_sqrt_approx(int n);
int		get_chunk_size(int n);
void	push_back_max(t_context *ctx);

static void	push_chunk(t_context *ctx, int lo, int hi)
{
	int	mid;
	int	pushed;
	int	size;

	mid = (lo + hi) / 2;
	pushed = 0;
	size = ctx->a.size;
	while (pushed < (hi - lo + 1) && size > 0)
	{
		if (ctx->a.data[0] >= lo && ctx->a.data[0] <= hi)
		{
			do_pb(ctx);
			if (ctx->b.data[0] < mid)
				do_rb(ctx);
			pushed++;
		}
		else
			do_ra(ctx);
		size--;
	}
}

static void	phase_one(t_context *ctx, int chunk_sz)
{
	int	lo;
	int	hi;
	int	n;

	n = ctx->a.size;
	lo = 0;
	while (lo < n)
	{
		hi = lo + chunk_sz - 1;
		if (hi >= n)
			hi = n - 1;
		push_chunk(ctx, lo, hi);
		lo = hi + 1;
	}
}

static void	phase_two(t_context *ctx)
{
	while (ctx->b.size > 0)
		push_back_max(ctx);
}

void	sort_medium(t_context *ctx)
{
	int	chunk_sz;

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
	chunk_sz = get_chunk_size(ctx->a.size);
	phase_one(ctx, chunk_sz);
	phase_two(ctx);
}
