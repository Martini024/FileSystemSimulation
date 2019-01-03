//
//  Command.cpp
//  File System Simulation
//
//  Created by JIN ZHAO on 12/30/18.
//  Copyright © 2018 JIN ZHAO. All rights reserved.
//

#include "Common.h"
#include "File.h"
#include "Unix.h"
#include "Block.h"

extern vector<MFD> UserList;
extern vector< vector<UFD> > FileList;
extern vector< vector<UOF> > StateList;
extern vector< Cluster> ClusterList;
extern vector<int> FreeBlockList;
extern MFD UserInput;
extern UFD FileInput;
extern UOF StateInput;
extern Cluster ClusterInput;
extern string currentUserName;
extern int currentUserId;
extern char* buffer;
extern Command cmd;

void do_Chmod() {
    if (currentUserId == -1) {
        cout << "Cannot change mode before login." << endl;
        return;
    }
    if (strcmp(cmd.cmdItem[1].c_str(), "") == 0 || strcmp(cmd.cmdItem[2].c_str(), "") == 0) {
        cout << "File name and new mode are required." << endl;
        return;
    }
    if (cmd.cmdItem[1].length() >= 14 || cmd.cmdItem[2].length() >= 14) {
        cout << "File name and new mode should be less than 14 letters." << endl;
        return;
    }
    if (strcmp(cmd.cmdItem[2].c_str(), "0") != 0 && strcmp(cmd.cmdItem[2].c_str(), "1") != 0 && strcmp(cmd.cmdItem[2].c_str(), "2") != 0) {
        cout << "Mode should be within 0, 1, 2 (read_only, write_only, read_and_write)." << endl;
        return;
    }
    
    int flag = 0;
    for (int i = 0; i < FileList[currentUserId].size(); i++) {
        if (strcmp(FileList[currentUserId][i].fileName, cmd.cmdItem[1].c_str()) == 0) {
            flag = 1;
            
            FileList[currentUserId][i].mode = atoi(cmd.cmdItem[2].c_str());
            writeBlock(currentUserId + 1);
            
            StateList[currentUserId][i].mode = atoi(cmd.cmdItem[2].c_str());
            writeBlock(currentUserId + 17);
            break;
        }
    }
    if (flag) {
        cout << "Change file mode successfully !!!" << endl;
    }
    else {
        cout << "Cannot find the file to change mode." << endl;
    }
}

void do_Chown() {
    if (currentUserId == -1) {
        cout << "Cannot change owner before login." << endl;
        return;
    }
    
    if (strcmp(cmd.cmdItem[1].c_str(), "") == 0 || strcmp(cmd.cmdItem[2].c_str(), "") == 0) {
        cout << "File name and new owner are required." << endl;
        return;
    }
    if (cmd.cmdItem[1].length() >= 14 || cmd.cmdItem[2].length() >= 14) {
        cout << "File name and new owner should be less than 14 letters." << endl;
        return;
    }
    //MARK: check if new user's file number is full
    int flag = 0;
    int newOwnerId = -1;
    for (int i = 0; i < UserList.size(); i++) {
        if (strcmp(cmd.cmdItem[2].c_str(), UserList[i].userName) == 0 && FileList[i].size() < 16) {
            flag = 1;
            newOwnerId = i;
            break;
        }
    }
    
    if (flag)
    {
        flag = 0;
        for (int i = 0; i < FileList[currentUserId].size(); i++) {
            if (strcmp(FileList[currentUserId][i].fileName, cmd.cmdItem[1].c_str()) == 0) {
                flag = 1;
                //MARK: 一边push 一边erase插空 FileList
                FileInput = FileList[currentUserId][i];
                FileList[newOwnerId].push_back(FileInput);
                writeBlock(newOwnerId + 1);
                
                strcpy(FileInput.fileName, "\0");
                FileList[currentUserId].erase(FileList[currentUserId].begin() + i);
                FileList[currentUserId].push_back(FileInput);
                writeBlock(currentUserId + 1);
                
                //MARK: 再来一遍 StateList
                StateInput = StateList[currentUserId][i];
                StateList[newOwnerId].push_back(StateInput);
                writeBlock(newOwnerId + 17);
                
                strcpy(StateInput.fileName, "\0");
                StateList[currentUserId].erase(StateList[currentUserId].begin() + i);
                StateList[currentUserId].push_back(StateInput);
                writeBlock(currentUserId + 17);
                break;
            }
        }
        if (flag) {
            cout << "Change file owner successfully !!!" << endl;
            return;
        }
        else {
            cout << "Cannot find the file to change owner." << endl;
            return;
        }
    }
    else
    {
        cout << "New owner's file number reaches maximum or cannot find new owner." << endl;
        return;
    }
    
    
    
}

