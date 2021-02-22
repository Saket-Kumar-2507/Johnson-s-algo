#include <bits/stdc++.h>

using namespace std;

int trees,size;

// --------------------------------------------------------------- start of ARRAY --------------------------------------------------------------------
int find_min_arr(int *arr, bool visited[])
{
    int i,min= INT_MAX , index;
    
    for(i=1;i<=size;i++)
    {
        if(visited[i]==false)
        {
            if( min > arr[i])
            {
                min= arr[i];
                index= i;
            }
        }
    }
    return index;
}

void decrease_key_arr(int *arr , int v , int new_path)
{ arr[v] = new_path; }
// ***************************************************************** end of ARRAY ********************************************************************




// ------------------------------------------------------ start of BINARY heap (using array) ---------------------------------------------------------
struct binary_heap
{
    int path;
    int vertex;
};

int min(int a,int b)
{
    if(a<b)
    { return a; }
    else
    { return b; }
}

void binary_swap(binary_heap *arr , int *ptr_binary , int i , int sib)
{
    int p,v;
    
    ptr_binary[arr[sib].vertex]= i;
    ptr_binary[arr[i].vertex]= sib;
    
    p= arr[sib].path;
    v= arr[sib].vertex;
    arr[sib].path = arr[i].path;
    arr[sib].vertex = arr[i].vertex;
    arr[i].path = p;
    arr[i].vertex = v;
}

void extract_min_binary(binary_heap *arr , int *ptr_binary , int curr_size)
{
    int i,left,right,minimum;
    int p,v,ctr;
    
    binary_swap(arr,ptr_binary,1,curr_size);
    
    for(i=1 ; i<curr_size ; i=ctr)
    {
        if(2*i > (curr_size-1))
        { break; }
        
        left= 2*i;
        right= (2*i)+1;
        
        if(right>(curr_size-1))
        { minimum= arr[left].path; }
        else
        { minimum= min(arr[left].path,arr[right].path); }
        
        ctr=i;
        
        if(minimum<arr[i].path)
        {
            if(minimum == arr[left].path)
            {
                ctr=left;
                binary_swap(arr,ptr_binary,i,left);
            }
            else
            {
                ctr=right;
                binary_swap(arr,ptr_binary,i,right);
            }
        }
        
        if(ctr==i)
        { break; }
        
    }
    
}

void decrease_key_binary(binary_heap *arr , int v , int new_path , int *ptr_binary)
{
    int i,par;
    i= ptr_binary[v];
    arr[i].path = new_path;
    
    while(i!=1)
    {
        par= i/2;
        if(arr[i].path < arr[par].path)
        { 
            binary_swap(arr,ptr_binary,i,par); 
            i= par;
        }
        else
        { break; }
    }
}
// ******************************************************** end of BINARY heap (using array) *********************************************************




// ---------------------------------------------------------- start of BINOMIAL heap ------------------------------------------------------------------
struct bino_heap
{
    int path , vertex , rank;  
    bino_heap *child;
    bino_heap *right;
    bino_heap *parent;
};

bino_heap *create_bino_node(int v)
{
    bino_heap *new_node = new bino_heap();
    new_node -> path = 999999;
    new_node -> vertex = v;
    new_node -> rank = 0;
    new_node -> right = NULL;
    new_node -> child = NULL;
    new_node -> parent = NULL;
    
    return new_node;
}

void combine(bino_heap *ptr , bino_heap *temp)             // merges "ptr" into "temp", ie. it makes "ptr" the child of "temp"
{
    if((temp->child) != NULL)
    { 
        (ptr->right) = (temp->child);
        temp->child = ptr;
        ptr->parent = temp;
    }
    else
    {
        (temp->child) = ptr;
        (ptr->parent) = temp;
    }
}

void consolidate(bino_heap **head , bino_heap *prev , bino_heap *temp)
{
    bino_heap *next;
    
    if(temp->right != NULL)
    {
        if(temp->rank == (temp->right->rank))
        {
            if((temp->path) <= (temp->right->path))
            {
                temp->rank = (temp->rank)+1;
                next= temp->right;
                temp->right = temp->right->right;
                next->right = NULL;
                combine(next,temp);
            }
            else
            {
                (temp->right->rank) = (temp->right->rank) +1;
                
                if(prev==temp)
                {
                    (*head)= temp->right;
                    next= (*head);
                    prev= (*head);
                }
                else
                {
                    next= temp->right;
                    prev->right = temp->right;
                }
                
                temp->right = NULL;
                combine(temp,next);
                temp= next;
            }
            
            if(temp->right != NULL)
            {
                if((temp->rank) == (temp->right->rank))
                { consolidate(head,prev,temp); }
            }
            
            
        }
    }
   
}

