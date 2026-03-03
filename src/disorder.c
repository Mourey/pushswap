/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disorder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static long	count_inversions(t_stack *a)
{
	long	inv;
	int		i;
	int		j;

	inv = 0;
	i = 0;
	while (i < a->size)
	{
		j = i + 1;
		while (j < a->size)
		{
			if (a->data[i] > a->data[j])
				inv++;
			j++;
		}
		i++;
	}
	return (inv);
}

double	compute_disorder(t_stack *a)
{
	long	inv;
	long	max_inv;

	if (a->size <= 1)
		return (0.0);
	inv = count_inversions(a);
	max_inv = (long)a->size * (a->size - 1) / 2;
	if (max_inv == 0)
		return (0.0);
	return ((double)inv / (double)max_inv);
}
