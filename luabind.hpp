#ifndef LUABIND_H
#define LUABIND_H
 
#include <string>
#include <iostream>

#include <glm/glm.hpp>
 
extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

class LuaBind {
    public:
        lua_State *L;
        LuaBind(const std::string &script) {
            L = luaL_newstate();
            luaL_openlibs(L);
            printf("%s loading..\n", script.c_str());
            luaL_dofile(L, script.c_str());
            level = 0;
        }

        ~LuaBind() {
            clean();
            lua_close(L);
        }

        // Generic get
        template<typename T> 
        T lua_get() {
            return 0;
        }

        // Generic default get
        template<typename T> 
        T lua_getdefault() {
            return 0;
        }

        // Anything used by template needs to be defined in header
        bool getTableField(const std::string &field) {
            std::string var;
            clean();
        
            // Loop pushing table onto stack until var contains field 
            for(unsigned int i = 0; i < field.size(); i++) {
                if(field.at(i) == '.') {
                    if(level == 0) {
                        lua_getglobal(L, var.c_str());
                    } else {
                        lua_getfield(L, -1, var.c_str());
                    }
        
                    if(lua_isnil(L, -1)) {
                        return false;
                    } else {
                        var = "";
                        level++;
                    }
                } else {
                    var += field.at(i);
                }
            }
        
            // If at root (not table field)
            if(level == 0) {
                lua_getglobal(L, var.c_str());

            // If was table field
            } else {
                lua_getfield(L, -1, var.c_str());
            }

            if(lua_isnil(L, -1)) {
                return false;
            }

            return true;
        }


        template<typename T> 
        T get(const std::string &field) {
            if(!L) {
                return lua_getdefault<T>();
            }

            T result;
            if(getTableField(field)) { // variable succesfully on top of stack
                result = lua_get<T>();  
            } else {
                result = lua_getdefault<T>();
            }
        
            return result;
        
        }


        // Doesn't clear the stack
        template<typename T> 
        T getTop() {
            if(!L) {
                return lua_getdefault<T>();
            }

            T result = lua_get<T>();  
            lua_remove(L, -1);
            return result;
        
        }

        void clean() {
            int n = lua_gettop(L);
            lua_pop(L, n);
            level = 0;
        }
            
        void execute(const std::string &field, int nargs, glm::vec3 * args, const int res=0) {
            if(!L) {
                return;
            }

            if(getTableField(field) && lua_isfunction(L, -1)) { 
                for (int i = 0; i < nargs; i ++) {
                    lua_createtable(L, 0, 3);
                    lua_pushnumber(L, args[i].x); 
                    lua_setfield(L, -2, "x"); 
                    lua_pushnumber(L, args[i].y); 
                    lua_setfield(L, -2, "y"); 
                    lua_pushnumber(L, args[i].z); 
                    lua_setfield(L, -2, "z"); 
                }

                if (lua_pcall(L, nargs, res, 0) != 0) {
                    printf("function failed %s\n", lua_tostring(L, - 1));fflush(0);
                }

            } else {
                printf("Something went REALLY wrong\n"); fflush(0);
            }
        }
        void execute(const std::string &field, int nargs, float * args, const int res=0) {

            if(!L) {
                return;
            }

            if(getTableField(field) && lua_isfunction(L, -1)) { 
                for (int i = 0; i < nargs; i ++)
                    lua_pushnumber(L, args[i]);

                if (lua_pcall(L, nargs, res, 0) != 0) {
                    printf("function failed %s\n", lua_tostring(L, - 1));
                }

            } else {
                printf("Something went REALLY wrong\n");
            }
        }
        void execute(const std::string &field, const int res=0) {
            if(!L) {
                return;
            }

            if(getTableField(field) && lua_isfunction(L, -1)) { 
                if (lua_pcall(L, 0, res, 0) != 0) {
                    printf("function failed %s\n", lua_tostring(L, - 1));
                }
            } else {
                printf("Something went wrong\n");
            }
        };


    protected:
        int level;
};


template <>
inline bool LuaBind::lua_get() {
    return (bool)lua_toboolean(L, -1);
}

template <>
inline float LuaBind::lua_get() {
    if(!lua_isnumber(L, -1)) {
        return 0;
    }
    return (float)lua_tonumber(L, -1);
}

template <>
inline double LuaBind::lua_get() {
    if(!lua_isnumber(L, -1)) {
        return 0;
    }
    return (double)lua_tonumber(L, -1);
}

template <>
inline glm::vec3 LuaBind::lua_get() {
    lua_getfield(L, -1, "x");
    if(!lua_isnumber(L, -1)) {
        return glm::vec3(0);
    }
    float x = lua_tonumber(L, -1); 
    lua_remove(L, -1);

    lua_getfield(L, -1, "y");
    if(!lua_isnumber(L, -1)) {
        return glm::vec3(0);
    }
    float y = lua_tonumber(L, -1); 
    lua_remove(L, -1);

    lua_getfield(L, -1, "z");
    if(!lua_isnumber(L, -1)) {
        return glm::vec3(0);
    }
    float z = lua_tonumber(L, -1); 
    lua_remove(L, -1);

    return glm::vec3(x, y, z);
}

template <>
inline int LuaBind::lua_get() {
    if(!lua_isnumber(L, -1)) {
        return 0;
    }
    return (int)lua_tonumber(L, -1);
}

template <>
inline std::string LuaBind::lua_get() {
    std::string s = "null";
    if(lua_isstring(L, -1)) {
        s = std::string(lua_tostring(L, -1));
    } else {
        s = "";
    }
    return s;
}

template<>
inline glm::vec3 LuaBind::lua_getdefault() {
    return glm::vec3(0);
}

#endif
