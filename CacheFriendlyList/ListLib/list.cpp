#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

ref_t List::head() const {
    return m_nodes[ghost()].next;
}

ref_t List::tail() const {
    return m_nodes[ghost()].prev;
}

ref_t List::next (ref_t ref) const {
    if (is_valid_ref (ref)) {
        return m_nodes[ref].next;
    }
    return List::BAD_REF;
}

ref_t List::prev (ref_t ref) const {
    if (is_valid_ref (ref)) {
        return m_nodes[ref].prev;
    }
    return List::BAD_REF;
}

val_t &List::get (ref_t ref) {
    if (is_valid_ref (ref)) {
        return m_nodes[ref].val;
    }
    return m_nodes[tail()].val;
}

const val_t &List::get (ref_t ref) const {
    if (is_valid_ref (ref)) {
        return m_nodes[ref].val;
    }
    return m_nodes[tail()].val;
}

ref_t List::size() const {
    return m_size;    
}    

ref_t List::allocate_val (val_t val) {
    auto ref = free_head();
    if (ref != List::BAD_REF) {
        shift_free_head();
        m_nodes[ref].val = val;
        m_nodes[ref].next = List::BAD_REF;
        m_nodes[ref].prev = List::BAD_REF;
        ++m_size;
    }

    return ref;
}

ref_t List::free_head() const {
    return m_free_head_ref;
}

void List::shift_free_head() {
    m_free_head_ref = m_nodes[free_head()].next;
}

ref_t List::erase (ref_t ref) {
    if (!is_valid_ref (ref)) {
        return List::BAD_REF;
    }

    m_nodes [ref].prev = ref;           // mark as free
    m_nodes [ref].next = free_head();   // tie with free list
    m_free_head_ref = ref; 
    --m_size;
    return ref;
}
#include <stdio.h>
List::List (ref_t cap) {
    m_capacity = (cap < 16) ? DEFAULT_CAPACITY : cap + 1; // for ghost elem
    printf ("m_capacity = %zu\n", m_capacity);
    m_nodes = new List::Node[m_capacity];
    m_size = 0;
    m_free_head_ref = 0;
    for (size_t i = 0; i < m_capacity; ++i) {
        m_nodes[i].prev = i;  // mark free vals
        m_nodes[i].next = i + 1;  // tie free list
    }
    m_nodes[m_capacity - 1].next = List::BAD_REF;

    m_ghost = allocate_val ({});
    if (ghost() != BAD_REF) {
        --m_size; 
        m_nodes[ghost()].next = m_nodes[ghost()].prev = ghost();
    }
}

List::List (const std::initializer_list<val_t> &init_list) :
    List (init_list.size()) { 
    for (const auto &i : init_list) {
        insert_back (i);
        printf ("%lg ", i);
    }
}

List::~List() {
    if (m_nodes) {
        delete [] m_nodes;
    }
    memset (this, 0, sizeof (*this));
    m_nodes = nullptr;
}

ref_t List::insert_front (val_t val) {
    return insert_before (head(), val);
}

ref_t List::insert_back (val_t val) {
    return insert_after (tail(), val);
}

ref_t List::insert_after (ref_t ref, val_t val) {
    if (!is_valid_ref (ref)) {
        return List::BAD_REF;
    }

    ref_t new_ref = allocate_val (val);
    if (new_ref != List::BAD_REF) {
        m_nodes[new_ref].prev = ref;
        m_nodes[new_ref].next = m_nodes[ref].next;    
        
        m_nodes[next (ref)].prev = new_ref;
        m_nodes[ref].next        = new_ref;
    }
    return new_ref;
}

ref_t List::insert_before (ref_t ref, val_t val) {
    if (!is_valid_ref (ref)) {
        return BAD_REF;
    }

    ref_t new_ref = allocate_val (val);
    if (new_ref != List::BAD_REF) {
        m_nodes[new_ref].next = ref;
        m_nodes[new_ref].prev = m_nodes[ref].prev;

        m_nodes[prev (ref)].next = new_ref;
        m_nodes[ref].prev        = new_ref;
    }
    return new_ref;
}

//ref_t list_erase (List *list, ref_t ref) {
//    assert (list);
//
//    if (!is_valid_ref (list, ref)) { // alreay erased
//        return ref;
//    }
//
//    if (ref == HEAD (list)) {
//        HEAD (list) = NEXT (list, HEAD (list));
//        list_erase_ref (list, ref); 
//        return ref;
//    } else {
//        auto prev_ref = PREV (list, ref);
//        if (prev_ref != List::BAD_REF) {
//            NEXT (list, prev_ref) = NEXT (list, ref);
//            if (NEXT (list, ref) != List::BAD_REF) { // is there a next to tie
//                PREV (list, NEXT (list, ref)) = prev_ref;
//            }
//            list_erase_ref (list, ref); 
//            return ref;   
//        }
//    }
//    return List::BAD_REF;
//}
//
bool List::is_valid_ref (ref_t ref) const {
    if (ref != List::BAD_REF) {
        return true;
    }
    return false;
}

void List::set_head (ref_t ref) {
    m_nodes[ghost()].next = ref;
}

void List::set_tail (ref_t ref) {
    m_nodes[ghost()].prev = ref;
}

ref_t List::ghost() const {
    return m_ghost;
}
