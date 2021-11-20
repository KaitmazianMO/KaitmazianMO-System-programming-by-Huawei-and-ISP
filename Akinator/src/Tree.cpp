#include "Tree.h"
#include "error.h"
#include "../../Utilities/stdlib_addition.h"

static const char *get_next_token(const char *dsl);
static size_t name_size (const char *name);
static void expect (char ch, char expected);
static Akinator::Tree::Node *process_attribute (const char *dls, size_t *size);

int Akinator::Tree::build (FILE *tree_dls) {
    if (!tree_dls) {
        AKINATOR_FATAL ("nullptr as tree_dsl file");
    }

    size_t size = 0;
    dsl_buff = read_buff (tree_dls, &size);
    if (!dsl_buff) {
        AKINATOR_FATAL ("DSL file reading failed");
    }

	const char *unknown = "Unknown";
	root = new Node (StringView (unknown, sizeof (unknown) - 1), nullptr);
    Node *curr_node = root; 
    for (auto curr_tok = get_next_token (dsl_buff); *curr_tok; 
         curr_tok = get_next_token (curr_tok + size)) {        
        switch (*curr_tok) {
            case '{': {
				++curr_tok;
				curr_node = allocate_child_and_tie (curr_node);
            } break;
				
            case '}': {
				++curr_tok;
				curr_node = curr_node->parent; // may be nullptr in sintax error case
            } break;
            
			case '\"' {
				curr_node->name = make_name ()
			} break;
			
            default:
                AKINATOR_FATAL ("Expected braces or name wich begin with \".");
        }
    }
}

Akinator::Tree::Node Node(StringView name_, Node *parent_) : 
	parent (parent_), name (name_), match (nullptr), not_match (nullptr) 
{}

const char *get_next_token(const char *dsl) {
    while (dsl[0] != '{' || dsl[0] != '}' ||
           dsl[0]) {
        ++dsl;
    }
    return dsl;
}

size_t name_size (const char *name) {
    size_t size = 0; 
    if (name[size] == '\"') {
        while (name[size] != '\"') {
            if (name[size] == '\0') {
                AKINATOR_FATAL ("Expected name closing '\"'");
            }
            ++size;
        }
        return size;
    }
    return 0;
}

static void expect (char ch, char expected) {
    if (ch != expected) {
        AKINATOR_FATAL ("Expected %c but had %c", expected, ch);
    }
}