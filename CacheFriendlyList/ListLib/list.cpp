#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

ref_t List::head() const {
    return m_buffer[ghost()].next;
}

ref_t List::tail() const {
    return m_buffer[ghost()].prev;
}

ref_t List::next (ref_t ref) const {
    if (is_valid_ref (ref)) {
        return m_buffer[ref].next;
    }
    return List::BAD_REF;
}

ref_t List::prev (ref_t ref) const {
    if (is_valid_ref (ref)) {
        return m_buffer[ref].prev;
    }
    return List::BAD_REF;
}

val_t &List::get (ref_t ref) {
    if (is_valid_ref (ref)) {
        return m_buffer[ref].val;
    }
    return m_buffer[tail()].val;
}

const val_t &List::get (ref_t ref) const {
    if (is_valid_ref (ref)) {
        return m_buffer[ref].val;
    }
    return m_buffer[tail()].val;
}

ref_t List::size() const {
    return m_size;    
}    

ref_t List::allocate_val (val_t val) {
    auto ref = free_head();

    if (m_buffer.capacity - 1 == m_size) {  // no free elem
        Buffer new_buffer (m_size*2 + 1);
        std::move (m_buffer.data, m_buffer.data + m_size, new_buffer.data);
        m_buffer.swap (new_buffer);
    }
    

    if (ref != List::BAD_REF) {
        shift_free_head();
        m_buffer[ref].val = val;
        m_buffer[ref].next = List::BAD_REF;
        m_buffer[ref].prev = List::BAD_REF;
        ++m_size;
    }

    return ref;
}

ref_t List::free_head() const {
    return m_free_head_ref;
}

void List::shift_free_head() {
    m_free_head_ref = m_buffer[free_head()].next;
}

ref_t List::erase (ref_t ref) {
    if (!is_valid_ref (ref)) {
        return List::BAD_REF;
    }

    m_buffer [ref].prev = ref;           // mark as free
    m_buffer [ref].next = free_head();   // tie with free list
    m_free_head_ref = ref; 
    --m_size;
    return ref;
}

List::List (ref_t cap) :
    m_buffer (((cap < 16) ? DEFAULT_CAPACITY : cap) + 1) {
    m_size = 0;
    m_free_head_ref = 0;
    for (size_t i = 0; i < m_buffer.capacity; ++i) {
        m_buffer[i].prev = i;  // mark free vals
        m_buffer[i].next = i + 1;  // tie free list
    }
    m_buffer[m_buffer.capacity - 1].next = List::BAD_REF;

    m_ghost = allocate_val ({});
    if (ghost() != BAD_REF) {
        --m_size; 
        m_buffer[ghost()].next = m_buffer[ghost()].prev = ghost();
    }
}

List::List (const std::initializer_list<val_t> &init_list) :
    List (init_list.size()) { 
    for (const auto &i : init_list) {
        insert_back (i);
    }
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
        m_buffer[new_ref].prev = ref;
        m_buffer[new_ref].next = m_buffer[ref].next;    
        
        m_buffer[next (ref)].prev = new_ref;
        m_buffer[ref].next        = new_ref;
    }
    return new_ref;
}

ref_t List::insert_before (ref_t ref, val_t val) {
    if (!is_valid_ref (ref)) {
        return BAD_REF;
    }

    ref_t new_ref = allocate_val (val);
    if (new_ref != List::BAD_REF) {
        m_buffer[new_ref].next = ref;
        m_buffer[new_ref].prev = m_buffer[ref].prev;

        m_buffer[prev (ref)].next = new_ref;
        m_buffer[ref].prev        = new_ref;
    }
    return new_ref;
}

gv_Graph List::list_to_gv_graph() const {
    gv_Graph graph = {};
    ref_t prev = 0;
    ref_t next = 0;

    if (gv_graph_init (&graph)) {
        for (size_t i = 0; i < m_buffer.capacity; ++i) {
            gv_graph_add_rank (&graph, i, GV_LAST_HANDLER);
        }

        for (size_t i = 0; i < m_buffer.capacity; ++i) {
            prev = m_buffer[i].prev;
            next = m_buffer[i].next;
            if (i != List::BAD_REF && prev != List::BAD_REF) {
                gv_graph_add_edje (&graph, i, prev, "");
            }
            if (i != List::BAD_REF && next != List::BAD_REF) {
                gv_graph_add_edje (&graph, i, next, "");
            }
        }
        for (size_t i = 0; i < m_buffer.capacity; ++i) {
            gv_graph_add_vertex (&graph, i, gv_record, 
                "{ %lg | { prev:%zu | curr:%zu | next:%zu } }", 
                get(i), m_buffer[i].prev,
                i, m_buffer[i].next);
        }
    } 
    return graph;
}

bool List::is_valid_ref (ref_t ref) const {
    if (ref != List::BAD_REF) {
        return true;
    }
    return false;
}

void List::set_head (ref_t ref) {
    m_buffer[ghost()].next = ref;
}

void List::set_tail (ref_t ref) {
    m_buffer[ghost()].prev = ref;
}

ref_t List::ghost() const {
    return m_ghost;
}

List::Buffer::Buffer (size_t sz) {
    data = new Node[sz];
    capacity = sz;
}

List::Buffer::~Buffer () {
    delete data;
}

void List::Buffer::swap (Buffer &another) {
    std::swap (data, another.data);
    std::swap (capacity, capacity);
}

List::Node &List::Buffer::operator[] (size_t idx) {
    return data[idx];
}

const List::Node & List::Buffer::operator[] (size_t idx) const {
    return data[idx];
}
