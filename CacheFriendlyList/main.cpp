#include "ListLib/list.h"
#include "Log/graphviz.h"
#include "Log/log.h"
#include <stdio.h>
#include <assert.h>
void list_dump (const List &list) {
    for (size_t i = list.head(); i != List::BAD_REF; i = list.next(i)) {
        printf ("%lg -> ", list.get(i));
    } putchar ('\n');
}

//gv_Graph list_to_gv_graph (const List *list) {
//    assert (list);
//    
//    gv_Graph graph = {};
//    ref_t prev = 0;
//    ref_t next = 0;
//
//    if (gv_graph_init (&graph)) {
//        for (size_t i = 0, cap = list->cap; i < cap; ++i) {
//            gv_graph_add_rank (&graph, i, GV_LAST_HANDLER);
//        }
//
//        for (size_t i = 0, cap = list->cap; i < cap; ++i) {
//            prev = list->nodes[i].prev;
//            next = list->nodes[i].next;
//            if (i != List::BAD_REF && prev != List::BAD_REF) {
//                gv_graph_add_edje (&graph, i, prev, "");
//            }
//            if (i != List::BAD_REF && next != List::BAD_REF) {
//                gv_graph_add_edje (&graph, i, next, "");
//            }
//        }
//        for (size_t i = 0, cap = list->cap; i < cap; ++i) {
//            gv_graph_add_vertex (&graph, i, gv_record, 
//                "{ %lg | { prev:%zu | curr:%zu | next:%zu } }", 
//                list->nodes[i].val, list->nodes[i].prev,
//                i, list->nodes[i].next);
//        }
//    } 
//    return graph;
//}

int main() {
    //log_init ("lsit.html");
//
    //LOG_MSG (INFO, "test message");
    //LOG_MSG (WARNING, "There is a cat!");
//
    //LOG_IMAGE ("cat.jpg");
//
    //LOG_MSG (INFO, "312321 3213 21312");
    //LOG_MSG (WARNING, "321 124  12421!");
    //LOG_MSG (FATAL, "fsDL:JRF;AEUKFhnwaeFDNJlwa!");

    //gv_Graph graph = {};
    //gv_graph_init (&graph);

    //gv_graph_add_vertex (&graph, 1, gv_reclangle, "first");
    //gv_graph_add_vertex (&graph, 2, gv_reclangle, "second");
    //gv_graph_add_vertex (&graph, 3, gv_reclangle, "third");
    //gv_graph_add_edje (&graph, 1, 2, "wheee");
    //gv_graph_add_edje (&graph, 1, 3, "WHOA");
//
    ////if (gv_graph_create_image (&graph, "graph.pdf", gv_PDF)) {
    ////    printf ("Image creating failed");
    ////}
    ////gv_graph_free (&graph);

    List list (32);

    //for (int i = 0; i < 2; ++i) {
    //    if (list_insert_front (plist, val_t (i)) == LSIT_BAD_REF) {
    //        printf ("error while inserting\n");
    //    }
    //}

    list.insert_front (4);

    list.insert_front (3);
    list_dump (list);

    auto er = list.insert_front (1);
    list_dump (list);
    er = list.insert_after (er, 2);
    if (er == List::BAD_REF) printf ("BADREF2\n");
    list.insert_back (val_t(-1));
    list.insert_back (val_t(-2));
    list.insert_back (val_t(-3));
    list_dump (list);

    //list_dump (plist);
    //
    //auto graph = list_to_gv_graph (&list);
    //gv_graph_create_image (&graph, "list2.png", gv_PNG);
//
    //LOG_MSG (INFO, "list dump");
    //LOG_IMAGE ("list2.png");
//
    //printf ("tail = %zu\n", list.tail_ref);

    //if (list_erase (plist, ins) == LSIT_BAD_REF) {
    //    printf ("erase failed");
    //}
    //list_dump (plist);

    //if (list_erase (plist, ins) == LIST_BAD_REF) {
    //    printf ("erase failed");
    //}
    //list_dump (plist);    
//
    //list_free (plist);



    //   gv_graph_free (&graph);

    return 0;
}