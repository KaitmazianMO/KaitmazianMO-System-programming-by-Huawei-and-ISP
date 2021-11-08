#ifndef GRAPHVIZ_H_INCLUDED
#define GRAPHVIZ_H_INCLUDED

#include <stddef.h>
#include <stdio.h>

struct gv_Grapg {
    char tmp_dot_name[sizeof ("/tmp/XXXXXX")] = "/tmp/XXXXXX";
    FILE *dot_file;
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

int  gv_graph_init (gv_Grapg *graph);
void gv_graph_add_vertex (gv_Grapg *graph, size_t handle, gv_Shape shape, const char *label, ...);
void gv_graph_add_edje (gv_Grapg *graph, size_t from, size_t to, const char *label, ...);
int  gv_create_graph_image (gv_Grapg *graph, const char *path, gv_ImageFormat format = gv_PNG);

#endif //GRAPHVIZ_H_INCLUDED