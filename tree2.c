#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//task 1
char prefix[1000]; //stores the prefix expression used to make the tree

// Stack structure and methods to hold operators during infix to prefix conversion 
struct preStack
{
    char elt; // holds operator
    preStack* next;
};                 //Stack structure  
struct preStack* preTop = NULL; // top pointer to stack 

struct preStack* Create_preStack_node(char x) // creating node for stack 
{
    struct preStack* ptr = (preStack*)malloc(sizeof(preStack));
    ptr->elt = x;
    ptr->next = NULL;
    return ptr;
}
void prePush(struct preStack* ptr)  // pushing into stack
{
    if(preTop==NULL)
     preTop = ptr;
    else
    {
        struct preStack* save = preTop; 
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
        free(save);
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
    struct node* left;
    struct node* right;
};

struct node* current = NULL; // holds the address of the last node which has been added

struct node* Create_Tree_Node(char x) // creates nodes in the binary tree
{
    struct node* ptr = (node*)malloc(sizeof(node)); //allocating memory for the node
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
    struct node* elt; // a pointer to a node of the binary tree
    struct Stack* next;
};
struct Stack* top = NULL; // points to the top of Stack

struct Stack* Create_Stack_node(struct node* x) // creating nodes in the stack
{
    struct Stack* ptr = (Stack*)malloc(sizeof(Stack));
    ptr->elt = x;
    ptr->next = NULL;
    return ptr;
}
void Push(struct Stack* ptr) // pushing nodes in the stack
{
    if(top==NULL)
     top = ptr;
    else
    {
        struct Stack* save = top; 
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
        struct Stack* save = top;
        top = top->next;
        free(save);
    }
}

void insert(struct node** ref, char x) // method to insert elements in the binary tree
{
    char val;
    int f,j;
    if(*ref == NULL)  // if passed pointer is null
    {
        struct node* ptr = Create_Tree_Node(x); // create a  new node in the binary tree
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
                printf("Truth value of %c ",ptr->atom);
                scanf("%c",&val);
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
            struct Stack* newptr = Create_Stack_node(current); // create a node in the stack to hold address of operators
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
            if(top->elt->atom == '~')
            {
                Pop();
            }
            if(top->elt->left == NULL) // if left side of the newest added operator is empty, add node to left side
            {
                insert(&top->elt->left, x);
            } 
            else                      // else add node to right side and pop the top of the stack 
            {
				struct node* hi = top->elt; 
				Pop();
                insert(&hi->right, x);
                 
            } 
        }  
        else return;
    }
} 

// task 3
void InOrder(struct node* root) // method to traverse the parse tree in an in order fashion and print the infix expression 
{
	if (root == NULL)
	{
		return;
	}
    InOrder(root->left);
    printf("%c",root->atom);
    InOrder(root->right);
}


//task 4
int Height(struct node* ptr) // method to calculate the height of a parse tree
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

struct boolStack* Create_boolStack_node(int x) // creates node 
{
    struct boolStack* ptr = (boolStack*)malloc(sizeof(boolStack));
    ptr->elt = x;
    ptr->next = NULL;
    return ptr;
}
void Push_calc(struct boolStack* ptr)  //pushing node in the stack
{
    if(boolTop==NULL)
     boolTop = ptr;
    else
    {
        struct boolStack* save = boolTop; 
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
        free(save);
        return c;
    }
}

void Traverse(struct node* root) //traverses the binary tree in a post order fashion and evaluates the truth value of the expression
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
            struct boolStack* ptr = Create_boolStack_node(val); //create a node in boolStack
            Push_calc(ptr); //pushing the obtained value in the stack
            
        }
        
    }
    return;
    
}
int Evaluate(struct node* root)   //invokes Traverse()
{
  Traverse(root);
  return(boolTop->elt);   
}

void FreeTree(struct node* root) // freeing the dynamically allocated memory by traversing in a post order fashion
{
    if(root==NULL)
     return;
    FreeTree(root->left);
    FreeTree(root->right);
    delete root;
}

