#[forward (a shipment) ] (https://www.redhat.com/sysadmin/linux-file-permissions-explained)

#instances:  
操作文件或目录的用户，有3种不同类型：文件所有者、群组用户、其他用户  
* chmod  754  filename  
>r	4	00000100	read，读取。当前用户可以读取文件内容，当前用户可以浏览目录。
>w	2	00000010	write，写入。当前用户可以新增或修改文件内容，当前用户可以删除、移动目录或目录内文件。
>x	1	00000001 execute，当前用户可以执行文件，当前用户可以进入目录。

* chmod a+x test.bin
>a是指所有的用户组，包括root用户组，文件拥有者的用户组，还有其他用户组。
>+x是指添加执行权限。
>+x是执行权限，+r是阅读权限，+w是写入权限
          
    fgbhdfs