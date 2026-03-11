/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate_both.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/03/11 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	emit_overlap(t_context *ctx, int n, int reverse)
{
	while (n-- > 0)
	{
		if (reverse)
			do_rrr(ctx);
		else
			do_rr(ctx);
	}
}

static void	emit_remainder(t_context *ctx, int n, int cmd)
{
	while (n-- > 0)
	{
		if (cmd == 0)
			do_ra(ctx);
		else if (cmd == 1)
			do_rb(ctx);
		else if (cmd == 2)
			do_rra(ctx);
		else
			do_rrb(ctx);
	}
}

static void	apply_combined(t_context *ctx, int *c)
{
	int	overlap;

	if (c[2] == c[3])
	{
		if (c[0] < c[1])
			overlap = c[0];
		else
			overlap = c[1];
		emit_overlap(ctx, overlap, c[2]);
		if (c[2] == 0)
		{
			emit_remainder(ctx, c[0] - overlap, 0);
			emit_remainder(ctx, c[1] - overlap, 1);
		}
		else
		{
			emit_remainder(ctx, c[0] - overlap, 2);
			emit_remainder(ctx, c[1] - overlap, 3);
		}
	}
	else
	{
		emit_remainder(ctx, c[0], c[2] * 2);
		emit_remainder(ctx, c[1], 1 + c[3] * 2);
	}
}

static int	eval_combo(int *c, int *sizes, int pos_a, int pos_b)
{
	if (c[2] == 0)
		c[0] = pos_a;
	else
		c[0] = sizes[0] - pos_a;
	if (c[3] == 0)
		c[1] = pos_b;
	else
		c[1] = sizes[1] - pos_b;
	if (c[2] == c[3] && c[0] > c[1])
		return (c[0]);
	if (c[2] == c[3])
		return (c[1]);
	return (c[0] + c[1]);
}

void	rotate_both_to_top(t_context *ctx, int pos_a, int pos_b)
{
	int	c[4];
	int	best[4];
	int	sizes[2];
	int	bt;
	int	t;

	sizes[0] = ctx->a.size;
	sizes[1] = ctx->b.size;
	bt = INT_MAX;
	c[2] = -1;
	while (++c[2] <= 1)
	{
		c[3] = -1;
		while (++c[3] <= 1)
		{
			t = eval_combo(c, sizes, pos_a, pos_b);
			if (t < bt)
			{
				bt = t;
				ft_memcpy(best, c, sizeof(int) * 4);
			}
		}
	}
	apply_combined(ctx, best);
}
