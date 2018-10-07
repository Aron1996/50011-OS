//
//  Stack.cpp
//  lab6
//
//  Created by 张耀坤 on 7/23/18.
//  Copyright © 2018 Yaokun Zhang. All rights reserved.
//

#include <stdio.h>
#include "List.h"
#include <stdlib.h>
#include <iostream>
#include "Stack.h"
Stack::Stack(){
    last_val = 0;
    _head = NULL;
}
int Stack::getLastVal(){
    return last_val;
}

