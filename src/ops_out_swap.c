/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_out_swap.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/26 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	do_sa(t_context *ctx)
{
	op_sa(&ctx->a);
	write(1, "sa\n", 3);
	ctx->ops[OP_SA]++;
	ctx->total_ops++;
}

void	do_sb(t_context *ctx)
{
	op_sb(&ctx->b);
	write(1, "sb\n", 3);
	ctx->ops[OP_SB]++;
	ctx->total_ops++;
}

void	do_ss(t_context *ctx)
{
	op_ss(&ctx->a, &ctx->b);
	write(1, "ss\n", 3);
	ctx->ops[OP_SS]++;
	ctx->total_ops++;
}

void	do_pa(t_context *ctx)
{
	op_pa(&ctx->a, &ctx->b);
	write(1, "pa\n", 3);
	ctx->ops[OP_PA]++;
	ctx->total_ops++;
}

void	do_pb(t_context *ctx)
{
	op_pb(&ctx->a, &ctx->b);
	write(1, "pb\n", 3);
	ctx->ops[OP_PB]++;
	ctx->total_ops++;
}
