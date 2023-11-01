# Advanced Programming in the UNIX Environment—Chapter 4  文件和目录

## 1. `stat`、`fstat`和`lstat`函数

### 1.1 功能

获取文件有关信息，其中`lstat`获取**符号链接本身**的信息，并不是链接所指向的文件的信息

### 1.2 函数原型

```C
#include <sys/stat.h>

// 成功返回0，出错返回-1
int stat(const char *restrict pathname, struct stat *restrict buf);
int fstat(int fd, struct stat *buf);
int lstat(const char *restrict pathname, struct stat *restrict buf);
```

### 1.3 结构体`stat`

```C
struct stat
{
    mode_t st_mode;       /* permissions */
    ino_t st_ino;         /* i-node number */
    dev_t st_dev;         /* device number */
    dev_t st_rdev;        /* device number for special files */
    nlink_t st_nlink;     /* number of links */
    uid_t st_uid;         /* user ID of owner */
    gid_t st_gid;         /* group ID of owner */
    off_t st_size;        /* size in bytes, for regular files */
    time_t st_atime;      /* access */
    time_t st_mtime;      /* modify */
    time_t st_ctime;      /* status change */
    blksize_t st_blksize; /* best I/O block size */
    blkcnt_t st_blocks;   /* number of disk blocks allocated */
};
```

### 1.4 应用

`ls -l`命令

## 2. 文件类型

### 2.1 文件分类

+ 普通文件（regular file）
+ 目录文件（directory file）
+ 块特殊文件（block special file）：提供对设备**带缓冲**的访问，每次访问以**固定长度**为单位进行
+ 字符特殊文件（character special file）：提供对设备**不带缓冲**的访问，每次**访问长度可变**
+ FIFO：用于进程间通信，也称为**管道**
+ 套接字（socket）：用于进程间网络通信，也可用在一台机器上进程间非网络通信
+ 符号链接（symbolic link）

### 2.2 文件类型宏

宏参数：`struct stat`中的`st_mode`成员

|      宏      |   文件类型   |
| :----------: | :----------: |
| `S_ISREG()`  |   普通文件   |
| `S_ISDIR()`  |   目录文件   |
| `S_ISCHR()`  | 字符特殊文件 |
| `S_ISBLK()`  |  块特殊文件  |
| `S_ISFIFO()` |  管道或FIFO  |
| `S_ISLNK()`  |   符号链接   |
| `S_ISSOCK()` |    套接字    |

### 2.3 IPC类型宏

宏参数：`struct stat *`

|       宏        |   对象类型   |
| :-------------: | :----------: |
| `S_TYPEISMQ()`  |   消息队列   |
| `S_TYPEISSEM()` |    信号量    |
| `S_TYPEISSHM()` | 共享存储对象 |

## 3. 与进程有关的ID

### 3.1 实际用户ID 实际组ID

+ 当前登录用户的ID及其所属组的ID
+ 登录时取自口令文件中的登录项

### 3.2 有效用户ID 有效组ID 附加组ID

+ 用于文件访问权限检查

### 3.3 保存的设置用户ID 保存的设置组ID

+ 当进程执行某文件时，进程的有效用户ID设置为文件所有者的用户ID
+ 当进程执行某文件时，进程的有效组ID设置为文件组所有者ID

## 4. 文件访问权限

### 4.1 打开某文件时

+ 该文件绝对路径中包含的所有目录都应有**执行权限**。如打开文件`/usr/include/stdio.h`，需要对目录`/`、`/usr`、`/usr/include`具有执行权限
+ 需要对文件本身的适当权限

### 4.2 目录文件的权限

+ 读权限：允许获取目录中所有文件名

+ 写权限：允许在目录中添加或删除文件
+ 执行权限：允许在目录中查找

## 5. 新文件和目录的所有权

+ 用户ID：进程的有效用户ID
+ 组ID
    + 有效组ID
    + 所在目录的组ID

## 6. `access`函数

### 6.1 功能

以**实际用户ID**和**实际组ID**测试访问能力*（因为内核以进程的有效用户ID和有效组ID为基础执行访问权限测试）*

### 6.2 函数原型

