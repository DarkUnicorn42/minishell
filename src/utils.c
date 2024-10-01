
#include "../include/minishell.h"

void print_tokens(t_token *tokens) {
    t_token *current = tokens;
    while (current != NULL) {
        printf("Token: Type = %d, Value = %s\n", current->type, current->value);
        current = current->next;
    }
}
void free_tokens(t_token *tokens)
{
    t_token *temp;
    while (tokens)
    {
        temp = tokens;
        tokens = tokens->next;
        if (temp->value)
            free(temp->value);
        free(temp);
    }
}
