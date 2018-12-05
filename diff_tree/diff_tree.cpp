#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lib/file_operations.h"
#include "tree/structures.h"
#include "tree/basic_operations.hpp"
#include "tree/diff_operations.hpp"
#include "tree/reducer.hpp"
#include "tree/parser.hpp"
#include "tree/latex_operations.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "\033[31mError:\033[0m Expected argument \"path\"\n");
        return 1;
    }
    
    Node* n = NULL;
    if (readFromFile(argv[1], &n) != 0) {
        return 1;
    }
    
    srand(time(NULL));

    Node* res = NULL;
    diff(n, &res);

    printLatex(stdout, res, 0);
    reduce(&res, stdout, res);
    
    //del(res);
    
   // reduce(&n);
    //printLatex(stdout, n);
    del(n);
   
    return 0;
}
