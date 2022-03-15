# 9315 assignment1



## 有效的PersonName



- 后面可以有一个空格, 也可以没有
- 必须同时有family name 和 given name
    - Jack 								WRONG!!!

- No title
    - like (Dr su)					WRONG!!!





Run Code

```
source /localstorage/$USER/env
```



```
z5378096@nw-syd-vxdb:~$ cd /localstorage/$USER
```

```
z5378096@nw-syd-vxdb:/localstorage/z5378096$ source /localstorage/$USER/env
```

```
z5378096@nw-syd-vxdb:/localstorage/z5378096$ pg_ctl start
```

```
z5378096@nw-syd-vxdb:/localstorage/z5378096$ cd postgresql-14.1/src/tutorial
```



VSCODE: /localstorage/z5378096/postgresql-14.1/src/



- 修改 pname.source



```
##修改
CREATE FUNCTION pname_in(cstring)			##修改
   RETURNS PersonName								##修改
   AS '_OBJWD_/pname'								##修改
   LANGUAGE C IMMUTABLE STRICT;
```

```
CREATE FUNCTION pname_out(PersonName)
   RETURNS PersonName
   AS '_OBJWD_/pname'
   LANGUAGE C IMMUTABLE STRICT;
```



- binary sent & receive 不需要

- #创建类型

```
-- now, we can create the type. The internallength specifies the size of the
-- memory block required to hold the type (we need two 8-byte doubles).
CREATE TYPE PersonName (
   internallength = VARIABLE,
   input = pname_in,
   output = pname_out,
   alignment = int
);

```



-  小于

```
CREATE FUNCTION pname_abs_lt(PersonName, PersonName) RETURNS bool					
   AS '_OBJWD_/pname' LANGUAGE C IMMUTABLE STRICT;
   
   
#pname_abs_lt   小于function


CREATE OPERATOR < (
   leftarg = complex, rightarg = complex, procedure = pname_abs_lt,
   commutator = > , negator = >= ,
   restrict = scalarltsel, join = scalarltjoinsel
);
```

`我们可以发现 operator 调用上面的`



- btree

```
-- create the support function too
CREATE FUNCTION pname_abs_cmp(PersonName, PersonName) RETURNS int4
   AS '_OBJWD_/pname' LANGUAGE C IMMUTABLE STRICT;

-- now we can make the operator class
CREATE OPERATOR CLASS pname_abs_ops
    DEFAULT FOR TYPE PersonName USING btree AS
        OPERATOR        1       < ,
        OPERATOR        2       <= ,
        OPERATOR        3       = ,
        OPERATOR        4       >= ,
        OPERATOR        5       > ,
        FUNCTION        1       pname_abs_cmp(PersonName, PersonName);

```



**我们需要模仿btree 创建 hash**

```
-- what i did 
-- create hash support function
CREATE FUNCTION pname_hash_cmp(PersonName, PersonName) RETURNS int4
   AS '_OBJWD_/pname' LANGUAGE C IMMUTABLE STRICT;

-- make the operator class for hash
CREATE OPERATOR CLASS pname_hash_ops
    DEFAULT FOR TYPE PersonName USING hash AS
        OPERATOR        1       = ,
        FUNCTION        1       pname_hash(PersonName);
```





- 辅助函数

```
-- create the support function too
CREATE FUNCTION family(PersonName) RETURNS text
   AS '_OBJWD_/pname' LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION given(PersonName) RETURNS text
   AS '_OBJWD_/pname' LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION show(PersonName) RETURNS text
   AS '_OBJWD_/pname' LANGUAGE C IMMUTABLE STRICT;
```







## pname.c

### 结构体定义

```c
typedef struct personName
{
	int32 length;
	char pname[FLEXIBLE_ARRAY_MEMBER];				//variable 
} PersonName;
```



- 告诉postgre, 我定义了pname_in 的函数

```c
PG_FUNCTION_INFO_V1(pname_in);
Datum 															// 数据库内置类型
pname_in(PG_FUNCTION_ARGS)
{
```

这个跟source里的function 对应

```
CREATE FUNCTION pname_in(cstring)
```



- **!!! postgre 必须满足iso_c90语法**

```c
int a = 0;

a = strlen(xxx)			//先声明,再定义
```







## To do

- check_input

```c
bool check_input(char *str) {
	bool result = false;
	if (strlen(str) > 0) {
		result = true;
	}
	return result;
}
```

- check name

```c
pname_abs_cmp_internal(PersonName * a, PersonName * b)
{
	//To DO: 需要自己实现, 比较name, 先比较 family name, 再比较given name
	return strcmp(a->pname, b->pname);		//直接这样不行哦
}
```

- family name

```c
PG_FUNCTION_INFO_V1(family);

Datum
family(PG_FUNCTION_ARGS)
{
	PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
	// 把family name取出来返回
	//TODO
	result = psprintf("%s", personName->pname);	//格式化输出
	PG_RETURN_CSTRING(result);			//return 一个字符串
}

```

- given

```c
//	TO given. return string
PG_FUNCTION_INFO_V1(given);

Datum
given(PG_FUNCTION_ARGS)
{
	char *result = NULL;
	PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
	// 把given name取出来返回
	//TODO
	result = psprintf("%s", personName->pname);	//格式化输出
	PG_RETURN_CSTRING(result);			//return 一个字符串
}
```

- show

```c
//	TO show		return string
PG_FUNCTION_INFO_V1(show);

Datum
show(PG_FUNCTION_ARGS)
{
	char *result = NULL;
	PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
	// show name取出来返回
	//TODO
	result = psprintf("%s", personName->pname);	//格式化输出
	PG_RETURN_CSTRING(result);			//return 一个字符串
}
```

- Hash 	自己看下postgresql怎么实现的hash_any

```c
//	TO hash.  return int
PG_FUNCTION_INFO_V1(pname_hash);

Datum
pname_hash(PG_FUNCTION_ARGS)
{
	char *result = NULL;
	PersonName    *a = (PersonName *) PG_GETARG_POINTER(0);
	// 把family name取出来返回
	//TODO
	result = psprintf("%s", personName->pname);	//格式化输出
	PG_RETURN_INT32(result);			//return 一个字符串
}
```





## 测试

`make`

```
cp postgresql-14.1/src/tutorial/pname.c testing/pname.c
```



```
/localstorage/z5378096/testing$ 
```



```
./run_test.py
```



## BUG

pname.sql 跟pname.source 有关



##### print message:

```
elog(NOTICE, "%s", personName.pname)
```



##### palloc 不用free, 但是一定要给刚好的内存

```
given_name = palloc(sizeof (char) * given_name_len+1);
```





## C 相关语法

```c
#copy substring
strncpy(char* target,char* source, int target_len);
target[end] = '\0'
```

```c
#string a + string b
strcat(char* a, char* b);
```

