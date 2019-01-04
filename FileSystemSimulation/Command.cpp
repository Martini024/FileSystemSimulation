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
#include "Stream.h"

extern vector<MFD> UserList;
extern vector< vector<UFD> > FileList;
extern vector< Cluster> ClusterList;
extern vector<int> FreeBlockList;
extern MFD UserInput;
extern UFD FileInput;
extern Cluster ClusterInput;
extern string currentUserName;
extern int currentUserId;
extern int currentUserUFD;
extern char* buffer;
extern Command cmd;

//MARK: User-related command
void do_Login() {
    //MARK: Input parameter validation
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

    //MARK: Record current user link and name
    int flag = 0;
    for (int i = 0; i < UserList.size(); i++) {
        if (strcmp(cmd.cmdItem[1].c_str(), UserList[i].userName) == 0 && strcmp(cmd.cmdItem[2].c_str(), UserList[i].userPwd) == 0) {
            currentUserId = i;
            currentUserUFD = UserList[i].link;
            currentUserName = cmd.cmdItem[1];
            flag = 1;
            break;
        }
    }
    if (flag) {
        cout << "Login successfully !!!" << endl;
        return;
    }
    else {
        cout << "User name or password is wrong, or you can use register command to create one." << endl;
    }
}

void do_Logout() {
    //MARK: Input parameter validation
    if (currentUserId == -1) {
        cout << "Bro you do not need to logout without login." << endl;
        return;
    }
    else {
        //MARK: Clear current user link and name
        currentUserId = -1;
        currentUserUFD = -1;
        currentUserName = "";
        cout << "Logout successfully !!! " << endl;
    }
}

void do_Register() {
    //MARK: Input parameter validation
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

    //MARK: Check if MFD is full
    if (UserList.size() < 16) {
        //MARK: Check if there is the same user name
        int flag = 1;
        for (int i = 0; i < UserList.size(); i++) {
            if (strcmp(cmd.cmdItem[1].c_str(), UserList[i].userName) == 0) {
                flag = 0;
                break;
            }
        }
        //MARK: Insert new user info
        if (flag) {
            strcpy(UserInput.userName, cmd.cmdItem[1].c_str());
            strcpy(UserInput.userPwd, cmd.cmdItem[2].c_str());
            UserInput.link = (int)UserList.size();
            UserList.push_back(UserInput);

            currentUserId = (int)UserList.size() - 1;
            currentUserUFD = UserInput.link;
            currentUserName = UserInput.userName;

            vector<UFD> tmpUFD;
            FileList.push_back(tmpUFD);

            writeBlock(0);
            cout << "Create account successfully and already login !!!" << endl;

        }
        else {
            cout << "User name has already existed, please try again." << endl;
        }
    }
    else
        cout << "Cannot register cause the total account number is full." << endl;
}

void do_Passwd() {
    //MARK: Input parameter validation
    if (currentUserId == -1) {
        cout << "Cannot change password before login." << endl;
        return;
    }
    if (strcmp(cmd.cmdItem[1].c_str(), "") == 0 || strcmp(cmd.cmdItem[2].c_str(), "") == 0) {
        cout << "Current and new password are required." << endl;
        return;
    }
    if (cmd.cmdItem[2].length() >= 14) {
        cout << "Password should be less than 14 letters." << endl;
        return;
    }
    //MARK: Change password
    if (strcmp(UserList[currentUserId].userPwd, cmd.cmdItem[1].c_str()) == 0) {
        strcpy(UserList[currentUserId].userPwd, cmd.cmdItem[2].c_str());
        writeBlock(0);
        cout << "Change password successful !!!" << endl;
    }
    else
        cout << "Current password is wrong." << endl;
}

void do_Cancel() {
    if (currentUserId == -1) {
        cout << "Cannot cancel user before login." << endl;
        return;
    }
    memset(&UserInput, '\0', sizeof(UserInput));
    UserList.erase(UserList.begin() + currentUserId);
    UserList.push_back(UserInput);
    writeBlock(0);
    UserList.pop_back();
    
    FileList.erase(FileList.begin() + currentUserUFD);
    vector<UFD> tmpUFD;
    FileList.push_back(tmpUFD);
    for (int i = currentUserUFD; i < FileList.size(); i++) {
        writeBlock(currentUserUFD + 1);
    }
    
    currentUserId = -1;
    currentUserUFD = -1;
    currentUserName = "";
    cout << "Cancel user successfully !!! " << endl;
}