void do_Mv() {
    int flag = 0;
    int fileId = -1;
    for (int i = 0; i < FileList[currentUserId].size(); i++) {
        if (strcmp(FileList[currentUserId][i].fileName, cmd.cmdItem[1].c_str()) == 0) {
            flag = 1;
            fileId = i;
            break;
        }
    }
    
    if (flag) {
        flag = 1;
        for (int i = 0; i < FileList[currentUserId].size(); i++) {
            if (strcmp(FileList[currentUserId][i].fileName, cmd.cmdItem[2].c_str()) == 0) {
                flag = 0;
                break;
            }
        }
        
        if (flag) {
            strcpy(FileList[currentUserId][fileId].fileName, cmd.cmdItem[2].c_str());
            writeBlock(currentUserId + 1);
            
            strcpy(StateList[currentUserId][fileId].fileName, cmd.cmdItem[2].c_str());
            writeBlock(currentUserId + 17);
            
            cout << "Change file name successfully !!!" << endl;
            return;
        }
        else {
            cout << "New file name has already existed." << endl;
            return;
        }
    }
    else {
        cout << "Cannot find the file to change file name." << endl;
        return;
    }
}

void do_Copy() {
    //Mv srcFile desFile
    int flag = 0;
    int firstFileId = -1;
    int secondFileId = -1;
    for (int i = 0; i < FileList[currentUserId].size(); i++) {
        if (strcmp(FileList[currentUserId][i].fileName, cmd.cmdItem[1].c_str()) == 0) {
            flag = 1;
            firstFileId = i;
            break;
        }
    }
    
    if (flag) {
        flag = 0;
        for (int i = 0; i < FileList[currentUserId].size(); i++) {
            if (strcmp(FileList[currentUserId][i].fileName, cmd.cmdItem[2].c_str()) == 0) {
                flag = 1;
                secondFileId = i;
                break;
            }
        }
        
        if (flag) {
            int firstBlockNum = FileList[currentUserId][firstFileId].length % (512 - sizeof(int));
            int secondBlockNum = FileList[currentUserId][secondFileId].length % (512 - sizeof(int));
            if (firstBlockNum > secondBlockNum) {
                if (FreeBlockList.size() < firstBlockNum - secondBlockNum) {
                    cout << "Cannot copy cause there's no free space." << endl;
                    return;
                }
                //开始cpoy
                int firstIterator = firstFileId;
                int secondIterator = secondFileId;
                int allocatedBlock =  -1;
                while (ClusterList[firstIterator].nextBlock != firstIterator + 33) {
                    if (ClusterList[secondIterator].nextBlock != secondIterator + 33) {
                        strcpy(ClusterList[secondIterator].content, ClusterList[firstIterator].content);
                        writeBlock(secondIterator + 33);
                        secondIterator = ClusterList[secondIterator].nextBlock;
                        firstIterator = ClusterList[firstIterator].nextBlock;
                    }
                    else {
                        strcpy(ClusterList[secondIterator].content, ClusterList[firstIterator].content);
                        firstIterator = ClusterList[firstIterator].nextBlock;
                        
                        allocatedBlock = FreeBlockList[0];
                        FreeBlockList.erase(FreeBlockList.begin());
                        ClusterList[secondIterator].nextBlock = allocatedBlock;
                        ClusterList[allocatedBlock - 33].nextBlock = allocatedBlock;
                        writeBlock(secondIterator + 33);
                        secondIterator = allocatedBlock - 33;
                    }
                }
                strcpy(ClusterList[secondIterator].content, ClusterList[firstIterator].content);
                writeBlock(secondIterator + 33);
            }
            else {
                int firstIterator = firstFileId;
                int secondIterator = secondFileId;
                int recycleIterator =  -1;
                while (ClusterList[firstIterator].nextBlock != firstIterator + 33) {
                    strcpy(ClusterList[secondIterator].content, ClusterList[firstIterator].content);
                    writeBlock(secondIterator + 33);
                    secondIterator = ClusterList[secondIterator].nextBlock;
                    firstIterator = ClusterList[firstIterator].nextBlock;
                }
                strcpy(ClusterList[secondIterator].content, ClusterList[firstIterator].content);
                recycleIterator = ClusterList[secondIterator].nextBlock - 33;
                ClusterList[secondIterator].nextBlock = secondIterator + 33;
                writeBlock(secondIterator + 33);
                
                if (firstBlockNum != secondBlockNum) {
                    while (ClusterList[recycleIterator].nextBlock != recycleIterator + 33) {
                        FreeBlockList.push_back(recycleIterator + 33);
                        secondIterator = recycleIterator;
                        recycleIterator = ClusterList[recycleIterator].nextBlock - 33;
                        ClusterList[secondIterator].nextBlock = -1;
                        writeBlock(secondIterator + 33);
                    }
                    FreeBlockList.push_back(recycleIterator + 33);
                    sort(FreeBlockList.begin(), FreeBlockList.end());
                    ClusterList[recycleIterator].nextBlock = -1;
                    writeBlock(recycleIterator + 33);
                }
            }
            
            cout << "Copy file successfully !!!" << endl;
            return;
        }
        else {
            cout << "Cannot find the destination file." << endl;
            return;
        }
    }
    else {
        cout << "Cannot find the source file." << endl;
        return;
    }
}


