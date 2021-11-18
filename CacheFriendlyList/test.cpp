#include <gtest/gtest.h>
#include "list.h"

TEST (ListTests, InsertBackWithEmpty) {
    val_t array[List::DEFAULT_CAPACITY] = {};
    for (int i = 0; i < List::DEFAULT_CAPACITY; ++i) {
        array[i] = i*i;
    }

    List list{};
    for (int i = 0; i < List::DEFAULT_CAPACITY; ++i) {
        list.insert_back (array[i]);
    }

    auto lcurr = list.head(); 
    for (int i = 0; i < List::DEFAULT_CAPACITY; ++i, lcurr = list.next (lcurr)) {
        ASSERT_EQ (list.get (lcurr), array[i]);
    }
}

int main (int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}