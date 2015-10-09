#include <iostream>
#include <iomanip>
using namespace std;

#include "Memory.h"

// c'tors / d'tor
Memory::MemoryFragment::MemoryFragment ()
{
    m_frag = 0;            // begin of memory block
    m_size = Memory::Max;  // maximum available memory
    m_next = this;         // circularly linked list
}

Memory::MemoryFragment::MemoryFragment (int frag, int size)
{
    m_frag = frag;  // begin of memory block
    m_size = size;  // size of memory block
    m_next = this;  // circularly linked list
}
