/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 21:36:20 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/06/30 14:49:21 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strchr(const char *s, int c)
{
	char			*res;
	unsigned char	d;

	d = (unsigned char) c;
	res = (char *) s;
	while (*res)
	{
		if (*res == d)
			return ((char *)res);
		res++;
	}
	if (d == '\0')
		return ((char *)res);
	return (0);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*substring;
	unsigned int	str_len;
	size_t			i;

	i = 0;
	if (s == NULL)
		return (NULL);
	str_len = ft_strlen(s);
	if (start >= str_len)
		return (ft_strdup(""));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	substring = (char *)malloc((len + 1) * sizeof(char));
	if (!substring)
		return (NULL);
	while (i < len)
	{
		substring[i] = s[i + start];
		i++;
	}
	substring[i] = '\0';
	return (substring);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	int		i;
	int		length;

	i = 0;
	length = (int)ft_strlen(s1) + (int)ft_strlen(s2);
	result = (char *)malloc((length + 1) * sizeof(char));
	if (!result)
		return (NULL);
	while (i < (int)ft_strlen(s1))
	{
		result[i] = s1[i];
		i++;
	}
	while (i < ((int)ft_strlen(s1) + (int)ft_strlen(s2)))
	{
		result[i] = s2[i - ft_strlen(s1)];
		i++;
	}
	result[i] = '\0';
	return (result);
}

char	*ft_strdup(const char *s1)
{
	int		len;
	char	*copy;
	int		i;

	i = 0;
	len = ft_strlen(s1);
	copy = (char *)malloc((len + 1) * sizeof(char));
	if (!copy)
		return (NULL);
	while (s1[i])
	{
		copy[i] = s1[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (src[i] && i < size - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}
