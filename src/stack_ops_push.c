/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_ops_push.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	push_front(t_stack *s, int val)
{
	int	i;

	i = s->size;
	while (i > 0)
	{
		s->data[i] = s->data[i - 1];
		i--;
	}
	s->data[0] = val;
	s->size++;
}

static int	pop_front(t_stack *s)
{
	int	val;
	int	i;

	val = s->data[0];
	i = 0;
	while (i < s->size - 1)
	{
		s->data[i] = s->data[i + 1];
		i++;
	}
	s->size--;
	return (val);
}

void	op_pa(t_stack *a, t_stack *b)
{
	if (b->size == 0)
		return ;
	push_front(a, pop_front(b));
}

void	op_pb(t_stack *a, t_stack *b)
{
	if (a->size == 0)
		return ;
	push_front(b, pop_front(a));
}
