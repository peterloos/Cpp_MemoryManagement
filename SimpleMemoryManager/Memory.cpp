#include <iostream>
#include <iomanip>
using namespace std;

#include "Memory.h"

// c'tors / d'tor
Memory::Memory ()
{
    // allocate memory
    m_len = Memory::Max;
    m_mem = new unsigned char[m_len];

    // initialize memory
    SetMemory (' ');

    // initialize free entry list
    m_root = new MemoryFragment (0, Memory::Max);
}

Memory::Memory (int len)
{
    // allocate memory
    m_len = len;
    m_mem = new unsigned char[m_len];

    // initialize memory
    SetMemory (' ');

    // initialize free entry list
    m_root = new MemoryFragment (0, m_len);
}

Memory::~Memory ()
{
    // delete memory
    delete[] m_mem;

    // delete free entry list
    MemoryFragment* anchor = m_root;
    MemoryFragment* item = m_root;

    // delete each single element
    do
    {
        // store current node pointer
        MemoryFragment* current = item;

        // advance to next node
        item = item -> m_next;

        // delete 'current' node pointer
        delete current;
    }
    while (item != anchor);
}

// getter / setter
int Memory::Length ()
{
    return m_len;
}

// public interface
void Memory::SetMemory (char c)
{
    for (int i = 0; i < m_len; i ++)
        m_mem[i] = c;
}

void Memory::WriteString (int ofs, char* s)
{
    for (int i = 0; s[i] != '\0'; i ++)
        m_mem[ofs + i] = s[i];
}

void Memory::WriteShort (HANDLE ofs, short value)
{
    * ((short*) (m_mem + ofs)) = value;
}

void Memory::WriteInteger (HANDLE ofs, int value)
{
    * ((int*) (m_mem + ofs)) = value;
}

short Memory::ReadShort(HANDLE ofs) const
{
    return * ((short*) (m_mem + ofs));
}

int Memory::ReadInteger(HANDLE ofs) const
{
    return * ((int*) (m_mem + ofs));
}

int Memory::Allocate (int bytes)
{
    // check argument
    if (bytes <= 0)
        return -1;

    // need extra two bytes for size
    bytes += 2;

    // traverse free entry list
    MemoryFragment* frag = m_root;
    MemoryFragment* last = m_root;

    do
    {
        /* fragment's size too small
        */
        if (frag -> m_size < bytes)
        {
            // advance to next entry
            last = frag;
            frag = frag -> m_next;
        }
        /* fragment's size equals requested size
        */
        else if (frag -> m_size == bytes)
        {
            // drop current block from list
            int ofs = frag -> m_frag;

            // poke size of this block into memory
            * (short*) (m_mem + ofs) = bytes;

            // free entry list contains of a single node
            if (frag -> m_next == m_root)
            {
                // setup single fragment with size of zero
                frag -> m_size = 0;
            }
            else
            {
                // drop current block from list
                MemoryFragment* tmp = frag;
                last -> m_next = frag -> m_next;
                delete tmp;
            }

            return ofs + 2;
        }
        /* fragment's size is larger than the requested size
        */
        else
        {
            // calculate start offset
            int ofs = frag -> m_frag + frag -> m_size - bytes;

            // poke requested block size into memory
            * (short*) (m_mem + ofs) = bytes;

            // reduce size of current memory fragment
            frag -> m_size -= bytes;

            return ofs + 2;
        }
    }
    while (frag != m_root);

    // no fragment found, suitable memory fragment not available
    return -1;
}

