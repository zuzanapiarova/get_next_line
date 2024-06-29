/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 22:21:54 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/06/28 20:45:46 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <libc.h>
#include <fcntl.h>

//cc -Wall -Wextra -Werror -D BUFFER_SIZE=5 get_next_line.c get_next_line_utils.c

char *set_line(char *line, char *buffer, int chars_read)
{
	char *temp;

	temp = ft_strdup(line);
	free(line);
	//printf("set_line buffer: %s\n", buffer);
	line = ft_strjoin(temp, set_buffer(buffer, chars_read));
	free(temp);
	// TODO: here call function that checks if there is \0 or \n or not
	//printf("newline index: %d\n", get_end_position(buffer, chars_read));
	return (line);
}

//collects and appends all buffer reads until it encounters buffer with newline
char *create_line(int fd, char *buffer, char *line)
{
	char	*temp;
	int		chars_read; //saves number of bytes read

	chars_read = 1;
	while (chars_read > 0) // while there is at least one char to read, process the BUFFER_SIZE number of bytes read
	{
		chars_read = read(fd, buffer, BUFFER_SIZE);	//buffer position is always remembered for the next function call
		//printf("chars read: %d\n", chars_read);
		if (chars_read == 0)
			break;
		else if (chars_read < 0)
		{
			free(buffer);
			return (NULL);
		}
		else
		{
			if (!line)
				line = ft_strdup("");
			buffer[chars_read] = '\0'; //must null terminate because it is not done automatically
			printf("buffer from cl: %s\n", buffer);
			//line = set_line(line, buffer, chars_read); //if there is sth read into buffer, add it to the line with this function
			temp = ft_strdup(line);
			free(line);
			line = ft_strjoin(temp, set_buffer(buffer, chars_read));
	free(temp);
	// TODO: here call function that checks if there is \0 or \n or not
	//printf("newline index: %d\n", get_end_position(buffer, chars_read));
	return (line);


			//printf("gnl line: %s\n", line);
		}
	}
	return (line);
}

char *set_newline()
{

}

char	*get_next_line(int fd)
{
	char		*buffer;
	char		*line; 	//stores entire line until \n is found, may be created in multiple reads
	static char *newline;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	if (BUFFER_SIZE <= 0 || fd < 0 || read(fd, 0, 0) < 0) //fd < 0 handles case if open() returns -1 in case there is error
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	line = create_line(fd, buffer, line); //create and append line from buffer reads until it encounters one with n or 0, then break
	free(buffer);
	newline = set_newline(line); //find position of n in newline and point to next character after it, handle case if line ends with 0
	//free *line somewhere
	free(line);
	return (newline);
}

int	main(void)
{
	int	fd;

	fd = open("file.txt", O_RDONLY);
	printf("new line: %s\n", get_next_line(fd));
	close(fd);
}




//TODO: free memory of functions tha use malloc and do other malloc checks
//then: call a function that reads from fd into buffer until it encounters \n or \0
//!!!!! buffer size may vary so we store the position in static variable because \n or \0 may not be found in the first function call !!!!!
//then: free the line buffer to avoid memory leaks - the function remembers where the line stopped
//then: call function that sets the next line on the position where we ended on the previous functions call
// this func keeps the position of the start of the line, even tho the buffer may be on next characters later without encountering \n or \0 in multiple function calls

char *set_buffer(char *buffer, int chars_read)
{
	//printf("set buffer: b = %s, chars read = %d\n", buffer, chars_read);
	// find index of \n or \0
	char *temp;
	int i;

	i = 0;
	while(i < chars_read && buffer[i] != '\0' && buffer[i] != '\n')
		i++;
	//printf("i: %d\n", i);
	temp = ft_substr(buffer, 0, i);
	printf("i: %d, temp: %s\n", i, temp);
	buffer = temp;
	free(temp);
	return (buffer);
}


//each time it is called it reads from fd BUFFER_SIZE characters until it encounters a newline and retuns pointer ot the newline
char	*get_next_line(int fd)
{
	char		*buffer;
	static char	*line; 	//stores entire line until \n is found, may be created in multiple reads
	int			chars_read; //saves number of bytes read

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
		//printf("chars read: %d\n", chars_read);
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
			//printf("gnl line: %s\n", line);
			//after line is created, we can free the buffer in this iteration to avoid memory leaks
			//free(buffer);
		}
	}
   free(buffer);
	//free *line somewhere
	return (line);
}
