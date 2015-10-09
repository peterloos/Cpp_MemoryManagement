#include <iostream>
using namespace std;

#include "Memory.h"
#include "Josephus.h"

#define  DUMP_MEMORY    1

// c'tor(s), d'tor
Josephus::Josephus()
    : m_mem(512), m_count(41), m_alive(41), m_lastEliminated(0)
{
#if defined (DUMP_MEMORY)
    m_mem.DumpFreeMemory(cout);
    cout << endl;
    m_mem.DumpMemory(cout);
#endif

    m_passby = Josephus::DefaultPassBy;
    CreateLinkedList();
}

Josephus::Josephus(int count)
    : m_mem(512), m_count(count), m_alive(count), m_lastEliminated(0)
{
#if defined (DUMP_MEMORY)
    m_mem.DumpFreeMemory(cout);
    cout << endl;
    m_mem.DumpMemory(cout);
#endif

    m_passby = Josephus::DefaultPassBy;
    CreateLinkedList();
}

Josephus::~Josephus()
{
    HANDLE soldier = m_root;
    
    // delete each single element
    do 
    {
        // store current node pointer
        HANDLE current = soldier;
    
        // advance to next node
        soldier = m_mem.ReadInteger (soldier + 2);
    
        // delete 'current' node pointer
        m_mem.Release (current);
    }
    while (soldier != m_root);

    cout << endl;

#if defined (DUMP_MEMORY)
    m_mem.DumpFreeMemory(cout);
    cout << endl;
    m_mem.DumpMemory(cout);
#endif
}

// public interface
bool Josephus::EliminateNextSoldier()
{
    // more than one soldier?
    if (m_alive == 1)
        return false;
        
    // locate next soldier
    for (int i = 0; i < m_passby - 1; i ++)
        m_current = m_mem.ReadInteger (m_current + 2);

    // remove soldier from list
    HANDLE node = m_mem.ReadInteger (m_current + 2);
    HANDLE next = m_mem.ReadInteger (node + 2);
    m_mem.WriteInteger (m_current + 2, next);

    // take care, if root object is removed
    if (m_root == node)
        m_root = m_mem.ReadInteger (m_root + 2);

    m_lastEliminated = m_mem.ReadShort (node);
    m_mem.Release (node);
    m_alive --;

#if defined (DUMP_MEMORY)
    m_mem.DumpFreeMemory(cout);
#endif

    return true;
}

void Josephus::EliminateAll()
{
    while (EliminateNextSoldier())
        ;
}

// private helper methods
void Josephus::CreateLinkedList()
{
    // allocate memory for first soldier
    m_root = m_mem.Allocate (6);
    m_mem.WriteShort (m_root, 1);
    m_mem.WriteInteger (m_root + 2, (HANDLE) 0);

    m_current = m_root;

    for (int i = 1; i < m_count; i ++)
    {
        // allocate memory for a new soldier
        HANDLE tmp = m_mem.Allocate (6);
        m_mem.WriteShort (tmp, i + 1);

#if defined (DUMP_MEMORY)
    m_mem.DumpFreeMemory(cout);
#endif

        // link previous soldier with current one
        m_mem.WriteInteger (m_current + 2, tmp);
        m_current = tmp;
    }

    // link last soldier with first one
    m_mem.WriteInteger (m_current + 2, m_root);
}

// output
ostream& operator<< (ostream& os, const Josephus& j)
{
    os << '[';
    HANDLE tmp = j.m_root;
    do
    {
        short number = j.m_mem.ReadShort (tmp);
        os << number;

        HANDLE next = j.m_mem.ReadInteger (tmp + 2);
        if (next != j.m_root)
            os << ',';

        tmp = j.m_mem.ReadInteger (tmp + 2);
    }
    while (tmp != j.m_root);
    os << ']';
    return os;
}
