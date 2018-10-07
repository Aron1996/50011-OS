//
//  List.cpp
//  lab6
//
//  Created by 张耀坤 on 7/22/18.
//  Copyright © 2018 Yaokun Zhang. All rights reserved.
//
//
// Implement the List class
//

#include <stdio.h>
#include "List.h"
#include <stdlib.h>
#include <iostream>
//
// Inserts a new element with value "val" in
// ascending order.
//
void
List::insertSorted( int val )
{
    // Complete procedure
    ListNode * e = new ListNode;
    e = _head;
    ListNode * temp = NULL;
    ListNode * newNode = new ListNode;
    newNode->_value = val;
    if (e == NULL) {
        _head = newNode;
        _head->_next=NULL;
    }else{
        while (e != NULL) {
            if (val <= e->_value) {
                break;
            }
            temp = e;
            e=e->_next;
        }
        if (temp != NULL) {
            temp->_next=newNode;
            newNode->_next=e;
        }else{
            _head=newNode;
            newNode->_next=e;
        }
    }

}

//
// Inserts a new element with value "val" at
// the end of the list.
//
void
List::append( int val )
{
    ListNode * e = new ListNode;
    e = _head;
    ListNode * temp = new ListNode;
    temp =NULL;
    while (e != NULL) {
        temp = e;
        e=e->_next;
    }
    ListNode * newNode = new ListNode;
    newNode->_value = val;
    newNode->_next = NULL;
    if (temp != NULL) {
        temp->_next=newNode;
    }else{
        _head = newNode;
    }
    
}

//
// Inserts a new element with value "val" at
// the beginning of the list.
//
void
List::prepend( int val )
{
    ListNode * e = new ListNode;
    e = _head;
    ListNode * newNode = new ListNode;
    newNode->_value = val;
    newNode->_next = _head;
    _head = newNode;
}

// Removes an element with value "val" from List
// Returns 0 if succeeds or -1 if it fails
int
List:: remove( int val )
{
    ListNode * e = new ListNode;
    ListNode * temp = NULL;
    e = _head;
    ListNode * newNode = new ListNode;
    newNode->_value = val;
    if (e == NULL) {
        return -1;
    }else{
        while (e != NULL) {
            if (val == e->_value) {
                break;
            }
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

// Prints The elements in the list.
void
List::print()
{
    ListNode * e = _head;
    
    if (e == NULL) {
        printf("{EMPTY}\n");
        return;
    }
    
    printf("{");
    while (e != NULL) {
        printf("%d", e->_value);
        e = e->_next;
        if (e!=NULL) {
            printf(", ");
        }
    }
    printf("}\n");
}

//
// Returns 0 if "value" is in the list or -1 otherwise.
//
int
List::lookup(int val)
{
    ListNode * e = new ListNode;
    e = _head;
    while (e != NULL) {
        if (val == e->_value) {
            break;
        }
        e=e->_next;
    }
    if (e != NULL) {
        return 0;
    }else{
        return -1;
    }
    return 0;
}
//
// List constructor
//
List::List()
{
    // Complete procedure
    _head = NULL;
}

//
// List destructor: delete all list elements, if any.
//
List::~List()
{
    ListNode * e = _head;
    while (e != NULL) {
        ListNode * node = e->_next;
        delete e;
        e = node;
    }
}


