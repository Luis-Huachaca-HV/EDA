#include <iostream>
#include "vector"
using namespace std;



class CVector
{
private:
    int* m_pVect = NULL; // Pointer to the buffer
int m_nCount = 0, // Control how many elements are actually used
m_nMax=0, // Control how many are allocated as maximum
m_nDelta=10; // To control the growing
void Init(int delta); // Init our private variables, etc
void Resize(); // Resize the vector when occurs an overflow
public:
CVector(int delta = 10); // Constructor
void Insert(int elem); // Insert a new element
// More methods go here
};


void CVector::Init(int delta)
{
}

void CVector::Resize()
{
    m_nDelta = 10;
    m_pVect = (int*)realloc(m_pVect, sizeof(int) * (m_nMax + m_nDelta));
}

CVector::CVector(int delta)
{
    delta = 10;
}

void CVector::Insert(int elem)
{
    if (m_nCount == m_nMax) // Verify the overflow
        Resize(); // Resize the vector before inserting elem
    m_pVect[m_nCount++] = elem; // Insert the element at the end
}

int main() {

    CVector a;
   
    
    a.Insert(1);
    a.Insert(2);
    a.Insert(3);

    cout << "Hello, World!" << endl;

    return 0;
}
