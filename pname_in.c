//
// Created by shao on 2022/3/10.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


int main(void) {
    char *result = "Xxx, Yyy Zzz Aaa Bbb Ccc Ddd";
    char *new_result = NULL;
    int i = 0;
    int new_result_len = 0;
    int comma_index = 0;
    for (i = 0; i < strlen(result); i++) {
        if (result[i] == ',') {
            comma_index = i;
            break;
        }
    }
    if(isspace(result[comma_index+1])) {
        comma_index += 1;
        new_result_len = strlen(result);
        new_result = malloc(sizeof (char) * new_result_len);
        strncpy(new_result, result, comma_index);
        printf("%s\n", new_result);
        strcat(new_result, result+comma_index+1);
    }
    else {
        new_result_len = strlen(result) + 1;
        new_result = malloc(sizeof (char) * new_result_len);
        strcpy(new_result, result);
    }
    printf("%s\n", new_result);
    free(new_result);
}