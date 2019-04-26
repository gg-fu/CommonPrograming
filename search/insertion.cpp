/************************************************
 *      Title: insertiont search
 *      Describe:
 *              Time-complexity: O(log2(log2n))
 *      Autor:BruvinLu
 *      Time:2019-4-26
************************************************/

#include <iostream>
#include <string>

using namespace std;

int Insertion_Search(int s[],int low,int high,int number)
{
	int mid = low + (number - s[low])/((s[high] - s[low])*(high - low));
	if(s[mid] == number)
		return mid;
	else if(s[mid] < number)
		Insertion_Search(s,mid+1,high,number);
	else if(s[mid] > number)
		Insertion_Search(s,low,mid-1,number);
}
int main(int aegc,char *argv[])
{
	int buf[] = {1,2,4,7,9,12,34};
	int len = sizeof(buf)/sizeof(int);

	int ret = Insertion_Search(buf,0,len-1,9);
	cout <<"the result is:"<<ret<<endl;
	return 0;
}
