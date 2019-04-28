/************************************************
*        Title: back_search
*        Describe:
*                  Time complexity  O(n)
*     
*        Autor:BruvinLu
*        Time:2019-4-27
**************************************************/

#include <iostream>
#include <stdbool.h>
#include <string.h>

using namespace std;
bool has_string_core(char *matrix,char *str,int &count,int rows,int cols,int row,int col,bool *visited);

bool has_string(char *matrix,char *str,int rows,int cols)
{
	if(str==NULL || matrix == NULL || rows <= 0 || cols <= 0)
		return false;
	bool *visited = new bool[rows*cols];
	memset(visited,0,rows*cols);
	int count = 0;
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
		{
			if(has_string_core(matrix,str,count,rows,cols,i,j,visited))
			{
				return true;
			}
		}
	}
	delete[] visited;
	return false;
}

/**************************************
 *	name:has_string_core
 *	param:	matrix	input matrix
 *		str	will be found string
 *		count	matched count
 *		rows	matrix row
 *		cols	matrix col
 *		row	X-axis
 *		col	Y-axis
 *		visited	Record 
**************************************/
bool has_string_core(char *matrix,char *str,int &count,int rows,int cols,int row,int col,bool *visited)
{
	if(str[count] == '\0')
		return true;
	bool flag = false;
	if(row>=0 && row<rows
		&& col >=0 && col < cols
		&& matrix[row*cols+col] == str[count]
		&& !visited[row*cols+col])
	{
		visited[row*cols+col] = true;
		++count;
		flag = has_string_core(matrix,str,count,rows,cols,row,col+1,visited)
			||has_string_core(matrix,str,count,rows,cols,row-1,col,visited)
			||has_string_core(matrix,str,count,rows,cols,row+1,col,visited)
			||has_string_core(matrix,str,count,rows,cols,row,col-1,visited);
		if(!flag)
		{
			--count;
			visited[row*cols+col]=false;
		}
	}
	return flag;
}

int main(int argc,char *argv[])
{
	char buf[] ="abcdefghijklmnop";
	char str[] = "aefjn";
	bool res = has_string(buf,str,4,4);
	if(res)
		cout<<"find:"<<str<<endl;
	else
		cout<<"can not find!"<<endl;
	return 0;
}
