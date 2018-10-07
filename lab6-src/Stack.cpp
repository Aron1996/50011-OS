//
//  Stack.cpp
//  lab6
//
//  Created by 张耀坤 on 7/23/18.
//  Copyright © 2018 Yaokun Zhang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include "Stack.h"

Stack::Stack(){
    _head =NULL;
}
int Stack::getLastVal(){
    return last_val;
}
void Stack::push(int val){
    append(val);
    last_val=val;
}
int Stack::pop(){
    ListNode * e = new ListNode;
    ListNode * temp = NULL;
    e = _head;
    if (e == NULL) {
        return -1;
    }else if(_head->_next == NULL){
        _head = NULL;
    }
    else{
        while (e->_next != NULL) {
            temp=e;
            e=e->_next;
        }
    }
    if (e != NULL) {
        temp->_next=e->_next;
    }else{
        return -1;
    }
    delete e;
    return 0;
}

Stack::~Stack(){
    ListNode * e = _head;
    while (e != NULL) {
        ListNode * node = e->_next;
        delete e;
        e = node;
    }
    _head = NULL;
}
int
main( int argc, char ** argv )
{
    int a[] = {45, 23, 78, 12, 100, 1, 100, 34, 90, 78 };
    Stack l;
    int i;
    
    int nelements = sizeof( a )/ sizeof( int );
    for ( i = 0; i < nelements; i++ ) {
        l.push( a[ i ] );
    }
    l.print();
    assert( l.pop() == 0 );
    assert( l.pop() == 0 );
    assert ( l.lookup( 45) == 0 );
    assert ( l.lookup( 23) == 0 );
    assert ( l.lookup( 78) == 0 );
    assert ( l.lookup( 1) == 0 );
    assert ( l.lookup( 90) == -1 );
    assert ( l.lookup( 34) == 0 );
    assert ( l.lookup( 95) == -1 );
    assert ( l.lookup( 2) == -1 );
    assert ( l.lookup( 12) == 0 );
    l.print();
    printf(">>> stack_tested!\n");
    return 0;
}

