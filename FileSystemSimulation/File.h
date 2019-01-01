//
//  File.hpp
//  File System Simulation
//
//  Created by JIN ZHAO on 12/30/18.
//  Copyright © 2018 JIN ZHAO. All rights reserved.
//

#ifndef File_h
#define File_h

struct MFD {
    char userName[14]; //用户名 14B
    char userPwd[14];  //密码14B
    int link;  //该用户的UFD所在的物理块号(4B)
};

struct UFD{
    char fileName[14];  //文件名14B;
    char mode;  ///文件权限0-readonly;1-writeonly;2-read/write
    int length; ///文件长度(以字节数计算)
    int addr;//该文件的第1个文件块对应的物理块号
};

struct UOF{
    char fileName[14];
    char mode;
    int length;
    int addr;
    int readPtr;
    int writePtr;
};

struct Cluster{
    int nextBlock = -1;
    char content[512 - sizeof(int)];
};

void format();

void initFileSystem();

void closeFileSystem();
#endif
