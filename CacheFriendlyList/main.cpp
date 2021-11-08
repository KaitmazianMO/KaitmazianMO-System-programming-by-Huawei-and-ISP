#include "ListLib/list.h"
#include "Log/graphviz.h"
#include "Log/log.h"
#include <stdio.h>

void list_dump (List *list) {

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

int main() {
    log_init ("lsit.html");

    LOG_MSG (INFO, "test message");
    LOG_MSG (WARNING, "There is a cat!");

    LOG_IMAGE ("cat.jpg");

    LOG_MSG (INFO, "312321 3213 21312");
    LOG_MSG (WARNING, "321 124  12421!");
    LOG_MSG (FATAL, "fsDL:JRF;AEUKFhnwaeFDNJlwa!");

    gv_Grapg graph = {};
    gv_graph_init (&graph);
    
    gv_graph_add_vertex (&graph, 1, gv_reclangle, "first");
    gv_graph_add_vertex (&graph, 2, gv_reclangle, "second");
    gv_graph_add_vertex (&graph, 3, gv_reclangle, "third");
    gv_graph_add_edje (&graph, 1, 2, "wheee");
    gv_graph_add_edje (&graph, 1, 3, "WHOA");

    if (gv_graph_create_image (&graph, "graph.pdf", gv_PDF)) {
        printf ("Image creating failed");
    }
    gv_graph_free (&graph);

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
    list_dump (plist);

    auto er = list_insert_after (plist, ins, 2);
    if (er == List::BAD_REF) printf ("BADREF1\n");
    list_dump (plist);
    er = list_insert_after (plist, er, 3);
    if (er == List::BAD_REF) printf ("BADREF2\n");
    //list_insert_back (plist, val_t(-1));
    //list_insert_back (plist, val_t(-2));
    //list_insert_back (plist, val_t(-3));

    list_dump (plist);
    
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