#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<conio.h>

//task 1
char prefix[1000]; //stores the prefix expression used to make the tree

// Stack structure and methods to hold operators during infix to prefix conversion 
struct preStack
{
    char elt; // holds operator
    preStack* next;
};                 //Stack structure  
struct preStack* preTop = NULL; // top pointer to stack 

preStack* Create_preStack_node(char x) // creating node for stack 
{
    preStack* ptr = new preStack;
    ptr->elt = x;
    ptr->next = NULL;
    return ptr;
}
void prePush(preStack* ptr)  // pushing into stack
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
char prePop()              //popping from stack
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

void reverse(char arr[])  // reverses a string 
{
    int i;
    int l=strlen(arr);
    for(i=0;i<l/2;++i)
    {
        char temp=arr[i];
        arr[i]=arr[l-1-i];
        arr[l-1-i]=temp;
    }

}
void makePrefix(char expr[])  // conversion of infix expression to prefix expression
{
	int i,j=0;
	char ch;
    reverse(expr);              // reverses infix expression  
    for(i=0;expr[i]!='\0';++i)  // finishes interchanging ( and )
    {
        if(expr[i]=='(')
        {expr[i]=')'; continue;}
        if(expr[i]==')')
            expr[i]='(';
    }
    
    //now code to convert to postfix expression 
	for(i=0;expr[i]!='\0';++i)
	{
		if(isalpha(expr[i]))  // if expression contains an alphabet character
		{
			prefix[j]=expr[i]; // store character in postfix expression
			++j;
		}
		else if(expr[i]=='(' || expr[i]=='+' || expr[i]=='*'||expr[i]=='~'||expr[i]=='>' ) // if any of these
		{
            preStack* ptr = Create_preStack_node(expr[i]); // create a node in the stack
			prePush(ptr); // push the node into the stack
		}
		else if(expr[i]==')')  // if expression contains a )
		{
			while(preTop->elt!='(')   // continue popping elements from the stack until you reach a (
			{
				ch = prePop();
				prefix[j]=ch;
				++j;
			}
			ch = prePop(); // pop the (
		}
		else continue;
	}
    
    prefix[j]='\0';

    reverse(prefix); // reverse the obtained postfix expression to make it prefix 
}

//task 2

// node for creating the binary tree
struct node
{
    char atom;   // stores the propositional atom or the operator
    int boolean; // stores the truth value of the atom, -1 for operators
    node* left;
    node* right;
};

struct node* current = NULL; // holds the address of the last node which has been added

node* Create_Tree_Node(char x) // creates nodes in the binary tree
{
    node* ptr = new node; //allocating memory for the node
    ptr->atom=x; 
    ptr->left=ptr->right=NULL;
    return ptr;
}

struct list // structure to hold the list of atoms without duplicates
{
    char name; // name of the atom
    int boolean; // truth value of the atom
};
struct list atoms[100];
int n=0; // number of elements in atoms

// Stack structure and methods to hold a list of tree nodes whose element is an operator
struct Stack
{
    node* elt; // a pointer to a node of the binary tree
    Stack* next;
};
struct Stack* top = NULL; // points to the top of Stack

Stack* Create_Stack_node(node* x) // creating nodes in the stack
{
    Stack* ptr = new Stack;
    ptr->elt = x;
    ptr->next = NULL;
    return ptr;
}
void Push(Stack* ptr) // pushing nodes in the stack
{
    if(top==NULL)
     top = ptr;
    else
    {
        Stack* save = top; 
        top = ptr; 
        ptr->next=save;
    }
}
void Pop()   //popping nodes from the stack
{
    if(top == NULL)
     return;
    else
    {
        Stack* save = top;
        top = top->next;
        delete save;
    }
}

void insert(node** ref, char x) // method to insert elements in the binary tree
{
    char val;
    int f,j;
    if(*ref == NULL)  // if passed pointer is null
    {
        node* ptr = Create_Tree_Node(x); // create a  new node in the binary tree
        //inserting code to input the truth values 
        if(isalpha(x))
        {
            f=0;
            for(j=0; j<n; ++j)
            {
                if(x == atoms[j].name)
                {
                    f=1; break;        // the atom x has been used multiple times in the expression 
                }
            }
            if(f==0)
            {
                //enter the values of T/F for each atom
                std::cout<<"Truth value of "<<ptr->atom<<" ";
                std::cin>>val;
                if(val == 'T' || val == 't')
                 ptr->boolean = 1;
                if (val == 'F' || val == 'f')
                 ptr->boolean = 0;
                atoms[n].name = x;
                atoms[n].boolean = ptr->boolean;
                ++n;
            }
            else 
             ptr->boolean = atoms[j].boolean;  // the T/F value has been decided when the atom was found for the first time
        }
        *ref = ptr; // passing new pointer to the root
        current=ptr; //passing new pointer to the current address holder
        
        if(!isalpha(x))  //if element  is an operator 
        {
            Stack* newptr = Create_Stack_node(current); // create a node in the stack to hold address of operators
            Push(newptr); // pushing node into the stack
        }
        return; 
    }
    else  // if passed pointer is not NULL
    {
        if(current->atom == '~')  // if the previously created node contained a ~ 
        {
            insert(&top->elt->right, x); // insert a new node in the right side of the parent node 
            if(isalpha(x))    // if the new node contains a single propositional atom, pop it from the stack
             Pop();
        }
        else if((current->atom == '+' || current->atom == '*' || current->atom == '>')) // if the previously created node contained any other operator 
        {
            if(top->elt->left == NULL) //if left of the parent node is empty, add new node to left side
             insert(&top->elt->left, x);
            else                      // else add new node to right side of parent
             {
                 insert(&top->elt->right, x); 
                 Pop(); // that particular operator is full, so do not need to store address of that operator, pop it 
             }
        }
        else if(isalpha(current->atom))  // if previously created node contains a propositional atom
        {
            if(top->elt->left == NULL) // if left side of the newest added operator is empty, add node to left side
            {
                insert(&top->elt->left, x);
            } 
            else                      // else add node to right side and pop the top of the stack 
            {
				node* hi = top->elt; 
				Pop();
                insert(&hi->right, x);
                 
            } 
        }  
        else return;
    }
} 

