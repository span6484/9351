//
// Created by shao on 2022/3/10.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


int main(void) {
    char *result = "Xxx,Yyy Zzz Aaa Bbb Ccc Ddd";
    char *new_result = NULL;
    new_result = malloc(sizeof (char) * strlen(result) + 1);
    int i,j;
    for (i = 0, j = 0; i < strlen(result); i++) {
        if (i > 0 && result[i-1] =="," && result[i] == " ") {
            continue;
        }
        new_result[j] = result[i];
        j++;
    }
    new_result[j] = '\0';
    printf("%s\n", new_result);
}