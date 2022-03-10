//
// Created by shao on 2022/3/10.
//version 1，

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
static int check_input(char *str) {
    int result = 0;
    if (strlen(str) > 0) {
        result = 1;
    }
    return result;
}

PG_FUNCTION_INFO_V1(pname_in);

Datum
pname_in(PG_FUNCTION_ARGS)
{
    // Shepherd, John Andrew
    char	   *str = PG_GETARG_CSTRING(0);
    PersonName    *result;
    //string length
    int length = 0;

    if (check_input(str) == 0)			//0 is false
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                        errmsg("invalid input syntax for type %s: \"%s\"",
                               "PersonName", str)));

    length = strlen(str) + 1;			//c语言字符串长度后面空一个
    result = (PersonName *) palloc(VARHDRSZ + length);
    SET_VARSIZE(result, VARHDRSZ + length);		// 分配好足够内存
    // result->x = x;
    // result->y  = y;				//要做处理
    strcpy(result->pname, str);		//复制输入的内容到postgresql中
    // 返回结构体指针
    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(pname_out);

Datum
pname_out(PG_FUNCTION_ARGS)
{
    PersonName    *personName = (PersonName *) PG_GETARG_POINTER(0);
    char	   *result;

    result = psprintf("%s", personName->pname);	//格式化输出
    PG_RETURN_CSTRING(result);
}




static int
pname_abs_cmp_internal(PersonName * a, PersonName * b)
{
    //To DO: 需要自己实现, 比较name, 先比较 family name, 再比较given name
    // 1. 若果a 和 b 的first name , last name 都相等, return 1
    //-允许空格, 大小写
    return strcmp(a->pname, b->pname);		//直接这样不行哦
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

//	TO family.   return string
PG_FUNCTION_INFO_V1(family);

Datum
family(PG_FUNCTION_ARGS)
{
    char *result;
    PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
    // 把family name取出来返回
    result = psprintf("%s", a->pname);	//格式化输出
    PG_RETURN_CSTRING(result);			//return 一个字符串
}



//	TO given. return string
PG_FUNCTION_INFO_V1(given);

Datum
given(PG_FUNCTION_ARGS)
{
    char *result = NULL;
    PersonName *a = (PersonName *) PG_GETARG_POINTER(0);
    // 把given name取出来返回
    //TODO
    result = psprintf("%s", a->pname);	//格式化输出
    PG_RETURN_CSTRING(result);			//return 一个字符串
}



//	TO show		return string
PG_FUNCTION_INFO_V1(show);

Datum
show(PG_FUNCTION_ARGS)
{
    char *result = NULL;
    PersonName *a = (PersonName *) PG_GETARG_POINTER(0);
    // show name取出来返回
    //TODO
    result = psprintf("%s", a->pname);	//格式化输出
    PG_RETURN_CSTRING(result);			//return 一个字符串
}


//	TO hash.  return int
PG_FUNCTION_INFO_V1(pname_hash);

Datum
pname_hash(PG_FUNCTION_ARGS)
{
    int hash_code = 0;
    PersonName *a = (PersonName *) PG_GETARG_POINTER(0);
    // 把family name, given name, 取出来后hash
    //TODO
    hash_code = DatumGetUInt32(hash_any((const unsigned char *) a->pname, strlen(a->pname)));
    PG_RETURN_INT32(hash_code);			//return 一个字符串
}