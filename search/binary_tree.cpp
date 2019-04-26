/************************************************
 *      Title: binary tree search
 *      Describe:
 *              Time-complexity: O(n)
 *      Autor:BruvinLu
 *      Time:2019-4-26
************************************************/

#include<iostream>
#include<string.h>
using namespace std;
 
struct TrieNode{
    int num;
    TrieNode *child[26];
    TrieNode()
    {
        num=0;
        memset(child,0,sizeof(child));
    }
};
 
TrieNode* root;
int temp;
 
//将字符串word添加到字典树中 
void insert(string word)
{
    if(word.empty()) return;
 
    TrieNode* p=root;
    for(int i=0;i<word.size();++i)
    {
        temp=word[i]-'a';
        if(p->child[temp]==NULL)
        {
            p->child[temp]=new TrieNode;//若节点不存在，则建立一个新节点
        }
        p=p->child[temp];
        p->num++;
    }
}
 
//返回所有以字符串pre为前缀的单词数量
int prefixNumber(string pre)
{
    if(pre.empty()) return 0;
 
    TrieNode* p=root;
    for(int i=0;i<pre.length();++i)
    {
        temp=pre[i]-'a';
        if(p->child[temp]==NULL)  
            return 0;
        p=p->child[temp];
    }
    return p->num;
}

/******************************************
 * input:n 		(the numbers of words)
 * 	 word 		(input n words for building Thesaurus)
 * 	 m 		(number of words to look for)
 *	 search_word  	(enter the words you want to find)
 * output:res		(Number of occurrences of words in the lexicon) 
******************************************/
int main()
{
    int n,m;
    string word,pre;
    root=new TrieNode;
    while(cin>>n)
    {
        while(n--)
        {
            cin>>word;
            insert(word);
        }
        cin>>m;
        while(m--)
        {
            cin>>pre;
            int res=prefixNumber(pre);
            cout<<res<<endl;
        }
    }
    return 0;
}

