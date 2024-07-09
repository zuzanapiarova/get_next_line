/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:56:58 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/07/09 11:11:29 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//cc -Wall -Wextra -Werror -g -D BUFFER_SIZE=5 get_next_line.c get_next_line_utils.c

#include "get_next_line.h"

// create line  and return this line - line is not static, it is being created new each time the function is called
//append new buffer reads to this line until a buffer with newline is found
//then, store the rest of chars after newline in buffer read in a static variable and remove those chars from the line
//in next function call use this static var left characters as the beginning of the line
//starting the line in each function call can be different
//a. if it is first function call, line and chars_left are empty - start line with strdup ""
//b. if on other function calls chars_left is not empty, start line with chars_left
//c. then in both cases begin a loop to append new buffer reads to the line
// --> so we create new line in each function call and return this line
// we need static variables to store the left characters adn have them stored for the next function call
// do we have to free this line ??

char *get_next_line(int fd)
{
    char *buffer;
    char *line;
    int chars_read;
    static char *left_chars;
    char *tempstr;

    if (BUFFER_SIZE <= 0 || fd < 0 || read(fd, 0, 0) < 0)
    {
        free(left_chars);
        left_chars = NULL;
        return (NULL);
    }

    buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
    if (!buffer)
        return (NULL);

    if (left_chars)
    {
        char *newline = ft_strchr(left_chars, '\n');
        if (newline)
        {
            int i = newline - left_chars;
            line = malloc((i + 2) * sizeof(char));
            if (!line)
            {
                free(buffer);
                return (NULL);
            }
            line[i] = '\n';
            line[i + 1] = '\0';
            while (i--)
                line[i] = left_chars[i];
            tempstr = ft_strdup(newline + 1);
            if (!tempstr)
            {
                free(buffer);
                free(line);
                return (NULL);
            }
            free(left_chars);
            left_chars = tempstr;
            free(buffer);
            return (line);
        }
        else
        {
            line = ft_strdup(left_chars);
            free(left_chars);
            left_chars = NULL;
            if (!line)
            {
                free(buffer);
                return (NULL);
            }
        }
    }
    else
    {
        line = ft_strdup("");
        if (!line)
        {
            free(buffer);
            return (NULL);
        }
    }

    chars_read = 1;
    while (chars_read > 0)
    {
        chars_read = read(fd, buffer, BUFFER_SIZE);
        if (chars_read == 0 && ft_strlen(line) == 0)
        {
            free(buffer);
            free(line);
			free(left_chars);
            line = NULL;
            buffer = NULL;
            left_chars = NULL;
            return (NULL);
        }
        if (chars_read < 0)
        {
            free(buffer);
            free(line);
			free(left_chars);
            line = NULL;
            buffer = NULL;
            left_chars = NULL;
            return (NULL);
        }
        else if (chars_read == 0)
            break;
        buffer[chars_read] = '\0';

        if (ft_strchr(buffer, '\n'))
        {
            char *newline = ft_strchr(buffer, '\n');
            int i = newline - buffer;
            char *temp = malloc((i + 2) * sizeof(char));
            if (!temp)
            {
                free(buffer);
                free(line);
                return (NULL);
            }
            temp[i] = '\n';
            temp[i + 1] = '\0';
            while (i--)
                temp[i] = buffer[i];

            char *new_line = ft_strjoin(line, temp);
            free(temp);
            if (!new_line)
            {
                free(buffer);
                free(line);
                return (NULL);
            }
            free(line);
            line = new_line;

            free(left_chars);
            left_chars = ft_strdup(newline + 1);
            if (!left_chars)
            {
                free(buffer);
                free(line);
                return (NULL);
            }
            free(buffer);
            return (line);
        }
        // PROBLEM WITH LONG LINE - IT READS THE LINE INTO BUFFER BUT APPARENTLY TOO SLOWLY, PROBLEM IN THIS SCOPE
        else
        {
           /*  // START
            char *new_line = ft_strjoin(line, buffer);
            if (!new_line)
            {
                free(buffer);
                free(line);
                return (NULL);
            }
            free(line);
            line = new_line;
            // END */
            int length;

            length = strlen(line) + strlen(buffer) + 1;
            char* result = malloc(length * sizeof(char));
            if (!result)
            {
                free(buffer);
                free(line);
                return (NULL);
            }
            ft_strlcpy(result, line, length);
            ft_strlcat(result, buffer, length);
            free(line);
            line = result;
        }
    }
    free(buffer);
    return (line);
}

// int	main(void)
// {
// 	int	fd;
//     int i;
//     char *line;

//     i = 0;
// 	fd = open("file.txt", O_RDONLY);
//     while ( i < 10)
//     {
//         line = get_next_line(fd);
//         printf("--%s", line);
//         i++;
//     }
// 	close(fd);
// }
