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
    char userName[14];
    char userPwd[14];
    int link;
};

struct UFD{
    char fileName[14];
    char mode;
    int length;
    int addr;
};

struct Cluster{
    int nextBlock = -1;
    char content[512 - sizeof(int)];
};

void initFileSystem();

#endif
