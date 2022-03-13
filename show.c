//
// Created by shao on 2022/3/13.
//

//
// Created by shao on 2022/3/10.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


int main(void) {
    char *pname = "AB, CD ns ef";
    int i;
    int comma_num = 0;          //; appear times
    int comma_index = 0;        // , index
    char* family_name;
    char* given_name;
    char* show_name;            // Yyy Xxx
    int family_name_len;
    int given_name_len;
    int space_after_commaTime;
    int second_space = -1;     // mark the space after given name
    for (i = 0; i < strlen(pname); i++) {
        if (!isspace(pname[i])) {
            space_after_commaTime = 0;
        }

        if (isspace(pname[i]) && (i == comma_index+1)) {
            space_after_commaTime ++;
        }
        if (pname[i] == ',') {
            comma_index = i;
            comma_num += 1;
            space_after_commaTime = 0;
        }
        if (isspace(pname[i]) && i > (comma_index + 1) && second_space == -1) {
            second_space = i;
        }

    }


    family_name_len = comma_index + 1;      //'a\0'
    family_name = malloc(sizeof (char) * family_name_len);
    strncpy(family_name,pname, comma_index);
    family_name[comma_index] = '\0';
    printf("%s\n", family_name);
    // show given name
    if (second_space == -1) {
        second_space = strlen(pname);
    }

//    printf("%d\n",second_space);
    if(isspace(pname[comma_index+1])) {         //Smith, John
        given_name_len = second_space-comma_index-1;
        //printf("%d\n", strlen(pname));
        //printf("%d\n", given_name_len);     //AB,CDE
        given_name = malloc(sizeof (char) * given_name_len);
        //printf("%d\n", sizeof (given_name));     //AB,CDE
        strncpy(given_name,pname+comma_index+1+1, given_name_len-1);
        given_name[given_name_len] = '\0';
    }
//    printf("%s\n", given_name);
    else {
        given_name_len = second_space-comma_index;                     //Smith,John
        //printf("%d\n", strlen(pname));
        //printf("%d\n", given_name_len);
        given_name = malloc(sizeof (char) * given_name_len);
        //printf("%d\n", sizeof (given_name));
        strncpy(given_name,pname+comma_index+1, given_name_len-1);
        given_name[given_name_len] = '\0';
    }

    //printf("First name: %s\n", family_name);
    printf("%s\n", given_name);
    printf("%d\n", family_name_len+given_name_len);
    // show
    show_name = malloc(sizeof (char) * (family_name_len+given_name_len) + 2); //space + '\0'
    strcat(show_name,given_name);
    strcat(show_name," ");
    strcat(show_name,family_name);
    printf("%d\n", strlen(family_name));
    printf("%d\n", strlen(given_name));
    printf("%s\n", show_name);
    free(family_name);
    free(given_name);
    free(show_name);
    return 0;
}