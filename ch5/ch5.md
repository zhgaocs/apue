# Advanced Programming in the UNIX Environment—Chapter 5 标准I/O库

## 1. 流的定向

### 1.1 概念

流的定向决定了读写的字符是单字节还是多字节。**流最初被创建时，没有定向**

### 1.2 分类

+ 宽定向：多字节
+ 字节定向：单字节

### 1.3 `fwide`函数

#### 1.3.1 功能

**设置**或**查询**流的定向属性

#### 1.3.2 函数原型

```C
#include <stdio.h>
#include <wchar.h>

/**
 * 流是宽定向的返回正值
 * 流是字节定向的返回负值
 * 流未定向返回0
 */
int fwide(FILE *fp, int mode);
```

#### 1.3.3 参数`mode`

+ `mode`为正，指定流为宽定向
+ `mode`为零，不试图设置流的定向，但返回流定向的值
+ `mode`为负，指定流为字节定向

#### 1.3.4 注意

+ `fwide`不改变**已定向**的流
+ `fwide`无出错返回

## 2. 缓冲

### 2.1 分类

+ 全缓冲：缓冲区填满后才进行实际I/O操作。*一个流上执行第一次I/O操作时，相关标准I/O函数通常调用`malloc`获得所需要的缓冲区*

+ 行缓冲：遇到换行符执行I/O操作。流涉及到终端（例如标准输入和标准输出）时，通常使用行缓冲

+ 不缓冲：不对字符进行缓冲存储。`stderr`通常不带缓冲

### 2.2 更改缓冲类型

#### 2.2.1 函数原型

```C
#include <stdio.h>

// 成功返回0，出错返回非0值
void setbuf(FILE *restrict fp, char *restrict buf);
int setvbuf(FILE *restrict fp, char *restrict buf, int mode, size_t size);
```

#### 2.2.2 参数`mode`

+ `_IOFBF`：全缓冲
+ `_IOLBF`：行缓冲
+ `_IONBF`：不带缓冲

#### 2.2.3 搭配不同参数时的效果

![setbuf和setvbuf函数](D:/GithubBlog/source/_posts/setbuf_setvbuf.png)

### 2.3 冲洗（flush）

#### 2.3.1 含义

+ 标准I/O库方面，flush将缓冲区内容写到磁盘上
+ 终端驱动程序方面，flush表示丢弃缓冲区中的数据

#### 2.3.2 `fflush`函数

```C
#include <stdio.h>

// 成功返回0，出错返回EOF
int fflush(FILE *fp);
```

## 3. 打开流

### 3.1 函数原型

```C
#include <stdio.h>

// 成功返回文件指针，出错返回NULL
FILE *fopen(const char *restrict pathname, const char *restrict type);
FILE *freopen(const char *restrict pathname, const char *restrict type, FILE *restrict fp);
FILE *fdopen(int fd, const char *type);
```

### 3.2 功能

+ `fopen`：打开一个指定的文件
+ `freopen`：在一个指定的流上打开一个指定的文件
+ `fdopen`：将一个已存在的文件描述符与一个流关联起来

### 3.3 参数`type`

|       `type`       |             说明             |
| :----------------: | :--------------------------: |
|     `r`或`rb`      |            读打开            |
|     `w`或`wb`      |   文件首写打开，或为写创建   |
|     `a`或`ab`      |   文件尾写打开，或为写创建   |
| `r`或`r+b`或`rb+`  |           读写打开           |
| `w`或`w+b`或`wb+`  | 文件首读写打开，或为读写创建 |
| `a+`或`a+b`或`ab+` | 文件尾读写打开，或为读写创建 |

## 4. 关闭流

```C
#include <stdio.h>

int fclose(FILE *fp);
```

## 5. 读流和写流

### 5.1 输入

```C
#include <stdio.h>

// 成功返回下一个字符，出错或到达文件结尾返回EOF
int getc(FILE *fp);
int fgetc(FILE *fp);
int getchar(void);
```

+ `getchar`等效于`getc(stdin)`
+ `getc`可以用宏实现，`fgetc`一定是函数

### 5.2 两个标志

