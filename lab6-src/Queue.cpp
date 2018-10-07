//
//  Queue.cpp
//  lab6
//
//  Created by 张耀坤 on 7/24/18.
//  Copyright © 2018 Yaokun Zhang. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Queue.h"
#include <assert.h>

Queue::Queue(){
    _head =NULL;
}

void Queue::enqueue(int val){
    append(val);
}
int Queue::dequeue(){
    ListNode * e = new ListNode;
    if (e == NULL) {
        return -1;
    }
    e = _head;
    _head = e->_next;

    delete e;
    return 0;
}

Queue::~Queue(){
    ListNode * e = _head;
    while (e != NULL) {
        ListNode * node = e->_next;
        delete e;
        e = node;
    }
    _head = NULL;
}
int a[] = {45, 23, 78, 12, 100, 1, 100, 34, 90, 78 };


int
main( int argc, char ** argv )
{
    Queue l;
    int i;
    
    int nelements = sizeof( a )/ sizeof( int );
    for ( i = 0; i < nelements; i++ ) {
        l.enqueue( a[ i ] );
    }
    l.print();
    assert( l.dequeue() == 0 );
    assert( l.dequeue() == 0 );
    assert ( l.lookup( 45) == -1 );
    assert ( l.lookup( 23) == -1 );
    assert ( l.lookup( 78) == 0 );
    assert ( l.lookup( 1) == 0 );
    assert ( l.lookup( 90) == 0 );
    assert ( l.lookup( 34) == 0 );
    assert ( l.lookup( 95) == -1 );
    assert ( l.lookup( 2) == -1 );
    assert ( l.lookup( 12) == 0 );
    l.print();
    printf(">>> Queue_tested!\n");
    return 0;
    
}
