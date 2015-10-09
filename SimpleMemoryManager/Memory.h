typedef int HANDLE;

class Memory
{
public:
    static const int Max = 128;

private:
    class MemoryFragment
    {
    public:
        int m_frag;              // index (pointer) to first byte of memory block
        int m_size;              // size of this memory block
        MemoryFragment* m_next;  // pointer to next free memory block

    public:
        // c'tors
        MemoryFragment ();
        MemoryFragment (int start, int size);
    };

private:
    unsigned char*  m_mem;   // pointer to first byte of memory
    int             m_len;   // total length of memory
    MemoryFragment* m_root;  // root of free entry list

public:
    // c'tors / d'tor
    Memory ();
    Memory (int);
    ~Memory ();

    // getter / setter
    int Length ();

    // allocation / deallocation methods
    HANDLE Allocate (int);
    void Release (HANDLE);

    // read / write access
    void SetMemory (char);
    void WriteShort (HANDLE ofs, short value);
    void WriteInteger (HANDLE ofs, int value);
    void WriteString (HANDLE ofs, char* s);
    short ReadShort(HANDLE ofs) const;
    int ReadInteger(HANDLE ofs) const;

    // dump support 
    void DumpFreeMemory (ostream& os);
    void DumpMemory (ostream& os);

private:
    // internal helper methods
    void DumpLine (ostream& os, HANDLE ofs, int count);
};
