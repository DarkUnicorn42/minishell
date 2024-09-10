
#include "../include/minishell.h"

int main(void)
{
    char *input;
    
    // Shell loop
    while (1)
    {
        // Display prompt and read input
        input = readline("my_(s)hell> ");
        
        // If readline returns NULL (Ctrl-D), exit the loop
        if (input == NULL)
        {
            printf("Exiting (s)hell...\n");
            break;
        }
        
        // Add the input to history
        if (input && *input)
        {
            add_history(input);
        }
        
        // Print the user's input for demonstration purposes (you can replace this with actual command handling)
        printf("You entered: %s\n", input);
        
        // Free the allocated memory for the input
        free(input);
    }
    
    return 0;
}