// task 3
void InOrder(node* root) // method to traverse the parse tree in an in order fashion and print the infix expression 
{
	if (root == NULL)
	{
		return;
	}
    InOrder(root->left);
    std::cout<<root->atom;
    InOrder(root->right);
}


//task 4
int Height(node* ptr) // method to calculate the height of a parse tree
{	
	int hleft, hright, height;
	if (ptr->left != NULL)
		hleft = Height(ptr->left);
	else
		hleft = 0;
	if (ptr->right != NULL)
		hright = Height(ptr->right);
	else
		hright = 0;

	if(hleft>hright)
     height = hleft;
    else
     height = hright;
	return height+1;
}

// task 5

//Stack structure and methods to store and evaluate the truth values
struct boolStack
{
    int elt; // holds the boolean values 0 or 1
    boolStack* next;
};
struct boolStack* boolTop = NULL; // points to the top of the above stack

boolStack* Create_boolStack_node(int x) // creates node 
{
    boolStack* ptr = new boolStack;
    ptr->elt = x;
    ptr->next = NULL;
    return ptr;
}
void Push_calc(boolStack* ptr)  //pushing node in the stack
{
    if(boolTop==NULL)
     boolTop = ptr;
    else
    {
        boolStack* save = boolTop; 
        boolTop = ptr; 
        ptr->next=save;
    }
}
int Pop_calc()  //popping node from the stack
{
    if(boolTop == NULL)
     return -1;
    else
    {
        int c = boolTop->elt;
        boolStack* save = boolTop;
        boolTop = boolTop->next;
        delete save;
        return c;
    }
}

void Traverse(node* root) //traverses the binary tree in a post order fashion and evaluates the truth value of the expression
{
    int c1,c2,val;
    if(root == NULL)
     return;
    else
    {
        Traverse(root->left);
        Traverse(root->right);
        if(isalpha(root->atom)) //if node in the tree has a propositional atom
        {
            boolStack* ptr = Create_boolStack_node(root->boolean); // create a node in boolStack
            Push_calc(ptr); //push the boolean value into the stack
        }
        else
        {
            //switch case 
            if(root->atom == '~') //if ~, pop the first element, as ~ is a unary operator
             {c1 = Pop_calc();
              val= !(c1);
             }
            else
            {c1 = Pop_calc(); // else, pop the first two elements as they are binary operators
             c2 = Pop_calc();
             switch(root->atom)  // evaluating value
             {
                case '*': val = c1 && c2; break;
                case '+': val = c1 || c2; break;
                case '>': val = ((!(c2)) || c1); break;
                default: val = 0; break;
             }
            }
            boolStack* ptr = Create_boolStack_node(val); //create a node in boolStack
            Push_calc(ptr); //pushing the obtained value in the stack
            
        }
        
    }
    
}
int Evaluate(node* root)   //invokes Traverse()
{
  Traverse(root);
  return(boolTop->elt);   
}

void FreeTree(node* root) // freeing the dynamically allocated memory by traversing in a post order fashion
{
    if(root==NULL)
     return;
    FreeTree(root->left);
    FreeTree(root->right);
    delete root;
}

//main function
int main()
{
    clrscr();
    char expr[1500]; // infix expression
    
    std::cout<<"Enter the fully  parenthesized infix expression:\n";
    std::cin>>expr;
    node* root = NULL; // pointer to root node of binary tree
	node** ref;  // pointer to the variable root
	ref = &root;
	
    //converting infix to prefix
    makePrefix(expr); 
    std::cout<<"\nPrefix expr = "<<prefix<<"\n";
    
    std::cout<<"\n";
    //inserting nodes in the binary tree
    for(int i=0; prefix[i]!='\0';++i)
     insert(ref, prefix[i]);   
    
    //traversing in order fashion and obtaining infix expression
    std::cout<<"\nInfix obtained again = ";
    InOrder(root);
    
    //calculating height of the binary tree
	int h=Height(root);
	std::cout <<"\n\nHeight of the tree = "<<h<<"\n";
    
    //evaluating the truth value of the expression
    int ans = Evaluate(root);
    char ch;
    if(ans == 1)
     ch = 'T';
    else 
     ch = 'F';
    std::cout<<"\nTruth value answer: "<<ch<<"\n";
    
    //freeing dynamically allocated memory
    FreeTree(root);

    std::cout<<"\nDone with all tasks\n";
    return 0;
}