/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_medium_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_sqrt_approx(int n)
{
	int	i;

	i = 1;
	while (i * i < n)
		i++;
	return (i);
}

int	get_chunk_size(int n)
{
	int	size;

	size = (n * 3) / 100 + 14;
	if (size < 5)
		size = 5;
	return (size);
}

void	push_back_max(t_context *ctx)
{
	int	pos;

	pos = find_max_pos(&ctx->b);
	rotate_to_top(ctx, pos, 1);
	do_pa(ctx);
}