void IMPL_FREE(struct node* root)
{
    if(isalpha(root->atom))
    {
        return;
    }
    switch(root->atom)
    {
        case '~': IMPL_FREE(root->right); break;
        case '*': case '+': IMPL_FREE(root->left);
                  IMPL_FREE(root->right);
                  break;
        case '>': {
                    node* newptr = Create_Tree_Node('~');
                    newptr->right = root->left;
                    root->left = newptr;
                    root->atom = '+';
                    IMPL_FREE(root);
                 } break;
        default: break;
    }
    return;
}

void NNF(struct node* root, int f1, int f2)
{
    int flag1 = 0, flag2 = 0;
    //std::cout << "\n%%%%%%%%" << root->atom << "%%%%%%%% \n" <<  std::endl;
    if(isalpha((root)->atom))
    {
        //std::cout << "\n$$$$$$$$$" << root->atom << "$$$$$$$$$ \n" <<  std::endl;
        return;
    }

    if((root)->left!=NULL && (root)->left->atom == '~')
    {
       flag1 = 1; 
       struct Stack* ptr = Create_Stack_node(root);
       Push(ptr);
    }
    if((root)->right->atom == '~')
    {
        flag2 = 1;
        if(flag1 == 0 && (root)->atom!='~')
        {
          struct Stack* ptr = Create_Stack_node(root);
          Push(ptr);
        }
    }
    
    switch((root)->atom)
    {
        case '~':{
                   if(isalpha((root)->right->atom))
                   {
                       //std::cout << "\n$$$$$$$$$" << root->atom << "$$$$$$$$$ \n" <<  std::endl;
                       NNF((root)->right, flag1, flag2);
                       return;
                   }
                   switch((root)->right->atom)
                   {
                       case '~': {  //std::cout<<"\nHello\n";
                                    //if(top == NULL) std::cout<<"NULL\n";
                                    if(top!=NULL)
                                    {
                                        if(f1 == 1)
                                            top->elt->left = (root)->right->right;
                                        if(f2 == 1)
                                            top->elt->right = (root)->right->right;
                                        free(root->right);
                                        free(root);
                                        flag1 = f1; flag2 = f2;
                                        NNF(top->elt->left, flag1, flag2);
                                    }
                                 } break;
                       case '+': { if(f1 == 0 && f2 == 0)
                                   {
                                     root->right->atom = '*'; 
                                     struct node* newptr1 = Create_Tree_Node('~');
                                     struct node* newptr2 = Create_Tree_Node('~');
                                     newptr1->right = (root)->right->left;
                                     newptr2->right = (root)->right->right;
                                     (root)->right->left = newptr1;
                                     (root)->right->right = newptr2;
                                     root = root->right;
                                     flag1 = f1; flag2 = f2;
                                     NNF(root, flag1, flag2);
                                   }
                                   if(f1 == 1)
                                   {
                                       top->elt->left = (root)->right;
                                       (root)->right->atom = '*';
                                       struct node* newptr1 = Create_Tree_Node('~');
                                       struct node* newptr2 = Create_Tree_Node('~');
                                       newptr1->right = (root)->right->left;
                                       newptr2->right = (root)->right->right;
                                       (root)->right->left = newptr1;
                                       (root)->right->right = newptr2;
                                       
                                       free((root));
                                       flag1 = f1; flag2 = f2;
                                       NNF(top->elt->left, flag1, flag2);
                                   }
                                   if(f2 == 1)
                                   {
                                       top->elt->right = (root)->right;
                                       (root)->right->atom = '*';
                                       struct node* newptr1 = Create_Tree_Node('~');
                                       struct node* newptr2 = Create_Tree_Node('~');
                                       newptr1->right = (root)->right->left;
                                       newptr2->right = (root)->right->right;
                                       (root)->right->left = newptr1;
                                       (root)->right->right = newptr2;
                                       
                                       free(root);
                                       flag1 = f1; flag2 = f2;
                                       NNF(top->elt->right, flag1, flag2);
                                   }
                                } break;
                        case '*': {if(f1 == 0 && f2 == 0)
                                   {
                                     root->right->atom = '+'; 
                                     struct node* newptr1 = Create_Tree_Node('~');
                                     struct node* newptr2 = Create_Tree_Node('~');
                                     newptr1->right = (root)->right->left;
                                     newptr2->right = (root)->right->right;
                                     (root)->right->left = newptr1;
                                     (root)->right->right = newptr2;
                                     root = root->right;
                                     flag1 = f1; flag2 = f2;
                                     NNF(root, flag1, flag2);
                                   } 
                                   if(f1 == 1)
                                   {
                                       top->elt->left = (root)->right;
                                       (root)->right->atom = '+';
                                       struct node* newptr1 = Create_Tree_Node('~');
                                       struct node* newptr2 = Create_Tree_Node('~');
                                       newptr1->right = (root)->right->left;
                                       newptr2->right = (root)->right->right;
                                       (root)->right->left = newptr1;
                                       (root)->right->right = newptr2;
                                       
                                       free(root);
                                       flag1 = f1; flag2 = f2;
                                       NNF(top->elt->left, flag1, flag2);
                                   }
                                   if(f2 == 1)
                                   {
                                       top->elt->right = (root)->right;
                                       (root)->right->atom = '+';
                                       struct node* newptr1 = Create_Tree_Node('~');
                                       struct node* newptr2 = Create_Tree_Node('~');
                                       newptr1->right = (root)->right->left;
                                       newptr2->right = (root)->right->right;
                                       (root)->right->left = newptr1;
                                       (root)->right->right = newptr2;
                                       
                                       free(root);
                                       flag1 = f1; flag2 = f2;
                                       NNF(top->elt->right, flag1, flag2);
                                   }
                                } break;
                       default: break;         
                   }
                   Pop();
                 } break;
       case '*': case '+': {    
                                //start
                                if(flag1==1 && flag2==1)
                                {
                                    NNF((root)->left, flag1, 0);
                                    NNF((root)->right, 0, flag2);
                                }
                                //end
                                else
                                {
                                    NNF((root)->left, flag1, flag2);
                                    NNF((root)->right, flag1, flag2);
                                }
                           } break;
       default: break;
      
    }
    //std::cout<<root->atom<<"^^^\n";
    return;
}

