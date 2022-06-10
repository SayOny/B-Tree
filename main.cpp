
#include<iostream>
#include <random>
#include <time.h>


using namespace std;

// b tree의 node를 나타내는 클래스
class bTreeNode{
    bTreeNode **node;
    int t;
    int *keys;
    bool l;
    int n;
    
public:
    bTreeNode(int a, bool b){
        t = a;
        l = b;
        keys = new int[2*t-1];
        node = new bTreeNode *[2*t];
        n = 0;
    }

    void traverse();
    void insert(int key);
    void split(bTreeNode *y, int i);
    void deleteN(int key);
    void merge(int idx);
    friend class bTree;
};

// b tree를 위한 클래스
class bTree{
    bTreeNode *root;
    int t;
    
public:
    bTree(int a){
        root = NULL;
        t = a;
    }
    void traverse(){
        if (root != NULL) root->traverse();
    }
    void insert(int key);
    void deleteN(int key);
};

// 트리를 순회하는 함수
void bTreeNode::traverse(){
    int i;
    for (i = 0; i < n; i++){
        if (l == false)
            node[i]->traverse();
        cout << " " << keys[i];
    }

    if (l == false){
        node[i]->traverse();
    }
}

// 트리 노드에 키를 삽입하는 함수
void bTreeNode::insert(int key){
    int i = n-1;

    if (l == true){
        while (i >= 0 && keys[i] > key){
            keys[i+1] = keys[i];
            i--;
        }
        keys[i+1] = key;
        n = n+1;
    }else{
        while (i >= 0 && keys[i] > key){
            i--;
        }
        if (node[i+1]->n == 2*t-1)
        {
            split(node[i+1], i+1);
            if (keys[i+1] < key)
                i++;
        }
        node[i+1]->insert(key);
    }
}

// 노드를 나누는 함수
void bTreeNode::split(bTreeNode *n1, int i){
    bTreeNode *n2 = new bTreeNode(n1->t, n1->l);
    n2->n = t - 1;

    for (int j = 0; j < t-1; j++)
        n2->keys[j] = n1->keys[j+t];

    if (n1->l == false){
        for (int j = 0; j < t; j++)
            n2->node[j] = n1->node[j+t];
    }

    n1->n = t - 1;

    for (int j = n; j >= i+1; j--)
        node[j+1] = node[j];

    node[i+1] = n2;

    for (int j = n-1; j >= i; j--)
        keys[j+1] = keys[j];

    keys[i] = n1->keys[t-1];

    n = n + 1;
}

// 노드에서 키를 삭제하는 함수
void bTreeNode::deleteN(int key){
    bTreeNode * c;
    bTreeNode *s;
    
    int idx = 0;
    while (idx<n && keys[idx] < key){
        ++idx;
    }

    if (idx < n && keys[idx] == key){
        if (l){
            for (int i=idx+1; i<n; ++i){
                keys[i-1] = keys[i];
            }
            n--;
        }
        else{
            bTreeNode *tmp;

            if (node[idx]->n >= t){
                tmp = node[idx];
                while (!tmp->l){
                    tmp = tmp->node[tmp->n];
                }
                int p = tmp->keys[tmp->n-1];
                keys[idx] = p;
                node[idx]->deleteN(p);
            }

            else if  (node[idx+1]->n >= t){
                tmp = node[idx+1];
                while (!tmp->l){
                    tmp = tmp->node[0];
                }
                int com = tmp->keys[0];
                keys[idx] = com;
                node[idx+1]->deleteN(com);
            }
            else{
                merge(idx);
                node[idx]->deleteN(key);
            }
        }
    }
    else{
        if (l){
//            cout << "Doesn't exist " << key << endl;
            return;
        }

        bool flag = ( (idx==n)? true : false );

        if (node[idx]->n < t){
            if (idx!=0 && node[idx-1]->n>=t){
                c=node[idx];
                s=node[idx-1];

                for (int i=c->n-1; i>=0; --i){
                    c->keys[i+1] = c->keys[i];
                }
                if (!c->l){
                    for(int i=c->n; i>=0; --i)
                        c->node[i+1] = c->node[i];
                }

                c->keys[0] = keys[idx-1];

                if(!c->l){
                    c->node[0] = s->node[s->n];
                }
                keys[idx-1] = s->keys[s->n-1];

                c->n += 1;
                s->n -= 1;
            }
            else if (idx!=n && node[idx+1]->n>=t){
                c=node[idx];
                s=node[idx+1];
                
                c->keys[(c->n)] = keys[idx];

                if (!(c->l))
                    c->node[(c->n)+1] = s->node[0];

                keys[idx] = s->keys[0];

                for (int i=1; i<s->n; ++i)
                    s->keys[i-1] = s->keys[i];

                if (!s->l){
                    for(int i=1; i<=s->n; ++i)
                        s->node[i-1] = s->node[i];
                }
                c->n += 1;
                s->n -= 1;
            }
            else{
                if (idx != n)
                    merge(idx);
                else
                    merge(idx-1);
            }
        }
        if (flag && idx > n){
            node[idx-1]->deleteN(key);
        }
        else{
            node[idx]->deleteN(key);
        }
    }
}

// 트리 노트를 병합하는 함수
void bTreeNode::merge(int idx){
    bTreeNode *c = node[idx];
    bTreeNode *s = node[idx+1];

    c->keys[t-1] = keys[idx];

    for (int i=0; i<s->n; ++i){
        c->keys[i+t] = s->keys[i];
    }

    if (!c->l){
        for(int i=0; i<=s->n; ++i)
            c->node[i+t] = s->node[i];
    }

    for (int i=idx+1; i<n; ++i){
        keys[i-1] = keys[i];
    }

    for (int i=idx+2; i<=n; ++i){
        node[i-1] = node[i];
    }

    c->n += s->n+1;
    n--;

    delete(s);
}

// 트리에 키를 삽입하는 함수
void bTree::insert(int key){
    if (root == NULL){
        root = new bTreeNode(t, true);
        root->keys[0] = key;
        root->n = 1;
    }
    else{
        if (root->n == 2*t-1){
            bTreeNode *s = new bTreeNode(t, false);

            s->node[0] = root;
            s->split(root, 0);

            int i = 0;
            if (s->keys[0] < key)
                i++;
            s->node[i]->insert(key);

            root = s;
        }
        else{
            root->insert(key);
        }
    }
}

// 트리에서 키 삭제하는 함수
void bTree::deleteN(int key){

    root->deleteN(key);

    if (root->n==0){
        bTreeNode *tmp = root;
        if (root->l){
            root = NULL;
        }
        else{
            root = root->node[0];
        }
        delete tmp;
    }
}

int main(int argc, const char * argv[]){
    clock_t start, end;

    int t;
    cout << "Enter Minimum degree t: ";
    cin >> t;
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 9999);
    
    bTree bTree(t);
    int i;
    
    start = clock();
    for(i=0; i<1000; i++){
        bTree.insert(dis(gen));
    }
    for(i=0; i<500; i++){
        bTree.deleteN(dis(gen));
    }
    end = clock();

    cout << "\nThe time is " << (double)(end-start) << endl;


    return 0;
}
