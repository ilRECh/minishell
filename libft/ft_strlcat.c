/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: name <name@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/06 00:04:15 by name              #+#    #+#             */
/*   Updated: 2021/08/06 00:04:17 by name             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dst_size)
{
	size_t	res_len;
	size_t	dst_len;

	dst_len = ft_strlen(dst);
	if (dst_size && dst_size > dst_len)
	{
		res_len = ft_strlen(src) + dst_len;
		dst_size -= dst_len + 1;
		dst += dst_len;
		while (dst_size-- && *src)
			*dst++ = *src++;
		*dst = 0;
		return (res_len);
	}
	res_len = ft_strlen(src) + dst_size;
	return (res_len);
}