```C
#include <unistd.h>

// 成功返回0，出错返回-1
int access(const char *pathname, int mode);
```

### 6.3 参数`mode`

| `mode` |       说明       |
| :----: | :--------------: |
| `R_OK` |    测试读权限    |
| `W_OK` |    测试写权限    |
| `X_OK` |   测试执行权限   |
| `F_OK` | 测试文件是否存在 |

## 7. `umask`函数

### 7.1 功能

创建文件屏蔽字（用于屏蔽或关闭某些文件权限，以确保文件只能以受限的方式被访问）

### 7.2 函数原型

```C
#include <sys/stat.h>

// 返回值：以前的文件模式创建屏蔽字
mode_t umask(mode_t mask);
```

### 7.3 示例

```C
// test.c
#include <sys/stat.h>

int main()
{
    mode_t mode = 0666; // S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
    mode_t mask = 0002; // S_IWOTH

    umask(mask);
    creat("test.txt", mode);

    return 0;
}
```

```BASH
$ gcc test.c
$ ./a.out
$ ls -l test.txt
-rw-rw-r--. 1 root root 0 Oct  1 04:09 test.txt
```

## 8. `chmod`和`fchmod`函数

### 8.1 功能

更改现有文件的访问权限，必须满足如下条件之一：

+ 进程的有效用户ID等于文件所有者ID
+ 进程具有超级用户权限

### 8.2 函数原型

```C
#include <sys/stat.h>

// 成功返回0，出错返回-1
int chmod(const char *pathname, mode_t mode);
int fchmod(int fd, mode_t mode);
```

### 8.3 `mode`参数

|                         `mode`                         |                             说明                             |
| :----------------------------------------------------: | :----------------------------------------------------------: |
|        `S_ISUID`<br />`S_ISGID`<br />`S_ISVTX`         | 执行时设置用户ID<br />执行时设置组ID<br />保存正文（粘滞位） |
| `S_IRWXU`<br />`S_IRUSR`<br />`S_IWUSR`<br />`S_IXUSR` | 用户（所有者）读、写和执行<br />用户（所有者）读<br />用户（所有者）写<br />用户（所有者）执行 |
| `S_IRWXG`<br />`S_IRGRP`<br />`S_IWGRP`<br />`S_IXGRP` |        组读、写和执行<br />组读<br />组写<br />组执行        |
| `S_IRWXO`<br />`S_IROTH`<br />`S_IWOTH`<br />`S_IXOTH` |    其他读、写和执行<br />其他读<br />其他写<br />其他执行    |

## 9. 粘滞位

目录上设置粘滞位，对目录具有写权限的用户满足以下任一条件才能删除或更名该目录下的文件：

+ 拥有此文件
+ 拥有此目录
+ 是超级用户

## 10. `chown`、`fchown`和`lchown`函数

### 10.1 功能

更改文件的用户ID和组ID，其中`lchown`更改**符号链接本身**的所有者，而不是符号链接所指向的文件

### 10.2 函数原型

```C
#include <unistd.h>

// 成功返回0，出错返回-1
int chown(const char *pathname, uid_t owner, gid_t group);
int fchown(int fd, uid_t owner, gid_t group);
int lchown(const char *pathname, uid_t owner, gid_t group);
```

### 10.3 注意

+ `owner`为-1，不修改用户ID

+ `group`为-1，不修改组ID

## 11. 文件长度

`stat`结构成员`st_size`表示以字节为单位的文件长度，此字段只对普通文件、目录文件和符号链接有意义

## 12. `truncate`和`ftruncate`函数

### 12.1 功能

删减文件尾部数据以缩小文件大小

### 12.2 函数原型

```C
#include <unistd.h>

// 成功返回0，出错返回-1
int truncate(const char *pathname, off_t length);
int ftruncate(int fd, off_t length);
```

### 12.3 特例

文件清空（打开文件使用`O_TRUNC`标志）

## 13. 硬链接

### 13.1 `link`函数

#### 13.1.1 功能

创建硬链接

#### 13.1.2 函数原型

```C
#include <unistd.h>

// 成功返回0，出错返回-1
int link(const char *existingpath, const char *newpath);
```

#### 13.1.3 注意

