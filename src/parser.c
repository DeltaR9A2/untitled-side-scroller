#include <stdio.h>
#include <stdlib.h>

void parse_file(const char *fn){
    FILE *script = fopen(fn, "r");
    
    if(script == NULL){
        return;
    }
    
    char line[256] = "a\0";
    char *retval = fgets(line, 256, script);
    while(retval != NULL){
        printf(line);
        retval = fgets(line, 256, script);
    }
    
    fclose(script);
}