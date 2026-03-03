/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	is_sorted(t_stack *a)
{
	int	i;

	i = 0;
	while (i < a->size - 1)
	{
		if (a->data[i] > a->data[i + 1])
			return (0);
		i++;
	}
	return (1);
}

int	find_min_pos(t_stack *s)
{
	int	i;
	int	pos;

	pos = 0;
	i = 1;
	while (i < s->size)
	{
		if (s->data[i] < s->data[pos])
			pos = i;
		i++;
	}
	return (pos);
}

int	find_max_pos(t_stack *s)
{
	int	i;
	int	pos;

	pos = 0;
	i = 1;
	while (i < s->size)
	{
		if (s->data[i] > s->data[pos])
			pos = i;
		i++;
	}
	return (pos);
}

static void	rot_dir(t_context *ctx, int stack_id, int reverse)
{
	if (!reverse && stack_id == 0)
		do_ra(ctx);
	else if (!reverse)
		do_rb(ctx);
	else if (stack_id == 0)
		do_rra(ctx);
	else
		do_rrb(ctx);
}

void	rotate_to_top(t_context *ctx, int pos, int stack_id)
{
	int	size;

	if (stack_id == 0)
		size = ctx->a.size;
	else
		size = ctx->b.size;
	if (pos <= size / 2)
	{
		while (pos-- > 0)
			rot_dir(ctx, stack_id, 0);
	}
	else
	{
		while (pos++ < size)
			rot_dir(ctx, stack_id, 1);
	}
}
