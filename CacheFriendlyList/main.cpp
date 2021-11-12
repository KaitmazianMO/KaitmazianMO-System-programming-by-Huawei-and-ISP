#include "ListLib/list.h"
#include "Log/graphviz.h"
#include "Log/log.h"
#include <stdio.h>

void list_dump (const List *list) {

    ref_t curr_ref = list->head_ref;
    for (int i = 0; i < list->size; ++i) {
        printf ("%lg -> ", list->nodes[curr_ref].val);
        curr_ref = list->nodes[curr_ref].next;
    } putchar ('\n');

    curr_ref = list->head_ref;
    for (int i = 0; i < list->size; ++i) {
        printf ("[%d]:<%12zu, %12lg, %12zu>\n", i, list->nodes[curr_ref].prev, list->nodes[curr_ref].val, list->nodes[curr_ref].next);
        curr_ref = list->nodes[curr_ref].next;
    }

    putchar ('\n');
}

gv_Graph list_to_gv_graph (const List *list) {
    gv_Graph graph = {};
    ref_t prev = 0;
    ref_t next = 0;
    if (gv_graph_init (&graph)) {
        for (size_t i = 0, cap = list->cap; i < cap; ++i) {
            prev = list->nodes[i].prev;
            next = list->nodes[i].next;
            if (i != List::BAD_REF && prev != List::BAD_REF) {
                gv_graph_add_edje (&graph, prev, i, "");
            }
            if (i != List::BAD_REF && next != List::BAD_REF) {
                gv_graph_add_edje (&graph, i, next, "");
            }
        }
        for (size_t i = 0, cap = list->cap; i < cap; ++i) {
            gv_graph_add_vertex (&graph, i, gv_record, 
                "{ %lg | { prev:%zu | curr:%zu | next:%zu } }", 
                list->nodes[i].val, list->nodes[i].prev,
                i, list->nodes[i].next);
        }
    } 
    return graph;
}

int main() {
    log_init ("lsit.html");

    LOG_MSG (INFO, "test message");
    LOG_MSG (WARNING, "There is a cat!");

    LOG_IMAGE ("cat.jpg");

    LOG_MSG (INFO, "312321 3213 21312");
    LOG_MSG (WARNING, "321 124  12421!");
    LOG_MSG (FATAL, "fsDL:JRF;AEUKFhnwaeFDNJlwa!");

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

    List list{};
    auto plist = &list;
    list_init (plist, 8);

    //for (int i = 0; i < 2; ++i) {
    //    if (list_insert_front (plist, val_t (i)) == LSIT_BAD_REF) {
    //        printf ("error while inserting\n");
    //    }
    //}

    list_insert_front (plist, 4);

    auto graph2 = list_to_gv_graph (&list);
    gv_graph_create_image (&graph2, "list0.png", gv_PNG);

    auto ins = list_insert_front (plist, 1);
    list_dump (plist);

    auto graph = list_to_gv_graph (&list);
    gv_graph_create_image (&graph, "list1.png", gv_PNG);

    auto er = list_insert_after (plist, ins, 2);
    if (er == List::BAD_REF) printf ("BADREF1\n");
    list_dump (plist);
    er = list_insert_after (plist, er, 3);
    if (er == List::BAD_REF) printf ("BADREF2\n");
    //list_insert_back (plist, val_t(-1));
    //list_insert_back (plist, val_t(-2));
    //list_insert_back (plist, val_t(-3));

    list_dump (plist);
    
    graph = list_to_gv_graph (&list);
    gv_graph_create_image (&graph, "list2.png", gv_PNG);

    printf ("tail = %zu\n", list.tail_ref);

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
    return 0;
}