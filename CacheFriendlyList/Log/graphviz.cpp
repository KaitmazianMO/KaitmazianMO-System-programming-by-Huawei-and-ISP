#include "graphviz.h"
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>

const char *str (gv_Shape shape);
const char *str (gv_ImageFormat);

int gv_graph_init (gv_Graph *graph) {
    assert (graph);
    
    int fd = mkstemp (graph->tmp_dot_name);
    if (fd == -1) {
        return 0;
    }
    graph->dot_file = fdopen (fd, "wb");
    if (graph->dot_file == NULL) {
        return 0;
    }

    fprintf (graph->dot_file, "digraph G{\n");
    graph->finished = false;

    return 1;
}

int gv_graph_free (gv_Graph *graph) {
    assert (graph);
    assert (graph->dot_file);

    fclose (graph->dot_file);
    graph->dot_file = NULL;
    remove (graph->tmp_dot_name);

    return 1;
}

void gv_graph_add_vertex (gv_Graph *graph, size_t handle, gv_Shape shape, const char *label, ...) {
    assert (graph);
    assert (label);
    assert (graph->dot_file);

    fprintf (graph->dot_file, "\t%zu", handle);
    fprintf (graph->dot_file, "[shape = \"%s\",", str (shape));
    fprintf (graph->dot_file, "label = \"");
    va_list list = {};
    va_start (list, label);
    vfprintf (graph->dot_file, label, list);
    va_end (list);
    fprintf (graph->dot_file, "\"]\n");
}

void gv_graph_add_edje (gv_Graph *graph, size_t from, size_t to, const char *label, ...) {
    assert (graph);
    assert (label);
    assert (graph->dot_file);

    fprintf (graph->dot_file, "\t%zu -> %zu", from, to);
    fprintf (graph->dot_file, "[label = \"");
    va_list list = {};
    va_start (list, label);
    vfprintf (graph->dot_file, label, list);
    va_end (list);
    fprintf (graph->dot_file, "\"]\n");
}

int gv_graph_create_image (gv_Graph *graph, const char *path, gv_ImageFormat format) {
    assert (graph);
    assert (path);
    assert (graph->dot_file);
    assert (graph->tmp_dot_name);
    
    if (!graph->finished) {
        fprintf (graph->dot_file, "}");
        graph->finished = true;
        fflush (graph->dot_file);
    }

    const int sz = 1024;
    static char buff[sz + 1] = {};

    snprintf (buff, sz, "dot -T%s %s -o %s", str (format), graph->tmp_dot_name, path);
    return system (buff);
}

const char *str (gv_Shape shape) {
    switch (shape) {
        case gv_circle:     return "circle";
        case gv_reclangle:  return "rectangle";
        case gv_record:     return "record";
    }
    return "";
}

const char *str (gv_ImageFormat format) {
    switch (format) {
        case gv_PNG: return "png";
        case gv_PDF: return "pdf";
    }
    return "png";
}