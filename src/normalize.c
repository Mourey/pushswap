/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/26 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	sort_copy(int *arr, int n)
{
	int	i;
	int	j;
	int	tmp;

	i = 0;
	while (i < n - 1)
	{
		j = i + 1;
		while (j < n)
		{
			if (arr[i] > arr[j])
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static int	bin_search(int *sorted, int n, int val)
{
	int	lo;
	int	hi;
	int	mid;

	lo = 0;
	hi = n - 1;
	while (lo <= hi)
	{
		mid = (lo + hi) / 2;
		if (sorted[mid] == val)
			return (mid);
		else if (sorted[mid] < val)
			lo = mid + 1;
		else
			hi = mid - 1;
	}
	return (-1);
}

void	normalize_stack(t_stack *a)
{
	int	*sorted;
	int	i;

	sorted = malloc(sizeof(int) * a->size);
	if (!sorted)
		return ;
	i = -1;
	while (++i < a->size)
		sorted[i] = a->data[i];
	sort_copy(sorted, a->size);
	i = -1;
	while (++i < a->size)
		a->data[i] = bin_search(sorted, a->size, a->data[i]);
	free(sorted);
}
