/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmessin <frmessin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 23:48:40 by frmessin          #+#    #+#             */
/*   Updated: 2022/06/14 19:08:20 by frmessin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_bonus.h"

void	ft_init_tab(t_out *ptr)
{
	ptr->wdt = 0;
	ptr->prf = 0;
	ptr->zero = 0;
	ptr->dot = 0;
	ptr->dash = 0;
	ptr->sign = 0;
	ptr->is_zero = 0;
	ptr->percent = 0;
	ptr->space = 0;
}

static t_out	*format_flag( t_out *tab, char *arg, size_t i)
{
	while (ft_is_in(arg[i], "cspdiuxX%") == -1)
	{
		if (arg[i] == '0' && tab->wdt == 0)
			tab->zero = 1;
		else if (arg[i] == '-')
			tab->dash = 1;
		else if (arg[i] == '.' && tab->dot == 0)
			tab->dot = ft_get_precision(arg, i + 1);
		else if (arg[i] == '+')
			tab->sign = 1;
		else if (arg[i] == ' ')
			tab->space = 1;
		else if (arg[i] == '%')
			tab->percent = 1;
		else if (ft_isdigit(arg[i]) && (tab->dot == 0) && tab->wdt == 0)
			tab->wdt = ft_get_wdt(arg, i);
		else if (arg[i] == '#')
			tab->prf += 1;
		i++;
	}
	return (tab);
}

static int	ft_format_output(t_out *tab, char *arg, size_t i)
{
	tab = format_flag(tab, arg, i);
	while (ft_is_in(arg[i], "cspdiuxX%") == -1)
		i++;
	if (ft_is_in(arg[i], "cspdiuxX%") != -1)
	{
		if (arg[i] == 'c')
			tab->tl += ft_print_out_char(tab);
		else if (arg[i] == 's')
			tab->tl += ft_print_out_str(tab);
		else if (arg[i] == 'p')
			tab->tl += ft_print_out_ptr(tab, "0123456789abcdef", "0x");
		else if (arg[i] == 'd' || arg[i] == 'i')
			tab->tl += ft_print_out_nbr(tab);
		else if (arg[i] == 'u')
			tab->tl += ft_print_out_unsigned(tab);
		else if (arg[i] == 'x')
			tab->tl += ft_print_out_hex(tab, "0123456789abcdef", "0x");
		else if (arg[i] == 'X')
			tab->tl += ft_print_out_hex(tab, "0123456789ABCDEF", "0X");
		else if (arg[i] == '%')
			tab->tl += ft_print_out_percent();
	}
	ft_init_tab(tab);
	return (i);
}

static int	jump_flag(char *content, int i)
{
	i++;
	while (content[i] != '\0' && (ft_is_in(content[i], "-0 .#+") != -1
			|| (ft_isdigit(content[i]) == 1)))
		i++;
	i++;
	return (i);
}

int	ft_printf(const char *content, ...)
{
	size_t	i;
	int		ret;
	t_out	tab;
	char	*content1;

	content1 = (char *)(content);
	ft_init_tab(&tab);
	tab.tl = 0;
	va_start(tab.args, content);
	i = 0;
	ret = 0;
	while (content1[i] != '\0')
	{
		if (content1[i] == '%' && check_valid_arg(&content1[i + 1]) == 0)
			i = jump_flag(&content1[i], i);
		if (content1[i] == '%' && check_valid_arg(&content1[i + 1]) == 1)
			i = ft_format_output(&tab, content1, i + 1);
		else if (content1[i] != '\0')
			ret += write(1, &content1[i], 1);
		i++;
	}
	va_end (tab.args);
	ret += tab.tl;
	return (ret);
}
