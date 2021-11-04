#include "ListLib/list.h"

void list_dump (List *list) {

    ref_t curr_ref = list->head_ref;
    for (int i = 0; i < list->size; ++i) {
        printf ("%lg -> ", list->pool[curr_ref]);
        curr_ref = list->next[curr_ref];
    }
    putchar ('\n');
}

int main() {
    List list{};
    auto plist = &list;
    list_init (plist);

    //for (int i = 0; i < 2; ++i) {
    //    if (list_insert_front (plist, val_t (i)) == LSIT_BAD_REF) {
    //        printf ("error while inserting\n");
    //    }
    //}

    list_insert_front (plist, 4);
    auto ins = list_insert_front (plist, 1);

    auto er = list_insert_after (plist, list_insert_after (plist, ins, 2), 3);
    //list_insert_back (plist, val_t(-1));
    //list_insert_back (plist, val_t(-2));
    //list_insert_back (plist, val_t(-3));

    list_dump (plist);
    
    //if (list_erase (plist, ins) == LSIT_BAD_REF) {
    //    printf ("erase failed");
    //}
    //list_dump (plist);

    if (list_erase (plist, ins) == LIST_BAD_REF) {
        printf ("erase failed");
    }
    list_dump (plist);    

    list_free (plist);
    return 0;
}