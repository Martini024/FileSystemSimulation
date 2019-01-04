//
//  Block.hpp
//  File System Simulation
//
//  Created by JIN ZHAO on 12/30/18.
//  Copyright © 2018 JIN ZHAO. All rights reserved.
//

#ifndef Block_h
#define Block_h

char* readBlock(int blockNum, int nbytes = 512);

void writeBlock(int blockNum);

void clearBlock(int blockNum);

void getDataBlock(int blockNum);

#endif /* Block_hpp */
