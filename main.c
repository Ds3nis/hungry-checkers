#include <stdio.h>
#include <stdlib.h>
#include "argvalidate.h"


int main(int argc, char *argv[]) {
    Config cfg;
    int result = 0;
    
    config_init(&cfg);
    result = parse_args(argc, argv, &cfg);

    if(result) {
        return result;
    }
    
    return EXIT_SUCCESS;
}