//void do_Dir() {
////    for (int i = 0; i < FileInfo[curID].size(); i++)
////    {
////        cout << FileInfo[curID][i].filename << "  ";
////    }
////    cout << endl;
//
//}
void do_Read() {
    if (currentUserId == -1) {
        cout << "Cannot show file content before login." << endl;
        return;
    }
    if (strcmp(cmd.cmdItem[1].c_str(), "") == 0) {
        cout << "File name is required." << endl;
        return;
    }
    if (cmd.cmdItem[1].length() >= 14) {
        cout << "File name should be less than 14 letters." << endl;
        return;
    }
    
    int flag = 0;
    int address = -1;
    for (int i = 0; i < FileList[currentUserId].size(); i++) {
        if (strcmp(FileList[currentUserId][i].fileName, cmd.cmdItem[1].c_str()) == 0) {
            flag = 1;
            address = FileList[currentUserId][i].addr;
            break;
        }
    }
    
    if (flag) {
        
        while (ClusterList[address - 33].nextBlock != address) {
            cout << ClusterList[address - 33].content;
            address = ClusterList[address - 33].nextBlock;
        }
        cout << ClusterList[address - 33].content << endl;
        cout << "Show file content successfully !!!" << endl;
        return;
    }
    else {
        cout << "Cannot find the file to show content." << endl;
        return;
    }
}

