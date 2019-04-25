/************************************************
 *      Title: heap_sort
 *      Describe:
 *              Average complexity  O(nlog2n)
 *              performance     Instable
 *      Autor:BruvinLu
 *      Time:2019-4-24
************************************************/

#include <iostream>
#include <stack>
#include <string.h>
using namespace std;

class Heap{
	public:
		Heap(){
			cout<<"starting"<<endl;
		}
		~Heap(){
			cout<<"ending"<<endl;
		}
		int Heap_Sort(int *s,int n);
		int Heap_Adjust(int *s,int head,int n);
		void TEST(int s[],int n);
	private:
		int Swap(int s[],int a,int b)
		{
			int tmp = s[a];
			s[a] = s[b];
			s[b] = tmp;
		}
};

int Heap::Heap_Adjust(int *s,int head,int n)
{
	int tmp = s[head];
	int k;
	for(k=2*head+1;k<n;k=k*2+1)
	{
		if(k+1 < n && s[k] < s[k+1])
		{
			k++;
		}
		if(s[k] > tmp)
		{
			s[head] = s[k];
			head = k;
		}
		else
			break;
		cout<<"hello:"<<k<<endl;
	}
	s[head] = tmp;
}

int Heap::Heap_Sort(int *s,int n)
{
	for(int i=n/2-1;i>=0;i--)
	{
		Heap_Adjust(s,i,n);
	}
	for(int j=n-1;j>=1;j--)
	{
		Swap(s,0,j);
		Heap_Adjust(s,0,j);
	}
	return 0;
}


void Heap::TEST(int s[],int n)
{
	cout<<"before sort:";
	for(int i=0;i<n;i++)
	{
		cout<<s[i]<<"\t";
	}
	cout<<"\n";
	Heap_Sort(s,n);
	cout<<"after sort:";
	for(int i=0;i<n;i++)
	{
		cout<<s[i]<<"\t";
	}
	cout<<"\n";
}

int main(int argc,char *argv[])
{
	Heap h;
	int buf[] = {4,6,8,5,9};
	int len = sizeof(buf)/sizeof(int);
	h.TEST(buf,len);
	return 0;
}
