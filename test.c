#include <stdio.h>
#include "heap.h"

int comp(unsigned int a, unsigned int b)
{
	return a < b;
}

int main()
{
	heap pq;
	InitHeap(&pq, comp);
	
	PushHeap(&pq, 3343, 1, 0);
	PushHeap(&pq, 13, 1, 0);
	PushHeap(&pq, 241, 1, 0);
	PushHeap(&pq, 54, 1, 0);
	PushHeap(&pq, 2324, 1, 0);
	PushHeap(&pq, 3, 1, 0);
	
	while(!EmptyHeap(&pq))
	{
		printf("%d\n", FrontHeapValue(&pq));
		PopHeap(&pq);
	}
	ClearHeap(&pq);
	return 0;
}
