/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohakola <ohakola@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/14 18:21:16 by ohakola           #+#    #+#             */
/*   Updated: 2020/03/15 20:23:50 by ohakola          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int		check_zero_flag(t_printf *data,
				int *index, int *found_zero)
{
	int		i;

	i = *index;
	data->pad_zeros = TRUE;
	*found_zero = TRUE;
	if (data->spec[i + 1] == '0')
	{
		data->pad_zeros = FALSE;
		*found_zero = FALSE;
	}
	else if (i - 1 > 0 && data->spec[i - 1] == '.')
	{
		data->pad_zeros = FALSE;
		*found_zero = FALSE;
	}
	while (i < data->spec_len && ft_isdigit(data->spec[i]))
		i++;
	i--;
	*index = i;
	return (TRUE);
}

int				check_flag(t_printf *data, int *index, int *found_zero)
{
	int		i;

	i = *index;
	if (data->spec[i] == '-')
		data->left_justify = TRUE;
	else if (data->spec[i] == '+')
		data->show_sign = TRUE;
	else if (data->spec[i] == ' ')
		data->blank_space = TRUE;
	else if (data->spec[i] == '0' && !(*found_zero))
		check_zero_flag(data, &i, found_zero);
	else if (ft_isdigit(data->spec[i]))
	{
		while (i < data->spec_len && ft_isdigit(data->spec[i]))
			i++;
		i--;
	}
	else if (data->spec[i] == '#')
		data->zerox = TRUE;
	*index = i;
	return (TRUE);
}

int				check_length(t_printf *data, int *index, char s)
{
	int		i;

	i = *index;
	if (i - 1 >= 0 && s == 'h' && data->spec[i - 1] == 'h')
		data->type = data->type > length_hh && data->type != length_h ?
			data->type : length_hh;
	else if (s == 'h')
		data->type = data->type > length_h ? data->type : length_h;
	else if (i - 1 >= 0 && s == 'l' && data->spec[i - 1] == 'l')
		data->type = data->type > length_ll && data->type != length_l ?
			data->type : length_ll;
	else if (s == 'l')
		data->type = data->type > length_l ? data->type : length_l;
	else if (s == 'j')
		data->type = data->type > length_j ? data->type : length_j;
	else if (s == 'z')
		data->type = data->type > length_z ? data->type : length_z;
	else if (s == 't')
		data->type = data->type > length_t ? data->type : length_t;
	else if (s == 'L')
		data->type = data->type > length_L ? data->type : length_L;
	return (TRUE);
}

int				check_parsed_zero(t_printf *data, char *res)
{
	int				i;
	int				is_zero;

	is_zero = TRUE;
	i = -1;
	while (res[++i])
		if (res[i] != '0' && !(is_zero = FALSE))
			break ;
	data->is_zero_res = is_zero;
	return (TRUE);
}

char			*scientific_double(t_printf *data, long double var)
{
	char			*res;
	char			*tmp;
	char			*expstr;
	long double		exp;
	long double		mantissa;

	exp = ft_exp_base(var, 10);
	mantissa = var / ft_powl(10, exp);
	res = ft_ftoa(mantissa, data->precision);
	tmp = exp >= 0 ? ft_strjoin(res, "e+") : ft_strjoin(res, "e-");
	expstr = ft_itoa_base(ft_abs(exp), 10);
	if (ft_abs(exp) < 10)
		expstr = add_str_to_beg(expstr, "0", ft_strlen(expstr), 1);
	ft_strdel(&res);
	res = ft_strjoin(tmp, expstr);
	ft_strdel(&expstr);
	ft_strdel(&tmp);
	if (data->c == 'E')
		ft_capitalize(res);
	data->var_len = ft_strlen(res);
	return (res);
}
