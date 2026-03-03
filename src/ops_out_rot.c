/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops_out_rot.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/26 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	do_ra(t_context *ctx)
{
	op_ra(&ctx->a);
	write(1, "ra\n", 3);
	ctx->ops[OP_RA]++;
	ctx->total_ops++;
}

void	do_rb(t_context *ctx)
{
	op_rb(&ctx->b);
	write(1, "rb\n", 3);
	ctx->ops[OP_RB]++;
	ctx->total_ops++;
}

void	do_rr(t_context *ctx)
{
	op_rr(&ctx->a, &ctx->b);
	write(1, "rr\n", 3);
	ctx->ops[OP_RR]++;
	ctx->total_ops++;
}
