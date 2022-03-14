show(PG_FUNCTION_ARGS)
        {
                char *pname = NULL;
                PersonName *a = (PersonName *) PG_GETARG_POINTER(0);
                // textsize= VARHDRSE + strlen(personName->pname) + 1
                // show name取出来返回
                //TODO
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
                // text *return_text;
                // int32 text_size;

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


                family_name_len = comma_index;      //'a\0'
                //// //elog(NOTICE, "comma index palloc size is %d", comma_index);
                //// //elog(NOTICE, "family name len is %d",family_name_len);
                //// //elog(NOTICE, "family name palloc size is %d", family_name_len+1);
                family_name = palloc(sizeof (char) * family_name_len+1);
                strncpy(family_name,pname, comma_index);
                family_name[comma_index] = '\0';
                // printf("%s\n", family_name);
                // show given name
                if (second_space == -1) {
                    second_space = strlen(pname);
                }
//    printf("%d\n",second_space);
                if(isspace(pname[comma_index+1])) {         //Smith, John
                    given_name_len = second_space-comma_index-1-1;
                    //elog(NOTICE, " spacc-------------");
                    //elog(NOTICE, " second_space is %d", second_space);
                    //elog(NOTICE, " comma_index is %d", comma_index);
                    //elog(NOTICE, " given length size is %d", given_name_len);
                    //elog(NOTICE, " given length size is %d", given_name_len);
                    //elog(NOTICE, "given name  palloc size is %d", given_name_len+1);
                    given_name = palloc(sizeof (char) * given_name_len+1);
                    //printf("%d\n", sizeof (given_name));     //AB,CDE
                    strncpy(given_name,pname+comma_index+1+1, given_name_len);
                    given_name[given_name_len] = '\0';
                    //elog(NOTICE, "given name is %s", given_name);
                }
//    printf("%s\n", given_name);
                else {
                    given_name_len = second_space-comma_index-1;                     //Smith,John
                    //printf("%d\n", strlen(pname));
                    //printf("%d\n", given_name_len);
                    // //elog(NOTICE, " second_space is %d", second_space);
                    // //elog(NOTICE, " comma_index is %d", comma_index);
                    // //elog(NOTICE, " given_name length size is %d", given_name_len);
                    // //elog(NOTICE, " given_name  palloc size is %d", given_name_len+1);
                    given_name = palloc(sizeof (char) * given_name_len+1);
                    //printf("%d\n", sizeof (given_name));
                    strncpy(given_name,pname+comma_index+1, given_name_len);
                    given_name[given_name_len] = '\0';
                    //elog(NOTICE, "given name is %s", given_name);
                }
                //// //elog(NOTICE, NOTICE, "the given_name_len is %d", given_name_len);
                //// //elog(NOTICE, NOTICE, "the given_nameis %s", given_name);
                //printf("First name: %s\n", family_name);
                // printf("%s\n", given_name);
                // printf("%d\n", family_name_len+given_name_len);
                // show
                show_name = palloc(sizeof (char) * (family_name_len+given_name_len) + 1); //space + '\0'
                strcpy(show_name,given_name);
                show_name[given_name_len] = '\0';
                //// //elog(NOTICE, NOTICE, "the pname is %s", given_name);
                strcat(show_name," ");
                strcat(show_name,family_name);
                //elog(NOTICE, "show name is %s", show_name);
                PG_RETURN_CSTRING(show_name);			//return 一个字符串, 返回text
        }