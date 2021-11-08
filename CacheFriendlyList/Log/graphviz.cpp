#include "graphviz.h"
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>

const char *str (gv_Shape shape);

int gv_graph_init (gv_Grapg *graph) {
    assert (graph);
    
    int fd = mkstemp (graph->tmp_dot_name);
    if (fd == -1) {
        return 0;
    }
    graph->dot_file = fdopen (fd, "wb");
    if (graph->dot_file == NULL) {
        return 0;
    }

    return 1;
}

void gv_graph_add_vertex (gv_Grapg *graph, size_t handle, gv_Shape shape, const char *label, ...) {
    assert (graph);
    assert (label);
    assert (graph->dot_file);

    fprintf (graph->dot_file, "\t%zu", handle);
    fprintf (graph->dot_file, "[shape = \"%s\",", str (shape));
    fprintf (graph->dot_file, "label = \"");
    va_list list = {};
    va_start (list, label);
    vfprintf (graph->dot_file, )

}

void gv_graph_add_edje (gv_Grapg *graph, size_t from, size_t to, const char *label, ...) {

}

int gv_create_graph_image (gv_Grapg *graph, const char *path, gv_ImageFormat format) {

}

const char *str (gv_Shape shape) {
    switch (shape) {
        case gv_circle:     return "circle";
        case gv_reclangle:  return "rectangle";
        case gv_record:     return "record";
    }
    return "";
}