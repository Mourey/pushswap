/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_tiny.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sort_two(t_context *ctx)
{
	if (ctx->a.data[0] > ctx->a.data[1])
		do_sa(ctx);
}

void	sort_three(t_context *ctx)
{
	int	a;
	int	b;
	int	c;

	a = ctx->a.data[0];
	b = ctx->a.data[1];
	c = ctx->a.data[2];
	if (a > b && b < c && a < c)
		do_sa(ctx);
	else if (a > b && b > c)
	{
		do_sa(ctx);
		do_rra(ctx);
	}
	else if (a > b && b < c && a > c)
		do_ra(ctx);
	else if (a < b && b > c && a < c)
	{
		do_sa(ctx);
		do_ra(ctx);
	}
	else if (a < b && b > c && a > c)
		do_rra(ctx);
}

static void	push_min_to_b(t_context *ctx)
{
	int	pos;

	pos = find_min_pos(&ctx->a);
	rotate_to_top(ctx, pos, 0);
	do_pb(ctx);
}

void	sort_five(t_context *ctx)
{
	while (ctx->a.size > 3)
		push_min_to_b(ctx);
	sort_three(ctx);
	while (ctx->b.size > 0)
		do_pa(ctx);
}
