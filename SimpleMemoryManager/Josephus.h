class Josephus
{
private:
    static const int DefaultPassBy = 10;  // default "decimatio"

private:
    // memory object
    Memory m_mem;

    // root of soldiers
    HANDLE  m_root;        // root of linked list of soldiers
    HANDLE  m_current;     // current soldier

    int m_count;           // total number of soldiers
    int m_alive;           // number of alive soldiers
    int m_lastEliminated;  // last eliminated soldier
    int m_passby;          // "decimatio"

public:
    // c'tor(s), d'tor
    Josephus ();
    Josephus (int);
    ~Josephus (); 

    // getter/setter
    int Count () const { return m_count; }
    int Alive () const { return m_alive; }
    int LastEliminated () { return m_lastEliminated; }
    int LastAlive () { return m_mem.ReadShort (m_current); } 
    int PassBy () { return m_passby; }
    void SetPassBy (int passby) { m_passby = passby; }

    // public interface
    bool EliminateNextSoldier();
    void EliminateAll();

private:
    // private helper methods
    void CreateLinkedList();

    // operators
    friend ostream& operator<< (ostream&, const Josephus&);
};