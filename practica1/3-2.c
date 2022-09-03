#include <stdio.h>

struct Vector {
  int *m_pVect, // Pointer to the buffer
      m_nCount, // Control how many elements are actually used
      m_nMax,   // Control how many are allocated as maximum
      m_nDelta; // To control the growing
};
void Resize(struct Vector *pThis) {
  pThis->m_pVect =
      realloc(pThis->m_pVect, sizeof(int) * (pThis->m_nMax + pThis->m_nDelta));
  // The Max has to be increased by delta
  pThis->m_nMax += pThis->m_nDelta;
}
void Insert(struct Vector *pThis, int elem) {
  if (pThis->m_nCount == pThis->m_nMax) // Verify the overflow
    Resize(pThis); // Resize the vector before inserting elem
  // Insert the element at the end of the sequence
  pThis->m_pVect[pThis->m_nCount++] = elem;
}

int main(void) {
  struct Vector a;
  a.m_pVect = NULL;
  a.m_nDelta = 10;
  a.m_nMax = 0;
  a.m_nCount = 0;
  Insert(&a, 1);
  printf("Hello World\n");
  return 0;
}