大多数实现中，每个流在FILE对象中维护两个标志

+ 出错标志

+ 文件结束标志

### 5.3 `ferror`、`feof`和`clearerr`函数

+ `ferror`函数：检测流是否发生错误

```C
 #include <stdio.h>

// 条件为真返回非0值，否则返回0
int ferror(FILE *fp);
```

+ `feof`函数：检测流是否到达文件结束

```C
#include <stdio.h>

// 条件为真返回非0值，否则返回0
int feof(FILE *fp);
```

+ `clearerr`函数：清除标志

```C
#include <stdio.h>

void clearerr(FILE *fp);
```

### 5.3 `ungetc`函数

#### 5.3.1 功能

将字符推回到输入流中

#### 5.3.2 函数原型

```C
#include <stdio.h>

// 成功返回c，出错返回EOF
int ungetc(int c, FILE *fp);
```

#### 5.3.3 注意

+ 字符推回流后，读出字符的顺序与压送回的顺序相反
+ 不能回送EOF
+ 回送的字符不必是上一次读到的字符
+ 到达文件尾端仍可以回送字符*（因为成功的`ungetc`调用会清除流的文件结束标志）*

### 5.4 输出

```C
#include <stdio.h>

// 成功返回c，出错返回EOF
int putc(int c, FILE *fp);
int fputc(int c, FILE *fp);
int putchar(int c);
```

+ `putchar(c)`等效于`putc(c, stdout)`
+ `putc`可以用宏实现，`fputc`一定是函数

## 6.  行I/O

### 6.1 行输入

#### 6.1.1 函数原型

```C
#include <stdio.h>

// 成功返回buf，到达文件结尾或出错返回NULL
char *fgets(char *restrict buf, int n, FILE *restrict fp);
char *gets(char *buf);
```

#### 6.1.2 注意

+ `fgets`读取至换行或`n`-1字符，存入缓冲区并以null结尾。超出部分在下次调用时读取

+ `gets`不推荐使用*（可能发生缓冲区溢出）*，且不将换行符存入缓冲区中

### 6.2 行输出

#### 6.2.1 函数原型

```C
#include <stdio.h>

// 成功返回非负值，出错返回EOF
int fputs(const char *restrict str, FILE *restrict fp);
int puts(const char *str);
```

#### 6.2.2 注意

+ `fputs`将字符串写入流，不包括null。不必每次输出一行，null前可无换行符
+ `puts`函数将字符串写入标准输出，不包括null，但包括换行符

## 7. 二进制I/O

### 7.1 函数原型

```C
#include <stdio.h>

// 返回值是读或写的对象数
size_t fread(void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
size_t fwrite(const void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
```

### 7.2 使用场景

+ 读写二进制数组

```C
float data[10];

fwrite(&data[2], sizeof(data[0]), 4, fp); // data[2] data[3] data[4] data[5]
```

+ 读写一个结构

```C
struct
{
    short count;
    long total;
    char name[NAMESIZE];
} item;

fwrite(&item, sizeof(item), 1, fp);
```

## 8. 定位流

### 8.1 `ftell`和`fseek`函数（`long`）

```C
#include <stdio.h>

// 返回当前文件位置指示，出错返回-1L
long ftell(FILE *fp);

// 成功返回0，出错返回非0值
int fseek(FILE *fp, long offset, int whence);
```

### 8.2 `ftello`和`fseeko`函数（`off_t`）

```C
#include <stdio.h>

// 成功返回当前文件位置指示，出错返回-1
off_t ftello(FILE *fp);

// 成功返回0，出错返回非0值
int fseeko(FILE *fp, off_t offset, int whence);
```

### 8.3 `fgetpos`和`fsetpos`函数（`fpos_t`）

```C
#include <stdio.h>

// 成功返回0，出错返回非0
int fgetpos(FILE *restrict fp, fpos_t *restrict pos);
int fsetpos(FILE *fp, const fpos_t *pos);
```

### 8.4 `rewind`函数

将文件指针重置到文件开头

```C
#include <stdio.h>

void rewind(FILE *fp);
```

## 9. 格式化I/O

### 9.1 格式化输出

#### 9.1.1 函数原型

