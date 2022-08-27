#include <iostream>
using namespace std;
// 3.1.1
// primera estructura de datos
/*
int gVect[100];
int gnCount;

void Insert(int elem) {
  if (gnCount < 100)
    gVect[gnCount++] = elem;
}
//main
  for (int i = 0; i < 100; i++) {
    Insert(i);
  }
  cout << "holi";
*/
// 3.1.2
/*
int* gpVect = NULL;
int gnCount = 0;
int gnMax = 0;

void Resize() {
    const int delta = 10;
    //gpVect = realloc(gpVect,sizeof(int)*gnMax+delta));
    int* pTemp, i;
    pTemp = new int[gnMax + delta];
    for (i = 0; i < gnMax; i++)
        pTemp[i] = gpVect[i];
    delete[] gpVect;
    gpVect = pTemp;
    gnMax += delta;
}
void Insert(int elem) {
    if (gnCount == gnMax)
        Resize();
    gpVect[gnCount++] = elem;
}
*/
int main() {

  // Vector b 3.1.2;
  // Insert(2);
  // Insert(3);
  // Insert(4);
  // Insert(5);
  // cout << "hOLI";

  // 3.1.1
  // for (int i = 0; i < 100; i++) {
  // Insert(i);
  // }
  // cout << "holi";

  // CVector<int> a;
  // a.Insert(1);
  // a.Insert(2);
  // a.Insert(3);

  return 0;
}