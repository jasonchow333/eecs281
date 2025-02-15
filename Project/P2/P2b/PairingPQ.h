// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            // TODO: After you add add one extra pointer (see below), be sure
            // to initialize it here.
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr }, previous{ nullptr }
            {}

            // Description: Allows access to the element at that Node's
            // position.  There are two versions, getElt() and a dereference
            // operator, use whichever one seems more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data
            // members of this Node class from within the PairingPQ class.
            // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
            // function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            // TODO: Add one extra pointer (parent or previous) as desired.
            Node *previous;
    }; // Node


    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        // TODO: Implement this function.
        root = nullptr;
        count = 0;
    } // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        // TODO: Implement this function.
        root = nullptr;
        count = 0;
        for (auto i = start; i < end; i++){
            push(*i);
        }
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other) :
        BaseClass{ other.compare } {
        // TODO: Implement this function.
        // NOTE: The structure does not have to be identical to the original,
        //       but it must still be a valid pairing heap.
        root = nullptr;
        count = 0;
        if (other.root == nullptr) return;
        std::deque<Node *> node_dq;
        node_dq.push_back(other.root);
        while (!node_dq.empty()){
            Node *temp = node_dq.front();
            node_dq.pop_front();
            if (temp->child != nullptr){
                node_dq.push_back(temp->child);
            }
            if (temp->sibling != nullptr){
                node_dq.push_back(temp->sibling);
            }
            push(temp->elt);
        }
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
        // TODO: Implement this function.
        // HINT: Use the copy-swap method from the "Arrays and Containers"
        // lecture.
        PairingPQ temp(rhs);
        std::swap(temp.root, root);
        std::swap(temp.count, count);
        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        // TODO: Implement this function.
        if (root == nullptr) return;
        std::deque<Node *> de_dq;
        de_dq.push_back(root);
        while (!de_dq.empty()){
            Node *temp = de_dq.front();
            de_dq.pop_front();
            if (temp->child != nullptr){
                de_dq.push_back(temp->child);
            }
            if (temp->sibling != nullptr){
                de_dq.push_back(temp->sibling);
            }
            delete temp;
        }
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant.  You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        // TODO: Implement this function.
        if (root == nullptr) return;
        std::deque<Node *> un_dq;
        un_dq.push_back(root);
        root = nullptr;
        while (!un_dq.empty()){
            Node *temp = un_dq.front();
            un_dq.pop_front();
            if (temp->child != nullptr){
                un_dq.push_back(temp->child);
            }
            if (temp->sibling != nullptr){
                un_dq.push_back(temp->sibling);
            }
            temp->sibling = nullptr;
            temp->previous = nullptr;
            temp->child = nullptr;
            root = meld(root, temp);
        }
    } // updatePriorities()


    // Description: Add a new element to the pairing heap. This is already
    //              done. You should implement push functionality entirely
    //              in the addNode() function, and this function calls
    //              addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) {
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the pairing heap.
    // Note: We will not run tests on your code that would require it to pop
    // an element when the pairing heap is empty. Though you are welcome to
    // if you are familiar with them, you do not need to use exceptions in
    // this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        // TODO: Implement this function.
        if (!empty()){
            if (root->child == nullptr){
                delete root;
                root = nullptr;
                count--;
                return;
            }
            Node *temp = root->child;
            std::deque<Node *> pop_dq;
            pop_dq.push_back(temp);
            while (temp->sibling != nullptr){
                temp = temp->sibling;
                pop_dq.push_back(temp);
            }
            Node *temp_sib;
            while (pop_dq.size() != 1){
                temp = pop_dq.front();
                pop_dq.pop_front();
                temp_sib = pop_dq.front();
                pop_dq.pop_front();
                temp->sibling = nullptr;
                temp->previous = nullptr;
                temp_sib->sibling = nullptr;
                temp_sib->previous = nullptr;
                pop_dq.push_back(meld(temp, temp_sib));
            }
            delete root;
            root = pop_dq.front();
            count--;
        }
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        // TODO: Implement this function
        return root->elt;
    } // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        // TODO: Implement this function
        return count;
    } // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        // TODO: Implement this function
        return root == nullptr;
    } // empty()


    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value.  Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //              extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node* node, const TYPE &new_value) {
        // TODO: Implement this function
        node->elt = new_value;
        if (node == root) return;
        if (node->previous->child == node){
            node->previous->child = node->sibling;
        }
        else{
            node->previous->sibling = node->sibling;
        }
        if (node->sibling != nullptr){
            node->sibling->previous = node->previous;
        }
        node->sibling = nullptr;
        node->previous = nullptr;
        root = meld(root, node);
    } // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // Runtime: O(1)
    // NOTE: Whenever you create a node, and thus return a Node *, you must
    //       be sure to never move or copy/delete that node in the future,
    //       until it is eliminated by the user calling pop(). Remember this
    //       when you implement updateElt() and updatePriorities().
    Node* addNode(const TYPE &val) {
        // TODO: Implement this function
        Node *ins = new Node(val);
        if (root == nullptr){
            root = ins;
            count++;
            return root;
        }
        root = meld(root, ins);
        count++;
        return ins;
    } // addNode()


private:
    // TODO: Add any additional member variables or member functions you
    // require here.
    Node *root;
    size_t count;
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap
    // papers).
    Node *meld(Node *prior, Node *less){
        if (prior == nullptr) return less;
        if (less == nullptr) return prior;
        if (this->compare(prior->getElt(), less->getElt())) std::swap(prior, less);
        less->sibling = prior->child;
        if (less->sibling != nullptr){
            less->sibling->previous = less;
        }
        less->previous = prior;
        prior->child = less;
        return prior;
    }

    // NOTE: For member variables, you are only allowed to add a "root
    //       pointer" and a "count" of the number of nodes. Anything else
    //       (such as a deque) should be declared inside of member functions
    //       as needed.
};


#endif // PAIRINGPQ_H
