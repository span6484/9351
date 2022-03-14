family(PG_FUNCTION_ARGS)
        {
                int i;
                int comma_num = 0;          //; appear times
                int comma_index = -1;        // , index
                char* family_name;
                int family_name_len;
                int space_after_commaTime = 0;
                char *pname;
                PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
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

                }
                family_name_len = comma_index;      //'a\0'
                //// elog(NOTICE, "comma index palloc size is %d", comma_index);
                //// elog(NOTICE, "family name len is %d",family_name_len);
                //// elog(NOTICE, "family name palloc size is %d", family_name_len+1);
                family_name = palloc(sizeof (char) * family_name_len+1);
                strncpy(family_name,pname, comma_index);
                family_name[comma_index] = '\0';
                PG_RETURN_CSTRING(family_name);			//return 一个字符串

        }