#ifndef STACK_H
#define STACK_H

#include <iostream>
using namespace std;

template<class E>
class Stack;

template<class E>
std::ostream& operator<< (std::ostream& os, const Stack<E>& s);

template <class E>
class Stack
{
    friend std::ostream& operator<< <E>(std::ostream& os, const Stack<E>& s);
 private:
   E* arr;
   int count;
   static const int capacity = 100;

 public:
   Stack();
   virtual ~Stack();

   E pop();
   void push(E item);

   E peek() const;

   bool isEmpty() const 
   {
     return (count==0);
   }
   int getCount() const 
   {
     return count;
   }

};

// operator for printing the stack to the console
template<class E>
std::ostream& operator<< (std::ostream& os, const Stack<E>& s)
{
    if (s.count > 0) {
        for (int i=s.count; i>0; --i)
        {
            os << s.arr[i-1].size() << " ";
        }
    } else {
        os << "<empty>";
    }
    return os;
}

#endif