struct node* copyTree(struct node* root) 
{
   if (root == NULL ) 
    return root;
   //create new node and make it a copy of node pointed by root
   struct node* temp = (node*)malloc(sizeof(node));
   temp->atom = root->atom;    //copying data
   temp->boolean = root->boolean;
   temp->left = copyTree(root -> left);    //cloning left child
   temp->right = copyTree(root -> right);  //cloning right child
   return temp;
} 

void DISTR(struct node* root)
{
  if(isalpha(root->atom) || root->atom == '~')
    return;
  if(root->left->atom == '*')
  {
    struct node* newptr = Create_Tree_Node('+');
    newptr->right = root->right;
    newptr->left = root->left->right;
    node* copied = copyTree(root->right);
    root->left->right = copied;
    root->right = newptr;
    root->left->atom = '+';
    root->atom = '*';
  }
  if(root->right->atom == '*')
  {
    node* newptr = Create_Tree_Node('+');
    newptr->left = root->left;
    newptr->right = root->right->left;
    node* copied = copyTree(root->left);
    root->right->left = copied;
    root->left = newptr;
    root->right->atom ='+';
    root->atom = '*';
  }
  DISTR(root->left);
  DISTR(root->right);
}

void CNF(node* root)
{
  if(isalpha(root->atom) || root->atom == '~')
  {
    return;
  }
  else if(root->atom == '*')
  {
    CNF(root->left);
    CNF(root->right);
  }
  else
  {
   CNF(root->left);
   CNF(root->right);
   DISTR(root);
  }
}

char CNFstr[1000];
int pos = -1;
void getString(node* root)
{
  if(root == NULL)
    return;
  getString(root->left);
  CNFstr[++pos] = root->atom;
  getString(root->right);
}

