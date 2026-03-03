/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 16:30:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/01/19 16:30:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *newt)
{
	t_list	*last;

	if (!lst || !newt)
		return ;
	if (!*lst)
	{
		*lst = newt;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = newt;
}
