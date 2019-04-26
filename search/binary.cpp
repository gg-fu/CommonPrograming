/************************************************
 *      Title: binary search
 *      Describe:
 *              Time-complexity: O(log2n)
 *      Autor:BruvinLu
 *      Time:2019-4-26
************************************************/

#include <iostream>

using namespace std;

int binary_search(int s[],int start,int end,int number)
{
	int r = end;
	int l = start;
	int mid = (r+l)/2;
	if(s[mid] == number)
		return mid;
	else if(s[mid] > number)
	{
		binary_search(s,l,mid-1,number);
	}
	else if(s[mid] < number)
	{
		binary_search(s,mid+1,r,number);
	}
}

int binary_search_(int s[],int len,int number)
{
	int r= len;
	int l = 0;
	while(l < len)
	{	
		int mid = (r+l)/2;
		if(s[mid] == number)
			return mid;
		else if(s[mid] > number)
			r = mid - 1;
		else
			l = mid + 1;
	}
}

int main(int argc,char *argv[])
{
	int buf[] = {1,2,3,4,5,6,7,8,9};
	int ret = binary_search_(buf,9,8);
	//int ret = binary_search(buf,0,9,8);
	cout << ret <<endl;
	return 0;
}
