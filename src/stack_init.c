/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	stack_create(t_stack *s, int capacity)
{
	s->data = malloc(sizeof(int) * capacity);
	if (!s->data)
		return (0);
	s->size = 0;
	s->capacity = capacity;
	return (1);
}

void	stack_destroy(t_stack *s)
{
	if (s->data)
		free(s->data);
	s->data = NULL;
	s->size = 0;
	s->capacity = 0;
}

void	free_context(t_context *ctx)
{
	stack_destroy(&ctx->a);
	stack_destroy(&ctx->b);
}
