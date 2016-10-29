#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
const int size=250;
char Stack[50];
int top=-1;
char post[100];
int Push(char ele)
{
	if (top==size-1)
		return -1;
	else if (top==-1)
	{top=0;
	 Stack[top]=ele;
	}
	else
	{
		++top;
		Stack[top]=ele;
	}
	return 0;
}

char Pop()
{
	char ret;
	if(top==-1)
		return 'X';
	else
	{ret=Stack[top];
	 top--;
	}
	return ret;
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
    printf("%s\n",arr);
}
void makePrefix(char expr[])
{
	int i,j=0,c;
	char ch;
    reverse(expr);
    for(i=0;expr[i]!='\0';++i)
    {
        if(expr[i]=='(')
        {expr[i]=')'; continue;}
        if(expr[i]==')')
            expr[i]='(';
    }
    printf("%s\n",expr);
	for(i=0;expr[i]!='\0';++i)
		{
			if(isalpha(expr[i]))//>='0' && expr[i]<='9')
			{
				post[j]=expr[i];
				++j;
			}
			else if(expr[i]=='(' || expr[i]=='+' || expr[i]=='*'||expr[i]=='~'||expr[i]=='>' )
			{
				c=Push(expr[i]);
				if(c==-1) break;
			}
			else if(expr[i]==')')
			{
				while(Stack[top]!='(')
				{
					ch=Pop();
					post[j]=ch;
					++j;
				}
				ch=Pop();
			}
			else continue;
		}
    post[j]='\0';
    printf("%s\n",post);
    reverse(post);
    printf("%s\n",post);
}

int main()
{
	char expr[100];
	scanf("%s",expr);
	makePrefix(expr);
    
    return 0;
}
