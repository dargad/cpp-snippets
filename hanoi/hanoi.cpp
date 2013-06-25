#include "stack.h"
#include "disk.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>

void printPoles(const Stack<Disk> (&poles)[3])
{
    for (int i=0; i<3; ++i)
    {
        std::cout << "pole[" << i << "]: " << poles[i] << std::endl;
    }
}

void recursive(int n, Stack<Disk> (&poles)[3], int idxA, int idxB, int idxC)
{
    if (n > 0)
    {
        recursive(n-1, poles, idxA, idxC, idxB);
        std::cout << "Moving disk " << poles[idxA].peek().size() << " from pole["
            << idxA << "] to pole[" << idxC << "]" << std::endl;
        poles[idxC].push(poles[idxA].pop());
        recursive(n-1, poles, idxB, idxA, idxC);
    }
}

void iterative(int numDisks, Stack<Disk> (&poles)[3])
{
    do
    {
        for (int i=0; i<3; ++i)
        {
            if (poles[i].getCount() != 0 && poles[i].peek().size() == 1)
            {
                std::cout << "Moving disk " << poles[i].peek().size() << " from pole["
                    << i << "] to pole[" << (i+1)%3 << "]" << std::endl;
                poles[(i+1) % 3].push(poles[i].pop());
                break;
            }
        }

        for (int i=0; i<3; ++i)
        {
            if (poles[i].getCount() == 0)
                continue;
            int size = poles[i].peek().size();
            if (size != 1)
            {
                for (int j=0; j<3; j++)
                {
                    if (i != j)
                    {
                        if (poles[j].getCount() == 0 || size < poles[j].peek().size())
                        {
                            std::cout << "Moving disk " << poles[i].peek().size() << " from pole["
                                << i << "] to pole[" << j << "]" << std::endl;
                            poles[j].push(poles[i].pop());
                            goto out;
                        }
                    }
                }
            }
        }
out:
    numDisks;
    } while (numDisks != poles[2].getCount());
}

void random(int numDisks, Stack<Disk> (&poles)[3])
{
    srandom(time(0));
    do {
        int idx = random() % 3;
        if ( poles[idx].getCount() > 0 )
        {
            int size = poles[idx].peek().size();
            for (int i=2; i>=0; --i)
            {
                if (idx != i)
                {
                    if (poles[i].getCount() == 0 || size < poles[i].peek().size())
                    {
                        std::cout << "Moving disk " << size << " from pole["
                            << idx << "] to pole[" << i << "]" << std::endl;
                        poles[i].push(poles[idx].pop());
                        break;
                    }
                }
            }
        }
    } while (numDisks != poles[2].getCount());
}

int main(int argc, char *argv[])
{
    std::cout << "Please enter the number of disks: ";
    int numDisks(-1);

    Stack<Disk> poles[3];

    // get input from the user and check it
    std::string s;
    do {
        std::cin >> s;
        std::stringstream ss(s);
        // check whether the provided string can be converted to int
        if ((ss >> numDisks).fail() || !(ss >> std::ws).eof() || numDisks <= 0)
        {
            std::cout << numDisks << std::endl;
            std::cout << "Incorrect number of disks provided. Please try again: ";
        }
    } while (numDisks <= 0);

    std::cout << "Initializing " << numDisks << " disks" << std::endl;

    // initialize the first pole with all the disks (the biggest at the bottom)
    for (int i=numDisks; i>0; --i)
    {
        poles[0].push(Disk(i));
    }

    // uncomment below to use recursive solution
    //recursive(numDisks, poles, 0, 1, 2);

    // uncomment below to use iterative solution
    //iterative(numDisks, poles);

    random(numDisks, poles);

    printPoles(poles);
}
