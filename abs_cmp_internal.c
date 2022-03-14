static int
pname_abs_cmp_internal(PersonName * a, PersonName * b)
{
    //To DO: 需要自己实现, 比较name, 先比较 family name, 再比较given name
    // 1. 若果a 和 b 的first name , last name 都相等, return 1
    //-允许空格, 大小写
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
    // elog(NOTICE, "%s",pname1);
    // elog(NOTICE, "%s",pname2);    
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