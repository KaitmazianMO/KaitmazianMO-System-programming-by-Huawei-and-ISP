#include <gtest/gtest.h>
#include "list.h"

TEST (ListTests, InsertBackWithEmpty) {
    val_t array[List::DEFAULT_CAPACITY] = {};
    for (size_t i = 0; i < List::DEFAULT_CAPACITY; ++i) {
        array[i] = i*i;
    }

    List list{};
    for (size_t i = 0; i < List::DEFAULT_CAPACITY; ++i) {
        list.insert_back (array[i]);
    }

    auto lcurr = list.head(); 
    for (size_t i = 0; i < List::DEFAULT_CAPACITY; ++i, lcurr = list.next (lcurr)) {
        ASSERT_EQ (list.get (lcurr), array[i]);
    }
}

TEST (ListTests, InsertFrontWithEmpty) {
    val_t array[List::DEFAULT_CAPACITY] = {};
    for (size_t i = 0; i < List::DEFAULT_CAPACITY; ++i) {
        array[i] = i*i;
    }

    List list{};
    for (size_t i = 0; i < List::DEFAULT_CAPACITY; ++i) {
        list.insert_front (array[i]);
    }

    auto lcurr = list.head(); 
    for (size_t i = 0; i < List::DEFAULT_CAPACITY; ++i, lcurr = list.next (lcurr)) {
        ASSERT_EQ (list.get (lcurr), array[List::DEFAULT_CAPACITY - i - 1]);
    }
}

TEST (ListTests, InsertPrev) {
    val_t array[List::DEFAULT_CAPACITY] = {};
    for (size_t i = 0; i < List::DEFAULT_CAPACITY; ++i) {
        array[i] = i*i;
    }

    List list{};
    ref_t pos[List::DEFAULT_CAPACITY] = {};
    for (size_t i = 1; i < List::DEFAULT_CAPACITY; i += 2) {
        pos [i] = list.insert_back (array[i]);
    }

    for (size_t i = 0; i < List::DEFAULT_CAPACITY; i += 2 ) {
        list.insert_before (pos[i+1], array[i]);
    }

    auto lcurr = list.head(); 
    for (size_t i = 0; i < List::DEFAULT_CAPACITY; ++i, lcurr = list.next (lcurr)) {
        ASSERT_EQ (list.get (lcurr), array[i]);
    }
}

TEST (ListTests, InsertAfter) {
    val_t array[List::DEFAULT_CAPACITY] = {};
    for (size_t i = 0; i < List::DEFAULT_CAPACITY; ++i) {
        array[i] = i*i;
    }

    List list{};
    ref_t pos[List::DEFAULT_CAPACITY] = {};
    for (size_t i = 0; i < List::DEFAULT_CAPACITY; i += 2) {
        pos [i] = list.insert_back (array[i]);
    }

    for (size_t i = 1; i < List::DEFAULT_CAPACITY; i += 2 ) {
        list.insert_after (pos[i-1], array[i]);
    }

    auto lcurr = list.head(); 
    for (size_t i = 0; i < List::DEFAULT_CAPACITY; ++i, lcurr = list.next (lcurr)) {
        ASSERT_EQ (list.get (lcurr), array[i]);
    }
}

int main (int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}