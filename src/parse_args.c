/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	check_duplicates(t_stack *a)
{
	int	i;
	int	j;

	i = 0;
	while (i < a->size)
	{
		j = i + 1;
		while (j < a->size)
		{
			if (a->data[i] == a->data[j])
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

static int	parse_flag(char *arg, t_context *ctx)
{
	if (ft_strncmp(arg, "--simple", 9) == 0)
		ctx->strategy = STRAT_SIMPLE;
	else if (ft_strncmp(arg, "--medium", 9) == 0)
		ctx->strategy = STRAT_MEDIUM;
	else if (ft_strncmp(arg, "--complex", 10) == 0)
		ctx->strategy = STRAT_COMPLEX;
	else if (ft_strncmp(arg, "--adaptive", 11) == 0)
		ctx->strategy = STRAT_ADAPTIVE;
	else if (ft_strncmp(arg, "--bench", 8) == 0)
		ctx->bench = 1;
	else
		return (0);
	return (1);
}

static int	add_number(t_context *ctx, char *str)
{
	long	val;

	if (!is_valid_integer(str))
		return (0);
	if (!ft_atol_safe(str, &val))
		return (0);
	ctx->a.data[ctx->a.size] = (int)val;
	ctx->a.size++;
	return (1);
}

static int	parse_split_arg(t_context *ctx, char *arg)
{
	char	**tokens;
	int		i;
	int		ret;

	tokens = ft_split(arg, ' ');
	if (!tokens)
		return (0);
	i = 0;
	ret = 1;
	while (tokens[i] && ret)
	{
		if (tokens[i][0] == '-' && tokens[i][1] == '-')
			ret = parse_flag(tokens[i], ctx);
		else
			ret = add_number(ctx, tokens[i]);
		i++;
	}
	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
	return (ret);
}

int	parse_args(int argc, char **argv, t_context *ctx)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '-' && argv[i][1] == '-')
		{
			if (!parse_flag(argv[i], ctx))
				return (0);
		}
		else if (!parse_split_arg(ctx, argv[i]))
			return (0);
		i++;
	}
	if (!check_duplicates(&ctx->a))
		return (0);
	return (1);
}