void do_Passwd() {
    if (strcmp(cmd.cmdItem[1].c_str(), "") == 0 || strcmp(cmd.cmdItem[2].c_str(), "") == 0) {
        cout << "Current and new password are required." << endl;
        return;
    }
    if (cmd.cmdItem[2].length() >= 14) {
        cout << "Password should be less than 14 letters." << endl;
        return;
    }
    
    if (strcmp(UserList[currentUserId].userPwd, cmd.cmdItem[1].c_str()) == 0) {
        strcpy(UserList[currentUserId].userPwd, cmd.cmdItem[2].c_str());
        writeBlock(0);
        cout << "Change password successful !!!" << endl;
    }
    else
        cout << "Current password is wrong." << endl;
}


void do_Login() {
    if (currentUserId != -1) {
        cout << "Cannot login before current user logout." << endl;
        return;
    }
    
    if (strcmp(cmd.cmdItem[1].c_str(), "") == 0 || strcmp(cmd.cmdItem[2].c_str(), "") == 0) {
        cout << "User name and password are required." << endl;
        return;
    }
    if (cmd.cmdItem[1].length() >= 14 || cmd.cmdItem[2].length() >= 14) {
        cout << "User name and password should be less than 14 letters." << endl;
        return;
    }
    
    int flag = 0;
    for (int i = 0; i < UserList.size(); i++) {
        if (strcmp(cmd.cmdItem[1].c_str(), UserList[i].userName) == 0 && strcmp(cmd.cmdItem[2].c_str(), UserList[i].userPwd) == 0) {
            currentUserId = i;
            currentUserName = cmd.cmdItem[1];
            flag = 1;
            break;
        }
    }
    if (flag)
    {
        cout << "Login successfully !!!" << endl;
        return;
    }
    else
    {
        cout << "User name or password is wrong, or you can use register command to create one." << endl;
    }
}

void do_Exit() {
    exit(0);
}

void do_Logout() {
    if (currentUserId == -1) {
        cout << "Bro you do not need to logout without login." << endl;
        return;
    }
    else {
        currentUserId = -1;
        currentUserName = "";
        cout << "Logout successfully !!! " << endl;
    }
}

void do_Register() {
    if (currentUserId != -1) {
        cout << "Cannot register before current user logout." << endl;
        return;
    }
    
    if (strcmp(cmd.cmdItem[1].c_str(), "") == 0 || strcmp(cmd.cmdItem[2].c_str(), "") == 0) {
        cout << "User name and password are required." << endl;
        return;
    }
    if (cmd.cmdItem[1].length() >= 14 || cmd.cmdItem[2].length() >= 14) {
        cout << "User name and password should be less than 14 letters." << endl;
        return;
    }
    
    if (UserList.size() < 16)
    {
        int flag = 1;
        for (int i = 0; i < UserList.size(); i++) {
            if (strcmp(cmd.cmdItem[1].c_str(), UserList[i].userName) == 0) {
                flag = 0;
                break;
            }
        }
        if (flag)
        {
            strcpy(UserInput.userName, cmd.cmdItem[1].c_str());
            strcpy(UserInput.userPwd, cmd.cmdItem[2].c_str());
            UserInput.link = (int)UserList.size();
            UserList.push_back(UserInput);
            
            currentUserId = UserInput.link;
            currentUserName = UserInput.userName;
            
            vector<UFD> tmpUFD;
            FileList.push_back(tmpUFD);
            
            vector<UOF> tmpUOF;
            StateList.push_back(tmpUOF);
            
            writeBlock(0);
            //                writeBlock((int)UserList.size());
            //                writeBlock((int)UserList.size() + 16);
            
            cout << "Create account successfully and already login !!!" << endl;
            
        }
        else
        {
            cout << "User name has already existed, please try again." << endl;
        }
    }
    else
        cout << "Cannot register cause the total account number is full." << endl;
}

