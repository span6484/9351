given(PG_FUNCTION_ARGS)
        {
                char *pname = NULL;
                PersonName *a = (PersonName *) PG_GETARG_POINTER(0);
                // 把given name取出来返回
                int i;
                int comma_num = 0;          //; appear times
                int comma_index = -1;        // , index
                char* given_name;
                int given_name_len;
                int space_after_commaTime = 0;
                int strlen_afterSpace_count = 0;            // count strnglen after each space
                pname = psprintf("%s", a->pname);	//格式化输出

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

                //// elog(NOTICE, "family name is %s", family_name);

                if(isspace(pname[comma_index+1])) {         //Smith, John
                    given_name_len = strlen(pname)-comma_index-1-1;

                    //// elog(NOTICE, "pname name length size is %d", given_name_len);
                    //// elog(NOTICE, "pname name  palloc size is %d", given_name_len+1);
                    //printf("%d\n", strlen(pname));
                    //printf("%d\n", given_name_len);     //AB,CDE
                    given_name = palloc(sizeof (char) * given_name_len+1);
                    //printf("%d\n", sizeof (given_name));     //AB,CDE
                    strcpy(given_name,pname+comma_index+1+1);
                    //// elog(NOTICE, "pname name is %s",given_name);
                    given_name[given_name_len] = '\0';
                }
                else {
                    given_name_len = strlen(pname)-comma_index;                     //Smith,John
                    //printf("%d\n", strlen(pname));
                    //printf("%d\n", given_name_len);

                    //// elog(NOTICE, "pname name length size is %d", given_name_len);
                    //// elog(NOTICE, "pname name  palloc size is %d", given_name_len+1);
                    given_name = palloc(sizeof (char) * given_name_len+1);
                    //printf("%d\n", sizeof (given_name));
                    strcpy(given_name,pname+comma_index+1);
                    given_name[given_name_len] = '\0';
                    //// elog(NOTICE, "pname name is %s",given_name);

                }

                PG_RETURN_CSTRING(given_name);			//return 一个字符串
        }