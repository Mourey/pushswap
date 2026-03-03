/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmourey- <rmourey-@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 12:00:00 by rmourey-          #+#    #+#             */
/*   Updated: 2026/02/24 12:00:00 by rmourey-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "../libft/libft.h"
# include "../libft/ft_printf.h"
# include <limits.h>

# define OP_SA   0
# define OP_SB   1
# define OP_SS   2
# define OP_PA   3
# define OP_PB   4
# define OP_RA   5
# define OP_RB   6
# define OP_RR   7
# define OP_RRA  8
# define OP_RRB  9
# define OP_RRR  10

# define STRAT_SIMPLE   0
# define STRAT_MEDIUM   1
# define STRAT_COMPLEX  2
# define STRAT_ADAPTIVE 3

typedef struct s_stack
{
	int		*data;
	int		size;
	int		capacity;
}	t_stack;

typedef struct s_context
{
	t_stack	a;
	t_stack	b;
	int		ops[11];
	int		total_ops;
	int		strategy;
	int		bench;
	double	disorder;
}	t_context;

/* stack_init.c */
int		stack_create(t_stack *s, int capacity);
void	stack_destroy(t_stack *s);
void	free_context(t_context *ctx);

/* normalize.c */
void	normalize_stack(t_stack *a);

/* stack_ops_swap.c */
void	op_sa(t_stack *a);
void	op_sb(t_stack *b);
void	op_ss(t_stack *a, t_stack *b);

/* stack_ops_push.c */
void	op_pa(t_stack *a, t_stack *b);
void	op_pb(t_stack *a, t_stack *b);

/* stack_ops_rotate.c */
void	op_ra(t_stack *a);
void	op_rb(t_stack *b);
void	op_rr(t_stack *a, t_stack *b);

/* stack_ops_revrot.c */
void	op_rra(t_stack *a);
void	op_rrb(t_stack *b);
void	op_rrr(t_stack *a, t_stack *b);

/* ops_out_swap.c */
void	do_sa(t_context *ctx);
void	do_sb(t_context *ctx);
void	do_ss(t_context *ctx);
void	do_pa(t_context *ctx);
void	do_pb(t_context *ctx);

/* ops_out_rot.c */
void	do_ra(t_context *ctx);
void	do_rb(t_context *ctx);
void	do_rr(t_context *ctx);

/* ops_out_revrot.c */
void	do_rra(t_context *ctx);
void	do_rrb(t_context *ctx);
void	do_rrr(t_context *ctx);

/* parse_utils.c */
int		ft_atol_safe(const char *str, long *result);
int		is_valid_integer(const char *str);

/* parse_args.c */
int		parse_args(int argc, char **argv, t_context *ctx);

/* disorder.c */
double	compute_disorder(t_stack *a);

/* utils.c */
int		is_sorted(t_stack *a);
int		find_min_pos(t_stack *s);
int		find_max_pos(t_stack *s);
void	rotate_to_top(t_context *ctx, int pos, int stack_id);

/* sort_tiny.c */
void	sort_two(t_context *ctx);
void	sort_three(t_context *ctx);
void	sort_five(t_context *ctx);

/* sort_simple.c */
void	sort_simple(t_context *ctx);

/* sort_medium.c */
void	sort_medium(t_context *ctx);

/* sort_complex.c */
void	sort_complex(t_context *ctx);

/* sort_adaptive.c */
void	sort_adaptive(t_context *ctx);

/* bench.c */
void	print_bench(t_context *ctx);

/* main.c */
void	error_exit(t_context *ctx);

#endif