void do_Create() {
    if (currentUserId == -1) {
        cout << "Cannot create file before login." << endl;
        return;
    }
    if (strcmp(cmd.cmdItem[1].c_str(), "") == 0 || strcmp(cmd.cmdItem[2].c_str(), "") == 0) {
        cout << "File name and mode are required." << endl;
        return;
    }
    if (cmd.cmdItem[1].length() >= 14) {
        cout << "File name should be less than 14 letters." << endl;
        return;
    }
    if (strcmp(cmd.cmdItem[2].c_str(), "0") != 0 && strcmp(cmd.cmdItem[2].c_str(), "1") != 0 && strcmp(cmd.cmdItem[2].c_str(), "2") != 0) {
        cout << "Mode should be within 0, 1, 2 (read_only, write_only, read_and_write)." << endl;
        return;
    }
    
    if (FileList[currentUserId].size() < 16) {
        int flag = 1;
        for (int i = 0; i < FileList[currentUserId].size(); i++) {
            if (strcmp(FileList[currentUserId][i].fileName, cmd.cmdItem[1].c_str()) == 0) {
                flag = 0;
                break;
            }
        }
        if (!flag) {
            cout << "Cannot create file with the same name." << endl;
            return;
        }

        strcpy(FileInput.fileName, cmd.cmdItem[1].c_str());
        FileInput.mode = atoi(cmd.cmdItem[2].c_str());
        FileInput.length = 0;

        if (FreeBlockList.size() > 0) {
            FileInput.addr = FreeBlockList[0];
            FreeBlockList.erase(FreeBlockList.begin());
            FileList[currentUserId].push_back(FileInput);
            writeBlock(currentUserId + 1);
            
            ClusterList[FileInput.addr - 33].nextBlock = FileInput.addr;
            writeBlock(FileInput.addr);
            
            strcpy(StateInput.fileName, FileInput.fileName);
            StateInput.mode = FileInput.mode;
            StateInput.length = FileInput.length;
            StateInput.addr = FileInput.addr;
            StateInput.readPtr = 0;
            StateInput.writePtr = 0;
            StateList[currentUserId].push_back(StateInput);
            writeBlock(currentUserId + 17);
            
            cout << "Create file successfully !!!" << endl;
        }
        else {
            cout << "No free block is available for creating file." << endl;
        }

    }
    else {
        cout << "The number of file reaches the maximum." << endl;
    }
}

void do_Delete() {
    if (currentUserId == -1) {
        cout << "Cannot delete file before login." << endl;
        return;
    }
    
    if (strcmp(cmd.cmdItem[1].c_str(), "") == 0) {
        cout << "File name is required." << endl;
        return;
    }
    if (cmd.cmdItem[1].length() >= 14) {
        cout << "File name should be less than 14 letters." << endl;
        return;
    }

    int flag = 0;
    for (int i = 0; i < FileList[currentUserId].size(); i++) {
        if (strcmp(FileList[currentUserId][i].fileName, cmd.cmdItem[1].c_str()) == 0) {
            flag = 1;
            FileInput = FileList[currentUserId][i];
            FreeBlockList.push_back(FileInput.addr);
            sort(FreeBlockList.begin(), FreeBlockList.end());
            
            ClusterList[FileInput.addr - 33].nextBlock = -1;
            writeBlock(FileInput.addr);
            
            strcpy(FileInput.fileName, "\0");
            FileList[currentUserId].erase(FileList[currentUserId].begin() + i);
            FileList[currentUserId].push_back(FileInput);
            writeBlock(currentUserId + 1);
            
            strcpy(StateInput.fileName, "\0");
            StateList[currentUserId].erase(StateList[currentUserId].begin() + i);
            StateList[currentUserId].push_back(StateInput);
            writeBlock(currentUserId + 17);
            break;
        }
    }
    if (flag) {
        cout << "Delete file successfully !!!" << endl;
    }
    else {
        cout << "Cannot find the file to be deleted." << endl;
    }
}



/*int num;
 int next_num;
 int is_data;
 char data[256];*/

