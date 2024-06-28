/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 22:21:54 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/06/27 23:38:50 by zuzanapiaro      ###   ########.fr       */
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
// this func keeps the position of the start of the line, even tho the buffer may be on next characters later without encountering \n or \0 in multiple function calls

char *set_buffer(char *buffer, int chars_read)
{
	printf("set buffer: b = %s, chars read = %d\n", buffer, chars_read);
	// find index of \n or \0
	//char *temp;
	int i;

	i = 0;
	while(i < chars_read && buffer[i] != '\0' && buffer[i] != '\n') //abcd\ne
		i++;
	printf("i: %d\n", i);
	// temp = ft_substr(temp, 0, i);
	// buffer = temp;
	// free(temp);
	return (buffer);
	//return t_substr(temp, 0, i);
}

char *set_line(char *line, char *buffer, int chars_read)
{
	char *temp;

	temp = ft_strdup(line);
	free(line);
	printf("set_line buffer: %s\n", buffer);
	line = ft_strjoin(temp, set_buffer(buffer, chars_read));
	//free(temp);
	// TODO: here call function that checks if there is \0 or \n or not
	//printf("newline index: %d\n", get_end_position(buffer, chars_read));
	return (line);
}
//each time it is called it reads from fd BUFFER_SIZE characters until it encounters a newline and retuns pointer ot the newline
char	*get_next_line(int fd)
{
	char	*buffer;
	static char	*line; 		//stores entire line until \n is found, may be created in multiple reads
	int			chars_read; //saves number of bytes read
	// char *newline_ptr;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	//fd < 0 handles case if open() returns -1 in case there is error
	if (BUFFER_SIZE <= 0 || fd < 0 || read(fd, 0, 0) < 0)
	{
		free(buffer);
		return (NULL);
	}
	if (!line)
		line = ft_strdup("");
	chars_read = 1;
	while (chars_read > 0) // while there is at least one char to read, process the BUFFER_SIZE number of bytes read
	{
		chars_read = read(fd, buffer, BUFFER_SIZE);	//buffer position is always remembered for the next function call
		printf("chars read: %d\n", chars_read);
		if (chars_read == 0)
		{
			break;
		}
		else if (chars_read < 0)
		{
			free(line);
			free(buffer);
			return (NULL);
		}
		else
		{
			buffer[chars_read] = '\0'; //must null terminate because it is not done automatically
			printf("gnl buffer: %s\n", buffer);
			line = set_line(line, buffer, chars_read); //if there is sth read into buffer, add it to the line with this function
			printf("gnl line: %s\n", line);
			//after line is created, we can free the buffer in this iteration to avoid memory leaks
			//free(buffer);
		}
	}
   //free buffer somewhere
   free(buffer);
	//free *line somewhere
	return (line);
}

int	main(void)
{
	int	fd;

	fd = open("file.txt", O_RDONLY);
	printf("new line: %s\n", get_next_line(fd));
	close(fd);
}
