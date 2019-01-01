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
//    int temp;
//    //Chmod    filename mode
//    for (int i = 0; i < FileInfo[curID].size(); i++)
//    {
//        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            stringstream ss;
//            ss << cmd_in.cmd_num[2];
//            ss >> temp;
//            FileInfo[curID][i].mode = temp;
//            break;
//        }
//    }
//    for (int i = 0; i < FileState[curID].size(); i++)
//    {
//        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            FileState[curID][i].mode = temp;
//            break;
//        }
//    }
//    cout << "修改权限成功!"<<endl;
}
void do_Chown() {
    // Chown filename new_owner
//    vector<UFD>::iterator it;
//    for (it = FileInfo[curID].begin(); it != FileInfo[curID].end(); it++)
//    {
//        if (strcmp((*it).filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            strcpy(FileInput.filename,(*it).filename);
//            FileInput.length = (*it).length;
//            FileInput.mode = (*it).mode;
//            FileInput.addr = (*it).addr;
//
//            FileInfo[curID].erase(it);
//            break;
//        }
//    }
//
//    //删除状态栏
//    vector<UOF>::iterator it_num1;
//    for (it_num1 = FileState[curID].begin(); it_num1 != FileState[curID].end(); it_num1++)
//    {
//        if (strcmp((*it_num1).filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            strcpy(StateInput.filename, (*it_num1).filename);
//            StateInput.mode = (*it_num1).mode;
//            StateInput.state = (*it_num1).state;
//            StateInput.read_poit = (*it_num1).read_poit;
//            StateInput.write_poit = (*it_num1).write_poit;
//
//            FileState[curID].erase(it_num1);
//            break;
//        }
//    }
//
//    //添加至新用户的文件栏和状态栏
//    int tempID;
//    for (int i = 0; i < UsrInfo.size(); i++)
//    {
//        if (strcmp(UsrInfo[i].usrname, cmd_in.cmd_num[2].c_str()) == 0)
//        {
//            tempID = i;
//            break;
//        }
//    }
//    int flag = 0;
//    //是否存在同名文件，是否替换等
//    for (int i = 0; i < FileInfo[tempID].size(); i++)
//    {
//        if (strcmp(FileInfo[tempID][i].filename, cmd_in.cmd_num[2].c_str()) == 0)
//        {
//            flag = 1;
//            break;
//        }
//    }
//    if (!flag)
//    {
//        FileInfo[tempID].push_back(FileInput);
//        FileState[tempID].push_back(StateInput);
//        cout << "改变文件拥有者成功！" << endl;
//    }
//    else
//    {
//        cout << "此用户中已经拥有本名称的文件！" << endl;
//    }
}