```C
#include <stdio.h>

// 成功返回输出字符数，出错返回负值
int printf(const char *restrict format, ...);
int fprintf(FILE *restrict fp, const char *restrict format, ...);

// 成功返回存入数组的字符数，编码出错返回负值
int sprintf(char *restrict buf, const char *restrict format, ...);            // 将格式化的数据写入字符串
int snprintf(char *restrict buf, size_t n, const char *restrict format, ...); // 将格式化的数据写入字符串
```

#### 9.1.2 可变参数`arg`替换`...`

```C
#include <stdio.h>
#include <stdarg.h>

// 成功返回输出字符数，出错返回负值
int vprintf(const char *restrict format, va_list arg);
int vfprintf(FILE *restrict fp, const char *restrict format, va_list arg);

// 成功返回存入数组的字符数，编码出错返回负值
int vsprintf(char *restrict buf, const char *restrict format, va_list arg);            // 将格式化的数据写入字符串
int vsnprintf(char *restrict buf, size_t n, const char *restrict format, va_list arg); // 将格式化的数据写入字符串
```

### 9.2 格式化输入

#### 9.2.1 函数原型

```C
#include <stdio.h>

// 返回指定的输入项数，若出错或任意变换前到达文件结尾返回EOF
int scanf(const char *restrict format, ...);
int fscanf(FILE *restrict fp, const char *restrict format, ...);
int sscanf(const char *restrict buf, const char *restrict format, ...); // 从字符串中读取格式化输入
```

#### 9.2.2 可变参数`arg`替换`...`

```C
#include <stdio.h>
#include <stdarg.h>

// 返回指定的输入项数，若出错或任意变换前到达文件结尾返回EOF
int vscanf(const char *restrict format, va_list arg);
int vfscanf(FILE *restrict fp, const char *restrict format, va_list arg);
int vsscanf(const char *restrict buf, const char *restrict format, va_list arg); // 从字符串中读取格式化输入
```

## 9. 获得流对应的描述符

```C
#include <stdio.h>

// 返回与流相关联的文件描述符
int fileno(FILE * fp);
```

## 10. 临时文件

### 10.1 `tmpnam`函数

#### 10.1.1 功能

生成并返回一个有效的临时文件名

#### 10.1.2 函数原型

```C
#include <stdio.h>

// 指向唯一路径名的指针
char *tmpnam(char *ptr);
```

#### 10.1.3 参数`ptr`

+ 为`NULL`时，函数返回指向静态区路径名的指针，下次调用将重写此区域
+ 不为`NULL`时，`ptr`指向一个长度至少为`L_tmpnam`*（定义在`<stdio.h>`中）*的字符数组

### 10.2 `tmpfile`函数

#### 10.2.1 功能

创建一个临时文件（类型为`wb+`）

#### 10.2.2 函数原型

```C
#include <stdio.h>

// 成功返回文件指针，出错返回NULL
FILE *tmpfile(void);
```

#### 10.2.3 注意

`tmpfile`创建的临时文件关闭或程序结束时会自动删除

### 10.3 `tempnam`函数

#### 10.3.1 功能

生成并返回一个有效的临时文件名

#### 10.3.2 函数原型

```C
#include <stdio.h>

// 返回指向唯一路径名的指针（指向动态内存，须手动释放）
char *tempnam(const char *directory, const char *prefix);
```

#### 10.3.3 参数`directory`的确定顺序

1. 环境变量`TMPDIR`
2. 非`NULL`的`directory`
3. 字符串`P_tmpdir`*（定义在`<stdio.h>`中）*
4. 本地目录（通常是`/tmp`）

#### 10.3.4 参数`prefix`

如果`prefix`不为`NULL`，则其最多包含五个字符将作为文件名的开头

### 10.4 `mkstemp`函数

#### 10.4.1 功能

创建一个临时文件并打开

#### 10.4.2 函数原型

```C
#include <stdlib.h>

// 成功返回文件描述符，出错返回-1
int mkstemp(char *template);
```

#### 10.4.3 注意

`mkstemp`创建的临时文件不会自动删除*（如果想要删除，需要自行`unlink`）*