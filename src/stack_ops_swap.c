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

void	op_sa(t_stack *a)
{
	int	tmp;

	if (a->size < 2)
		return ;
	tmp = a->data[0];
	a->data[0] = a->data[1];
	a->data[1] = tmp;
}

void	op_sb(t_stack *b)
{
	int	tmp;

	if (b->size < 2)
		return ;
	tmp = b->data[0];
	b->data[0] = b->data[1];
	b->data[1] = tmp;
}

void	op_ss(t_stack *a, t_stack *b)
{
	op_sa(a);
	op_sb(b);
}