void do_Mv() {
//    //Mv srcFile desFile
//    for (int i = 0; i < FileInfo[curID].size(); i++)
//    {
//        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            strcpy(FileInfo[curID][i].filename, cmd_in.cmd_num[2].c_str());
//            break;
//        }
//    }
//
//    for (int i = 0; i < FileState[curID].size(); i++)
//    {
//        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            strcpy(FileState[curID][i].filename, cmd_in.cmd_num[2].c_str());
//            break;
//        }
//    }
//    cout << "重命名文件成功" << endl;
}
//void do_Write();
void doTempWrite();
void make_new_copy() {
//    //Type filename
//    string Tempbuff;
//    int addre;
//    for (int i = 0; i < FileInfo[curID].size(); i++)
//    {
//        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            addre = FileInfo[curID][i].addr;
//            break;
//        }
//    }
//    int index;
//    for (int i = 0; i < FileState[curID].size(); i++)
//    {
//        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            index = i;
//            break;
//        }
//    }
//
//    while (1)
//    {
//        if (FileCluster[addre].next_num == addre)
//        {
//            for (int i = 0; i < FileState[curID][index].write_poit; i++)
//            {
//                //cout << FileCluster[addre].data[i];
//                Tempbuff += FileCluster[addre].data[i];
//            }
//            break;
//        }
//        else
//        {
//            for (int i = 0; i < 256; i++)
//            {
//                //cout << FileCluster[addre].data[i];
//                Tempbuff += FileCluster[addre].data[i];
//            }
//            if (FileCluster[addre].next_num != addre)
//            {
//                addre = FileCluster[addre].next_num;
//            }
//            else
//                break;
//        }
//    }
//
//    //Write    filename buffer nbytes 写文件   物理空间68
//    cmd_in.cmd_num[0] = "Write";
//    cmd_in.cmd_num[1] = cmd_in.cmd_num[2];
//    cmd_in.cmd_num[2] = Tempbuff;
//    stringstream ss;
//    ss << Tempbuff.length();
//    string curtp;
//    ss >> curtp;
//    cmd_in.cmd_num[3] = curtp;
//    //cout << cmd_in.cmd_num[0] << " " << cmd_in.cmd_num[1] << "  " << cmd_in.cmd_num[2] << " " << cmd_in.cmd_num[3] << endl;
//    //do_Write();
//    doTempWrite();
    
}
void do_Create();
void do_Copy() {
//    //Copy  srcFile desFile
//    int address;
//    for (int i = 0; i < FileInfo[curID].size(); i++)
//    {
//        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            address = FileInfo[curID][i].addr;
//            break;
//        }
//    }
//
//    int flag = 0;
//    for (int i = 0; i < FileInfo[curID].size(); i++)
//    {
//        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[2].c_str()) == 0)
//        {
//            flag = 1;
//            break;
//        }
//    }
//    string s1, s2, s3;
//    s1 = cmd_in.cmd_num[0];
//    s2 = cmd_in.cmd_num[1];
//    s3 = cmd_in.cmd_num[2];
//    //新的文件名存在....
//    if (flag)
//    {
//        make_new_copy();
//    }
//    else   //新的文件名不存在的话要新建一个
//    {
//        //Create name mode
//        cmd_in.cmd_num[0]="Create";
//        cmd_in.cmd_num[1]=cmd_in.cmd_num[2];
//        cmd_in.cmd_num[2]="2";
//        do_Create();
//        cmd_in.cmd_num[0] = s1;
//        cmd_in.cmd_num[1] = s2;
//        cmd_in.cmd_num[2] = s3;
//        make_new_copy();
//
//    }
    
}
void do_Dir() {
//    for (int i = 0; i < FileInfo[curID].size(); i++)
//    {
//        cout << FileInfo[curID][i].filename << "  ";
//    }
//    cout << endl;
    
}
void do_Type() {
//    //Type filename
//    int address;
//    for (int i = 0; i < FileInfo[curID].size(); i++)
//    {
//        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            address = FileInfo[curID][i].addr;
//            break;
//        }
//    }
//    int index;
//    for (int i = 0; i < FileState[curID].size(); i++)
//    {
//        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            index = i;
//            break;
//        }
//    }
//
//    while (1)
//    {
//        if (FileCluster[address].next_num == address)
//        {
//            for (int i = 0; i < FileState[curID][index].write_poit; i++)
//                cout << FileCluster[address].data[i];
//            break;
//        }
//        else
//        {
//            for (int i = 0; i < 256; i++)
//            {
//                cout << FileCluster[address].data[i];
//            }
//            if (FileCluster[address].next_num != address)
//            {
//                address = FileCluster[address].next_num;
//            }
//            else
//                break;
//        }
//    }
//    cout << endl;
}

void do_Passwd() {
    if (strcmp(cmd.cmdItem[1].c_str(), "") == 0 || strcmp(cmd.cmdItem[2].c_str(), "") == 0) {
        cout << "Current and new password is required." << endl;
        return;
    }
    else if (cmd.cmdItem[2].length() >= 14) {
        cout << "Password should be less than 14 letters." << endl;
        return;
    }
    
    if (strcmp(UserList[currentUserId].userPwd, cmd.cmdItem[1].c_str()) == 0)
    {
        strcpy(UserList[currentUserId].userPwd, cmd.cmdItem[2].c_str());
        writeBlock(0);
        cout << "Change password successful !!!" << endl;
    }
    else
        cout << "Current password is wrong." << endl;
}


