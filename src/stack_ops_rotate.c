/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_ops_rotate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	rotate(t_stack *s)
{
	int	tmp;
	int	i;

	if (s->size < 2)
		return (0);
	tmp = s->data[0];
	i = 0;
	while (i < s->size - 1)
	{
		s->data[i] = s->data[i + 1];
		i++;
	}
	s->data[s->size - 1] = tmp;
	return (1);
}

int	op_ra(t_stack *a)
{
	return (rotate(a));
}

int	op_rb(t_stack *b)
{
	return (rotate(b));
}

int	op_rr(t_stack *a, t_stack *b)
{
	int	ret;

	ret = rotate(a);
	ret += rotate(b);
	return (ret > 0);
}
