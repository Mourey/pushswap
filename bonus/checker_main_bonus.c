/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"
#include "../libft/get_next_line.h"

int		exec_operation(t_context *ctx, char *line);
int		read_and_exec(t_context *ctx);

static void	checker_error_exit(t_context *ctx)
{
	free_context(ctx);
	write(2, "Error\n", 6);
	exit(1);
}

static void	init_checker_ctx(t_context *ctx)
{
	int	i;

	ctx->total_ops = 0;
	ctx->strategy = 0;
	ctx->bench = 0;
	ctx->disorder = 0.0;
	i = 0;
	while (i < 11)
		ctx->ops[i++] = 0;
}

static void	check_result(t_context *ctx)
{
	if (is_sorted(&ctx->a) && ctx->b.size == 0)
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
}

int	main(int argc, char **argv)
{
	t_context	ctx;

	if (argc < 2)
		return (0);
	init_checker_ctx(&ctx);
	if (!stack_create(&ctx.a, 10000))
		return (1);
	if (!stack_create(&ctx.b, 10000))
	{
		stack_destroy(&ctx.a);
		return (1);
	}
	if (!parse_args(argc, argv, &ctx))
		checker_error_exit(&ctx);
	if (ctx.a.size == 0)
	{
		free_context(&ctx);
		return (0);
	}
	if (!read_and_exec(&ctx))
		checker_error_exit(&ctx);
	check_result(&ctx);
	free_context(&ctx);
	return (0);
}
