#include <iostream>
#include <string.h>

using namespace std;

void Fibonacci(int *s,int len)
{
	s[0] = 0;
	s[1] = 1;
	for(int i=2;i<len;i++)
	{
		s[i] = s[i-1] +s[i-2];
	}
}

int search(int s[],int len,int number)
{
	int low = 0;
	int high = len - 1;
	int F[20];
	Fibonacci(F,19);
	
	int k = 0;
	while(len > F[k] - 1)
		k++;

	int *temp = new int[F[k]-1];
	memcpy(temp,s,len*sizeof(int));
	for(int i=len;i<F[k]-1;i++)
		temp[i]=s[high];
	while(low<=high)
	{
		int mid = low +F[k-1]-1;
		if(s[mid] > number)
		{
			high = mid -1;
			k -=1;
		}
		else if(s[mid] < number)
		{
			low = mid + 1;
			k -= 2;
		}
		else
		{
			if(mid < len)
				return mid;
			else
				return len-1;
		}
	}
	delete[] temp;
	return -1;
}
int main(int argc,char *argv[])
{
	int buf[] = {0,6,9,23,45,67,89,100};
	int len = sizeof(buf)/sizeof(int);

	int ret =search(buf,len,45);
	cout << "the result is:"<< ret <<endl;
	return 0;
}

