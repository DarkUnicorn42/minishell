
#include "../include/minishell.h"

char *expand_variables(const char *str, t_shell *shell) {
    char *result = ft_strdup("");
    if (!result) {
        perror("malloc");
        return NULL;
    }
    
    int i = 0;
    while (str[i]) {
        if (str[i] == '$') {
            char *expansion = NULL;

            if (str[i + 1] == '?') {
                // Expand $? to the exit code
                expansion = ft_itoa(shell->exit_code);
                i += 2; // Skip over $?
            } else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_') {
                // Expand $VAR (an environment variable)
                int var_start = i + 1;
                while (str[i + 1] && (ft_isalnum(str[i + 1]) || str[i + 1] == '_')) {
                    i++;
                }
                size_t var_len = i + 1 - var_start;

                // Extract variable name
                char *var_name = ft_substr(str, var_start, var_len);
                if (!var_name) {
                    perror("malloc");
                    free(result);
                    return NULL;
                }

                // Get value using getenv
                expansion = getenv(var_name);
                free(var_name);

                if (!expansion) {
                    expansion = ""; // If variable doesn't exist, expand to an empty string
                }
                i++; // Move past the variable name
            } else {
                // If $ is not followed by ?, alphabetic character, or _, treat it literally
                expansion = "$";
                i++;
            }

            // Concatenate expansion to result
            char *temp = ft_strjoin(result, expansion);
            free(result);
            if (!temp) {
                perror("malloc");
                return NULL;
            }
            result = temp;
        } else {
            // Append regular character
            char *temp = ft_strncat_char(result, str[i]);
            free(result);
            if (!temp) {
                perror("malloc");
                return NULL;
            }
            result = temp;
            i++;
        }
    }

    return result;
}
