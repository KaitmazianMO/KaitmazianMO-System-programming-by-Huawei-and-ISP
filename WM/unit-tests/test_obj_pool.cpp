#include <gtest/gtest.h>
#include "obj_pool.h"

TEST(ObjectPool, Inserting) {
    ObjectPool pool;
    ASSERT_NE (obj_pool_init (&pool, 16), 0) << "Object pool initialization failed";

    Object obj1 = {
        .tag = DEC,
        .val = { .decimal = 1 }
    };
    Object obj2 = {
        .tag = INT,
        .val = { .integer = 1 }
    };

    pool_idx idx1 = obj_pool_insert(&pool, obj1);
    ASSERT_NE (idx1, OBJ_POOL_BAD_IDX);
    ASSERT_EQ (obj_pool_size (&pool), 1);
    ASSERT_EQ (obj_compare (obj_pool_get (&pool, idx1), obj1), EQUAL);

    idx1 = obj_pool_insert(&pool, obj1);
    ASSERT_NE (idx1, OBJ_POOL_BAD_IDX);
    ASSERT_EQ (obj_pool_size (&pool), 1);
    ASSERT_EQ (obj_compare (obj_pool_get (&pool, idx1), obj1), EQUAL);

    pool_idx idx2 = obj_pool_insert(&pool, obj2);
    ASSERT_NE (idx2, OBJ_POOL_BAD_IDX);
    ASSERT_EQ (obj_pool_size (&pool), 2);
    ASSERT_EQ (obj_compare (obj_pool_get (&pool, idx2), obj2), EQUAL);
}

int main (int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}