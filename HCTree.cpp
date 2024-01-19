#include "HCTree.hpp"
#include "Helper.hpp"
#include <vector>
#include <queue>
#
void helper(HCNode * node);

void helper(HCNode * node){
    if(node == nullptr){
        return;
    }
    if(node -> c1 != nullptr){
        helper(node -> c1);
    }
    if(node -> c0 != nullptr){
        helper(node -> c0);
    }
    delete node;
    node = nullptr;
}
HCTree::~HCTree(){
    if(root != nullptr){
        helper(root);
    }
    root = nullptr;
    leaves.clear();

}
void HCTree::build(const vector<int>& freqs){
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    //insert everything in the pq
    for(unsigned long i = 0; i < freqs.size(); i++){
        if(freqs[i] > 0){
            HCNode *create = new HCNode(freqs[i], (unsigned char)i+1);
            leaves[i] = create;
            pq.push(create); //what do we do with leaf?
        }
    }
    //already insert everything in the priority_queue
    while(pq.size() != 0 ){
        
        HCNode * temp1 = pq.top();
        pq.pop();
        HCNode * temp2 = pq.top();
        pq.pop();
        HCNode * create_new = new HCNode(temp1->count + temp2 -> count, 'a');
        temp1->p = create_new;
        temp2->p = create_new;
        create_new->c0 = temp1;
        create_new->c1 = temp2;
        pq.push(create_new);
        if(pq.size() == 1){
            root = pq.top();//??
            pq.pop();
            break;
        }
    }
    

}
void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const{
    HCNode *curr = leaves[(int)symbol-1];
    vector<int> eightbits;
    while(curr->p != nullptr){/*
        if(curr->p == nullptr){
            break;
        }*/
        if(curr->p->c0 == curr){
            eightbits.push_back(0);
        }
        else{
            eightbits.push_back(1);
        }
        curr = curr->p;
    }
    for(int i = eightbits.size(); i > 0; i--){
        out.write_bit(eightbits[i-1]);
    }
}
unsigned char HCTree::decode(FancyInputStream & in) const{
    HCNode *start = root;
    while(start -> c0 != nullptr && start -> c1 != nullptr){
        int number = in.read_bit();
        if(number == 1){
            start = start->c1;
        }else if(number==0){
            start = start->c0;
        }else if(number==-1){
            break;
        }
    }
    return start->symbol;

}
//return the vector 



