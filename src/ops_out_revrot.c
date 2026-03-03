/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_out_revrot.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/26 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	do_rra(t_context *ctx)
{
	op_rra(&ctx->a);
	write(1, "rra\n", 4);
	ctx->ops[OP_RRA]++;
	ctx->total_ops++;
}

void	do_rrb(t_context *ctx)
{
	op_rrb(&ctx->b);
	write(1, "rrb\n", 4);
	ctx->ops[OP_RRB]++;
	ctx->total_ops++;
}

void	do_rrr(t_context *ctx)
{
	op_rrr(&ctx->a, &ctx->b);
	write(1, "rrr\n", 4);
	ctx->ops[OP_RRR]++;
	ctx->total_ops++;
}
