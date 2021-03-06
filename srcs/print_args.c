/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 19:35:03 by jjaniec           #+#    #+#             */
/*   Updated: 2018/04/26 20:48:35 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_select.h>

/*
** Calculate number of elements to print per row
*/

static unsigned int		get_elems_per_row(t_term_caps *tcaps)
{
	return (tcaps->e_infos.elems_per_row = \
		tcaps->ts.ws_col / (tcaps->e_infos.width + 2));
}

/*
** Print element based on tcaps->e_infos.width with FORMAT_RESET if specified
*/

static void				print_arg_fmt_str(t_term_caps *tcaps, \
							char *s, int fmt_len)
{
	ft_dprintf(STDIN_FILENO, "%-*s", \
		((tcaps->e_infos.width > 0) ? \
			((int)tcaps->e_infos.width) : ((int)ft_strlen(s))) + \
		2 + fmt_len, s);
}

/*
** Join and print element with FORMAT_RESET if needed
** and pass it to print_arg_fmt_str, then free allocated ptr
*/

static void				print_arg_fmt(t_term_caps *tcaps, t_ft_select_arg *e)
{
	char	*str_fmt;

	str_fmt = e->str;
	if (e->stat_r == 0 || e->selected == true || tcaps->cursor_pos_ptr == e)
	{
		if (e->selected == true)
			ft_dprintf(STDIN_FILENO, ELEM_SELECTED_FMT);
		if (tcaps->cursor_pos_ptr == e)
			ft_dprintf(STDIN_FILENO, CURSOR_POS_FMT);
		if (e->stat_r == 0)
			print_arg_color(tcaps, e);
		str_fmt = ft_strjoin(e->str, FORMAT_RESET);
		print_arg_fmt_str(tcaps, str_fmt, \
			(int)ft_strlen(FORMAT_RESET));
		free(str_fmt);
	}
	else
		print_arg_fmt_str(tcaps, str_fmt, 0);
}

/*
**	If the window's size is too small,
**	print an error message instead of the args.
*/

static void				print_win_too_small(const char *clear_s)
{
	ft_putstr_fd(clear_s, STDIN_FILENO);
	ft_putstr_fd("WIN TOO SMALL", STDIN_FILENO);
}

/*
** Calls print_arg_fmt on every element in the linked list,
** prints a newline when necessary
*/

void					print_args(t_term_caps *tcaps, t_ft_select_arg *li)
{
	unsigned int	j;
	unsigned int	i;
	t_ft_select_arg	*ptr;

	j = 0;
	i = 1;
	if (tcaps->e_infos.width > 0 && get_elems_per_row(tcaps) == 0)
		return (print_win_too_small(tcaps->clear_s));
	ptr = li;
	while (ptr)
	{
		print_arg_fmt(tcaps, ptr);
		ptr->index = i++;
		if (++j == tcaps->e_infos.elems_per_row)
		{
			ft_dprintf(STDIN_FILENO, "\n");
			j = 0;
		}
		ptr = ptr->next;
	}
	ft_dprintf(STDIN_FILENO, "\n");
}
