/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 12:30:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/01/27 12:30:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_dispatch(char specifier, va_list *args)
{
	int	count;

	count = 0;
	if (specifier == 'c')
		count = ft_print_char((char)va_arg(*args, int));
	else if (specifier == 's')
		count = ft_print_str(va_arg(*args, char *));
	else if (specifier == 'p')
		count = ft_print_ptr(va_arg(*args, void *));
	else if (specifier == 'd' || specifier == 'i')
		count = ft_print_nbr(va_arg(*args, int));
	else if (specifier == 'u')
		count = ft_print_unsigned(va_arg(*args, unsigned int));
	else if (specifier == 'x' || specifier == 'X')
		count = ft_print_hex(va_arg(*args, unsigned int), specifier);
	else if (specifier == '%')
		count = ft_print_char('%');
	return (count);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		i;
	int		count;

	if (!format)
		return (-1);
	va_start(args, format);
	i = 0;
	count = 0;
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			i++;
			count += ft_dispatch(format[i], &args);
		}
		else
			count += ft_putchar_count(format[i]);
		i++;
	}
	va_end(args);
	return (count);
}
