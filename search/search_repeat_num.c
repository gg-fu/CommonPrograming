/************************************************
 *      Title: search_repeat_num
 *      Describe:
 *  		Time complexity  O(n)
 *
 *      Autor:BruvinLu
 *      Time:2019-4-27
*************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int do_dup(int a[],int n)
{
	int i,count=0,tmp=a[0];
	int result = -1;
	for (i=1;i<n;i++)
	{
		if(a[i]>tmp)
			tmp=a[i];
	}
	int *b=malloc(tmp*sizeof(int));

	for(i=0;i<n;i++)
	{
		if(b[a[i]]==0)
		{
			if(a[i]==0)
			{
				count+=1;
			}
			else
				b[a[i]]=a[i];
			if(count>=2)
			{
				result =0;
				break;
			}
		}
		else
		{
			result =b[a[i]];
			break;
		}
	}
	free(b);
	return result;
}

int main(int argc,char *argv[])
{
	int test[]={0,2,2,6,7,8,9,3};
	int res = do_dup(test,8);
	printf("res:%d\n",res);
	return 0;
}
