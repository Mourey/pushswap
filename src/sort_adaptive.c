/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_adaptive.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	is_rotated_sorted(t_stack *a)
{
	int	breaks;
	int	i;

	breaks = 0;
	i = 0;
	while (i < a->size - 1)
	{
		if (a->data[i] > a->data[i + 1])
			breaks++;
		if (breaks > 1)
			return (0);
		i++;
	}
	if (breaks == 1 && a->data[a->size - 1] > a->data[0])
		return (0);
	return (1);
}

static int	find_insert_pos(t_stack *a, int val)
{
	int	i;
	int	best_pos;
	int	best_val;

	best_pos = find_min_pos(a);
	best_val = -1;
	i = 0;
	while (i < a->size)
	{
		if (a->data[i] < val && a->data[i] > best_val)
		{
			best_val = a->data[i];
			best_pos = (i + 1) % a->size;
		}
		i++;
	}
	return (best_pos);
}

static void	reinsert_sorted(t_context *ctx)
{
	int	pos;

	while (ctx->b.size > 0)
	{
		pos = find_max_pos(&ctx->b);
		rotate_to_top(ctx, pos, 1);
		pos = find_insert_pos(&ctx->a, ctx->b.data[0]);
		rotate_to_top(ctx, pos, 0);
		do_pa(ctx);
	}
	rotate_to_top(ctx, find_min_pos(&ctx->a), 0);
}

static void	sort_low_disorder(t_context *ctx)
{
	int	n;
	int	last;
	int	i;

	rotate_to_top(ctx, find_min_pos(&ctx->a), 0);
	n = ctx->a.size;
	last = -1;
	i = -1;
	while (++i < n)
	{
		if (ctx->a.data[0] >= last)
		{
			last = ctx->a.data[0];
			do_ra(ctx);
		}
		else
			do_pb(ctx);
	}
	reinsert_sorted(ctx);
}

void	sort_adaptive(t_context *ctx)
{
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
	if (is_rotated_sorted(&ctx->a))
	{
		rotate_to_top(ctx, find_min_pos(&ctx->a), 0);
		return ;
	}
	if (ctx->disorder < 0.2)
		sort_low_disorder(ctx);
	else if (ctx->disorder < 0.5)
		sort_medium(ctx);
	else
		sort_complex(ctx);
}
