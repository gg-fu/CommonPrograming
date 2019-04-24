/************************************************
 *      Title: bubbling
 *      Describe:
 *              Average 	O(n2)
 *              performance    stable
 *      Autor:BruvinLu
 *      Time:2019-4-24
************************************************/

#include <iostream>
#include <stdbool.h>
#include <string.h>
using namespace std;

void bubbling(int s[],int n)
{
	int tmp;
	bool flag = false;
	for(int i=0;i<n-1;i++)
	{
		for(int j=n-2;j>=i;j--)
		{
			if(s[j] > s[j+1])
			{
				tmp = s[j];
				s[j] = s[j+1];
				s[j+1] = tmp;
				flag = true;	
			}
		}
		if(!flag)
		{
			cout<<"has been sorted"<<endl;
			break;
		}
	}
}

int main(int argc,char *argv[])
{
	int buf[] = {1,2,3,4,5,6,7,8,9};
	int len = sizeof(buf)/sizeof(int);
	cout<<"before sort:";
	for(int i=0;i<len;i++)
		cout<<buf[i]<<"\t";
	cout<<"\n";

	bubbling(buf,len);
	cout<<"after sort:";
	for(int i=0;i<len;i++)
		cout<<buf[i]<<"\t";
	cout<<"\n";
	return 0;
}
