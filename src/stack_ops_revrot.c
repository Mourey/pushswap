/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_ops_revrot.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/03/10 14:36:13 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	rev_rotate(t_stack *s)
{
	int	tmp;
	int	i;

	if (s->size < 2)
		return (0);
	tmp = s->data[s->size - 1];
	i = s->size - 1;
	while (i > 0)
	{
		s->data[i] = s->data[i - 1];
		i--;
	}
	s->data[0] = tmp;
	return (1);
}

int	op_rra(t_stack *a)
{
	return (rev_rotate(a));
}

int	op_rrb(t_stack *b)
{
	return (rev_rotate(b));
}

int	op_rrr(t_stack *a, t_stack *b)
{
	int	ret;

	ret = rev_rotate(a);
	ret += rev_rotate(b);
	return (ret > 0);
}