void un ( bino_heap **head , bino_heap *ptr)
{
    if((*head)==NULL)
    {
        (*head)=ptr;
        return;
    }
    
    bino_heap *temp , *x , *prev;
    temp= (*head);
    prev= temp;
    (ptr->right) = NULL;
    
    while(temp != NULL)
    {
        if((temp->rank) < (ptr->rank))
        { 
            if(temp->right != NULL)
            { 
                if((temp->right->rank) < (ptr->rank))
                { 
                    prev= temp;
                    temp= temp->right;
                }
                else if((temp->right->rank) > (ptr->rank))
                {
                    (ptr->right) = temp->right;
                    (temp->right) = ptr;
                    break;
                }
                else
                { 
                    if((ptr->path) < (temp->right->path))
                    {
                        ptr->rank = (ptr->rank) + 1;
                        ptr->right = temp->right->right;
                        temp->right->right = NULL;
                        x= temp->right;
                        temp->right = ptr;
                        combine(x,ptr);
                        prev= temp;
                        temp = ptr;
                    }
                    else
                    {
                        (temp->right->rank) = (temp->right->rank)+1;
                        combine(ptr,temp->right);
                        prev= temp;
                        temp= temp->right;
                    }
                    
                    if(temp->right != NULL)
                    {
                        if((temp->rank) == (temp->right->rank))
                        { consolidate(head,prev,temp); }
                    }
                    
                    break;
                }
            }
            else
            { 
                temp->right = ptr; 
                break;
            }
        }
        else if((temp->rank) > (ptr->rank))
        {
            ptr->right = (*head);
            (*head)= ptr;
            break;
        }
        else
        { 
            if((ptr->path) < (temp->path))
            {
                ptr->rank = (ptr->rank) + 1;
                ptr->right = temp->right;
                (*head)= ptr;
                temp->right = NULL;
                combine(temp,ptr);                 // makes "temp" the child of "ptr"
                temp = ptr;
                prev = ptr;
            }
            else
            {
                temp->rank = (temp->rank)+1;
                combine(ptr,temp);                 // merges "ptr" into "temp", ie. it makes "ptr" the child of "temp"
            }
            
            if(temp->right != NULL)
            {
                if((temp->rank) == (temp->right->rank))
                { consolidate(head,prev,temp); }
            }
            break;
        }
        
        
    }
}



bino_heap *find_min_bino(bino_heap *head)
{
    bino_heap *temp = head , *ptr = NULL;
    int min = INT_MAX;
    
    while(temp!=NULL)
    {
        if((temp->path) < min)
        {
            min = (temp->path);
            ptr = temp;
        }
        temp= temp->right;
    }
    
    return ptr;
}

void extract_min_bino(bino_heap **head ,  bino_heap **ptr_bino)          // possible error -> ptr_bino
{
    if(*head == NULL)
    { return; }
    
    bino_heap *min_bino , *temp, *x , *ptr;
    temp = (*head);
    min_bino = find_min_bino(*head);
    
    if(min_bino != NULL)
    { ptr_bino[min_bino->vertex] = NULL; }
    else
    { return; }
    
    if((*head)==min_bino)
    {
        (*head)= min_bino->right;
        min_bino->right = NULL;
        x= min_bino->child;
        min_bino->child = NULL;
        
        while(x!=NULL)
        {
            ptr= x->right;
            x->right = NULL;
            x->parent = NULL;
            un(head,x);
            x= ptr;
        }
        free(min_bino);
        return;
    }
    
    while(temp!=NULL)
    {
        if( (temp->right == min_bino) && (min_bino != NULL) )
        { 
            temp->right = temp->right->right; 
            min_bino->right = NULL;
            x= min_bino->child;
            min_bino->child = NULL;
            
            while(x!=NULL)
            {
                ptr= x->right;
                x->right = NULL;
                x->parent = NULL;
                un(head,x);
                x= ptr;
            }
            
            free(min_bino);
            break;
        }
        temp= temp->right;
    }
}

