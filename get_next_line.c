/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 22:21:54 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/06/29 12:03:28 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <libc.h>
#include <fcntl.h>

// add this option to  compiler call: -D BUFFER_SIZE=n
// but ifndef, define this header also in the header file like: BUFFER_SIZE 10
// it will define the buffer size for read()
// compile your code as cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 <files>.c

//cc -Wall -Wextra -Werror -D BUFFER_SIZE=5 get_next_line.c get_next_line_utils.c

//TODO: handle cases of file descriptors
//TODO: free memory of functions tha use malloc and do other malloc checks
//then: call a function that reads from fd into buffer until it encounters \n or \0
//!!!!! buffer size may vary so we store the position in static variable because \n or \0 may not be found in the first function call !!!!!
//then: free the line buffer to avoid memory leaks - the function remembers where the line stopped
//then: call function that sets the next line on the position where we ended on the previous functions call

// char *set_buffer(char *buffer, int chars_read)
// {
// 	char *temp;
// 	int i;

// 	i = 0;
// 	while(i < chars_read && buffer[i] != '\0' && buffer[i] != '\n')
// 		i++;
// 	temp = ft_substr(buffer, 0, i);
// 	buffer = strdup(temp);
// 	free(temp);
// 	return (buffer);
// }

// create line  and return this line - line is not static
//append new buffer reads to this line until a buffer with newline is found
//then, store the rest of chars after newline in buffer read in a static variable
//and remove those chars from the line
//in next function call use this static var left characters as the beginning of the line
//starting the line in each function call can be different
//a. if it is first function call, line and chars_left are empty - start line with strdup ""
//b. if on other function calls chars_left is not empty, start line with chars_left
//c. then in both cases begin a loop to append new buffer reads to the line
// --> so we create new line in each function call and return this line
// we need static variables to store the left characters adn have them stored for the next function call
// do we have to free this line ???


char	*get_next_line(int fd)
{
	char		*buffer;
	static char		*line; 	//stores entire line including whole buffer in which \n is found, may be created in multiple reads
	int			chars_read; //saves number of bytes read
	static char *left_chars;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	if (BUFFER_SIZE <= 0 || fd < 0 || read(fd, 0, 0) < 0) //fd < 0 handles case if open() returns -1 in case there is error
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
			if (!line)
				line = ft_strdup("");


	chars_read = 1;
	while (chars_read > 0) // while there is at least one char to read, process the BUFFER_SIZE number of bytes read
	{
		chars_read = read(fd, buffer, BUFFER_SIZE);	//buffer position is always remembered for the next function call
		if (chars_read <= 0)
			break;
		else
		{
			buffer[chars_read] = '\0'; //must null terminate because it is not done automatically
			left_chars = strchr(buffer, '\n');
			if (left_chars)
			{
				*left_chars = '\0';
				line = ft_strjoin(line, left_chars);
				left_chars = ft_strdup(left_chars + 1);
				free(buffer);
				return (line);
			}
			else
			{
				line = ft_strjoin(line, left_chars);
				free(left_chars);
				left_chars = NULL;
			}
			printf("gnl buffer: %s\n", buffer);
		}
	}
	printf("line form gnl: %s\n", line);
	free(buffer);
	printf("left chars: %s\n", left_chars);
	free(left_chars);
	return (line);
}

int	main(void)
{
	int	fd;

	fd = open("file.txt", O_RDONLY);
	printf("new line: %s\n", get_next_line(fd));
	printf("new line: %s\n", get_next_line(fd));
	// printf("new line: %s\n", get_next_line(fd));
	// printf("new line: %s\n", get_next_line(fd));
	// printf("new line: %s\n", get_next_line(fd));
	// printf("new line: %s\n", get_next_line(fd));
	// printf("new line: %s\n", get_next_line(fd));
	// printf("new line: %s\n", get_next_line(fd));
	close(fd);
}