//checking validity
void validity()
{
  char conjunct[1000];
  int j = 0, flag=-1,valid = 1; int i, k, m, x=0, y=0;
  for(i = 0; i<pos+1; ++i)
  {
    //std::cout<<"When we start an iteration "<<CNFstr[i]<<" "<<flag<<"\n";
    if(CNFstr[i] == '~')
    {
      conjunct[j] = CNFstr[i+1]; 
      ++j;
      //std::cout<<"When we hit ~ "<<flag<<"\n";
    }
    if(CNFstr[i] == '*' || CNFstr[i] == '\0')
    {
      //std::cout<<"When we hit * "<<flag<<"\n";
      conjunct[j] = CNFstr[i];
      ++j;
      for(k = x; k<i; ++k) // iterating through CNFstr 
      {
        flag = 0;
        if(isalpha(CNFstr[k]) && CNFstr[k-1]!='~')
        {
          //std::cout<<"When we hit a plain literal "<<CNFstr[k]<<" "<<flag<<"\n";
          for(m=y; m<j; ++m) // iterating over conjunct 
          {
            if(CNFstr[k] == conjunct[m])
            {
              //std::cout<<conjunct[m]<<"***\n";
              flag=1; break;
            }
          }
        }
        if(flag==1)
        {
          //std::cout<<flag<<"&&&\n";
          break;
        }
      }
      //std::cout<<"When we hit end of * if "<<flag<<"\n";
      x = i+1; y = j;
    }
    if(flag == 0)
    {
      //std::cout<<"When we break out if its invalid "<<flag<<"\n";
      valid = 0;
      break;
    }
    else if(flag == 1)
      flag = -1;
    else continue;
  }
  if(valid == 0)
  {
    printf("\nThe given formula is not valid!\n");
  }
  else
  {
    printf("\nThe given formula is valid!\n");
  }
}

//main function
int main()
{
    system("clear");
    clock_t begin =  clock();
    char expr[1500]; // infix expression
    
    printf("Enter the fully  parenthesized infix expression:\n");
    scanf("%s",expr);
    struct node* root = NULL; // pointer to root node of binary tree
	struct node** ref;  // pointer to the variable root
	ref = &root;
	
    //converting infix to prefix
    makePrefix(expr); 
    printf("\nPrefix expr = %s\n\n",prefix);
    
    //inserting nodes in the binary tree
    for(int i=0; prefix[i]!='\0';++i)
     insert(ref, prefix[i]);   
    
    //traversing in order fashion and obtaining infix expression
    printf("\nInfix obtained again = ");
    InOrder(root);
    
    //calculating height of the binary tree
	int h1=Height(root);
	printf("\n\nHeight of the tree = %d\n",h1);
    
    //evaluating the truth value of the expression
    int ans = Evaluate(root);
    char ch;
    if(ans == 1)
     ch = 'T';
    else 
     ch = 'F';
    printf("\nTruth value answer: %c\n",ch);
    
    while(top!=NULL)
    {
      Pop();
    }
    top = NULL;

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nExecution time : %lf seconds\n",time_spent);
    
    printf("\n--------- Assignment 2 --------- \n");

    node* rootOriginal = copyTree(root);

    IMPL_FREE(root);
    printf("\nInfix obtained again after IMPL_FREE = ");
    InOrder(root);

    if(root->atom == '~' && root->right->atom == '~')
    {
        //printf("Free!\n";
        node* temp1 = root;
        node* temp2 = root->right;
        root=root->right->right;
        free(temp2);
        free(temp1);
        //std::cout<<root->atom<<"\n";                                       
    }
    NNF(root,0,0);
    //std::cout<<root->atom<<"***\n";
    
    if(root->atom == '~' && !isalpha(root->right->atom) && root->right->atom!='~')
    {
      node* temp = root;
      root = root->right;
      free(temp);
    }
    //std::cout<<root->atom<<"***\n";
    printf("\nInfix obtained again after NNF = ");
    InOrder(root);

    CNF(root);
    printf("\nInfix obtained again after CNF = ");
    InOrder(root);

    h1 = Height(rootOriginal);
    int h2 = Height(root);
    printf("\n\nHeight of the original tree = %d\nHeight of the tree in CNF = %d\n",h1,h2);

    while(top!=NULL)
    {
      //std::cout<<"Popping\n";
      Pop();
    }
    top = NULL;

    getString(root);
    CNFstr[++pos] = '\0';
    //std::cout<<"\nThe CNF string is "<<CNFstr<<"\n";

    validity();

    //freeing dynamically allocated memory
    FreeTree(root);
    FreeTree(rootOriginal);
    printf("\nDone with all tasks\n\n");
    
    return 0;
}