void swap(bino_heap *ptr , bino_heap *temp)
{
    int r,v,p;
    
    r= ptr->rank;
    v= ptr->vertex;
    p= ptr->path;
    
    ptr->rank = temp->rank;
    ptr->vertex = temp->vertex;
    ptr->path = temp->path;
    
    temp->rank = r;
    temp->vertex = v;
    temp->path = p;
}

void decrease_key_bino(bino_heap **head , int v , int new_path , bino_heap **ptr_bino)              // possible error -> ptr_bino
{
    if(*head == NULL)
    { return; }
    
    bino_heap *ptr , *temp , *x;
    ptr = ptr_bino[v];
    if(ptr==NULL)
    { return; }
    
    ptr->path = new_path;
    
    // percolate up
    while(ptr->parent != NULL)
    {
        temp = ptr->parent;
        
        if((temp->path) <= (ptr->path))
        { return; }
        else
        {
            x= ptr_bino[ptr->vertex];
            ptr_bino[ptr->vertex] = ptr_bino[temp->vertex];
            ptr_bino[temp->vertex] = x;
            //cout<<"+";
            swap(ptr,temp);
            ptr= temp;
            //cout<<"*";
        }
    }
}

void delete_bino(bino_heap **head , bino_heap **ptr_bino , int N)           // possible error -> ptr_bino
{
    int i;
    
    if((*head)==NULL)
    { return; }
    
    for(i=1;i<=N;i++)
    {
        if(ptr_bino[i]!=NULL)
        {
            decrease_key_bino(head, ptr_bino[i]->vertex , -999999 , ptr_bino);
            extract_min_bino(head,ptr_bino);
        }
    }
    
    (*head)=NULL;
}
// ************************************************************ end of BINOMIAL heap *******************************************************************




// ---------------------------------------------------------- start of FIBONACCI heap ------------------------------------------------------------------
struct fibo_heap
{
    int path , vertex , rank;   
    bool marked;
    fibo_heap *left;
    fibo_heap *right;
    fibo_heap *child;
    fibo_heap *parent;
};

fibo_heap *create_fibo_node(int v)
{
    fibo_heap *new_node = new fibo_heap();
    new_node -> path = 999999;
    new_node -> vertex = v;
    new_node -> marked = false;
    new_node -> rank = 0;
    new_node -> left = NULL;
    new_node -> right = NULL;
    new_node -> child = NULL;
    new_node -> parent = NULL;
    
    return new_node;
}

fibo_heap *insert_fibo(fibo_heap *head, int v)
{
    fibo_heap *new_node , *temp;
    new_node = create_fibo_node(v);
    
    temp = head->right;
    head->right = new_node;
    new_node->right = temp;
    temp->left = new_node;
    new_node->left = head;
    
    return new_node;
}

void merge_fibo(fibo_heap **head , fibo_heap *ptr)
{
    fibo_heap *temp;
    ptr->parent = NULL;
    
    if((*head)==NULL)
    {
        ptr->right= ptr;
        ptr->left = ptr;
        (*head)= ptr;
        trees=1;
        return;
    }
    
    temp= (*head)->right;
    (*head)->right = ptr;
    ptr->right = temp;
    temp->left = ptr;
    ptr->left = (*head);
    
    trees++;
}

int find_trees(fibo_heap *head)
{
    if(head==NULL)
    { return 0; }
    
    int i=1;
    fibo_heap *temp;
    temp= head->right;
    
    if(temp->right != temp)
    {
        while(temp!=head)
        { 
            i++;
            temp = temp->right;
        }
    }
    else
    { return 1; }
    
    return i;
}

void pluck_and_plant(fibo_heap *temp , fibo_heap *ptr)     // makes "ptr" , the child of "temp"
{
    fibo_heap  *itr1 , *itr2;
    
    if(ptr->left != NULL)
    { ptr->left->right = ptr->right; }
    if(ptr->right != NULL)
    { ptr->right->left = ptr->left; }
                
    if(temp->rank == 0 )
    {
        (temp->rank) = (temp->rank) + 1;
        temp->child = ptr;
        ptr->parent = temp;
        ptr->left = ptr;
        ptr->right = ptr;
    }
    else
    {
        (temp->rank) = (temp->rank) + 1;
        itr1 = temp->child;
        itr2 = itr1->right;
        
        itr1->right = ptr;
        ptr->left = itr1;
        itr2->left = ptr;
        ptr->right = itr2;
        ptr->parent = temp;
    }
}

