//
//  Stack.hpp
//  lab6
//
//  Created by 张耀坤 on 7/23/18.
//  Copyright © 2018 Yaokun Zhang. All rights reserved.
//

#ifndef Stack_h
#define Stack_h

#include <stdio.h>
#include "List.h"

class Stack : public List {
    int last_val;
public:
    Stack();
    void push(int val);
    int pop();
    int getLastVal();
    ~Stack();
    
};
#endif /* Stack_hpp */
