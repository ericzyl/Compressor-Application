#include "HCTree.hpp"
#include "Helper.hpp"
#include <stack>
void postorderDelete(HCNode* n);


void HCTree::build(const vector<int>& freqs){
    // Create a priority queue as the underlying structure
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> hcq;
    //int total = 0;
    
    for(unsigned int i = 0; i < freqs.size(); i++){
        if(freqs[i] != 0){
            HCNode* n = new HCNode(freqs[i], i);
            //total += freqs[i];         
            leaves[i] = n;
            hcq.push(n);
        }
    }
    /*if(total == 0){
        HCNode* n = new HCNode(freqs[i], i);
        hcq.push(root);
    }*/

    // Construct Huffman Coding Tree
    // Assign 0 to the subtree with the lower total frequency (left child)
    while(hcq.size() > 1){
        // Create a parent node with character i
        HCNode* left = hcq.top();
        hcq.pop();
        HCNode* right = hcq.top();
        hcq.pop();
        int sum = left->count + right->count;
        HCNode* parent = new HCNode(sum, 'i');
        parent->c0 = left;
        parent->c1 = right;
        left->p = parent;
        right->p = parent;

        hcq.push(parent);
    }

    root = hcq.top();
}



HCTree::~HCTree(){
    postorderDelete(root);
    root = nullptr;
}



void postorderDelete(HCNode* n){
    if(n!=nullptr){
        postorderDelete(n->c0);
        postorderDelete(n->c1);
        delete n;
    }
}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const{
    stack<int> stack;

    // Map to the corresponding leaf in HCTree
    HCNode* curr = leaves[int(symbol)];

    if(curr == nullptr){
        return;
    }

    // Start from a leaf, going up to the root and store code to stack
    while(curr->p != nullptr){
        if(curr == curr->p->c0){
            stack.push(0);
        }else{
            stack.push(1);
        }
        curr = curr->p;
    }

    // Pop from stack to write to the file
    while(!stack.empty()){
        out.write_bit(stack.top());
        stack.pop();
    }
}

unsigned char HCTree::decode(FancyInputStream & in) const{
    HCNode* curr = root;
    /*if(curr == nullptr){
        error("Nullpointer");
    }*/
    int bit = 0;
    while(curr->c0 != nullptr){
        bit = in.read_bit();
        if(bit == 0){
            curr = curr->c0;
        }else if(bit == 1){
            curr = curr->c1;
        }else{
            return '\0';
        }
    }
    return curr->symbol;
}





