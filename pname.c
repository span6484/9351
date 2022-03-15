//
// Created by z5378096 on 2022/3/10.
//version final

/*
 * src/tutorial/pname.c
 *
 ******************************************************************************
  This file contains routines that can be bound to a Postgres backend and
  called by the backend in the process of processing queries.  The calling
  format for these routines is dictated by Postgres architecture.
******************************************************************************/

#include "postgres.h"

#include "fmgr.h"
#include "access/hash.h"
#include "libpq/pqformat.h"		/* needed for send/recv functions */

PG_MODULE_MAGIC;

typedef struct personName
{
    int32 length;
    char pname[FLEXIBLE_ARRAY_MEMBER];				//variable
} PersonName;

/*****************************************************************************
 * Input/Output functions
 *****************************************************************************/
// TO DO
// pgname check
//there may be a single space after the comma
//there may be a single space after the comma
//there will be no numbers (e.g. noGates, William 3rd)
//there will be no titles (e.g. no Dr, Prof, Mr, Ms)
//there will be no initials (e.g. no Shepherd,John A)
static int check_input(char *pname) {
    int i;
    int condition = 1;     //   0： false 1: true
    int comma_num = 0;          //; appear times
    int comma_index = -1;        // , index
    char* family_name;
    char* given_name;
    int family_name_len;
    int given_name_len;
    int space_after_commaTime = 0;
    int strlen_afterSpace_count = 0;            // count strnglen after each space
    if (!isalpha(pname[0])){
        condition = 0;
        return condition;
    }
    if (isspace(pname[0])){         //' Smith, John'
        condition = 0;
        return condition;
    }
    for (i = 0; i < strlen(pname); i++) {
        if (!isalpha(pname[i]) && !isspace(pname[i]) && pname[i] != ',' && pname[i] != '\'' && pname[i] != '-') {
            condition = 0;
            return condition;
        }

        if (!isspace(pname[i])) {
            space_after_commaTime = 0;
        }
        if (isspace(pname[i]) && space_after_commaTime == 1) {
            condition = 0;
            return condition;
        }
        if (isspace(pname[i]) && (i == comma_index+1)) {
            space_after_commaTime += 1;
        }
        if (pname[i] == ',') {
            if (isspace(pname[i-1])) {
                condition = 0;
                return condition;
            }
            comma_index = i;
            comma_num += 1;
            if(comma_num == 2){                     // two time comma is error
                condition = 0;
                return condition;
            }
            space_after_commaTime = 0;
        }
        if (isspace(pname[i])) {
            if (strlen_afterSpace_count < 2) {
                condition = 0;
                return condition;
            }
            strlen_afterSpace_count = 0;
        }
        else {
            strlen_afterSpace_count ++;
        }

    }

    if (strlen_afterSpace_count < 2) {
        condition = 0;
        return condition;
    }
    if (comma_num == -1) {
        condition = 0;
        return condition;
    }

    family_name_len = comma_index;      //'a\0'
    family_name = palloc(sizeof (char) * family_name_len+1);
    strncpy(family_name,pname, comma_index);
    family_name[comma_index] = '\0';

    if(isspace(pname[comma_index+1])) {         //Smith, John
        given_name_len = strlen(pname)-comma_index-1-1;
        given_name = palloc(sizeof (char) * given_name_len+1);
        strcpy(given_name,pname+comma_index+1+1);
        given_name[given_name_len] = '\0';
    }
    else {
        given_name_len = strlen(pname)-comma_index;                     //Smith,John
        given_name = palloc(sizeof (char) * given_name_len+1);
        strcpy(given_name,pname+comma_index+1);
        given_name[given_name_len] = '\0';
    }

    if (!isupper(family_name[0]) || !isupper(given_name[0])) {         //names begin with an upper-case letter
        condition = 0;
        return condition;
    }

    if (strlen(family_name) < 2 || strlen(given_name) < 2) {         //names begin with an upper-case letter
        condition = 0;
        return condition;
    }
    return condition;
}

PG_FUNCTION_INFO_V1(pname_in);

Datum
pname_in(PG_FUNCTION_ARGS)
{
    char	   *str = PG_GETARG_CSTRING(0);
    PersonName    *result;
    int length = 0;

    if (check_input(str) == 0)			//0 is false
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                        errmsg("invalid input syntax for type %s: \"%s\"",
                               "PersonName", str)));

    length = strlen(str) + 1;
    result = (PersonName *) palloc(VARHDRSZ + length);
    SET_VARSIZE(result, VARHDRSZ + length);		// allocate enough memory
    strcpy(result->pname, str);
    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(pname_out);

