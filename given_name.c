//
// Created by shao on 2022/3/10.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


int main(void) {
    char* pname = "abc, def";
    int i;
    int condition = 1;     //0： false 1: true
    int comma_num = 0;          //; appear times
    int comma_index = 0;        // , index
//    char* family_name;
    char* given_name;
//    int family_name_len;
    int given_name_len;
    int space_after_commaTime = 0;

    for (i = 0; i < strlen(pname); i++) {

        if (!isspace(pname[i])) {
            space_after_commaTime = 0;
        }

        if (isspace(pname[i]) && (i == comma_index+1)) {
            space_after_commaTime += 1;
        }
        if (pname[i] == ',') {
            comma_index = i;
            comma_num += 1;
            space_after_commaTime = 0;
        }

    }


//    family_name_len = comma_index + 1;      //'a\0'
//    family_name = malloc(sizeof (char) * family_name_len);
//    strncpy(family_name,pname, comma_index);
//    family_name[comma_index] = '\0';


    if(isspace(pname[comma_index+1])) {         //Smith, John
        given_name_len = strlen(pname)-comma_index;
        given_name = malloc(sizeof (char) * given_name_len);
        strcpy(given_name,pname+comma_index+1+1);
        given_name[given_name_len] = '\0';
    }
    else {
        given_name_len = strlen(pname)-comma_index;                     //Smith,John
        given_name = malloc(sizeof (char) * given_name_len);
        strcpy(given_name,pname+comma_index+1);
        given_name[given_name_len] = '\0';
    }


//    printf("Given name: %s\n", given_name);
    free(given_name);
    return condition;
}