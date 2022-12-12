#include <iostream>
#include <list>
#include <algorithm>
#include <queue>
#include <chrono>
using namespace std::chrono;
using namespace std;

template<typename T>
struct Node {
    T key;
    Node* left;
    Node* right;
    int rank;    
};

template<typename T>
class ArbAvl{
    public:
    Node<T> *root=NULL;
    bool find_node(struct Node<T>* node, T x);
    void avg_len(struct Node<T> * node);
    int height(struct Node<T>* node);
    T max(T x, T y);
    struct Node<T> *newNode(T key);
    struct Node<T>*rightRotate(struct Node<T>*y);
    struct Node<T>*leftRotate(struct Node<T>*x);
    struct Node<T>*insert(struct Node<T>*node, T key);
    struct Node<T>*deleteNode(struct Node<T>*node, T key);
    struct Node<T>*minValueNode(struct Node<T>* node);
    void preOrder(struct Node<T> *root);
    int getBalance(struct Node<T>*N);
    void printNiveles(struct Node<T>*p);
    ~ArbAvl(); 

};

static int no_rot=0;
static float len=0,no_ops=0;
static int bal_update=0;
static int max_len=0;
static int miss=0;
template <typename T>
void ArbAvl<T>::avg_len(struct Node<T>* node) {
    len= len + node->rank+1;
    if (node->rank > max_len){
        max_len=node->rank;
    }
    no_ops++;}

template<typename T>
int ArbAvl<T>::height(struct Node<T>* N){
    if (N==NULL) return -1;
    return N->rank;
}


template<typename T>
bool ArbAvl<T>::find_node(struct Node<T>* node, T x)
{
    if (node == NULL)
        return false;
 
    if (node->key == x)
        return true;

    bool res1 = find_node(node->left, x);
    if(res1) return true;
    bool res2 = find_node(node->right, x);
    return res2;
}
template<typename T>
T ArbAvl<T>::max(T a, T b){
    return (a>b)? a:b;
}

template<typename T>
struct Node<T>* ArbAvl<T>::newNode(T key){
    struct Node<T>* node = (struct Node<T>*) malloc(sizeof(struct Node<T>));
    node->key = key;
    node->left = node->right = NULL;
    node->rank = 0;
    return (node);}
template<typename T>
struct Node<T> * ArbAvl<T>::rightRotate(struct Node<T> *y){
    no_rot++;
    struct Node<T> *x = y->left;
    struct Node<T> *T2 = x->right;
    
    x->right = y;
    y->left = T2;

    y->rank = x->rank;
    x->rank -= 1;
    return x;

}
template<typename T>
struct Node<T> * ArbAvl<T>::leftRotate(struct Node<T> *x)
{
    no_rot++;
    struct Node<T> *y = x->right;
    struct Node<T> *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    //  demote rank of node

    x->rank-=1;

    // Return new root
    return y;
}

template<typename T>
int ArbAvl<T>::getBalance(struct Node<T>*N){
    if (N == NULL)
        return 0;
    return height(N->left) -
           height(N->right);
}

template<typename T>
struct Node<T>* ArbAvl<T>::insert(struct Node<T>* node, T key){
    if (node == NULL) return (newNode(key));
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;

    // calculate height of node difference
    int leftDiff = height(node) - height(node->left);
    int rightDiff = height(node) - height(node->right);

    //unbalancedtree

    if ((leftDiff == 0 && rightDiff ==1) || (leftDiff==1 && rightDiff == 0)){
        node->rank += 1;
        return node;
    }
    if (rightDiff == 0){
        int temp_left = height(node->right) - height(node->right->left);
        int temp_right = height(node->right) - height(node->right->right);
        //LEFT ROTATION
        if ((temp_left == 2) || (height(node->right->left) == -1)){
            bal_update++;
            return leftRotate(node);
        } else{
            node->right = rightRotate(node->right);
            node->right->rank += 1;
            bal_update++;
            return leftRotate(node);
        }
    } 
    if (leftDiff == 0){
        int temp_left = height(node->left) - height(node->left->left);
        int temp_right = height(node->left) - height(node->left->right);
        //RIGHT ROTATION
        if ((temp_left == 2) || (height(node->left->right) ==-1)){
            bal_update++;
            return rightRotate(node);
        } else{
            node->left = leftRotate(node->left);
            node->left->rank += 1;
            bal_update++;
            return rightRotate(node);
        }
    }
    return node;
}
template<typename T>
struct Node<T> * ArbAvl<T>::minValueNode(struct Node<T>* node)
{
    struct Node<T>* current = node;

  
    while (current->left != NULL)
        current = current->left;

