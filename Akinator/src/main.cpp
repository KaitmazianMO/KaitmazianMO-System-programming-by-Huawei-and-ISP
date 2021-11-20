#include "Akinator.h"
#include "error.h"

int main(int argc, char *argv[]) {
    if (argc == 2) {
        std::FILE *tree_file = std::fopen (argv[2], "wb");
        if (!tree_file) {
            AKINATOR_FATAL ("Can not open the file \'%s\'", argv[2]);
        }
    }

    Akinator::Akinator akiantor;

    akiantor.run();
}