void consolidation(fibo_heap **head_root_list , fibo_heap **min_ptr , int N )      // possible error -> min_ptr
{
    if((*head_root_list)==NULL)
    { return; }
    
    int i,r,min;
    fibo_heap *arr_rank[N+1]= {NULL};
    fibo_heap *temp , *ptr ;
    temp= (*head_root_list);
    (*min_ptr)= temp;
    min = temp->path;
    
    if(trees<=1)
    { return; } 
    
    
    for(i=0;i<=2*trees;i++)    // Wrong loop condition(may be)
    {
        r= temp->rank;
        if(arr_rank[r] != NULL)
        {
            ptr= arr_rank[r]; 
            
            if(ptr!=temp)
            {
                if(ptr->path >= temp->path)
                {
                    if((*head_root_list)==ptr)
                    { 
                        if(ptr != ptr->right)
                        { (*head_root_list) = ptr->right; }
                        else
                        { 
                            (*head_root_list) = NULL; 
                            trees= 0;
                            return;
                        }
                    }
                    
                    pluck_and_plant(temp,ptr);
                }
                else
                {
                    if((*head_root_list)==temp)
                    { 
                        if(temp != temp->right)
                        { (*head_root_list) = temp->right; }
                        else
                        { 
                            (*head_root_list) = NULL; 
                            trees= 0;
                            return;
                        }
                        
                    }
                    
                    pluck_and_plant(ptr,temp);
                    temp= ptr;
                }
                
                if((min >= temp->path)&&(temp->parent == NULL))
                {
                    min= temp->path;
                    (*min_ptr) = temp;
                }
                arr_rank[r] = NULL;
            }
        }
        else
        { 
            arr_rank[r] = temp; 
            
            if((min >= temp->path)&&(temp->parent == NULL))
            {
                min= temp->path;
                (*min_ptr) = temp;
            }
            
            temp = temp->right;
        }
        
    }
    
    trees= find_trees(*head_root_list);
}

void extract_min_fibo(fibo_heap **head_root_list , fibo_heap **min_ptr , int N , fibo_heap **ptr_fibo)     // possible error-> ptr_fibo
{
    if(*head_root_list == NULL)
    { return; }
    
    fibo_heap *ptr , *temp , *temp1;
    ptr = (*min_ptr);
    ptr_fibo[ptr->vertex]= NULL;
    
    if(trees>1)
    { trees = trees+(ptr->rank)-1; }
    else
    {
        if(ptr->child != NULL)
        {
            temp= ptr->child;
            ptr->child = NULL;
            
            while(temp->parent != NULL)
            {
                temp->parent = NULL;
                temp= temp->right;
            }
            
            (*head_root_list)= temp;
            trees = ptr->rank;
            ptr->right = NULL;
            ptr->left = NULL;
            free(ptr);
            consolidation(head_root_list , min_ptr , N);
        }
        else
        {
            (*head_root_list)= NULL;
            ptr->left = NULL;
            ptr->right = NULL;
            free(ptr);
        }
        return;
    }
    
    if(ptr->child != NULL)
    {
        temp= ptr->child;
        ptr->child = NULL;
        
        while(temp->parent != NULL)
        {
            temp->parent = NULL;
            temp= temp->right;
        }
        
        
        temp1 = temp->left;
        
        temp1->right = ptr->right;
        if(ptr->right != NULL)
        { ptr->right->left = temp1; }
        
        temp->left = ptr->left;
        if(ptr->left != NULL)
        { ptr->left->right = temp; }
        
        if(ptr == (*head_root_list))
        { 
            if(ptr != ptr->right)
            { (*head_root_list) = ptr->right; }
            else
            { (*head_root_list) = NULL; }
            
        }
        
        ptr->right = NULL;
        ptr->left = NULL;
        free(ptr);
    }
    else
    {
        if(ptr->right != NULL)
        { ptr->right->left = ptr->left; }
        if(ptr->left != NULL)
        { ptr->left->right = ptr->right; }
        
        if(ptr == (*head_root_list))
        { 
            if(ptr != ptr->right)
            { (*head_root_list) = ptr->right; }
            else
            { (*head_root_list) = NULL; } 
            
        }
        
        ptr->right = NULL;
        ptr->left = NULL;
        ptr->parent= NULL;
        free(ptr);
    }
    
    consolidation(head_root_list , min_ptr , N);
}

