/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 11:58:40 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/03/07 13:49:45 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char		*joined_str;
	size_t		len1;
	size_t		len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined_str = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!joined_str)
		return (NULL);
	ft_memcpy(joined_str, s1, len1);
	ft_memcpy(joined_str + len1, s2, len2);
	joined_str[len1 + len2] = '\0';
	return (joined_str);
}

/*
#include <stdio.h>
#include <stdlib.h>

// Function declaration for ft_strjoin
char    *ft_strjoin(char const *s1, char const *s2);

int main() {
    // Example strings
    char const *s1 = "Hello, ";
    char const *s2 = "world!";

    // Call ft_strjoin to concatenate s1 and s2
    char *result = ft_strjoin(s1, s2);

    // Check if result is not NULL (memory allocation successful)
    if (result != NULL) {
        // Print the concatenated string
        printf("Concatenated string: %s\n", result);

        // Free the memory allocated for result
        free(result);
    } else {
        // Print an error message if memory allocation failed
        printf("Memory allocation failed.\n");
    }

    return 0;
}
*/
