//
// Created by İsmail Yiğit Önal 31214 on 19.10.2024.
//
#include <iostream>
#include "stack.h"
using namespace std;
//implementing linked-list into stack (taken from W4-StacksAndQueues.pdf) but just the parts that will be used. 
struct Queen {
    int row, col;
    Queen(int r = 0, int c = 0) : row(r), col(c) {}
};

template <class Object>
Stack<Object>::Stack( )
{
    topOfStack = NULL;
}
template <class Object>
Stack<Object>::~Stack( )
{
    makeEmpty( );
}
template <class Object>
bool Stack<Object>::isEmpty( ) const
{
    return topOfStack == NULL;
}
template <class Object>
const Object & Stack<Object>::top( ) const
{
    if (isEmpty( ))
        cout<<"Underflow";
    return topOfStack->element;
}
template <class Object>
void Stack<Object>::pop( )
{
    if ( isEmpty( ) )
        cout<<"Underflow";
    ListNode *oldTop = topOfStack;
    topOfStack = topOfStack->next;
    delete oldTop;
}
template <class Object>
void Stack<Object>::push( const Object & x )
{
    topOfStack = new ListNode( x, topOfStack );
}
template <class Object>
void Stack<Object>::makeEmpty( )
{
    while ( ! isEmpty( ) )
        pop( );
}
template class Stack<Queen>;
