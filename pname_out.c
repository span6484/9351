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
    //elog(NOTICE, "result is %s", result);
    //elog(NOTICE, "result_len is %d", result_len);
    //printf("%d\n", given_name_len);

    //// elog(NOTICE, "pname name length size is %d", given_name_len);
    //// elog(NOTICE, "pname name  palloc size is %d", given_name_len+1);
    for (i = 0; i < strlen(result); i++) {
        if (result[i] == ',') {
            comma_index = i;
            break;
        }
    }

    if(isspace(result[comma_index+1])) {
        comma_index += 1;
        new_result_len = strlen(result)-1;
        // elog(NOTICE, "space after comma");
        // elog(NOTICE, "comma_index is %d", comma_index);
        // elog(NOTICE, "new_result_len is %d", new_result_len);
        // elog(NOTICE, "new_result palloc is %d", new_result_len+1);
        new_result = palloc(sizeof (char) * new_result_len+1);

        strncpy(new_result, result, comma_index);
        new_result[comma_index] = '\0';
        // elog(NOTICE, "new_result front is %s", new_result);
        strcat(new_result, result+comma_index+1);
        new_result[new_result_len] = '\0';
        // elog(NOTICE, "new_result final is %s", new_result);

    }
    else {
        new_result_len = strlen(result);
        // elog(NOTICE, "no space after comma");
        // elog(NOTICE, "comma_index is %d", comma_index);
        // elog(NOTICE, "new_result_len is %d", new_result_len);
        // elog(NOTICE, "new_result palloc is %d", new_result_len+1);
        new_result = palloc(sizeof (char) * new_result_len+1);
        strcpy(new_result, result);
        new_result[new_result_len] = '\0';
        // elog(NOTICE, "new_result final is %s", new_result);
    }

    // printf("%s\n", new_result);
    // result = psprintf("%s", personName->pname);	//格式化输出
    PG_RETURN_CSTRING(new_result);

}