#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#include "../../Utilities/Strings/StringView.h"
#include <cstdio>

namespace Akinator {

class Tree {
public:
    struct Node {
        Node *parent;
        StringView name;
        Node *match, *not_match;
		
		Node (StringView name, Node *parent);
    };

private:
    Node *root;
    const char *dsl_buff;

public:
    int build (FILE *tree_dls);
};

} // Akinator

#endif // TREE_H_INCLUDED