    return current;
}

template<typename T>
struct Node<T>* ArbAvl<T>::deleteNode(struct Node<T>* root, T key)
{
   
    if (root == NULL)
        return root;
 
    if ( key < root->key ){
     
        root->left = deleteNode(root->left, key);
    }
 
    else if( key > root->key ){
        
        root->right = deleteNode(root->right, key);
    }
    else
    {
        
        if( (root->left == NULL) ||
            (root->right == NULL) )
        {
            Node<T> *temp = root->left ?
                         root->left :
                         root->right;
 
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else 
            *root = *temp; 
            free(temp);
        }
        else
        {
            Node<T>* temp = minValueNode(root->right);
 
            root->key = temp->key;
 
            root->right = deleteNode(root->right,
                                     temp->key);
        }
    }
 
    if (root == NULL)
    return root;
 
    root->rank = 1 + max(height(root->left),
                           height(root->right));
 
    int balance = getBalance(root);
 
   
 
    // Left Left Case
    if (balance > 1 &&
        getBalance(root->left) >= 0)
        return rightRotate(root);
 
    // Left Right Case
    if (balance > 1 &&
        getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
 
    // Right Right Case
    if (balance < -1 &&
        getBalance(root->right) <= 0)
        return leftRotate(root);
 
    // Right Left Case
    if (balance < -1 &&
        getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
 
    return root;


}
template <typename T>
void ArbAvl<T>::preOrder(struct Node<T> *root)
{
    if(root != NULL)
    {
        cout<< root->key << " ";
        preOrder(root->left);
        preOrder(root->right);
    }

}

template<typename T>
void printLevelOrder(struct Node<T>* root)
{
    // Base Case
    if (root == NULL)
        return;
 
    // Create an empty queue for level order traversal
    queue<Node<T>*> q;
 
    // Enqueue Root and initialize height
    q.push(root);
 
    while (q.empty() == false) {
        // nodeCount (queue size) indicates number
        // of nodes at current level.
        int nodeCount = q.size();
 
        while (nodeCount > 0) {
            Node<T>* node = q.front();
            cout << node->key << " ";
            q.pop();
            if (node->left != NULL)
                q.push(node->left);
            if (node->right != NULL)
                q.push(node->right);
            nodeCount--;
        }
        cout << endl;
    }
}

template<typename T>
ArbAvl<T>::~ArbAvl(){
    while(root != NULL){
        deleteNode(root,root->key);
    }
}

int main()
{
    // inicializamos el root
    ArbAvl<int> arb;
    arb.root = NULL;
    int value = 0;
    //INSERT
    auto start = high_resolution_clock::now();
    for (long long i=0; i<=100;i++){ 
        arb.root=arb.insert(arb.root, i);
        //arb.avg_len(arb.root);
    }
    //arb.preOrder(arb.root);
    //printLevelOrder(arb.root);
    cout << "busca5: "<<arb.find_node(arb.root,50) << endl;
    for(long long i=0; i<=100;i++){
        arb.deleteNode(arb.root,i);
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
 
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;
    /*     
    cout << "---preorder-----------------"<<endl;
    cout << endl;
    cout << "---levelorder-----------------"<<endl;
    printLevelOrder(arb.root);
    
        
    cout << "busca10: " <<arb.find_node(arb.root,10) <<endl;
    cout << "---preorder-----------------"<<endl;
    arb.preOrder(arb.root);
    cout << "\nno of rotations:\n" << no_rot << endl;
    cout << endl;
    cout << "---levelorder-----------------"<<endl;
    //arb.deleteNode(arb.root,3);
    float avg_length=len/no_ops;
    
    cout << "---levelorder-----------------"<<endl;
    printLevelOrder(arb.root);*/
    return 0;
}