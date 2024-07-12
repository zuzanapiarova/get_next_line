/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:56:58 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/07/12 11:04:42 by zuzanapiaro      ###   ########.fr       */
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

char *handle_error(char *line, char *buffer, char *left_chars)
{
    if (line)
    {
        free(line);
        //line = NULL;
    }
    if (buffer)
    {
        free(buffer);
        //buffer = NULL;
    }
    if (left_chars)
    {
        free(left_chars);
        //left_chars = NULL;
    }
    return (NULL);
}

char *start_line(char *str)
{
    char *newline = ft_strchr(str, '\n');
    char *line;

    int i = newline - str;
    line = malloc((i + 2) * sizeof(char));
    if (!line)
        return (NULL);
    line[i] = '\n';
    line[i + 1] = '\0';
    while (i--)
        line[i] = str[i];
    return (line);
}

char *handle_left_chars(char *left_chars, char *line)
{
    char *tempstr;
    tempstr = ft_strdup(ft_strchr(left_chars, '\n') + 1);
    free(left_chars);
    if (!tempstr)
    {
        free(line);
        return (NULL);
    }
    left_chars = tempstr;
    return (left_chars);
}

char *handle_no_newline_read(char *line, char *buffer, char *left_chars)
{
    int length;

    length = strlen(line) + strlen(buffer) + 1;
    char* result = malloc(length * sizeof(char));
    if (!result)
        return (handle_error(line, buffer, left_chars));
    ft_strlcpy(result, line, length);
    ft_strlcat(result, buffer, length);
    free(line);
    return (result);
}

char *get_next_line(int fd)
{
    char *buffer;
    char *line;
    int chars_read;
    static char *left_chars;

    if (BUFFER_SIZE <= 0 || fd < 0 || read(fd, 0, 0) < 0) // handle invalid or empty input parameters
    {
        free(left_chars);
        left_chars = NULL;
        return (NULL);
    }
    if (left_chars) //handle input if there was anything left after newline from the previous buffer read
    {
        if (ft_strchr(left_chars, '\n'))
        {
            line = start_line(left_chars);
            left_chars = handle_left_chars(left_chars, line);
            return (line);
        }
        line = ft_strdup(left_chars);
        free(left_chars);
        left_chars = NULL;
    }
    else // if there was nothing left from the previous buffer read allocate and create an empty line
        line = ft_strdup("");
    buffer = malloc((BUFFER_SIZE + 1) * sizeof(char)); // create buffer with proper size
    if (!buffer)
        return (NULL);
    chars_read = 1;
    while (chars_read > 0) // start reading from buffer in iterations until there is something to read
    {
        chars_read = read(fd, buffer, BUFFER_SIZE);
        if (chars_read < 0 || (chars_read == 0 && ft_strlen(line) == 0))
            return (handle_error(line, buffer, left_chars));
        else if (chars_read == 0)
            break;
        buffer[chars_read] = '\0';
        if (ft_strchr(buffer, '\n'))
        {
            char *temp = start_line(buffer);
            char *new_line = ft_strjoin(line, temp);
            free(temp);
            free(line);
            line = new_line;
            left_chars = ft_strdup(ft_strchr(buffer, '\n') + 1);
            free(buffer);
            return (line);
        }
        line = handle_no_newline_read(line, buffer, left_chars);
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