void do_Login() {
    if (strcmp(cmd.cmdItem[1].c_str(), "") == 0 || strcmp(cmd.cmdItem[2].c_str(), "") == 0) {
        cout << "User name and password is required." << endl;
        return;
    }
    else if (cmd.cmdItem[1].length() >= 14 || cmd.cmdItem[2].length() >= 14) {
        cout << "User name and password should be less than 14 letters." << endl;
        return;
    }
    
    if (currentUserId != -1) {
        cout << "Cannot login before current user logout." << endl;
        return;
    }
    else {
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
}

void do_Exit() {
    exit(0);
}

void do_Logout() {
    if (currentUserId == -1) {
        cout << "Bro you don not need to logout without login." << endl;
        return;
    }
    else {
        currentUserId = -1;
        currentUserName = "";
        cout << "Logout successfully !!! " << endl;
    }
}

void do_Register() {
    if (strcmp(cmd.cmdItem[1].c_str(), "") == 0 || strcmp(cmd.cmdItem[2].c_str(), "") == 0) {
        cout << "User name and password is required." << endl;
        return;
    }
    else if (cmd.cmdItem[1].length() >= 14 || cmd.cmdItem[2].length() >= 14) {
        cout << "User name and password should be less than 14 letters." << endl;
        return;
    }
    
    if (currentUserId != -1) {
        cout << "Cannot register before current user logout." << endl;
        return;
    }
    else {
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
}

void do_Create() {
    if (strcmp(cmd.cmdItem[1].c_str(), "") == 0 || strcmp(cmd.cmdItem[2].c_str(), "") == 0) {
        cout << "File name and mode is required." << endl;
        return;
    }
    else if (cmd.cmdItem[1].length() >= 14) {
        cout << "File name should be less than 14 letters." << endl;
        return;
    }
    else if (strcmp(cmd.cmdItem[2].c_str(), "0") != 0 && strcmp(cmd.cmdItem[2].c_str(), "1") != 0 && strcmp(cmd.cmdItem[2].c_str(), "2") != 0) {
        cout << "Mode should be within 0, 1, 2 (read_only, write_only, read_and_write)." << endl;
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
    if (strcmp(cmd.cmdItem[1].c_str(), "") == 0) {
        cout << "File name is required." << endl;
        return;
    }
    else if (cmd.cmdItem[1].length() >= 14) {
        cout << "File name should be less than 14 letters." << endl;
        return;
    }

    int flag = 0;
    for (int i = 0; i < FileList[currentUserId].size(); i++) {
        if (strcmp(FileList[currentUserId][i].fileName, cmd.cmdItem[1].c_str()) == 0) {
            flag = 1;
            FileInput = FileList[currentUserId][i];
            FileList[currentUserId].erase(FileList[currentUserId].begin() + i);
            FreeBlockList.push_back(FileInput.addr);
            sort(FreeBlockList.begin(), FreeBlockList.end());
            writeBlock(currentUserId + 1);
            
            ClusterList[FileInput.addr - 33].nextBlock = -1;
            writeBlock(FileInput.addr);
            
            StateList[currentUserId].erase(StateList[currentUserId].begin() + i);
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

void do_Open()
{
//    //Open  filename mode
//    stringstream ss;
//    ss << cmd_in.cmd_num[2];
//    int temp;
//    ss >> temp;
//    //判断是否有权限,还没做呢......
//    int flag = 0;
//    for (int i = 0; i < FileState[curID].size(); i++)
//    {
//        if (strcmp(FileState[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            FileState[curID][i].state = 1;
//            flag = 1;
//            break;
//        }
//    }
//    if (flag)
//        cout << "打开文件成功!" << endl;
//    else
//    {
//        strcpy(StateInput.filename, cmd_in.cmd_num[1].c_str());
//        StateInput.mode = temp;
//        StateInput.state = 1;
//        StateInput.read_poit = StateInput.write_poit = 0;
//        FileState[curID].push_back(StateInput);
//        cout << "打开文件成功！" << endl;
//    }
    
}
void do_Close()
{
//    //close filename
//    vector<UOF>::iterator it;
//    for (it = FileState[curID].begin(); it != FileState[curID].end(); it++)
//    {
//        if (strcmp((*it).filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            FileState[curID].erase(it);
//            cout << "关闭文件成功！" << endl;
//            break;
//        }
//    }
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

void do_Read()
{
//    //Read     filename buffer nbytes
//    char buf[1024];
//    stringstream ss;
//    ss << cmd_in.cmd_num[3];
//    int temp;
//    ss >> temp;
//
//    for (int i = 0; i < FileInfo[curID].size(); i++)
//    {
//        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            if (FileInfo[curID][i].mode == 0 || FileInfo[curID][i].mode == 2)//判断权限
//            {
//                break;
//            }
//            else
//            {
//                cout << "没有读的权限!" << endl;
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
//
//    int address;
//    for (int i = 0; i < FileInfo[curID].size(); i++)
//    {
//        if (strcmp(FileInfo[curID][i].filename, cmd_in.cmd_num[1].c_str()) == 0)
//        {
//            address = FileInfo[curID][i].addr;
//            break;
//        }
//    }
//
//    int rbegin;
//    rbegin = FileState[curID][index].read_poit;
//    int addcur = rbegin / 256;
//
//    for (int i = 0; i < addcur; i++)
//    {
//        address = FileCluster[address].next_num;
//    }
//
//    for (int i = 0; i < temp; i++)
//    {
//        if (rbegin % 256 == 255)
//        {
//            address = FileCluster[address].next_num;
//        }
//        buf[i] = FileCluster[address].data[rbegin % 256];
//        rbegin++;
//    }
//
//    FileState[curID][index].read_poit = rbegin;
//
//    cout << "读出的数据是：" << endl;
//    for (int i = 0; i < temp; i++)
//        cout << buf[i];
//    cout << endl;
    
}
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

void doTempWrite()
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
//    /*cout << "请输入buff的内容：" << endl;
//     gets(buf);
//     fflush(stdin);*/
//
//
//    strcpy(buf, cmd_in.cmd_num[2].c_str());
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
//        if (newspace_num.size() == num + 1)
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

void do_Clear()
{
    system("cls");
}
