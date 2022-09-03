#include <iostream>
typedef long long ll;
using namespace std;
#include <math.h>
#include <list>
#include <vector>
#include <iterator>

//Parte 3.4
/*
template <typename Type> class CVector {
private:
    Type* m_pVect=NULL;
    int m_nCount,
        m_nMax,           // Control how many are allocated as maximum
        m_nDelta = 10;    // To control the growing
    void Init(int delta); // Init our private variables, etc
    void Resize();

public:
    CVector(int delta = 10); // Constructor
    void Insert(Type& elem);
};
template <typename Type>void CVector<Type>::Init(int delta) {}
template <typename Type>void CVector<Type>::Resize() {
    m_nDelta = 10;
    m_pVect = (Type*)realloc(m_pVect, sizeof(Type) * (m_nMax + m_nDelta));
}
template <typename Type>CVector<Type>::CVector(int delta)
{
    delta = 10;
}
template <typename Type> void CVector<Type>::Insert(Type& elem) {
    if (m_nCount == m_nMax)     // Verify the overflow
        Resize();                 // Resize the vector before inserting elem
    m_pVect[m_nCount++] = elem; // Insert the element at the end
}

*/

//parte 3.5->fig19
/*
template <typename Container> void Write(Container& ds, ostream& os){
    Container::template iterator iter = ds.begin();
    for (; iter != ds.end(); iter++)
        os << *iter << "\n";
}*/


//parte 3.5 -> fig20
/*
class CMyComplexDataStructure
{
    vector<float> m_container;
    public:
        void Insert(float fVal) { m_container.push_back(fVal); }
        template <typename objclass>
        void sumone(objclass funobj)
    {
        vector<float>::iterator iter = m_container.begin();
        for (; iter != m_container.end(); iter++)
            funobj(*iter);
    }
};
template <typename objclass>
class funcobjclass
{
public:
    void operator ()(objclass& objinstance)
    {
        objinstance++;
    }
};*/


int main() {
    //3.4
    /*
    CVector<int> a;
    int d, b, c;
    d, b, c = 1;
    a.Insert(d);
    a.Insert(b);
    a.Insert(c);*/
	
    //3.5 fig19
    /*
    list<float> mylist;
    vector<float> myvector;
    for (int i = 0; i < 10; i++)
    {
        mylist.push_back(i);
        myvector.push_back(i + 50);
    }
    Write(mylist, cout);
    Write(myvector, cout);
    return 0;*/


    //3.5->20
    /*
    CMyComplexDataStructure ds;
    ds.Insert(3.5);
    ds.Insert(4.5);
    ds.Insert(6.5);
    ds.Insert(3.9);
    funcobjclass<float> x;
    ds.sumone(x);
    return 0;*/


    
}
