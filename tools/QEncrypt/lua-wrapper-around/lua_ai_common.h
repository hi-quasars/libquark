#ifndef _LUA_AI_COMMON_H
#define _LUA_AI_COMMON_H

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

//carefully use
#define __DEF_LUA_INVOKER(__NAME, __LUASTATE) static int __NAME(lua_State *__LUASTATE)

#define __DEF_LUA_ERRRETURN(__EMSG, __LUASTATE)	( luaL_error(__LUASTATE, __EMSG) )

#define __CHECK_LUA_PARAM_NUMBER(__N, __LUASTATE) do { \
	if (lua_gettop(__LUASTATE) != __N){ \
		 return __DEF_LUA_ERRRETURN("number of parameter not " #__N "!", __LUASTATE); \
	} \
 }while(0)

#define __GET_LUA_PARAM_NUMBER(__LUASTATE) (lua_gettop(__LUASTATE))

#define __GET_LS_CHARX(__N, __LUASTATE) ( (char *) lua_tostring(__LUASTATE, __N) )
#define __GET_LS_FLOAT(__N, __LUASTATE)	( (float) lua_tonumber(__LUASTATE, __N) )
#define __GET_LS_INT(__N, __LUASTATE)	( (int) lua_tointeger(__LUASTATE, __N) )

#define __PUT_LS_INT(__VAR, __LUASTATE) do { \
	lua_pushinteger(__LUASTATE, __VAR); \
}while(0)


#define CONCAT_(A,B) A##B
#define CONCAT(A,B) CONCAT_(A,B)
#define STR_(A)	#A
#define STR(A) STR_(A)

#define __AI_LIB_PREFIX ai_
#define __AI_LIB_NAME(__NAME) CONCAT(__AI_LIB_PREFIX, __NAME)
#define __DEF_AI_FUNC_ARRAY(__NAME) static const struct luaL_Reg __AI_LIB_NAME(__NAME)[]
#define __REGISTER_AI_FUNC_ARRAY(__NAME, __LUASTATE) int CONCAT(luaopen_, __AI_LIB_NAME(__NAME)) (lua_State *__LUASTATE) { \
	luaL_register(__LUASTATE, STR(__AI_LIB_NAME(__NAME)), __AI_LIB_NAME(__NAME));	\
	return 1;	\
}

#define __EX_LIB_PREFIX ex_
#define __EX_LIB_NAME(__NAME) CONCAT(__EX_LIB_PREFIX, __NAME)
#define __DEF_EX_FUNC_ARRAY(__NAME) static const struct luaL_Reg __EX_LIB_NAME(__NAME)[]
#define __REGISTER_EX_FUNC_ARRAY(__NAME, __LUASTATE) int CONCAT(luaopen_, __EX_LIB_NAME(__NAME)) (lua_State *__LUASTATE) { \
	luaL_register(__LUASTATE, STR(__EX_LIB_NAME(__NAME)), __EX_LIB_NAME(__NAME));	\
	return 1;	\
}


#endif
