#include "bstree.hpp"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <chrono>
using namespace std::chrono;
using namespace std;
#define MAXIMUM 1e9
pthread_mutex_t writeM;
class avlTree: public BST_int{
    private:
        bool avl_contains(node * root, int n){

            if(root == NULL)
                return false;
            else{
                //std::cout << "What have we here" <<std::endl;
                //pthread_mutex_lock(&lock);
                if(root->value == n){
                    //pthread_mutex_unlock(&lock);
                    return true;
                }
                bool val =  n > root->value ? avl_contains(root->right,n) : avl_contains(root->left,n);
                //pthread_mutex_unlock(&lock);
                return val;
            }
        }
        node* avl_rebalance(node* root){
            /*
            * While inside this function it is assumed
            * root is locked.
            */
            fixheight(root);

            if(balance_factor(root) >= 2){
                if(balance_factor(root->right) < 0){
                    root->right = rotateright(root->right);
                }
                //pthread_mutex_unlock(&(root->lock));
                return avl_rotateleft(root);
            }
            if(balance_factor(root) <= -2){
                if(balance_factor(root->left) > 0){
                    root->left = rotateleft(root->left);
                }
                //pthread_mutex_unlock(&(root->lock));
                return avl_rotateright(root);
            }
            //pthread_mutex_lock(&(root->lock));
            return root;
        }
        node* avl_rotateright(node* p){
            //pthread_mutex_lock(&(p->lock));
            //pthread_mutex_lock(&(p->left->lock));
            node* q = p->left;

            p->left = q->right;
            q->right = p;
            fixheight(p);
            fixheight(q);

            //pthread_mutex_unlock(&(p->lock));
            //pthread_mutex_unlock(&(q->lock));
            return q;
        }
        node* avl_rotateleft(node* q){
            //pthread_mutex_lock(&(q->lock));
            //pthread_mutex_lock(&(q->right->lock));
            node* p = q->right;

            q->right = p->left;
            p->left = q;
            fixheight(q);
            fixheight(p);

            //pthread_mutex_unlock(&(q->lock));
            //pthread_mutex_unlock(&(p->lock));
            return p;
        }
    //class AVL
    public:
        pthread_mutex_t lock;
        avlTree(){
            root = NULL;
            nNodes = 0;
            //pthread_mutex_init(&lock, NULL);
        }
        bool avl_contains(int n){
            pthread_mutex_lock(&lock);
            //bool b = searchTree(root, n);
            bool b = searchTree(n);
            pthread_mutex_unlock(&lock);
            return true;
        };
        void avl_add(int n){
            pthread_mutex_lock(&lock);
            //add_avl(root,n);
            add(n);
            pthread_mutex_unlock(&lock);
            //add_avl(root,n);
            return;
        };
        void avl_delete(int n){
            /*if(!avl_contains(n))
                return;
            else{*/
                pthread_mutex_lock(&lock);
                delNode(n);
                pthread_mutex_unlock(&lock);
            //}
        }
};
typedef struct package{
    avlTree *tree;
    int id;
    int maxOps;
    long timeTaken;
}pkg;

struct container{
    avlTree * tree;
    int ini;
    int fin;
};
void *perform_work(void *argument);
long trial(avlTree * tree, int numOps,bool explain);


void *insert_parallel(void *ptr){
    container *val;
    val = (container *) ptr;

    avlTree *trees;

    trees = val->tree;
    for(int i = val->ini; i<=val->fin; i++){
        trees->avl_add(i);
    }
}
void *delete_parallel(void *ptr){
    container *val;
    val = (container *) ptr;

    avlTree *trees;

    trees = val->tree;
    for(int i = val->ini; i<=val->fin; i++){
        trees->avl_delete(i);
    }
}


/*
test1:
 pthread_t t1,t2;
    int r1,r2;
    int ini = 0;
    int fin = 500000;
    int ini1 = 500001;
    int fin1 = 1000000;
    container val;
    val.tree = &tree;
    val.ini = ini;
    val.fin = fin;
    container  val2;
    val2.tree = &tree;
    val2.ini = ini1;
    val2.fin = fin1;


    r1 = pthread_create(&t1, NULL,insert_parallel,(void *) &val);
    r2 = pthread_create(&t1, NULL,insert_parallel,(void *) &val2);  

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_t t3,t4;
    
    int r3,r4;
    r3 = pthread_create(&t3, NULL,delete_parallel,(void *) &val);
    r4 = pthread_create(&t4, NULL,delete_parallel,(void *) &val2);  
    pthread_join(t3,NULL);
    pthread_join(t4,NULL);
*/

int main(void){
    avlTree tree;
    int NUM_THREADS = 2;
    auto start = high_resolution_clock::now();
    pthread_mutex_init(&tree.lock, NULL);
    tree.init(-1);//Sentinel value

    for(long long i=0; i<=100; i++){
        tree.avl_add(i);
    }
    cout << "busca 5omil" << tree.avl_contains(50);
    //tree.printLevelOrder(tree.get_root());
    for(long long i=0; i<=100; i++){
        tree.avl_delete(i);
    }
    pthread_mutex_destroy(&tree.lock);
	

   
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;
    


    //tree.delTree();
    //pthread_mutex_destroy(&tree.lock);
    return 0;
}
