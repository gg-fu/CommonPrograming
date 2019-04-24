/************************************************
 *	Title: Merge
 *	Describe:
 *		Average complexity  nlogn
 *		performance	stable
 *	Autor:BruvinLu
 *	Time:2019-4-24
************************************************/
#include <iostream>
#include <string>

using namespace std;

void merge(int *s,int start,int mid,int end)
{
	int *tmp = new int[end - start +1];
	if(tmp == NULL)
	{
		cout << "new int failed"<<endl;
		return;
	}
	int i = start;
	int j = mid+1;
	int k = 0;
	while(i <= mid && j <= end)
	{
		if(s[i] < s[j])
		{
			tmp[k++] = s[i++];
		}
		else
			tmp[k++] = s[j++];
	}
	while(i <= mid)
	{
		tmp[k++] = s[i++];
	}
	while(j <= end)
	{
		tmp[k++] = s[j++];
	}

	for(i = 0;i<k;i++)
	{
		s[i+start] = tmp[i];
	}
	delete []tmp;
}

void Merge_Sort(int *s,int start,int end)
{
	if(s == NULL)
		return;
	if(start < end)
	{
		int mid = (start+end)/2;
		Merge_Sort(s,start,mid);
		Merge_Sort(s,mid+1,end);
		merge(s,start,mid,end);
	}
}
int main(int argc,char *argv[])
{
	int buf[] = {3,7,4,1,0,9,7,5,2,15,23,12,10};
	int len = sizeof(buf)/sizeof(int);
	
	cout <<"before sort:";
	for(int i=0;i<len;i++)
		cout<<buf[i]<<"\t";
	cout<<"\n";

	Merge_Sort(buf,0,len-1);

	cout <<"after sort:";
	for(int i=0;i<len;i++)
		cout<<buf[i]<<"\t";
	cout<<"\n";
	return 0;
}
