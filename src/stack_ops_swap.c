/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_ops_swap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	swap(t_stack *s)
{
	int	tmp;

	if (s->size < 2)
		return (0);
	tmp = s->data[0];
	s->data[0] = s->data[1];
	s->data[1] = tmp;
	return (1);
}

int	op_sa(t_stack *a)
{
	return (swap(a));
}

int	op_sb(t_stack *b)
{
	return (swap(b));
}

int	op_ss(t_stack *a, t_stack *b)
{
	int	ret;

	ret = op_sa(a);
	ret += op_sb(b);
	return (ret > 0);
}
