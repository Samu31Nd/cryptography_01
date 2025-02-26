#include "utils.h"
/**
 * SPLIT THE WORD IN TWO BY SEP EXPR
 * TODO: FREE THE RETURN CHAR**
 */
char **split(const char *word, const char sep)
{
    char **splitted_word = (char **)malloc(sizeof(char *) * 2);
    splitted_word[0] = (char *)malloc(sizeof(char) * 256);
    splitted_word[1] = (char *)malloc(sizeof(char) * 256);

    int index = 0;
    while (1)
    {
        if (word[index] == '\0')
        {
            fprintf(stderr, "Not found the separation [%c] in [%s]!", sep, word);
            exit(-1);
        }

        if (word[index] == sep)
            break;
        splitted_word[0][index] = word[index];
        index++;
    }
    splitted_word[0][index] = '\0';
    int i = 0;
    while (word[index++] != '\0')
        splitted_word[1][i++] = word[index];
    splitted_word[1][index] = '\0';

    return splitted_word;
}

void freeSplitted(char **splitted)
{
    free(splitted[0]);
    free(splitted[1]);
    free(splitted);
}