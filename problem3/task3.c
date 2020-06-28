#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/ipc.h>

int *arr= NULL;
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

  int n1 = m-l+1;
  int n2 =  r-m;
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
  while (i < n1 && j < n2)
  {
    if (L[i] <= R[j])
    {
        arr[k] = L[i];
          i++;
    }
    else
    {
        arr[k] = R[j];
        j++;
    }
    k++;
  }
  while (i < n1)
  {
      arr[k] = L[i];
      i++;
      k++;
  }
  while (j < n2)
  {
      arr[k] = R[j];
      j++;
      k++;
	}
	return;
}
void mergeSort(int l, int r)
{
	if(l<r)
	{
    int mid = r+(l-r)/2;
    if(l-r<=5)
    {
				selectionSort(l,r);
        return;
    }
    else if (l < r)
    {
        pid_t fork1,fork2;
        fork1 = fork();
				fork2 = fork();
        if(fork1 < 0)
        {
            perror("Couldn't fork");
						exit(1);
        }
        else if(fork1==0)
        {
            mergeSort(l, mid);
            exit(0);
        }
        if(fork2 < 0)
        {
            perror("Couldn't fork");
        }
				else if(fork2 == 0)
				{
						mergeSort(mid+1, r);
						exit(0);
				}
        int s;
        waitpid(fork1, &s, 0);
        waitpid(fork2, &s, 0);
        merge(l, mid, r);
			}
    }
		else
		{
			return;
		}
}
int main()
{
  int shmid;
  int i,n;
  scanf("%d",&n);
	key_t key = IPC_PRIVATE;
	shmid = shmget(key, 10*n, IPC_CREAT| 0777);
	if(shmid == -1)
	{
		perror("Shmget didn't work");
		exit(1);
	}
  arr = shmat(shmid, 0, 0);
  for(i=0;i<n;i++)
  {
      scanf("%d",&arr[i]);
  }
  mergeSort(0, n-1);
  for (i=0; i < n; i++)
  {
      printf("%d ",arr[i]);
  }
  printf("\n");
  return 0;
}
