//family+given name 去掉所有逗号和空格
// Created by shao on 2022/3/15.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char* pname = "Jason, chen";
    char* new_str = NULL;
    int signal_count = 0;
    int pname_len = strlen(pname);
    int i,j;
    char a;
    new_str = malloc(sizeof (char) * pname_len);             //不用加一， 因为要去逗号
    for (i = 0, j = 0; i < pname_len; i++) {
        a = pname[i];
        if (a == ',' || a == ' ') {
            signal_count++;
            continue;
        }

        new_str[j] = pname[i];
        j++;
    }
    new_str[j] = '\0';
    printf("%s\n", new_str);
    return 0;
}