void do_Write()
{
//    //Write    filename buffer nbytes 写文件   物理空间68
//
//    char buf[1024];
//    stringstream ss;
//    ss << cmd_in.cmd_num[3];
//    int temp;
//    ss >> temp;
//
//
//    for (int i = 0; i < FileInfo[curID].size(); i++)
//    {
//        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            if (FileInfo[curID][i].mode == 1 || FileInfo[curID][i].mode == 2)//判断权限
//            {
//                break;
//            }
//            else
//            {
//                cout << "没有写的权限!" << endl;
//                return;
//            }
//        }
//    }
//
//    int index;
//    for (int i = 0; i < FileState[curID].size(); i++)
//    {
//        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            index = i;
//            break;
//        }
//    }
//    //起始物理块
//    int address;
//    for (int i = 0; i < FileInfo[curID].size(); i++)
//    {
//        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            address = FileInfo[curID][i].addr;
//            break;
//        }
//    }
//    //注意：此处发生了更改！
//    cout << "请输入buff的内容：" << endl;
//    gets(buf);
//    fflush(stdin);
//
//    //strcpy(buf, cmd_in.cmd_num[2].c_str());
//
//    int wbegin;
//    wbegin = FileState[curID][index].write_poit;
//
//    //找到写指针所在的最后一个磁盘
//    while (FileCluster[address].next_num != address)
//        address = FileCluster[address].next_num;
//
//    vector <int> newspace_num;//计算将要占用的物理块的数量
//    newspace_num.clear();
//
//    //int num = (256-wbegin+temp) / 256-1;
//    if (temp <= 256 - wbegin)
//        num = 0;
//    else
//    {
//        num = ceil((temp - (256 - wbegin))*1.0 / 256);
//    }
//
//    newspace_num.push_back(address);
//
//    //cout << newspace_num.size() << endl;//
//
//    for (int i = 0; i < FileCluster.size(); i++)
//    {
//        if (newspace_num.size() == num+1)
//            break;
//        if (FileCluster[i].is_data == 0)
//        {
//            newspace_num.push_back(i);
//            FileCluster[i].is_data = 1;
//        }
//    }
//
//    for (int k = 0; k < newspace_num.size() - 1; k++)
//    {
//        FileCluster[newspace_num[k]].next_num = newspace_num[k + 1];
//    }
//    for (int i = 0; i < temp; i++)
//    {
//        if (wbegin == 256)
//        {
//            wbegin = 0;
//            address = FileCluster[address].next_num;
//        }
//        FileCluster[address].data[wbegin] = buf[i];
//        wbegin++;
//    }
//
//    //更新写指针
//    FileState[curID][index].write_poit = wbegin;
//    cout << "磁盘写入成功!" << endl;
//    return;
    
}

//write_poit记录最终磁盘的写指针，read_poit记录全局总数的读指针的位置。

void do_Help()
{
//    cout << "Login    userName pwd    用户登陆" << endl;
//    cout << "Logout    用户登出" << endl;
//    cout << "Register usrName pwd   用户注册" << endl;
//    cout << "Passwd    oldPwd  newPwd    修改用户口令" << endl;
//    cout << "Open   filename mode   打开文件" << endl;
//    cout << "Close  filename   关闭文件" << endl;
//    cout << "Create     filename mode      建立文件" << endl;
//    cout << "Delete     filename   删除文件" << endl;
//    cout << "Write    filename buffer nbytes   写文件" << endl;
//    cout << "Read     filename buffer nbytes   读文件" << endl;
//    cout << "dir   列出该用户下所有文件" << endl;
//    cout << "Chmod    filename mode       改变文件权限" << endl;
//    cout << "Chown    filename new_owner    改变文件拥有者" << endl;
//    cout << "Mv    srcFile desFile       改变文件名" << endl;
//    cout << "Copy   srcFile desFile   文件拷贝" << endl;
//    cout << "Type    filename     显示文件内容" << endl;
//    cout << "Exit   退出程序" << endl;
//    cout << "sysc   同步到磁盘 " << endl;
}

void do_Ls() {
    
}

void do_Clear()
{
    system("cls");
}
