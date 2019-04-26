/************************************************
 *      Title: order search
 *      Describe:
 *              Time-complexity: O(n)
 *      Autor:BruvinLu
 *      Time:2019-4-26
************************************************/

#include <iostream>
#include <string>
using namespace std;

int order_search(int s[],int len,int number)
{
	int i;
	for(i=0;i<len;i++)
	{
		if(s[i] == number)
			return i;
	}
}

int main(int argc,char *argv[])
{
	int buf[] = {6,3,2,8,0,1,8};
	int len = sizeof(buf)/sizeof(int);
	int ret = order_search(buf,len,6);
	cout<<"the result is:"<<ret<<endl;
	return 0;
}