+ 若`newpath`已存在，函数返回出错
+ 函数只创建`newpath`中的最后一个分量，路径中的其他部分应当已经存在
+ 避免创建指向目录的硬链接
+ 不支持跨文件系统创建硬链接

#### 13.1.4 示例

```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    if (creat("test.txt", 0666) < 0)
    {
        fprintf(stderr, "creat error");
        exit(1);
    }

    if (link("test.txt", "test.txt.link") < 0)
    {
        fprintf(stderr, "link error");
        exit(1);
    }

    exit(0);
}
```

### 13.2 `unlink`函数

#### 13.2.1 功能

删除链接

#### 13.2.2 函数原型

```C
#include <unistd.h>

// 成功返回0，出错返回-1
int unlink(const char *pathname);
```

#### 13.2.3 注意

若`pathname`是符号链接，则删除符号链接本身，而不是符号链接指向的文件

#### 13.2.4 应用：创建临时文件

`unlink`函数创建临时文件的原理：通过`open`创建文件，`unlink`删除目录项但保留inode节点，最后文件描述符关闭时释放资源

```C
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int fd;

    /* 创建一个新文件 */
    fd = open("tempfile", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        /* 错误处理 */
        exit(1);
    }

    /* 删除文件 */
    if (unlink("tempfile") == -1)
    {
        /* 错误处理 */
        exit(1);
    }

    /* 此时可以通过文件描述符fd来读写文件的内容 */

    /* 程序结束，打开的文件描述符会被自动关闭，相关资源也会被自动释放 */

    exit(0);
}
```

## 14. 符号链接 

### 14.1`symlink`函数

#### 14.1.1 功能

创建符号链接

#### 14.1.2 函数原型

```C
#include <unistd.h>

// 成功返回0，出错返回-1
int symlink(const char *actualpath, const char *sympath);
```

#### 14.1.3 注意

`actualpath`并不要求一定存在*（`ln -s`创建符号链接亦是如此）*

### 14.2 `readlink`函数

#### 14.2.1 功能

读取符号链接本身内容

#### 14.2.2 函数原型

```C
#include <unistd.h>

// 成功返回读到的字节数，出错返回-1
ssize_t readlink(const char *restrict pathname, char *restrict buf, size_t bufsize);
```

#### 14.2.3 注意

+ `readlink`函数组合了`open`、`read`和`close`的所有操作
+ **`buf`中返回的符号链接内容不以null字符终止**

## 15. 文件/目录操作

### 15.1 `mkdir`函数

#### 15.1.1 功能

创建目录

#### 15.1.2 函数原型

```C
#include <sys/stat.h>

// 成功返回0，出错返回-1
int mkdir(const char *pathname, mode_t mode);
```

### 15.2 `rename`函数

#### 15.2.1 功能

对文件或目录进行更名

#### 15.2.2 函数原型

```C
#include <stdio.h>

// 成功返回0，出错返回-1
int rename(const char *oldname, const char *newname);
```

#### 15.2.3 注意

+ `oldname`和`newname`引用同一文件，函数不做任何更改而成功返回
+ `oldname`和`newname`引用符号链接，则处理的是符号链接本身，而不是所引用的文件
+ `oldname`引用的是文件，则为文件更名
    + `newname`**不能引用一个目录**
    + 若`newname`已存在，则先将`newname`删除，后将`oldname`更名为`newname`
    + 对包含`oldname`的目录和`newname`的目录，进程必须具有写权限
+ `oldname`引用的是目录，则为目录更名
    + `newname`**必须引用一个目录（且应是空目录）**
    + 若`newname`已存在，则先将`newname`删除，后将`oldname`更名为`newname`
    + `newname`不能以`oldname`作为路径前缀。例如，不能将`/usr/foo`更名为`/usr/foo/testdir`

### 15.3 `rmdir`函数

#### 15.3.1 功能

删除空目录

#### 15.3.2 函数原型

```C
#include <unistd.h>

// 成功返回0，出错返回-1
int rmdir(const char pathname);
```

### 15.4 `remove`函数

#### 15.4.1 功能

+ 对于文件：`remove`与`unlink`功能相同
+ 对于目录：`remove`与`rmdir`功能相同

