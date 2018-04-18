#ifndef LIBQ_RANDOM_H
#define LIBQ_RANDOM_H

#include <ctime>
#include <cstdlib>
#include <string>

char* seed = NULL;
char _l_src_chs[] = "0123456789abcdef";

#define _libq_length(s) (sizeof(s)/sizeof(s[0]))
char* init(){
    if (!seed) {
        srand(time(NULL));
        seed = _l_src_chs;
    }
    return seed;
}


void randomstring(int l, std::string & res) {
    init();
    int len = _libq_length(_l_src_chs) - 1;
    for (int i = 0; i < l; i++)
    {
        int idx = rand() % len;
        //printf("%d %c\n", idx, _l_src_chs[idx]);
        res.push_back( _l_src_chs[idx] );
    }
}

#endif
