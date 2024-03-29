/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmessin <frmessin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 23:48:40 by frmessin          #+#    #+#             */
/*   Updated: 2022/06/05 20:42:49 by frmessin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

t_out *ft_init_tab(t_out *ptr)
{
	ptr->wdt = 0;
	ptr->prc = 0;
	ptr->zero = 0;
	ptr->dot = 0;
	ptr->dash = 0;
	ptr->sign = 0;
	ptr->is_zero = 0;
	ptr->percent = 0;
	ptr->space = 0;
//	ptr->tl = 0;
	return (ptr);
}

int	ft_get_precision( char *num, int i) //arg[i] = '.'
{
	int	len;

	len = 0;
	while(ft_isdigit((int)num[i]) != 0)
	{
		len = (len * 10) + ((num[i]) - '0');
		i++;
	}
		fflush(NULL);
	return (len + 1);//(sign*(len + 1));
}

int ft_get_wdt(char *num, int i)
{
	int len;
	len = 0;
	while (ft_isdigit((int)num[i]) != 0)
	{
		len = (len * 10) + ((num[i]) - '0');
		i++;
	}
	return (len);
}

t_out *format_flag( t_out *tab, char* arg,size_t i)
{
	while (ft_is_in(arg[i], "cspdiuxX%") == -1 )
	{
		if(arg[i] == '0' && tab->wdt == 0)
			tab->zero = 1;
		else if(arg[i] == '-')
			tab->dash = 1;
		else if(arg[i] == '.' && tab->dot == 0)
			tab->dot = ft_get_precision(arg, i + 1); //i numeri per l'indice i li conto comunque perche' i++ alla fine va' +1 finche' becca numeri e non entra i wdt per tan->dot == 0;
		else if(arg[i] == '+')
			tab->sign = 1;
		else if(arg[i] == ' ')
			tab->space = 1;
		else if(arg[i] == '%')
			tab->percent = 1;
		else if(ft_isdigit(arg[i]) && (tab->dot == 0) && tab->wdt == 0) //%7.434d
			tab->wdt = ft_get_wdt(arg,i);
		i++;
	}
	return(tab);
}

int	ft_format_output	(t_out *tab, char *arg, size_t i) //%jhfjkwhhfkjweh
{
	tab = format_flag(tab, arg, i);
	while (ft_is_in(arg[i], "cspdiuxX%") == -1 )
		i++;
	if(ft_is_in(arg[i], "cspdiuxX%") != -1)
	{
		if(arg[i] == 'c')
			tab->tl += ft_print_out_char(tab);
		else if (arg[i] == 's')
			tab->tl += ft_print_out_str(tab);
		else if (arg[i] == 'p')
			tab->tl += ft_print_out_ptr(tab);
		else if (arg[i] == 'd' || arg[i] == 'i')
			tab->tl += ft_print_out_nbr(tab);
		else if (arg[i] == 'u')
			tab->tl += ft_print_out_unsigned(tab);
		else if (arg[i] == 'x')
			tab->tl += ft_print_out_hex(tab, "0123456789abcdef");
		else if (arg[i] == 'X')
			tab->tl += ft_print_out_hex(tab, "0123456789ABCDEF");
		else if (arg[i] == '%')
			tab->tl += ft_print_out_percent(tab);
	}
	printf("get: %d", tab->wdt);
	ft_init_tab(tab); //after i do the stuff initialize the tab again or get fucked because after the % u can get another % and you are not calling again printf but arte simply keeping reading
	return (i);
}

int ft_printf(/*const*/ char *content, ...)
{
	int i;
	int ret;
	t_out *tab;

	tab = (t_out *)malloc(sizeof(t_out));
	if (!tab)
		return (-1);
	tab = ft_init_tab(tab);
	tab->tl = 0;
	va_start(tab->args, content);
	i = 0;
	ret = 0;
	while (content[i] != '\0')
	{
		if (content[i] == '%')
			i += ft_format_output(tab, content, i + 1);
		else
			ret += write(1, &content[i], 1);
		i++;
	}
	va_end(tab->args);
	ret += tab->tl;
	free (tab);
	return (ret);
}
