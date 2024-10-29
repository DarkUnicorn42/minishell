/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 14:31:46 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/03/07 13:21:20 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	int		len;
	char	*dup;

	len = 0;
	while (*(char *)(s + len))
		len++;
	dup = (char *)malloc((len * sizeof(char)) + 1);
	if (dup == NULL)
		return (NULL);
	while (*s)
		*dup++ = *s++;
	*dup = 0;
	return (dup - len);
}
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    const char	*original_string = "Hello, world!"; // Original string
    char	*duplicate_string;

    // Duplicate the original string
	duplicate_string = ft_strdup(original_string);

	if (duplicate_string == NULL) {
	printf("Memory allocation failed\n");
	return 1;
	}

    // Print the duplicate string
	printf("Original string: %s\n", original_string);
	printf("Duplicate string: %s\n", duplicate_string);

    // Free the allocated memory for the duplicate string
	free(duplicate_string);

	return 0;
}
*/
