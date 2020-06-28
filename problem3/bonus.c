#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/ipc.h>
#include<pthread.h>
int arr[10007];
typedef struct unit
{
	int low;
	int high;
}unit;
void selectionSort(int l,int r)
{
	int index=l;
	int temp;
	int i,j;
	for(i=l;i<r;i++)
	{
		int index=i;
		for(j=i+1;j<=r;j++)
		{
			if(arr[j]<arr[index])
			{
				index=j;
			}
		}
		temp=arr[index];
		arr[index]=arr[i];
		arr[i]=temp;
	}
	return;
}
void merge(int l, int m, int r)
{

  int n1=m-l+1;
  int n2=r-m;
  int L[n1], R[n2];
  for (int i = 0; i < n1; i++)
	{
      L[i] = arr[l+i];
	}
  for (int j=0; j<n2; j++)
	{
      R[j] = arr[m+1+j];
	}
  int i,j;
	i=j=0;
  int k=l;
  while (i<n1&&j<n2)
  {
    if (L[i]<=R[j])
    {
        arr[k]=L[i];
          i++;
    }
    else
    {
        arr[k]=R[j];
        j++;
    }
    k++;
  }
  while (i<n1)
  {
      arr[k]=L[i];
      i++;
      k++;
  }
  while (j<n2)
  {
      arr[k]=R[j];
      j++;
      k++;
	}
	return;
}
void * mergeSort(void * args)
{
	unit * parent=(unit *)args;
	int l=parent->low;
	int r=parent->high;
	if(l<r)
	{
    int mid = r+(l-r)/2;
    if(l-r<=5)
    {
				selectionSort(l,r);
        return NULL;
    }
    else if (l<r)
    {
			unit left,right;
			left.low=l;
			left.high=mid;
			right.low=mid+1;
			right.high=r;
			pthread_t leftchild;
			pthread_t rightchild;
			pthread_create(&leftchild,NULL,mergeSort,(void *)&left);
			pthread_create(&rightchild,NULL,mergeSort,(void *)&right);
			pthread_join(leftchild,NULL);
			pthread_join(rightchild,NULL);
      merge(l, mid, r);
			pthread_exit(NULL);
    }
	}
	else
	{
		return NULL;
	}
	return NULL;
}
int main()
{
  int i,n;
  scanf("%d",&n);
  for(i=0;i<n;i++)
  {
      scanf("%d",&arr[i]);
  }
	unit root;
	root.low=0;
	root.high=n-1;
	pthread_t start;
	pthread_create(&start,NULL,mergeSort,(void *)&root);
	pthread_join(start,NULL);
  for (i=0; i < n; i++)
  {
      printf("%d ",arr[i]);
  }
  printf("\n");
  return 0;
}