Datum
pname_out(PG_FUNCTION_ARGS)
{
    PersonName    *personName = (PersonName *) PG_GETARG_POINTER(0);
    char	   *result = NULL;
    char *new_result = NULL;
    int i = 0;
    int new_result_len = 0;
    int comma_index = 0;
    result = palloc(sizeof(char)*strlen(personName->pname));
    strcpy(result,personName->pname);
    for (i = 0; i < strlen(result); i++) {
        if (result[i] == ',') {
            comma_index = i;
            break;
        }
    }

    if(isspace(result[comma_index+1])) {
        comma_index += 1;
        new_result_len = strlen(result)-1;
        new_result = palloc(sizeof (char) * new_result_len+1);
        strncpy(new_result, result, comma_index);
        new_result[comma_index] = '\0';
        strcat(new_result, result+comma_index+1);
        new_result[new_result_len] = '\0';
    }
    else {
        new_result_len = strlen(result);
        new_result = palloc(sizeof (char) * new_result_len+1);
        strcpy(new_result, result);
        new_result[new_result_len] = '\0';
    }
    PG_RETURN_CSTRING(new_result);

}




//To DO: compare the family name, and given name,
// 0 for equal, -1 for a < b, 1 for a > b
static int
pname_abs_cmp_internal(PersonName * a, PersonName * b)
{
    int result = 0;
    char* pname1 = NULL;
    char* pname2 = NULL;
    int i = 0;
    int j = 0;
    char char_1;
    char char_2;
    pname1 = palloc(sizeof(char) * strlen(a->pname));
    pname2 = palloc(sizeof(char) * strlen(b->pname));

    strcpy(pname1, a->pname);
    strcpy(pname2, b->pname);
    while(i < strlen(pname1) || j < strlen(pname2)) {
        char_1 = pname1[i];
        char_2 = pname2[j];
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


PG_FUNCTION_INFO_V1(pname_abs_lt);

Datum
pname_abs_lt(PG_FUNCTION_ARGS)
{
    PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
    PersonName    *b = (PersonName *) PG_GETARG_POINTER(1);

    PG_RETURN_BOOL(pname_abs_cmp_internal(a, b) < 0);
}

PG_FUNCTION_INFO_V1(pname_abs_le);

Datum
pname_abs_le(PG_FUNCTION_ARGS)
{
    PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
    PersonName    *b = (PersonName *) PG_GETARG_POINTER(1);

    PG_RETURN_BOOL(pname_abs_cmp_internal(a, b) <= 0);
}

PG_FUNCTION_INFO_V1(pname_abs_eq);

Datum
pname_abs_eq(PG_FUNCTION_ARGS)
{
    PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
    PersonName    *b = (PersonName *) PG_GETARG_POINTER(1);

    PG_RETURN_BOOL(pname_abs_cmp_internal(a, b) == 0);
}

PG_FUNCTION_INFO_V1(pname_abs_ge);

Datum
pname_abs_ge(PG_FUNCTION_ARGS)
{
    PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
    PersonName    *b = (PersonName *) PG_GETARG_POINTER(1);

    PG_RETURN_BOOL(pname_abs_cmp_internal(a, b) >= 0);
}

PG_FUNCTION_INFO_V1(pname_abs_gt);

Datum
pname_abs_gt(PG_FUNCTION_ARGS)
{
    PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
    PersonName    *b = (PersonName *) PG_GETARG_POINTER(1);

    PG_RETURN_BOOL(pname_abs_cmp_internal(a, b) > 0);
}

PG_FUNCTION_INFO_V1(pname_abs_cmp);

Datum
pname_abs_cmp(PG_FUNCTION_ARGS)
{
    PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
    PersonName    *b = (PersonName *) PG_GETARG_POINTER(1);

    PG_RETURN_INT32(pname_abs_cmp_internal(a, b));
}

//To DO
// construct by mself, for < > part
PG_FUNCTION_INFO_V1(pname_abs_df);
Datum
pname_abs_df(PG_FUNCTION_ARGS)
{
    PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
    PersonName    *b = (PersonName *) PG_GETARG_POINTER(1);

    PG_RETURN_BOOL(pname_abs_cmp_internal(a, b) != 0);
}

/////////////

//TODO
//return family name, TEXT type
PG_FUNCTION_INFO_V1(family);

Datum
family(PG_FUNCTION_ARGS)
{

    text *new_text;
    int32 size = 0;
    int i;
    int comma_num = 0;          //, appear times
    int comma_index = -1;        // , index
    char* family_name;
    int family_name_len;
    int space_after_commaTime = 0;
    char *pname;
    PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
    pname = psprintf("%s", a->pname);
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
    family_name_len = comma_index;      //'a\0'
    family_name = palloc(sizeof (char) * family_name_len+1);
    strncpy(family_name,pname, comma_index);
    family_name[comma_index] = '\0';
    size = VARHDRSZ + strlen(family_name);
    new_text= (text*) palloc(size-1);
    SET_VARSIZE(new_text, size);
    strcpy(VARDATA(new_text), family_name);
    PG_RETURN_TEXT_P(new_text);

}


//TODO
//return given name, TEXT type
PG_FUNCTION_INFO_V1(given);

Datum
given(PG_FUNCTION_ARGS)
{
    text *new_text;
    char *pname = NULL;
    PersonName *a = (PersonName *) PG_GETARG_POINTER(0);
    int32 size = 0;
    int i;
    int comma_num = 0;          //; appear times
    int comma_index = -1;        // , index
    char* given_name;
    int given_name_len;
    int space_after_commaTime = 0;
    int strlen_afterSpace_count = 0;            // count strlen after each space
    pname = psprintf("%s", a->pname);

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
        if (isspace(pname[i])) {
            strlen_afterSpace_count = 0;
        }
        else {
            strlen_afterSpace_count ++;
        }

    }


    if(isspace(pname[comma_index+1])) {         //Smith, John
        given_name_len = strlen(pname)-comma_index-1-1;
        given_name = palloc(sizeof (char) * given_name_len+1);
        strcpy(given_name,pname+comma_index+1+1);
        given_name[given_name_len] = '\0';
    }
    else {
        given_name_len = strlen(pname)-comma_index;                     //Smith,John
        given_name = palloc(sizeof (char) * given_name_len+1);
        strcpy(given_name,pname+comma_index+1);
        given_name[given_name_len] = '\0';
    }

    size = VARHDRSZ + strlen(given_name);
    new_text= (text*) palloc(size-1);
    SET_VARSIZE(new_text, size);
    strcpy(VARDATA(new_text), given_name);
    PG_RETURN_TEXT_P(new_text);
}



//TODO
//return familyName + "," + givenName, the given Name is the first substring before the space
//Type: TEXT
PG_FUNCTION_INFO_V1(show);

Datum
show(PG_FUNCTION_ARGS)
{
    PersonName *a = (PersonName *) PG_GETARG_POINTER(0);
    text *new_text;

    int32 size = 0;

    char *pname = NULL;
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
    pname = psprintf("%s", a->pname);
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

    }

    for (i = 0; i < strlen(pname); i++) {
        if (isspace(pname[i]) && i > (comma_index + 1) && second_space == -1) {
            second_space = i;
        }
    }


    family_name_len = comma_index;      //'a\0'
    family_name = palloc(sizeof (char) * family_name_len+1);
    strncpy(family_name,pname, comma_index);
    family_name[comma_index] = '\0';
    // show given name
    if (second_space == -1) {
        second_space = strlen(pname);
    }

    if(isspace(pname[comma_index+1])) {         //Smith, John
        given_name_len = second_space-comma_index-1-1;
        given_name = palloc(sizeof (char) * given_name_len+1);
        strncpy(given_name,pname+comma_index+1+1, given_name_len);
        given_name[given_name_len] = '\0';

    }
    else {
        given_name_len = second_space-comma_index-1;                     //Smith,John
        given_name = palloc(sizeof (char) * given_name_len+1);
        strncpy(given_name,pname+comma_index+1, given_name_len);
        given_name[given_name_len] = '\0';
    }

    // show
    show_name = palloc(sizeof (char) * (family_name_len+given_name_len) + 1); //space + '\0'
    strcpy(show_name,given_name);
    show_name[given_name_len] = '\0';

    strcat(show_name," ");
    strcat(show_name,family_name);
    size = VARHDRSZ + strlen(show_name);
    new_text= (text*) palloc(size+1);
    SET_VARSIZE(new_text, size);
    strcpy(VARDATA(new_text), show_name);
    PG_RETURN_TEXT_P(new_text);
}


//	TO hash.  return int
// new_str = family + givenName, delete all comma, space
PG_FUNCTION_INFO_V1(pname_hash);

Datum
pname_hash(PG_FUNCTION_ARGS)
{
    int hash_code = 0;
    PersonName *a = (PersonName *) PG_GETARG_POINTER(0);
    char* pname = NULL;
    char* new_str = NULL;
    int signal_count = 0;
    int pname_len = strlen(a->pname);
    int i,j;
    char x;
    pname =  psprintf("%s", a->pname);
    new_str = palloc(sizeof (char) * pname_len);
    for (i = 0, j = 0; i < pname_len; i++) {
        x = pname[i];
        if (x == ',' || x == ' ') {
            signal_count++;
            continue;
        }

        new_str[j] = pname[i];
        j++;
    }
    new_str[j] = '\0';
    //
    hash_code = DatumGetUInt32(hash_any((const unsigned char *) new_str, strlen(new_str)));
    PG_RETURN_INT32(hash_code);
}