void decrease_key_fibo(fibo_heap **head_root_list, int v , int new_path , fibo_heap **ptr_fibo , fibo_heap **min_ptr , int code)  // possible error-> ptr_fibo
{
    if((*head_root_list)==NULL)
    { return; }
    
    int min;
    fibo_heap *ptr , *ancestor;
    ptr= ptr_fibo[v];
    if(ptr!=NULL)
    { ancestor= ptr->parent; }
    else
    { return; }
    
    if(ancestor != NULL)
    {
        if(ancestor->path <= new_path && code==0)
        {
            ptr->path = new_path;
            return;
        }
        else
        {
            if(ancestor->child == ptr)
            {
                if(ancestor->rank > 1)
                { ancestor->child = ptr->right; }
                else
                { ancestor->child = NULL; }
            }
            
            if(ptr->right != NULL)
            { ptr->right->left = ptr->left; }
            if(ptr->left != NULL)
            { ptr->left->right = ptr->right; }
            
            if(code==0)    
            { 
                ptr->path = new_path; 
                if((*min_ptr)->path > new_path)
                { (*min_ptr)= ptr; }
            }
            
            (ancestor->rank) = (ancestor->rank) - 1;
            ptr->parent = NULL;
            ptr->right = NULL;
            ptr->left = NULL;
            
            merge_fibo(head_root_list , ptr);
            
            if(ancestor->parent != NULL)
            {
                if(ancestor->marked == false && ancestor->parent != NULL)
                {
                    ancestor->marked = true;
                    return;
                }
                else
                { decrease_key_fibo(head_root_list, ancestor->vertex , ancestor->path , ptr_fibo , min_ptr , -1); }
            }
        }
    }
    else
    { 
        if(code==0)
        {
            ptr->path = new_path; 
            
            if((*min_ptr)->path > new_path)
            { (*min_ptr)= ptr; }
        }
    }
    
}

void delete_fibo(fibo_heap **head_root_list , fibo_heap **min_ptr , int N , fibo_heap **ptr_fibo , int count)
{
    if((*head_root_list)==NULL)
    { return; }
    
    int i;
    
    for(i=1;i<=N;i++)
    {
        if(ptr_fibo[i]!=NULL)
        {
            
            decrease_key_fibo(head_root_list , ptr_fibo[i]->vertex , -999999 , ptr_fibo , min_ptr , 0);
            extract_min_fibo(head_root_list , min_ptr , N-count , ptr_fibo);
            count++;
        }
    }
    
    (*head_root_list)=NULL;

}
// ************************************************************ end of FIBONACCI heap *****************************************************************

void print(int *shortest_path , int N)
{
    int i;
    for(i=1;i<=N;i++)
    { cout<<shortest_path[i]<<" "; }
    
    cout<<endl;
}



int bellman_ford(list<pair<int,int>> *graph , int source ,int N , int *shortest_path)
{
    int i=0 ,stat,j ,k;
    list<pair<int,int>>::iterator ptr;
    list<int> queue;
    shortest_path[source]= 0;
    
    
    
    for(j=1;j<=N;j++)
    {
        i=1;
        
        queue.push_back(source);
    
        for(k=1;k<=N-1;k++)
        { queue.push_back(k); }
        
        while(!queue.empty())
        {
            stat= queue.front();
            queue.pop_front();
            
            if(!(graph[stat].empty()))
            {
                for(ptr= graph[stat].begin(); ptr!=graph[stat].end() ; ptr++)
                { 
                    //relaxation
                    if( (shortest_path[stat]+(ptr->second)) < shortest_path[ptr->first])
                    { 
                        shortest_path[ptr->first]= (shortest_path[stat]+(ptr->second));
                        i=0;
                    }
                    
                }
            }
        }
        
        if(j==N && i==0)
        { return 0; }
    }
    
    return i;
    
}

