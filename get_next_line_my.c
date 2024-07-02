//cc -Wall -Wextra -Werror -D BUFFER_SIZE=5 get_next_line.c get_next_line_utils.c
//  leaks --atExit -- ./a.out

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	char		*buffer;
	char		*line;
	int			chars_read;
	static char	*left_chars;

	if (BUFFER_SIZE <= 0 || fd < 0 || read(fd, 0, 0) < 0)
    {
        free(left_chars);
        left_chars = NULL;
		return (NULL);
    }

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	//STARTING THE LINE
	line = "";

	if (left_chars)
	{
		char *newline;

		newline = ft_strchr(left_chars, '\n');
		if (newline)
		{
			*newline = '\0';
			//1. appending just the correct amount of characters to the end of line
				// int i;
				// int j;

				// i = 0;
				// j = 0;
				// while (left_chars[i] != '\n')
				// 	i++;

				// line = malloc((i + 1) * sizeof(char));
				// while (j < i)
				// {
				// 	line[j] = left_chars[j];
				// 	j++;
				// }
				// line[j] = '\0';
			char *temp_line = ft_strjoin(line, left_chars);
            if (!temp_line)
            {
                free(buffer);
                free(left_chars);
                return (NULL);
            }
            line = temp_line; // FREE?
			//free(temp_line);
			//2. storing the amount left in a static string left_char
			char *temp_left_chars = ft_strdup(newline + 1);
			if (!temp_left_chars)
            {
                free(buffer);
                free(line);
                return (NULL);
            }
			left_chars = newline + 1;
			return (line);
		}
		line = left_chars;
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
		if (ft_strchr(buffer, '\n'))
		{
			//1. appending just the correct amount of characters to the end of line
			int i;
			char *temp;

			i = 0;
			while (buffer[i] != '\n')
				i++;
			temp = malloc((i + 1) * sizeof(char));
            ft_strlcpy(temp, buffer, i + 1);
            line = ft_strjoin(line, temp); //TODO: handle if strjoin returns NULL
			free(temp);
			//2. storing the amount left in a static string left_char
			left_chars = ft_strchr(buffer, '\n') + 1;
			left_chars = ft_strdup(left_chars); //WHY DOES IT NOW WORK IF THIS IS NOT THERE ?????
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
    char *line;
    int i;

    i = 0;
	fd = open("file.txt", O_RDONLY);
    while (i < 12)
    {
        line =  get_next_line(fd);
	    printf("new line: %s\n", line);
        free(line);
        i++;
    }
	close(fd);
    return (0);
}
