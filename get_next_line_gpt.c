/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 15:21:42 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/06/30 19:01:57 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char *get_next_line(int fd)
{
    char *buffer;
    char *line;
    int chars_read;
    static char *left_chars;

    if (BUFFER_SIZE <= 0 || fd < 0 || read(fd, 0, 0) < 0)
        return (NULL);

    buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
    if (!buffer)
        return (NULL);

    line = ft_strdup("");
    if (!line)
    {
        free(buffer);
        return (NULL);
    }

    if (left_chars)
    {
        char *newline_pos = strchr(left_chars, '\n');
        if (newline_pos)
        {
            *newline_pos = '\0';
            char *temp_line = ft_strjoin(line, left_chars);
            free(line);
            if (!temp_line)
            {
                free(buffer);
                free(left_chars);
                return (NULL);
            }
            line = temp_line;

            char *temp_left_chars = ft_strdup(newline_pos + 1);
            free(left_chars);
            if (!temp_left_chars)
            {
                free(buffer);
                free(line);
                return (NULL);
            }
            left_chars = temp_left_chars;
            free(buffer);
            return (line);
        }
        else
        {
            char *temp_line = ft_strjoin(line, left_chars);
            free(line);
            if (!temp_line)
            {
                free(buffer);
                free(left_chars);
                return (NULL);
            }
            line = temp_line;
            free(left_chars);
            left_chars = NULL;
        }
    }

    chars_read = 1;
    while (chars_read > 0)
    {
        chars_read = read(fd, buffer, BUFFER_SIZE);
        if (chars_read < 0)
        {
            free(buffer);
            free(line);
            return (NULL);
        }
        else if (chars_read == 0)
            break;

        buffer[chars_read] = '\0';

        char *newline_pos = strchr(buffer, '\n');
        if (newline_pos)
        {
            *newline_pos = '\0';
            char *temp_line = ft_strjoin(line, buffer);
            free(line);
            if (!temp_line)
            {
                free(buffer);
                return (NULL);
            }
            line = temp_line;

            left_chars = ft_strdup(newline_pos + 1);
            if (!left_chars)
            {
                free(buffer);
                free(line);
                return (NULL);
            }
            free(buffer);
            return (line);
        }
        else
        {
            char *temp_line = ft_strjoin(line, buffer);
            free(line);
            if (!temp_line)
            {
                free(buffer);
                return (NULL);
            }
            line = temp_line;
        }
    }
    free(buffer);
    return (line);
}

// int	main(void)
// {
// 	int	fd;

// 	fd = open("file.txt", O_RDONLY);
// 	printf("new line: %s\n", get_next_line(fd));
// 	printf("new line: %s\n", get_next_line(fd));
// 	printf("new line: %s\n", get_next_line(fd));
// 	printf("new line: %s\n", get_next_line(fd));
// 	printf("new line: %s\n", get_next_line(fd));
// 	printf("new line: %s\n", get_next_line(fd));
// 	printf("new line: %s\n", get_next_line(fd));
// 	printf("new line: %s\n", get_next_line(fd));
// 	printf("new line: %s\n", get_next_line(fd));
// 	printf("new line: %s\n", get_next_line(fd));
// 	close(fd);
// }
