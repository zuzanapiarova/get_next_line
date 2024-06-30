/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zuzanapiarova <zuzanapiarova@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 22:21:54 by zuzanapiaro       #+#    #+#             */
/*   Updated: 2024/06/30 14:56:29 by zuzanapiaro      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//cc -Wall -Wextra -Werror -D BUFFER_SIZE=5 get_next_line.c get_next_line_utils.c

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
// do we have to free this line ???

//substr, join and strdup must be freed
char	*get_next_line(int fd)
{
	char		*buffer;
	char		*line; 	//stores entire line including whole buffer in which \n is found, may be created in multiple reads
	int			chars_read; //saves number of bytes read
	static char	*left_chars;

	if (BUFFER_SIZE <= 0 || fd < 0 || read(fd, 0, 0) < 0) //fd < 0 handles case if open() returns -1 in case there is error
		return (NULL);
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	//STARTING THE LINE
	line = ft_strdup("");
	if (left_chars)
	{
		// must check  if left_chars already contains \n, if yes, store contents before newline in line variable and return
		// and store the rest in a new left_char variable that will be static for next function call
		if (strchr(left_chars, '\n'))
		{
			//1. appending just the correct amount of characters to the end of line
			int i;
			int j;

			i = 0;
			j = 0;
			while (left_chars[i] != '\n')
				i++;
			line = malloc((i + 1)* sizeof(char));
			while (j < i)
			{
				line[j] = left_chars[j];
				j++;
			}
			line[i] = '\0';
			//2. storing the ammount left in a static string left_char
			left_chars = strchr(left_chars, '\n');
			left_chars = ft_strdup(left_chars + 1); //TODO: handle if strdup returns NULL
			//printf("left_chars: %s\n", left_chars);
			return (line);
		}
		line = ft_strjoin(line, left_chars);
		left_chars = NULL;
	}

	//READING FROM BUFFER AND APPENDING OTHER FILE READS TO OUR LINE
	chars_read = 1;
	while (chars_read > 0) // while there is at least one char to read, process the BUFFER_SIZE number of bytes read
	{
		chars_read = read(fd, buffer, BUFFER_SIZE);	//buffer position is always remembered for the next function call
		if (chars_read < 0)
		{
			free(buffer);
			free(line);
			return (NULL);
		}
		else if (chars_read == 0)
			break;
		buffer[chars_read] = '\0'; //must null terminate because it is not done automatically
		if (strchr(buffer, '\n'))
		{
			//1. appending just the correct amount of characters to the end of line
			int i;
			int j;
			char *temp;

			i = 0;
			j = 0;
			while (buffer[i] != '\n') //abc\n
				i++;
			temp = malloc((i + 1)* sizeof(char));
			while (j < i)
			{
				temp[j] = buffer[j];
				j++;
			}
			temp[i] = '\0';
			line = ft_strjoin(line, temp); //TODO: handle if strjoin returns NULL
			free(temp);
			//2. storing the ammount left in a static string left_char
			left_chars = strchr(buffer, '\n');
			left_chars = ft_strdup(left_chars + 1); //TODO: handle if strdup returns NULL
			free(buffer);
			return (line);
		}
		else //if there is no newline in the buffer read
			line = ft_strjoin(line, buffer);
	}
	free(buffer);
	return (line);
}

int	main(void)
{
	int	fd;

	fd = open("file.txt", O_RDONLY);
	printf("new line: %s\n", get_next_line(fd));
	printf("new line: %s\n", get_next_line(fd));
	printf("new line: %s\n", get_next_line(fd));
	printf("new line: %s\n", get_next_line(fd));
	printf("new line: %s\n", get_next_line(fd));
	printf("new line: %s\n", get_next_line(fd));
	printf("new line: %s\n", get_next_line(fd));
	printf("new line: %s\n", get_next_line(fd));
	printf("new line: %s\n", get_next_line(fd));
	printf("new line: %s\n", get_next_line(fd));
	close(fd);
}