#### 15.4.2 函数原型

```C
#include <stdio.h>

// 成功返回0，出错返回-1
int remove(const char *pathname);
```

## 16. 读目录

### 16.1 结构体`dirent`

```C
#include <dirent.h>

struct dirent
{
    ino_t d_ino;               /* i-node number */
    char d_name[NAME_MAX + 1]; /* null-terminated filename */
};
```

*注：`NAME_MAX`依赖于文件系统*

### 16.2 `opendir`函数

#### 16.2.1 功能

打开目录

#### 16.2.2 函数原型

```C
#include <dirent.h>

// 成功返回指针，出错返回NULL
DIR *opendir(const char *pathname);
```

### 16.3 `readdir`函数

#### 16.3.1 功能

读取目录每一项

#### 16.3.2 函数原型

```C
#include <dirent.h>

// 成功返回指针，出错或目录结尾返回NULL
struct dirent *readdir(DIR *dp);
```

### 16.4 `telldir`函数

#### 16.4.1 功能

获取当前目录流读取位置

#### 16.4.2 函数原型

```C
#include <dirent.h>

long telldir(DIR *dp);
```

### 16.5 `rewinddir`函数

#### 16.5.1 功能

重置目录句柄，使得下次读取目录从头开始

#### 16.5.2 函数原型

```C
#include <dirent.h>

void rewinddir(DIR *dp);
```

### 16.6 `seekdir`函数

#### 16.6.1 功能

设置目录流读取位置

#### 16.6.2 函数原型

```C
#include <dirent.h>

void seekdir(DIR *dp, off_t offset);
```

### 16.7 `closedir`函数

#### 16.7.1 功能

关闭处于打开状态的目录

#### 16.7.2 函数原型

```C
#include <dirent.h>

// 成功返回0，出错返回-1
int closedir(DIR *dirp);
```

## 17. `chdir`、`fchdir`和`getcwd`函数

### 17.1 功能

+ `chdir`和`fchdir`用来改变进程当前工作目录
+ `getcwd`用来获取当前工作目录**绝对路径**

### 17.2 函数原型

```C
#include <unistd.h>

// 成功返回0，出错返回-1
int chdir(const char *pathname);
int fchdir(int fd);

// 成功返回buf，出错返回NULL
char *getcwd(char *buf, size_t size);
```

### 17.3 参数

`pathname`和`fd`用来指定新的当前工作目录

## 18. 文件的时间

### 18.1 与文件相关的三个时间值

|    字段    |               说明                | 可以修改此时间值的函数举例 | ls选项 |
| :--------: | :-------------------------------: | :------------------------: | :----: |
| `st_atime` | 文件数据的最后访问（access）时间  |           `read`           |   -u   |
| `st_mtime` | 文件数据的最后修改（modify）时间  |          `write`           |  默认  |
| `st_ctime` | i节点状态的最后更改（change）时间 |      `chmod`、`chown`      |   -c   |

*注意：系统不保存对一个i节点的最后一次访问时间，因此`access`和`stat`函数并不更改这三个时间中任一个*

### 18.2 `utime`函数

#### 18.2.1 功能

更改文件的访问时间（atime）和修改时间（mtime）

#### 18.2.2 函数原型

```C
#include <utime.h>

struct utimbuf
{
    time_t actime;  /* access time */
    time_t modtime; /* modification time */
};

// 成功返回0，出错返回-1
int utime(const char *pathname, const struct utimbuf *times);
```

#### 18.2.3 参数`times` 

+ `times`为空指针：atime和ctime均设置为当前时间。条件：进程的有效用户ID必须等于文件所有者ID；或者进程对文件具有写权限
+ `times`为非空指针：atime和ctime均设置为结构体中的时间值。条件：进程的有效用户ID必须等于文件所有者ID；或者进程是一个超级用户进程

#### 18.2.4 注意

不能更改ctime为一个指定值，当调用`utime`函数时，此字段将被自动更新

## 19. 设备特殊文件

+ `st_dev`表示文件所在的设备的ID
+ `st_rdev`是特殊文件（如字符特殊文件和块特殊文件）所代表的设备的设备号
