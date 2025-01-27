//
// Created by İsmail Yiğit Önal 31214 on 19.10.2024.
//
#include <iostream>
using namespace std;
//implementing linked-list into stack header file (taken from W4-StacksAndQueues.pdf)
template <class Object>
class Stack{
public:
   Stack();
   ~Stack( );
   bool isEmpty( ) const;
   void makeEmpty( );
   void pop( );
   void push( const Object & x );
   const Object & top( ) const;

private:
    struct ListNode
    {
        Object element;
        ListNode *next;
        ListNode(const Object & theElement, ListNode * n = NULL )
                : element( theElement ), next( n ) { }
    };
    ListNode *topOfStack; // list itself is the stack

};

