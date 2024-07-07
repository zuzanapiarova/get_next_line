//cc -Wall -Wextra -Werror -g -D BUFFER_SIZE=5 get_next_line.c get_next_line_utils.c
//  leaks --atExit -- ./a.out || valgrind --leak-ckeck=full ./a.out

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

/* char	*get_next_line(int fd)
{
	char		*buffer;
	char		*line;
	int			chars_read;
	static char	*left_chars;
	char		*tempstr;

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
	//line = ft_strdup("");
	if (left_chars)
	{
		// must check  if left_chars already contains \n, if yes, store contents before newline in line variable and return
		// and store the rest in a new left_char variable that will be static for next function call
		char *newline;

		newline = ft_strchr(left_chars, '\n');
		if (newline)
		{
			//1. appending just the correct amount of characters to the end of line
			int i;
			int j;

			i = 0;
			j = 0;
			while (left_chars[i] != '\n')
				i++;
			line = malloc((i + 2) * sizeof(char));
			while (j < i)
			{
				line[j] = left_chars[j];
				j++;
			}
			line[j] = '\n';
			line[j + 1] = '\0';
			tempstr = line;
			//2. storing the ammount left in a static string left_char
			left_chars = strchr(left_chars, '\n') + 1;
			return (line);
		}
		else
		{
			//line = ft_strdup(left_chars);
			line = left_chars;
			left_chars = NULL;
		}
	}
	else
		line = "";

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
			temp = malloc((i + 2)* sizeof(char));
			while (j < i)
			{
				temp[j] = buffer[j];
				j++;
			}
			temp[i] = '\n'; //HERE WE PRINT THE NEWLINE WHEN WE WNCOUNTER IT but we also have to print newline it there is empty line, also null terminate
			temp[i + 1] = '\0';
			line = ft_strjoin(line, temp); //TODO: handle if strjoin returns NULL
			free(temp);
			//2. storing the ammount left in a static string left_char
			if (left_chars)
			{
				free(left_chars);
				left_chars = NULL;
			}
			left_chars = strchr(buffer, '\n') + 1;
			left_chars = ft_strdup(left_chars); //TODO: handle if strdup returns NULL
			free(buffer);
			return (line);
		}
		else //if there is no newline in the buffer read
			line = ft_strjoin(line, buffer);
	}
	free(buffer);
	return (line);
} */

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
	//STARTING THE LINE
    // must check  if left_chars already contains \n, if yes, store contents before newline in line variable and return
	// and store the rest in a new left_char variable that will be static for next function call
    if (left_chars)
    {
        char *newline = ft_strchr(left_chars, '\n');
        if (newline)
        {
            //1. appending just the correct amount of characters to the end of line
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
			//2. storing the ammount left in a static string left_char
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
            //if there is no newline in the buffer read
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
        // if there is nothing stored from previous line and its last buffer read
        line = ft_strdup("");
        if (!line)
        {
            free(buffer);
            return (NULL);
        }
    }
	//READING FROM BUFFER AND APPENDING OTHER FILE READS TO OUR LINE
    chars_read = 1;
    while (chars_read > 0)
    {
        // while there is at least one char to read, process the BUFFER_SIZE number of bytes read
        chars_read = read(fd, buffer, BUFFER_SIZE);
        if (chars_read < 0)
        {
            free(buffer);
            free(line);
			free(left_chars);
            return (NULL);
        }
        else if (chars_read == 0)
            break;
        buffer[chars_read] = '\0'; //must null terminate teh buffer read because it is not done automatically

        char *newline = ft_strchr(buffer, '\n');
        if (newline)
        {
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
        else
        {
            char *new_line = ft_strjoin(line, buffer);
            if (!new_line)
            {
                free(buffer);
                free(line);
                return (NULL);
            }
            free(line);
            line = new_line;
        }
    }
    free(buffer);
    return (line);
}

//(DONE)ADD NEWLINE AT THE END OF LINE ONLY IF IT IS THERE - when you encounter a newline add it to the end of the line
//now just create if statement to just print newline if there is empty newline

int	main(void)
{
	int	fd;

	fd = open("file.txt", O_RDONLY);
	printf("new line: %s", get_next_line(fd));
	printf("new line: %s", get_next_line(fd));
	printf("new line: %s", get_next_line(fd));
	printf("new line: %s", get_next_line(fd));
	printf("new line: %s", get_next_line(fd));
	printf("new line: %s", get_next_line(fd));
	printf("new line: %s", get_next_line(fd));
	printf("new line: %s", get_next_line(fd));
	printf("new line: %s", get_next_line(fd));
	printf("new line: %s", get_next_line(fd));
	printf("new line: %s", get_next_line(fd));
	printf("new line: %s", get_next_line(fd));
	printf("new line: %s", get_next_line(fd));
	printf("new line: %s", get_next_line(fd));
	printf("new line: %s", get_next_line(fd));
	close(fd);
}
