/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	error_exit(t_context *ctx)
{
	free_context(ctx);
	write(2, "Error\n", 6);
	exit(1);
}

static void	init_context(t_context *ctx)
{
	int	i;

	ctx->total_ops = 0;
	ctx->strategy = STRAT_ADAPTIVE;
	ctx->bench = 0;
	ctx->disorder = 0.0;
	i = 0;
	while (i < 11)
		ctx->ops[i++] = 0;
}

static void	dispatch_sort(t_context *ctx)
{
	if (ctx->a.size <= 1 || is_sorted(&ctx->a))
		return ;
	if (ctx->strategy == STRAT_SIMPLE)
		sort_simple(ctx);
	else if (ctx->strategy == STRAT_MEDIUM)
		sort_medium(ctx);
	else if (ctx->strategy == STRAT_COMPLEX)
		sort_complex(ctx);
	else
		sort_adaptive(ctx);
}

static int	create_stacks(t_context *ctx)
{
	if (!stack_create(&ctx->a, 10000))
		return (0);
	if (!stack_create(&ctx->b, 10000))
	{
		stack_destroy(&ctx->a);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_context	ctx;

	if (argc < 2)
		return (0);
	init_context(&ctx);
	if (!create_stacks(&ctx))
		return (1);
	if (!parse_args(argc, argv, &ctx))
		error_exit(&ctx);
	if (ctx.a.size == 0)
	{
		free_context(&ctx);
		return (0);
	}
	normalize_stack(&ctx.a);
	ctx.disorder = compute_disorder(&ctx.a);
	dispatch_sort(&ctx);
	if (ctx.bench)
		print_bench(&ctx);
	free_context(&ctx);
	return (0);
}
