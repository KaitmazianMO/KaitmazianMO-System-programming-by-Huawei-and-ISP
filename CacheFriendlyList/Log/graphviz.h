#ifndef GRAPHVIZ_H_INCLUDED
#define GRAPHVIZ_H_INCLUDED

#include <stddef.h>
#include <stdio.h>

static constexpr size_t GV_LAST_HANDLER = (size_t)-1;

struct gv_Graph {
    char tmp_dot_name[sizeof ("/tmp/XXXXXX")] = "/tmp/XXXXXX";
    FILE *dot_file;
    bool finished;
};

enum gv_Shape
{
	gv_circle,
	gv_reclangle,
    gv_record
};

enum gv_ImageFormat {
    gv_PNG,
    gv_PDF
};

int  gv_graph_init (gv_Graph *graph);
int  gv_graph_free (gv_Graph *graph);
void gv_graph_add_vertex (gv_Graph *graph, size_t handle, gv_Shape shape, const char *label, ...);
void gv_graph_add_edje (gv_Graph *graph, size_t from, size_t to, const char *label, ...);
void gv_graph_add_rank (gv_Graph *graph, ...);
int  gv_graph_create_image (gv_Graph *graph, const char *path, gv_ImageFormat format = gv_PNG);

#endif //GRAPHVIZ_H_INCLUDED