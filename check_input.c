//
// Created by shao on 2022/3/10.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


int main(void) {
    char *pname = "Xxx,Yyy Zzz Aaa Bbb Ccc Ddd";
    int i;
    int condition = 1;     //0： false 1: true
    int comma_num = 0;          //; appear times
    int comma_index = 0;        // , index
    char* family_name;
    char* given_name;
    int family_name_len;
    int given_name_len;
    int space_after_commaTime;
    if (!isalpha(pname[0])){
        //printf("first char is not alpha");
        condition = 0;
        return condition;
    }
    for (i = 0; i < strlen(pname); i++) {
        if (!isalpha(pname[i]) && !isspace(pname[i]) && pname[i] != ',') {
            printf("if not space, not comma, not alpha\n");
            condition = 0;
            return condition;
        }
        if (!isspace(pname[i])) {
            space_after_commaTime = 0;
        }
        if (isspace(pname[i]) && space_after_commaTime == 1) {
            condition = 0;
            //printf("space error, there should not 2 more space after comma");
            return condition;
        }
        if (isspace(pname[i]) && (i == comma_index+1)) {
            space_after_commaTime ++;
        }
        if (pname[i] == ',') {
            if (isspace(pname[i-1])) {
                condition = 0;
                //printf("space before the comma\n");
                return condition;
            }
            comma_index = i;
            comma_num += 1;
            if(comma_num == 2){                     // two time comma is error
                //printf("Comma False\n");
                condition = 0;
                return condition;
            }
            space_after_commaTime = 0;
        }

    }

    if (comma_num == 0) {
        condition = 0;

        //printf("there will be no people with just one name");
        return condition;
    }

    family_name_len = comma_index + 1;      //'a\0'
    family_name = malloc(sizeof (char) * family_name_len);
    strncpy(family_name,pname, comma_index);
    family_name[comma_index] = '\0';


    if(isspace(pname[comma_index+1])) {         //Smith, John
        given_name_len = strlen(pname)-comma_index-1;
        //printf("%d\n", strlen(pname));
        //printf("%d\n", given_name_len);     //AB,CDE
        given_name = malloc(sizeof (char) * given_name_len);
        //printf("%d\n", sizeof (given_name));     //AB,CDE
        strcpy(given_name,pname+comma_index+1+1);
        given_name[given_name_len] = '\0';
    }
    else {
        given_name_len = strlen(pname)-comma_index;                     //Smith,John
        //printf("%d\n", strlen(pname));
        //printf("%d\n", given_name_len);
        given_name = malloc(sizeof (char) * given_name_len);
        //printf("%d\n", sizeof (given_name));
        strcpy(given_name,pname+comma_index+1);
        given_name[given_name_len] = '\0';
    }

    if (!isupper(family_name[0]) || !isupper(given_name[0])) {         //names begin with an upper-case letter
        //printf("First letter should be Caps\n");
        condition = 0;
        return condition;
    }

    if (strlen(family_name) < 2 || strlen(given_name) < 2) {         //names begin with an upper-case letter
        //printf("names must at least 2 letters\n");
        condition = 0;
        return condition;
    }

    if (isspace(family_name[strlen(family_name)-1])|| isspace(given_name[strlen(given_name)-1])) {         //names begin with an upper-case letter
        printf("end of space\n");
        condition = 0;
        return condition;
    }
    //printf("First name: %s\n", family_name);
    //printf("Given name: %s\n", given_name);

    free(family_name);
    free(given_name);
    return condition;
}