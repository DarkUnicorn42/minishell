
#include "../include/minishell.h"

int main(void)
{
    char *input;
    
    while (1)
    {
        input = readline("📟 \e[0;32mwrite_commend_here >> \e[0m");

        if (input == NULL)
        {
            printf("\n\n\e[0;32mExiting shell...\e[0m\n");
            printf("\n\e[1;32m✖️ Shell closed.\e[0m\n\n");
            break;
        }
        if (input && *input)
            add_history(input);

        printf("You entered: %s\n", input);
        free(input);
    }
    return 0;
}