/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"
#include "../libft/get_next_line.h"

static int	exec_swap_push(t_context *ctx, char *line)
{
	if (ft_strncmp(line, "sa\n", 3) == 0)
		op_sa(&ctx->a);
	else if (ft_strncmp(line, "sb\n", 3) == 0)
		op_sb(&ctx->b);
	else if (ft_strncmp(line, "ss\n", 3) == 0)
		op_ss(&ctx->a, &ctx->b);
	else if (ft_strncmp(line, "pa\n", 3) == 0)
		op_pa(&ctx->a, &ctx->b);
	else if (ft_strncmp(line, "pb\n", 3) == 0)
		op_pb(&ctx->a, &ctx->b);
	else
		return (0);
	return (1);
}

static int	exec_rotate(t_context *ctx, char *line)
{
	if (ft_strncmp(line, "rra\n", 4) == 0)
		op_rra(&ctx->a);
	else if (ft_strncmp(line, "rrb\n", 4) == 0)
		op_rrb(&ctx->b);
	else if (ft_strncmp(line, "rrr\n", 4) == 0)
		op_rrr(&ctx->a, &ctx->b);
	else if (ft_strncmp(line, "ra\n", 3) == 0)
		op_ra(&ctx->a);
	else if (ft_strncmp(line, "rb\n", 3) == 0)
		op_rb(&ctx->b);
	else if (ft_strncmp(line, "rr\n", 3) == 0)
		op_rr(&ctx->a, &ctx->b);
	else
		return (0);
	return (1);
}

int	exec_operation(t_context *ctx, char *line)
{
	if (exec_swap_push(ctx, line))
		return (1);
	if (exec_rotate(ctx, line))
		return (1);
	return (0);
}

int	read_and_exec(t_context *ctx)
{
	char	*line;

	line = get_next_line(0);
	while (line)
	{
		if (!exec_operation(ctx, line))
		{
			free(line);
			return (0);
		}
		free(line);
		line = get_next_line(0);
	}
	return (1);
}
