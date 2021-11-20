#ifndef AKINATOR_H_INCLUDED
#define AKINATOR_H_INCLUDED

#include "Tree.h"

namespace Akinator {

class Akinator {
    Tree tree;

public:
    Akinator (const char *tree_dsl);
    int run(); 
};

} // Akinator

#endif // AKINATOR_H_INCLUDED