void Memory::Release (int idx)
{
    // make idx point to begin of memory block
    idx -= 2;

    // retrieve size of current memory block
    short size = * (short*) (m_mem + idx);

    // only one free entry available
    if (m_root == m_root -> m_next)
    {
        /* is begin of current block adjacent with end of preceding free block
        */
        if (m_root -> m_frag + m_root -> m_size == idx)
        {
            // expand preceding block
            m_root -> m_size += size;
        }
        /* is end of current block adjacent with next free block
        */
        else if (idx + size == m_root -> m_frag)
        {
            m_root -> m_frag = idx;
            m_root -> m_size += size;
        }
        /* current block is not adjacent with single block in free entry list
        */
        else
        {
            // single block in free entry list describes non-empty memory block
            if (m_root -> m_size != 0)
            {
                MemoryFragment* fp = new MemoryFragment (idx, size);

                if (idx < m_root -> m_frag)
                {
                    fp -> m_next = m_root;
                    m_root -> m_next = fp;
                    m_root = fp;
                }
                else
                {
                    m_root -> m_next = fp;
                    fp -> m_next = m_root;
                }
            }
            else
            {
                m_root -> m_frag = idx;
                m_root -> m_size = size;
            }
        }
    }
    else
    {
        /* locate left and right neighbour in free entry list
        */
        MemoryFragment* leftFrag = m_root;
        MemoryFragment* rightFrag = m_root -> m_next;

        MemoryFragment* tmp = m_root;
        while (true)
        {
            // end of list reached
            if (tmp -> m_next == m_root)
                break;

            leftFrag = tmp;
            rightFrag = tmp -> m_next;

            // is memory block between these two free memory blocks?
            if (tmp -> m_frag < idx && idx < tmp -> m_next -> m_frag)
                break;

            tmp = tmp -> m_next;
        }

        // 1.) block to release is before the first free memory area
        if (idx < leftFrag -> m_frag)
        {
            // a) blocks are adjacent
            if (idx + size == leftFrag -> m_frag)
            {
                leftFrag -> m_frag = idx;
                leftFrag -> m_size += size;
            }
            // b) blocks are not adjacent
            else
            {
                MemoryFragment* fp = new MemoryFragment (idx, size);

                // insert node at begin of list
                fp -> m_next = leftFrag;

                // last node has to point to new first node
                while (rightFrag -> m_next != m_root)
                    rightFrag = rightFrag -> m_next;
                rightFrag -> m_next = fp;

                // make root pointing to new first node
                m_root = fp;
            }
        }

        // 2.) block to release is after the last free memory area
        else if (idx >= rightFrag -> m_frag + rightFrag -> m_size)
        {
            // a) blocks are adjacent
            if (idx == rightFrag -> m_frag + rightFrag -> m_size)
            {
                rightFrag -> m_size += size;
            }
            // b) blocks are not adjacent
            else
            {
                MemoryFragment* fp = new MemoryFragment (idx, size);
                fp -> m_next = m_root;
                rightFrag -> m_next = fp;
            }
        }

        // 3.) block to release is between two free memory areas
        else
        {
            /* is current block adjacent with both neighbouring blocks
            */
            if (leftFrag -> m_frag + leftFrag -> m_size == idx &&
                rightFrag -> m_frag == idx + size)
            {
                // expand preceding block
                leftFrag -> m_size += size;
                leftFrag -> m_size += rightFrag -> m_size;

                // drop next block from list
                leftFrag -> m_next = rightFrag -> m_next;
                delete rightFrag;
            }
            /* is begin of current block adjacent with end of preceding free block
            */
            else if (leftFrag -> m_frag + leftFrag -> m_size == idx)
            {
                // expand preceding block
                leftFrag -> m_size += size;
            }
            /* is end of current block adjacent with next free block
            */
            else if (rightFrag -> m_frag == idx + size)
            {
                // expand next block
                rightFrag -> m_frag = idx;
                rightFrag -> m_size += size;
            }
            /* current block is not adjacent with both neighbouring blocks
            */
            else
            {
                // insert new MemoryFragment descriptor into linked list
                MemoryFragment* fp = new MemoryFragment (idx, size);
                fp -> m_next = rightFrag;
                leftFrag -> m_next = fp;
            }
        }
    }
}

void Memory::DumpFreeMemory (ostream& os)
{
    os << "Dump of Free List:" << endl;

    MemoryFragment* frag = m_root;
    do
    {
        os << "Start: " << setfill (' ') << dec << setw (3) << frag -> m_frag << " - "
           << "Size: "  << setfill (' ') << dec << setw (3) << frag -> m_size << endl;

        frag = frag -> m_next;
    }
    while (frag != m_root);

    os << endl;
}
    
void Memory::DumpMemory (ostream& os)
{
    for (int line = 0;; line += 16)
    {
        int count = (m_len - line) < 16 ? m_len - line : 16;
        DumpLine (os, line, count);

        if (count < 16)
            break;
    }
}

// internal helper methods
void Memory::DumpLine (ostream& os, int ofs, int count)
{
    if (count== 0)
        return;

    os << setfill ('0') << setw (8) << hex << ofs << "  " ;

    // writing binary representation of byte
    for (int i = 0; i < count; i ++)
    {
        int byte = m_mem[ofs + i];
        os << setw (2) << setfill ('0') << hex << uppercase << byte << ' ';

        if ((i + 1) % 8 == 0)
            os << ' ';
    }
    os << ' ';

    // writing readable representation of byte
    for (int i = 0; i < count; i ++)
    {
        unsigned char c = (m_mem[ofs + i] >= ' ' && m_mem[ofs + i] < '~') ?
            m_mem[ofs + i] : '.';
        os << c;
    }
    os << endl;
}
