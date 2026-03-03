/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bench.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	print_disorder(t_context *ctx)
{
	int	pct_int;
	int	pct_dec;

	pct_int = (int)(ctx->disorder * 100);
	pct_dec = (int)(ctx->disorder * 10000) % 100;
	ft_putstr_fd("[bench] disorder: ", 2);
	ft_putnbr_fd(pct_int, 2);
	ft_putstr_fd(".", 2);
	if (pct_dec < 10)
		ft_putstr_fd("0", 2);
	ft_putnbr_fd(pct_dec, 2);
	ft_putstr_fd("%\n", 2);
}

static const char	*strat_name(int strat, double disorder)
{
	if (strat == STRAT_SIMPLE)
		return ("Simple / O(n^2)");
	if (strat == STRAT_MEDIUM)
		return ("Medium / O(n*sqrt(n))");
	if (strat == STRAT_COMPLEX)
		return ("Complex / O(n*log(n))");
	if (disorder < 0.2)
		return ("Adaptive / O(n)");
	if (disorder < 0.5)
		return ("Adaptive / O(n*sqrt(n))");
	return ("Adaptive / O(n*log(n))");
}

static void	print_strategy(t_context *ctx)
{
	ft_putstr_fd("[bench] strategy: ", 2);
	ft_putstr_fd((char *)strat_name(ctx->strategy, ctx->disorder), 2);
	ft_putstr_fd("\n", 2);
	ft_putstr_fd("[bench] total_ops: ", 2);
	ft_putnbr_fd(ctx->total_ops, 2);
	ft_putstr_fd("\n", 2);
}

static void	print_ops_line(t_context *ctx, int start, int end)
{
	static const char	*names[] = {"sa", "sb", "ss", "pa", "pb",
		"ra", "rb", "rr", "rra", "rrb", "rrr"};
	int					i;

	ft_putstr_fd("[bench] ", 2);
	i = start;
	while (i <= end)
	{
		ft_putstr_fd((char *)names[i], 2);
		ft_putstr_fd(": ", 2);
		ft_putnbr_fd(ctx->ops[i], 2);
		if (i < end)
			ft_putstr_fd("  ", 2);
		i++;
	}
	ft_putstr_fd("\n", 2);
}

void	print_bench(t_context *ctx)
{
	print_disorder(ctx);
	print_strategy(ctx);
	print_ops_line(ctx, 0, 4);
	print_ops_line(ctx, 5, 10);
}
