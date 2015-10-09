#include <iostream>
using namespace std;

#include "Memory.h"
#include "Josephus.h"

void Test_MemoryBasics_01 ();
void Test_MemoryBasics_02 ();
void Test_MemoryBasics_03();

void Test01  ();
void Test02  ();
void Test03  ();
void Test03a ();
void Test04  ();
void Test05  ();

void TestJosephus01();
void TestJosephus02();
void TestJosephus03();

void main ()
{
	Test_MemoryBasics_01 ();
	Test_MemoryBasics_02 ();
	Test_MemoryBasics_03();

	Test01 ();
	Test02 ();
	Test03 ();
	Test03a();
	Test04 ();
	Test05 ();

	TestJosephus01();
	TestJosephus02();
	TestJosephus03();

	getchar();
}

void Test01 ()
{
    // initialize memory object
    Memory mem (32);
    mem.DumpFreeMemory (cout);

    int request = 6;
    cout << "Allocate: " << request << endl;
    HANDLE n = mem.Allocate (request);
    cout << "n = " << n << endl;
    mem.DumpFreeMemory (cout);

    cout << "Allocate: " << endl;
    request = 8;
    HANDLE m = mem.Allocate (request);
    cout << "m = " << n << endl;
    mem.DumpFreeMemory (cout);

    cout << "Release: " << n << endl;
    mem.Release (n);
    mem.DumpFreeMemory (cout);

    cout << "Release: " << m << endl;
    mem.Release (m);
    mem.DumpFreeMemory (cout);
}

void Test02 ()
{
    // initialize memory object
    Memory mem (128);

    HANDLE n = mem.Allocate (18);
    cout << "n=" << n << endl;
    n = mem.Allocate (28);
    cout << "n=" << n << endl;
    n = mem.Allocate (8);
    cout << "n=" << n << endl;
    n = mem.Allocate (18);
    cout << "n=" << n << endl;
    n = mem.Allocate (28);
    cout << "n=" << n << endl;
    n = mem.Allocate (8);
    cout << "n=" << n << endl << endl;

    mem.DumpFreeMemory (cout);
}

void Test03 ()
{
    Memory mem (128);
    mem.DumpFreeMemory (cout);

    HANDLE n;
    n = mem.Allocate (8);
    cout << "n=" << n << endl;
    n = mem.Allocate (8);
    cout << "n=" << n << endl;
    n = mem.Allocate (8);
    cout << "n=" << n << endl;
    n = mem.Allocate (8);
    cout << "n=" << n << endl;
    n = mem.Allocate (8);
    cout << "n=" << n << endl;
    n = mem.Allocate (8);
    cout << "n=" << n << endl;
    n = mem.Allocate (8);
    cout << "n=" << n << endl;
    n = mem.Allocate (8);
    cout << "n=" << n << endl;
    n = mem.Allocate (8);
    cout << "n=" << n << endl;
    n = mem.Allocate (8);
    cout << "n=" << n << endl;
    n = mem.Allocate (8);
    cout << "n=" << n << endl;
    n = mem.Allocate (8);
    cout << "n=" << n << endl;
    n = mem.Allocate (6);
    cout << "n=" << n << endl;

    mem.DumpFreeMemory (cout);

    cout << "Release " << endl;
    mem.Release (30);
    mem.DumpFreeMemory (cout);

    cout << "Release " << endl;
    mem.Release (80);
    mem.DumpFreeMemory (cout);

    cout << "Release " << endl;
    mem.Release (70);
    mem.DumpFreeMemory (cout);

    cout << "Release " << endl;
    mem.Release (60);
    mem.DumpFreeMemory (cout);

    cout << "Release " << endl;
    mem.Release (50);
    mem.DumpFreeMemory (cout);

    cout << "Release " << endl;
    mem.Release (40);
    mem.DumpFreeMemory (cout);

    //cout << "Release " << endl;
    //mem.Release (30);
    //mem.DumpFreeMemory (cout);
}

