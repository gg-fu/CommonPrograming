/************************************************
 *      Title: quick_sort
 *      Describe:
 *              Average complexity  O(nlog2n),Worst O(n2) 
 *              performance     Instable
 *      Autor:BruvinLu
 *      Time:2019-4-24
************************************************/

#include <iostream>
#include <stack>
#include <string.h>
using namespace std;

void Quick_Sort(int s[],int n)
{
	int l = 0;
	int r = n - 1;
	int start,end;
	stack<int> S;
	S.push(l);
	S.push(r);
	while(!S.empty())
	{
		r = S.top();
       		S.pop();
		l = S.top();
		S.pop();
		int tmp = s[l];
		start = l;
		end = r;
		while(l < r)
		{
			while(l < r && tmp <= s[r])
				r--;
			if(l < r)
			{
				s[l++] = s[r];
			}
			
			while(l < r && tmp >= s[l])
				l++;
			if(l < r)
			{
				s[r--] = s[l];
			}
		}
		s[l] = tmp;
		if(l-1 > start)
		{
			S.push(0);
			S.push(l-1);
		}
		if(l+1 < end)
		{
			S.push(l+1);
			S.push(n-1);		
		}
	}
}

void Qucik_Sort_Recursion(int s[],int start,int end)
{
	int l = start;
	int r = end - 1;
	int tmp = s[l];
	while(l < r)
	{
		while(l < r && tmp <= s[r])
			r--;
		if(l < r)
		{
			s[l++] = s[r];
		}
		
		while(l < r && tmp >= s[l])
			l++;
		if(l < r)
		{
			s[r--] = s[l];
		}
	}
	s[l] = tmp;
	if(l-1 > start)
		Qucik_Sort_Recursion(s,0,l-1);
	if(l+1 < end)
		Qucik_Sort_Recursion(s,l+1,end);
}

void TEST(int s[],int n)
{
	cout<<"before sort:";
	for(int i=0;i<n;i++)
	{
		cout<<s[i]<<"\t";
	}
	cout<<"\n";
	//Qucik_Sort_Recursion(s,0,n);
	Quick_Sort(s,n);
	cout<<"after sort:";
	for(int i=0;i<n;i++)
	{
		cout<<s[i]<<"\t";
	}
	cout<<"\n";
}

int main(int argc,char *argv[])
{
	int buf[] = {2,5,1,0,9,7,4,2,6,3,13,11};
	int len = sizeof(buf)/sizeof(int);
	TEST(buf,len);
	return 0;
}
