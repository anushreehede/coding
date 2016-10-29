void insert(node** ref, char x)
{
    char val;
    int f,j;
    if(*ref == NULL) 
    {
        node* ptr = CreateNode(x);
        *ref = current = ptr;
        if(!isalpha(x))
         int c = Push(current);
        if(isalpha(x))
        {
            f=0;
            for(j=0; j<n; ++j)
            {
                if(x == atoms[j])
                {
                    f=1; break;
                }
            }
            if(f==0)
            {
                //enter the values of T/F for each node
                std::cout<<"Truth value of "<<ptr->atom<<" ";
                std::cin>>val;
                if(val == 'T' || val == 't')
                 ptr->boolean = 1;
                else if (val == 'F' || val == 'f')
                 ptr->boolean = 0;
                else ptr->boolean = -1;
                atoms[n] = x;
                ++n;
            }
        }
        //std::cout<<ptr->atom<<" ";
        return; 
    }
    else
    {
        if(current->atom == '~')
        {
            //int c  = Push(current);
            insert(&current->right, x); 
            int c = Pop();
        }
        else if((current->atom == '+' || current->atom == '*' || current->atom == '>'))
        {
            //int c = Push(current);
            if(Stack[top]->left == NULL)
             insert(&Stack[top]->left, x);
            else
             {
                 insert(&Stack[top]->right, x);
                 int c = Pop();
             }
        }
        else if(isalpha(current->atom))
        {
            if(Stack[top]->left == NULL)
            {
                insert(&Stack[top]->left, x);
            } 
            else
            {
				node* hi = Stack[top]; //trying something here
				int c = Pop();
                insert(&hi->right, x);
                 
            } 
        }  
    }
} 