void Test03a ()
{
    Memory mem (128);
    mem.DumpFreeMemory (cout);

    HANDLE n;

    for (int i = 0; i < 12; i ++)
    {
        n = mem.Allocate (8);
        cout << "Allocate: n=" << n << endl;
    }

    // allocate final remaining 6(+2) bytes
    n = mem.Allocate (6);
    cout << "Allocate: n=" << n << endl;
    mem.DumpFreeMemory (cout);

    for (int i = 1; i <= 12; i ++)
    {
        cout << "Release: " << i * 10 << endl;
        mem.Release (i * 10);
    }    

    cout << "Release: " << 2 << endl;
    mem.Release (2);
    mem.DumpFreeMemory (cout);
}

void Test04 ()
{
	/*
     * Testing single great block
     */

	// initialize memory object
    Memory mem;
    mem.DumpFreeMemory (cout);

	HANDLE n = mem.Allocate (mem.Length() - 2);
    cout << "n=" << n << endl;
    mem.DumpFreeMemory (cout);

	mem.Release (n);
    mem.DumpFreeMemory (cout);
}

void Test05()
{
	// Testing single great block
	// initialize memory object
    Memory mem;

	HANDLE n1 = mem.Allocate (62);
    cout << "n1=" << n1 << endl;
    mem.DumpFreeMemory (cout);

	HANDLE n2 = mem.Allocate (62);
    cout << "n2=" << n2 << endl;
    mem.DumpFreeMemory (cout);

	mem.Release (n1);
    mem.DumpFreeMemory (cout);

	mem.Release (n2);
    mem.DumpFreeMemory (cout);
}

void Test_MemoryBasics_01 ()
{
    Memory m (64);
    m.SetMemory ('!');
    m.DumpMemory (cout);
}

void Test_MemoryBasics_02 ()
{
    Memory m (32);
    m.SetMemory (' ');
    m.WriteString (0, "1234");
    m.WriteString (12, "6789");
    m.WriteString (20, "ABCDEFGH");
    m.WriteString (31, "Z");

    m.DumpMemory (cout);
}

void Test_MemoryBasics_03()
{
	// initialize memory object
	Memory mem(128);
	mem.DumpFreeMemory(cout);

	HANDLE n1 = mem.Allocate(62);
	cout << "n1 = " << n1 << endl;
	HANDLE n2 = mem.Allocate(62);
	cout << "n2 = " << n2 << endl;

	mem.DumpFreeMemory(cout);

	cout << "Release: " << n1 << endl;
	mem.Release(n1);
	mem.DumpFreeMemory(cout);

	cout << "Release: " << n2 << endl;
	mem.Release(n2);
	mem.DumpFreeMemory(cout);
}

// ================================================================

void TestJosephus01()
{
    Josephus j (10);
    j.SetPassBy (3);
    cout << j << endl;

    j.EliminateNextSoldier();
    cout << j << endl;
    j.EliminateNextSoldier();
    cout << j << endl;
    j.EliminateNextSoldier();
    cout << j << endl;
}

void TestJosephus02()
{
    Josephus j(41);
    j.SetPassBy(10);

    cout << "Number of soldiers: " << j.Count() << endl;
    cout << "Eliminating: Each " << j.PassBy() << ". soldier" << endl << endl;

    while (j.Alive() > 1)
    {
        j.EliminateNextSoldier();
        cout << "Removed ";
        cout.width(2);
        cout << j.LastEliminated() << "  " << j  << endl;
    }

    cout << endl;
    cout << "Last soldier: " << j.LastAlive() << endl;
    cout << "Last eliminated: " << j.LastEliminated() << endl;
}

void TestJosephus03()
{
    Josephus j(41);
    j.SetPassBy(3);

    cout << "==> Number of soldiers: " << j.Count() << endl;
    cout << "==> Eliminating: Each " << j.PassBy() << ". soldier" << endl << endl;

    while (j.Alive() > 1)
    {
        j.EliminateNextSoldier();
        cout << "==> Removed ";
        cout.width(2);
        cout << j.LastEliminated() << endl;
    }

    cout << endl;
    cout << "==> Last soldier: " << j.LastAlive() << endl;
    cout << "==> Last eliminated: " << j.LastEliminated() << endl;
}
