#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <libc.h>

//
// Created by shao on 2022/3/14.
//
int main(void) {
    char* pname1 = "Smith,John";
    char* pname2 = "Smith,Jane";
    int i = 0;
    int j = 0;
    char char_1;
    char char_2;
    int result = 0;
    int a = 0;
    while(i < strlen(pname1) || j < strlen(pname2)) {
        char_1 = tolower(pname1[i]);
        char_2 = tolower(pname2[j]);
        if (char_1 == ',') {
            i++;
            continue;
        }
        if (char_2 == ',') {
            j++;
            continue;
        }
        if (i > 2 && char_1 == ' ' && pname1[i-1] == ',') {
            i ++;
            continue;
        }
        if (j > 2 && char_2 == ' ' && pname1[j-1] == ',') {
            j ++;
            continue;
        }
        result = (int)char_1 - (int)char_2;
        if (result != 0) {
            break;
        }
        i++;
        j++;
    }

    if (result > 0) {
        return 1;
    }
    else if (result < 0) {
        return -1;
    }
    else {
        return 0;
    }
}

