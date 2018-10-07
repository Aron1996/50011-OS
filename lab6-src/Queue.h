//
//  Queue.hpp
//  lab6
//
//  Created by 张耀坤 on 7/24/18.
//  Copyright © 2018 Yaokun Zhang. All rights reserved.
//

#ifndef Queue_h
#define Queue_h

#include <stdio.h>
#include "List.h"

class Queue : public List {
public:
    Queue();
    void enqueue(int val);
    int dequeue();
    ~Queue();
};
#endif /* Queue_hpp */
