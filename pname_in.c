//
// Created by shao on 2022/3/10.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


int main(void) {
    char *str = "Xxx, Yyy";
    char *new_str = NULL;
    int i = 0;
    int new_str_len = 0;
    int comma_index = 0;
    for (i = 0; i < strlen(str); i++) {
        if (str[i] == ',') {
            comma_index = i;
            break;
        }
    }
    if(isspace(str[comma_index+1])) {
        comma_index += 1;
        new_str_len = strlen(str);
        new_str = malloc(sizeof (char) * new_str_len);
        strncpy(new_str, str, comma_index);
        strcat(new_str, str+comma_index+1);
    }
    printf("%s\n", new_str);
}