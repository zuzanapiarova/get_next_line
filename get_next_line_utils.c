/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 21:36:20 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/06/27 23:28:42 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