//MARK: File-related command
void do_Create() {
    //MARK: Input parameter validation
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
    if (strcmp(cmd.cmdItem[2].c_str(), "0") != 0 && strcmp(cmd.cmdItem[2].c_str(), "1") != 0) {
        cout << "Mode should be within 0, 1 (read_only, read_and_write)." << endl;
        return;
    }
    //MARK: Check if current user's UFD is full
    if (FileList[currentUserUFD].size() < 16) {
        int flag = 1;
        for (int i = 0; i < FileList[currentUserUFD].size(); i++) {
            if (strcmp(FileList[currentUserUFD][i].fileName, cmd.cmdItem[1].c_str()) == 0) {
                flag = 0;
                break;
            }
        }
        if (!flag) {
            cout << "Cannot create file with the same name." << endl;
            return;
        }
        //MARK: Create file
        strcpy(FileInput.fileName, cmd.cmdItem[1].c_str());
        FileInput.mode = atoi(cmd.cmdItem[2].c_str());
        FileInput.length = 0;

        if (FreeBlockList.size() > 0) {
            FileInput.addr = FreeBlockList[0];
            FreeBlockList.erase(FreeBlockList.begin());
            FileList[currentUserUFD].push_back(FileInput);
            writeBlock(currentUserUFD + 1);

            ClusterList[FileInput.addr - 17].nextBlock = FileInput.addr;
            clearBlock(FileInput.addr);
            writeBlock(FileInput.addr);

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
    //MARK: Input parameter validation
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
    //MARK: Check if the file exsits
    int flag = 0;
    for (int i = 0; i < FileList[currentUserUFD].size(); i++) {
        if (strcmp(FileList[currentUserUFD][i].fileName, cmd.cmdItem[1].c_str()) == 0) {
            //MARK: Delete file
            flag = 1;
            int iterator = FileList[currentUserUFD][i].addr - 17;
            int tmp;
            while (ClusterList[iterator].nextBlock != iterator + 17) {
                tmp = iterator;
                iterator = ClusterList[iterator].nextBlock - 17;
                ClusterList[tmp].nextBlock = -1;
                getDataBlock(iterator + 17);
                writeBlock(iterator + 17);
                FreeBlockList.push_back(iterator + 17);
            }
            ClusterList[iterator].nextBlock = -1;
            getDataBlock(iterator + 17);
            writeBlock(iterator + 17);
            FreeBlockList.push_back(iterator + 17);
            sort(FreeBlockList.begin(), FreeBlockList.end());

            memset(&FileInput, '\0', sizeof(FileInput));
            FileList[currentUserUFD].erase(FileList[currentUserUFD].begin() + i);
            FileList[currentUserUFD].push_back(FileInput);
            writeBlock(currentUserUFD + 1);
            FileList[currentUserUFD].pop_back();

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

void do_Read() {
    //MARK: Input parameter validation
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
    //MARK: Get file's first block address
    int flag = 0;
    int blockAddress = -1;
    for (int i = 0; i < FileList[currentUserUFD].size(); i++) {
        if (strcmp(FileList[currentUserUFD][i].fileName, cmd.cmdItem[1].c_str()) == 0) {
            flag = 1;
            blockAddress = FileList[currentUserUFD][i].addr;
            break;
        }
    }
    //MARK: Read file content
    if (flag) {

        while (ClusterList[blockAddress - 17].nextBlock != blockAddress) {
            getDataBlock(blockAddress);
            cout << ClusterList[blockAddress - 17].content;
            blockAddress = ClusterList[blockAddress - 17].nextBlock;
        }
        getDataBlock(blockAddress);
        cout << ClusterList[blockAddress - 17].content << endl;
        cout << "Show file content successfully !!!" << endl;
        return;
    }
    else {
        cout << "Cannot find the file to show content." << endl;
        return;
    }
}

void do_Write() {
    //MARK: Input parameter validation
    if (currentUserId == -1) {
        cout << "Cannot write file before login." << endl;
        return;
    }
    if (strcmp(cmd.cmdItem[1].c_str(), "") == 0 || strcmp(cmd.cmdItem[2].c_str(), "") == 0) {
        cout << "File name and content are required." << endl;
        return;
    }
    if (cmd.cmdItem[1].length() >= 14) {
        cout << "File name should be less than 14 letters." << endl;
        return;
    }

    //MARK: Get file's first block address
    string buffer = cmd.cmdItem[2];
    int flag = 0;
    int blockAddress = -1;
    int bufferBlockNum = (int)buffer.size() / (512 - sizeof(int)) + ((int)buffer.size() % (512 - sizeof(int)) > 0);
    int bufferFinalBlockByteNum = buffer.size() % (512 - sizeof(int));
    int secondBlockNum = -1;
    int secondFileId = -1;
    for (int i = 0; i < FileList[currentUserUFD].size(); i++) {
        if (strcmp(FileList[currentUserUFD][i].fileName, cmd.cmdItem[1].c_str()) == 0) {
            if (FileList[currentUserUFD][i].mode == 1) {
                flag = 1;
                blockAddress = FileList[currentUserUFD][i].addr;
                if (FileList[currentUserUFD][i].length == 0) {
                    secondBlockNum = 1;
                }
                else {
                    secondBlockNum = FileList[currentUserUFD][i].length / (512 - sizeof(int)) + (FileList[currentUserUFD][i].length % (512 - sizeof(int)) > 0);
                }
                secondFileId = i;
                break;
            }
            else {
                cout << "Cannot write cause you have no right to do so." << endl;
                return;
            }
        }
    }
    //MARK: Write file content
    if (flag) {
        if (bufferBlockNum > secondBlockNum) {
            if (FreeBlockList.size() < bufferBlockNum - secondBlockNum) {
                cout << "Cannot write cause there's no free space." << endl;
                return;
            }
            FileList[currentUserUFD][secondFileId].length = (int)buffer.size();
            writeBlock(currentUserUFD + 1);
            
            int firstIterator = 0;
            int secondIterator = blockAddress - 17;
            int allocatedBlock =  -1;
            while (firstIterator + 1 != bufferBlockNum) {
                if (ClusterList[secondIterator].nextBlock != secondIterator + 17) {
                    strcpy(ClusterList[secondIterator].content, buffer.substr(firstIterator * (512 - sizeof(int)), (512 - sizeof(int))).c_str());
                    writeBlock(secondIterator + 17);
                    secondIterator = ClusterList[secondIterator].nextBlock - 17;
                    firstIterator++;
                }
                else {
                    strcpy(ClusterList[secondIterator].content, buffer.substr(firstIterator * (512 - sizeof(int)), (512 - sizeof(int))).c_str());
                    firstIterator++;

                    allocatedBlock = FreeBlockList[0];
                    FreeBlockList.erase(FreeBlockList.begin());
                    ClusterList[secondIterator].nextBlock = allocatedBlock;
                    ClusterList[allocatedBlock - 17].nextBlock = allocatedBlock;
                    writeBlock(secondIterator + 17);
                    secondIterator = allocatedBlock - 17;
                }
            }
            clearBlock(secondIterator + 17);
            strcpy(ClusterList[secondIterator].content, buffer.substr(firstIterator * (512 - sizeof(int)), bufferFinalBlockByteNum).c_str());
            writeBlock(secondIterator + 17);
        }
        else {
            int firstIterator = 0;
            int secondIterator = blockAddress - 17;
            int recycleIterator =  -1;
            
            FileList[currentUserUFD][secondFileId].length = (int)buffer.size();
            writeBlock(currentUserUFD + 1);
            
            while (firstIterator + 1 != bufferBlockNum) {
                strcpy(ClusterList[secondIterator].content, buffer.substr(firstIterator * (512 - sizeof(int)), (512 - sizeof(int))).c_str());
                writeBlock(secondIterator + 17);
                secondIterator = ClusterList[secondIterator].nextBlock - 17;
                firstIterator++;
            }
            strcpy(ClusterList[secondIterator].content, buffer.substr(firstIterator * (512 - sizeof(int)), bufferFinalBlockByteNum).c_str());
            recycleIterator = ClusterList[secondIterator].nextBlock - 17;
            ClusterList[secondIterator].nextBlock = secondIterator + 17;
            writeBlock(secondIterator + 17);

            if (bufferBlockNum != secondBlockNum) {
                while (ClusterList[recycleIterator].nextBlock != recycleIterator + 17) {
                    FreeBlockList.push_back(recycleIterator + 17);
                    secondIterator = recycleIterator;
                    recycleIterator = ClusterList[recycleIterator].nextBlock - 17;
                    ClusterList[secondIterator].nextBlock = -1;
                    writeBlock(secondIterator + 17);
                }
                FreeBlockList.push_back(recycleIterator + 17);
                sort(FreeBlockList.begin(), FreeBlockList.end());
                ClusterList[recycleIterator].nextBlock = -1;
                writeBlock(recycleIterator + 17);
            }
        }
        cout << "Write file successfully !!!" << endl;
        return;
    }
    else {
        cout << "Cannot find the file to show content." << endl;
        return;
    }
}

void do_Ls() {
    if (currentUserId == -1) {
        cout << "Cannot list the file directory before login." << endl;
        return;
    }
    for (int i = 0; i < FileList[currentUserUFD].size(); i++) {
        cout << FileList[currentUserUFD][i].fileName << endl;
    }
}

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
    if (strcmp(cmd.cmdItem[2].c_str(), "0") != 0 && strcmp(cmd.cmdItem[2].c_str(), "1") != 0) {
        cout << "Mode should be within 0, 1 (read_only, read_and_write)." << endl;
        return;
    }

    int flag = 0;
    for (int i = 0; i < FileList[currentUserUFD].size(); i++) {
        if (strcmp(FileList[currentUserUFD][i].fileName, cmd.cmdItem[1].c_str()) == 0) {
            flag = 1;

            FileList[currentUserUFD][i].mode = atoi(cmd.cmdItem[2].c_str());
            writeBlock(currentUserUFD + 1);

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
        for (int i = 0; i < FileList[currentUserUFD].size(); i++) {
            if (strcmp(FileList[currentUserUFD][i].fileName, cmd.cmdItem[1].c_str()) == 0) {
                flag = 1;
                //MARK: 一边push 一边erase插空 FileList
                FileInput = FileList[currentUserUFD][i];
                FileList[newOwnerId].push_back(FileInput);
                writeBlock(newOwnerId + 1);

                memset(&FileInput, '\0', sizeof(FileInput));
                FileList[currentUserUFD].erase(FileList[currentUserUFD].begin() + i);
                FileList[currentUserUFD].push_back(FileInput);
                writeBlock(currentUserUFD + 1);
                FileList[currentUserUFD].pop_back();

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
    if (currentUserId == -1) {
        cout << "Cannot change file name before login." << endl;
        return;
    }
    int flag = 0;
    int fileId = -1;
    for (int i = 0; i < FileList[currentUserUFD].size(); i++) {
        if (strcmp(FileList[currentUserUFD][i].fileName, cmd.cmdItem[1].c_str()) == 0) {
            flag = 1;
            fileId = i;
            break;
        }
    }
    
    if (flag) {
        flag = 1;
        for (int i = 0; i < FileList[currentUserUFD].size(); i++) {
            if (strcmp(FileList[currentUserUFD][i].fileName, cmd.cmdItem[2].c_str()) == 0) {
                flag = 0;
                break;
            }
        }
        
        if (flag) {
            strcpy(FileList[currentUserUFD][fileId].fileName, cmd.cmdItem[2].c_str());
            writeBlock(currentUserUFD + 1);
            
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
    if (currentUserId == -1) {
        cout << "Cannot copy fiel before login." << endl;
        return;
    }
    int flag = 0;
    int firstFileId = -1;
    int secondFileId = -1;
    for (int i = 0; i < FileList[currentUserUFD].size(); i++) {
        if (strcmp(FileList[currentUserUFD][i].fileName, cmd.cmdItem[1].c_str()) == 0) {
            flag = 1;
            firstFileId = i;
            break;
        }
    }

    if (flag) {
        flag = 0;
        for (int i = 0; i < FileList[currentUserUFD].size(); i++) {
            if (strcmp(FileList[currentUserUFD][i].fileName, cmd.cmdItem[2].c_str()) == 0) {
                flag = 1;
                secondFileId = i;
                break;
            }
        }

        if (flag) {
            int firstBlockNum = FileList[currentUserUFD][firstFileId].length % (512 - sizeof(int));
            int secondBlockNum = FileList[currentUserUFD][secondFileId].length % (512 - sizeof(int));
            if (firstBlockNum > secondBlockNum) {
                if (FreeBlockList.size() < firstBlockNum - secondBlockNum) {
                    cout << "Cannot copy cause there's no free space." << endl;
                    return;
                }
                FileList[currentUserUFD][secondFileId].length = FileList[currentUserUFD][firstFileId].length;
                //开始cpoy
                int firstIterator = FileList[currentUserUFD][firstFileId].addr;
                int secondIterator = FileList[currentUserUFD][secondFileId].addr;
                int allocatedBlock =  -1;
                while (ClusterList[firstIterator].nextBlock != firstIterator + 17) {
                    if (ClusterList[secondIterator].nextBlock != secondIterator + 17) {
                        getDataBlock(firstIterator);
                        strcpy(ClusterList[secondIterator].content, ClusterList[firstIterator].content);
                        writeBlock(secondIterator + 17);
                        secondIterator = ClusterList[secondIterator].nextBlock;
                        firstIterator = ClusterList[firstIterator].nextBlock;
                    }
                    else {
                        getDataBlock(firstIterator);
                        strcpy(ClusterList[secondIterator].content, ClusterList[firstIterator].content);
                        firstIterator = ClusterList[firstIterator].nextBlock;

                        allocatedBlock = FreeBlockList[0];
                        FreeBlockList.erase(FreeBlockList.begin());
                        ClusterList[secondIterator].nextBlock = allocatedBlock;
                        ClusterList[allocatedBlock - 17].nextBlock = allocatedBlock;
                        writeBlock(secondIterator + 17);
                        secondIterator = allocatedBlock - 17;
                    }
                }
                clearBlock(secondIterator + 17);
                getDataBlock(firstIterator);
                strcpy(ClusterList[secondIterator].content, ClusterList[firstIterator].content);
                writeBlock(secondIterator + 17);
            }
            else {
                int firstIterator = FileList[currentUserUFD][firstFileId].addr;
                int secondIterator = FileList[currentUserUFD][secondFileId].addr;
                int recycleIterator =  -1;
                while (ClusterList[firstIterator].nextBlock != firstIterator + 17) {
                    getDataBlock(firstIterator);
                    strcpy(ClusterList[secondIterator].content, ClusterList[firstIterator].content);
                    writeBlock(secondIterator + 17);
                    secondIterator = ClusterList[secondIterator].nextBlock;
                    firstIterator = ClusterList[firstIterator].nextBlock;
                }
                getDataBlock(firstIterator);
                strcpy(ClusterList[secondIterator].content, ClusterList[firstIterator].content);
                recycleIterator = ClusterList[secondIterator].nextBlock - 17;
                ClusterList[secondIterator].nextBlock = secondIterator + 17;
                writeBlock(secondIterator + 17);

                if (firstBlockNum != secondBlockNum) {
                    while (ClusterList[recycleIterator].nextBlock != recycleIterator + 17) {
                        FreeBlockList.push_back(recycleIterator + 17);
                        secondIterator = recycleIterator;
                        recycleIterator = ClusterList[recycleIterator].nextBlock - 17;
                        ClusterList[secondIterator].nextBlock = -1;
                        writeBlock(secondIterator + 17);
                    }
                    FreeBlockList.push_back(recycleIterator + 17);
                    sort(FreeBlockList.begin(), FreeBlockList.end());
                    ClusterList[recycleIterator].nextBlock = -1;
                    writeBlock(recycleIterator + 17);
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

//MARK: System-related command
void do_Help() {
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

void do_Exit() {
    exit(0);
}

void do_Clear() {
    system("cls");
    return;
}

void do_Format() {
    format();
    initFileSystem();
    currentUserId = -1;
    currentUserUFD = -1;
    currentUserName = "";
    cout << "Disk format successfully !!!" <<endl;
    return;
}
