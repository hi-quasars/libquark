#include <lua_ai_common.h>
#include <libqencrypt.h>
#include <malloc.h>

// c-wrapper for lua

struct qaf_handler_pool_s {
    qaf_handle_t    **pool;
    int             max_;
    int             now_;
    uint64_t        *bitmap;

};
typedef struct qaf_handler_pool_s qaf_handle_pool_t;

qaf_handle_t** qaf_handle_pool = NULL;

//  param - max_open_file_number
//  xxx.qe_init(m)
__DEF_LUA_INVOKER(qe_init, L) {
    int max_open_file_number;
    __CHECK_LUA_PARAM_NUMBER(1, L);
    max_open_file_number = __GET_LS_INT(-1, L);
    

    return 0;
}

__DEF_LUA_INVOKER(qe_exit, L) {

    return 0;
}

__DEF_LUA_INVOKER(qe_open_nonempty_readfile, L) {
}

__DEF_LUA_INVOKER(qe_open_nonempty_appendfile, L) {
}

__DEF_LUA_INVOKER(qe_open_empty_appendfile, L) {
}
__DEF_LUA_INVOKER(qe_closefile, L) {
}

__DEF_LUA_INVOKER(qe_alloc_buffer, L) {
}

__DEF_LUA_INVOKER(qe_buffer_size, L) {
}

__DEF_LUA_INVOKER(qe_secure_append, L) {
}

__DEF_LUA_INVOKER(qe_sync, L) {
}

__DEF_LUA_INVOKER(qe_secure_read, L) {
}

__DEF_LUA_INVOKER(qe_block_count, L) {
}

__DEF_LUA_INVOKER(qe_block_size, L) {
}

__DEF_LUA_INVOKER(qe_get_block, L) {
}

__DEF_EX_FUNC_ARRAY(qencrypt) = {
    {"qe_init", qe_init},
    {"qe_exit", qe_exit},

    {NULL, NULL}
};
__REGISTER_EX_FUNC_ARRAY(qencrypt, L)