void johnsons_algo(int option , int N , list<pair<int,int>> graph[])
{
    int i,j,min=999999,value,count;
    fibo_heap *head_root_list= NULL , *temp , *min_ptr;
    bino_heap *head=NULL, *tempo;
    list<pair<int,int>>::iterator itr;
    size = N;
    // STEP 1 :   Apply bellman-ford taking new vertex as source
    int s_path[N+2];
    
    for(i=1;i<=N+1;i++)
    {
        if(i==(N+1))
        { s_path[i]= 0; }
        else
        { s_path[i] = 999999; }
    }
    
    i= bellman_ford(graph,N+1,N+1, s_path);
    // cout<<"bellman_shortest path: ";
    // for(j=1;j<=N+1;j++)
    // { cout<<s_path[j]<<" "; }
    //cout<<endl<<endl;
    
    // STEP 2 :   If negative cycle present , value is -1
    if(i==0)         
    { 
        cout<<-1<<endl;
        return ;
    }
    else          
    {
        // STEP 3 :   Remove the extra vertex
        graph[N+1].clear();
        
        // STEP 4 :   Convert the graph edges to non-negative edges
        for(i=1;i<=N;i++)
        {
            if(!graph[i].empty())
            {
                for(itr= graph[i].begin(); itr!=graph[i].end();itr++)
                { itr->second = (itr->second) + s_path[i] - s_path[itr->first]; }
            }
        }
        
        // STEP 5 :   Apply dijkstra algo for each vertex using different data structure
        
        for(i=1;i<=N;i++)                       // "i" is the source from which dijkstra is applied
        {
            int arr[N+1]={999999};
            int ptr_binary[N+1];
            binary_heap arr_binary[N+1];
            bino_heap *ptr_bino[N+1]={NULL};
            fibo_heap *ptr_fibo[N+1]={NULL};
            
            bool visited[N+1]= {false};
            visited[i]=true;
            int shortest_path[N+1],answer[N+1];
            
            for(j=0;j<=N;j++)
            { 
                shortest_path[j]= 999999; 
                answer[j]= 999999;
            }
            
            shortest_path[i]=0;
            answer[i]= 0;
            
            if(option==2)
            { count=0; }
            else
            { count= 1; }
                
            if(option==1)          // array creation
            { 
                for(j=1;j<=N;j++)
                { arr[j]= 999999; }
                arr[i]= INT_MAX;
            }
            else if(option==2)     // binary heap creation
            { 
                for(j=1;j<=N;j++)
                {
                    arr_binary[j].vertex= j;
                    arr_binary[j].path= 999999;
                    ptr_binary[j]= j;
                }
            }
            else if(option==3)     // binomial heap creation
            { 
                for(j=1;j<=N;j++)
                {
                    if(i!=j)
                    {
                        tempo = create_bino_node(j);
                        ptr_bino[j]= tempo;
                        
                        if(head != NULL)
                        { un(&head,tempo); }
                        else
                        { head = tempo; }
                    }
                }
            }
            else                   // fibonacci heap creation
            {
                for(j=1;j<=N;j++)
                {
                    if(i!=j)
                    {
                        if(head_root_list != NULL)
                        {
                            temp = insert_fibo(head_root_list,j);
                            ptr_fibo[j]= temp;
                        }
                        else
                        {
                            head_root_list = create_fibo_node(j);
                            head_root_list -> right = head_root_list;
                            head_root_list -> left = head_root_list;
                            ptr_fibo[j]= head_root_list;
                        }
                    }
                }
                min_ptr= head_root_list;
                trees= (N-1);
                // *****  End of creation  *****
            }
            
            //cout<<"Creation complete"<<endl<<endl;
            
            // -----  Dijkstra on one vertex  -----
            for(itr= graph[i].begin() ; itr!=graph[i].end() ; itr++)     // source vertex
            {
                if( (shortest_path[i] + (itr->second)) < shortest_path[itr->first] )
                {
                    shortest_path[itr->first] = (shortest_path[i] + (itr->second));    
                    answer[itr->first] = answer[i] + (itr->second) + s_path[itr->first] - s_path[i];
                    
                    if(option==1)          // array
                    { decrease_key_arr(arr , (itr->first) , shortest_path[itr->first]); }
                    else if(option==2)     // binary heap
                    { decrease_key_binary(arr_binary , (itr->first) , shortest_path[itr->first] , ptr_binary); }
                    else if(option==3)     // binomial heap
                    { decrease_key_bino(&head , (itr->first) , shortest_path[itr->first] , ptr_bino); }
                    else                   // fibonacci heap
                    { decrease_key_fibo(&head_root_list, (itr->first) , shortest_path[itr->first] , ptr_fibo , &min_ptr , 0); } 
                }
            }
            
            //cout<<"Dijkstra on source vertex complete."<<endl;
            
            for(j=1;j<=N-1;j++)
            {
                if(option==1)
                { value= find_min_arr(arr,visited);}
                else if(option==2)
                { value= arr_binary[1].vertex; }
                else if(option==3)
                {
                    tempo = find_min_bino(head);
                    if(tempo==NULL)
                    { 
                        //cout<<"Horray"<<endl;
                        break;
                    }
                    value= tempo->vertex;
                }
                else
                { 
                    if(min_ptr==NULL)
                    { break; }
                    else
                    { value= min_ptr->vertex; }
                }
                
                //cout<<"<------   value: "<<value<<"    ----->"<<endl;
                
                if(visited[value]==false)
                {
                    visited[value]= true;
                    
                    // extract_min and updating "min_ptr"
                    if(option==1)
                    { arr[value]= INT_MAX; }
                    else if(option==2)
                    {
                        extract_min_binary(arr_binary , ptr_binary , N-count);
                        count++;
                    }
                    else if(option==3)
                    { extract_min_bino(&head,ptr_bino); }
                    else
                    {
                        //cout<<"count: "<<count<<endl;
                        extract_min_fibo(&head_root_list , &min_ptr , N-count , ptr_fibo);           
                        count++;
                    }
                        
                    for(itr=graph[value].begin() ; itr!=graph[value].end() ; itr++)
                    {
                        if( (shortest_path[value] + (itr->second)) < shortest_path[itr->first] )
                        {
                            shortest_path[itr->first] = (shortest_path[value] + (itr->second) );
                            answer[itr->first] = answer[value] + (itr->second) + s_path[itr->first] - s_path[value];
                            
                            // decrease_key
                            if(option==1)          // array
                            { decrease_key_arr(arr , (itr->first) , shortest_path[itr->first]); }
                            else if(option==2)     // binary heap
                            { decrease_key_binary(arr_binary , (itr->first) , shortest_path[itr->first] , ptr_binary); }
                            else if(option==3)     // binomial heap
                            { decrease_key_bino(&head , (itr->first) , shortest_path[itr->first] , ptr_bino); }
                            else                   // fibonacci heap
                            { 
                                //cout<<"vertex: "<<itr->first<<"    ";
                                decrease_key_fibo(&head_root_list, (itr->first) , shortest_path[itr->first] , ptr_fibo , &min_ptr , 0); 
                            }   
                        }
                    }
                    
                }
                //cout<<endl<<endl;
            }
            // ***** End of dijkstra  *****
               
            //cout<<"Dijkstra ends"<<endl;
            
            // Deletion of data structure
            if(option==3)
            { delete_bino(&head , ptr_bino , N); }
            else if(option!=1 || option!=2)
            { delete_fibo(&head_root_list , &min_ptr , N-count , ptr_fibo , count); }
            
            //cout<<"Deletion ends"<<endl<<endl;
            
            print(answer,N);
        }
            
            
    }
     
}


