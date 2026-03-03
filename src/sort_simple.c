/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sort_simple(t_context *ctx)
{
	int	pos;

	if (ctx->a.size <= 1 || is_sorted(&ctx->a))
		return ;
	if (ctx->a.size <= 3)
	{
		if (ctx->a.size == 2)
			sort_two(ctx);
		else
			sort_three(ctx);
		return ;
	}
	while (ctx->a.size > 0)
	{
		pos = find_min_pos(&ctx->a);
		rotate_to_top(ctx, pos, 0);
		do_pb(ctx);
	}
	while (ctx->b.size > 0)
		do_pa(ctx);
}
