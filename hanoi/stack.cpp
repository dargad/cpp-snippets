#include "stack.h"
#include "disk.h"
#include <cassert>

template< class E >
Stack<E>::Stack()
: arr( new E[capacity] ), count(0)
{
}

template< class E >
Stack<E>::~Stack()
{
delete [] arr;
}

template< class E >
E Stack<E>::pop()
{
   count--;
   assert(count >= 0);
   return arr[count];
}

template< class E >
void Stack<E>::push(E item)
{
arr[count] = item;
count++;
}

template< class E >
E Stack<E>::peek() const
{
assert(count > 0);
return arr[count-1];
}

template class Stack<Disk>;