int main(int argc , char *argv[])
{
    
    int t,N,D,i,j,inp,option,error,k=0,test;
    clock_t time_req;
    
    if(argc>=2)
    {
        if(*argv[1]=='1')
        { option=1; }
        else if(*argv[1]=='2')
        { option=2; }
        else if(*argv[1]=='3')
        { option=3; }
        else
        { option=4; }
    }
    else
    { option=4; }
    
    cin>>t;
    test= t;
    float time_complexity[t+2];
    while(t--)
    {
        cin>>N>>D;
        list<pair<int,int>> graph[N+2];
        pair<int,int> p1;
        //int arr[N+1][N+1];
        
        error=0;
        
        // -----  taking input  -----
        
        
        for(i=1;i<=N;i++)
        {
            for(j=1;j<=N;j++)
            {
                cin>>inp;
                
                if(i!=j && inp<999999)
                {
                    p1= make_pair(j,inp);
                    graph[i].push_back(p1);
                    
                    if(D==0)
                    {
                        if(inp<0)
                        { error= -1; }
                    }
                }
            }
        }
        
        for(i=1;i<=N;i++)
        {
            p1= make_pair(i,0);
            graph[N+1].push_back(p1);
        }
        // *****  End of taking input  *****
        
        time_req= clock();
        
        if(error!=-1)
        { johnsons_algo(option,N,graph); }
        else
        { cout<<-1<<endl; }
        
        time_req= clock() - time_req;
        
        time_complexity[k]= (float)time_req/CLOCKS_PER_SEC;
        k++;
        for(i=1;i<=N;i++)
        { graph[i].clear(); }
        
        
    }
    
    for(k=0;k<test;k++)
    { cout<<time_complexity[k]<<" "; }
    cout<<endl;
    return 0;
}









