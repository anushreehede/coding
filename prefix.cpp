#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
char prefix[100];
struct preStack
{
    char elt;
    preStack* next;
};
struct preStack* preTop = NULL;

preStack* Create_preStack_node(char x)
{
    preStack* ptr = new preStack;
    ptr->elt = x;
    ptr->next = NULL;
    return ptr;
}
void prePush(preStack* ptr)
{
    if(preTop==NULL)
     preTop = ptr;
    else
    {
        preStack* save = preTop; 
        preTop = ptr; 
        ptr->next=save;
    }
}
char prePop()
{
    if(preTop == NULL)
     return -1;
    else
    {
		char ch = preTop->elt;
        preStack* save = preTop;
        preTop = preTop->next;
        delete save;
		return ch;
    }
}

void reverse(char arr[])
{
    int i;
    int l=strlen(arr);
    for(i=0;i<l/2;++i)
    {
        char temp=arr[i];
        arr[i]=arr[l-1-i];
        arr[l-1-i]=temp;
    }
    //std::cout<<arr<<"\n";
}
void makePrefix(char expr[])
{
	int i,j=0;
    int c;
	char ch;
    reverse(expr);
    for(i=0;expr[i]!='\0';++i)
    {
        if(expr[i]=='(')
        {expr[i]=')'; continue;}
        if(expr[i]==')')
            expr[i]='(';
    }
    //std::cout<<expr<<"\n";
	for(i=0;expr[i]!='\0';++i)
		{
			if(isalpha(expr[i]))//>='0' && expr[i]<='9')
			{
				prefix[j]=expr[i];
				++j;
			}
			else if(expr[i]=='(' || expr[i]=='+' || expr[i]=='*'||expr[i]=='~'||expr[i]=='>' )
			{
                preStack* ptr = Create_preStack_node(expr[i]);
				prePush(ptr);
			}
			else if(expr[i]==')')
			{
				while(preTop->elt!='(')
				{
					ch = prePop();
					prefix[j]=ch;
					++j;
				}
				ch = prePop();
			}
			else continue;
		}
    prefix[j]='\0';
    std::cout<<prefix;
    reverse(prefix);
    std::cout<<prefix;
}
int main()
{
	char expr[100];
	std::cin>>expr;
	makePrefix(expr);
	return 0;
}
