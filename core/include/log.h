#ifndef quasar_LOG_H
#define quasar_LOG_H

#include <cstdio>
#include <cstdarg>
#include <string>

#include <common.h> //export header

namespace quasar {

    // log down some infos with a timestamp/level prefix.
#define LOG_FM_PREFIX_SAMPLE "[ 2018-03-05 10:08:38 INF ]"
#define LOG_FM_PREFIX_SIZE sizeof(LOG_FM_PREFIX_SAMPLE) - 1 // last char is '\0'
    enum {
      LOG_INF,
      LOG_WRN,
      LOG_ERR
    };
    extern std::string g_fm_prefix;
    extern const char* g_fm_lvl[];

    inline int fm_prefix_length() {
        return LOG_FM_PREFIX_SIZE;
    }
    inline std::string& fm_prefix(int lvl = 0) {
        std::string tstr = now_in_string();
        if ( 0 != tstr.compare(NULLSTR) ) {
            //return std::string("[ ") + tstr + " " + g_fm_lvl[lvl] +" ] ";  <---- return a temp var is a bad idea. 
            
            g_fm_prefix = std::string("[ ") + tstr + " " + g_fm_lvl[lvl] +" ] ";
            return g_fm_prefix;
        }
    }
    inline void fm_prefix( std::string &str1 , int lvl = 0) {
        std::string tstr = now_in_string();
        str1 = std::string("[ ") + tstr + " " + g_fm_lvl[lvl] +" ] ";
    }

    inline void _sLog(FILE* fp, int lvl, int line, const char* file, const char *format, ...) {
        va_list pargs;
        char ff[TBUFSIZE] = "";

        if (NULL != file || -1 != line) {
            sprintf(ff, "%s,%d: ", file, line);
        }
        std::string origin_fm = std::string(format);
        if ( '\n' != *(origin_fm.data() + origin_fm.size() - 1)) {
            origin_fm += '\n';
        }
        std::string fmstr = fm_prefix(lvl) + std::string(ff) + origin_fm;
        va_start(pargs, format);
        
        std::vfprintf(fp, CSTR1(fmstr), pargs);  // it's safe to use v*printf to print va_list
        
        va_end(pargs);
        return;
    }
    
    //va_list version
    inline void _sLog(FILE* fp, int lvl, int line, const char* file, const char *format, va_list vl) {
        char ff[TBUFSIZE] = "";

        if (NULL != file || -1 != line) {
            sprintf(ff, "%s,%d: ", file, line);
        }
        std::string origin_fm = std::string(format);
        if ( '\n' != *(origin_fm.data() + origin_fm.size() - 1)) {
            origin_fm += '\n';
        }
        std::string fmstr = fm_prefix(lvl) + std::string(ff) + origin_fm;
        
        std::vfprintf(fp, CSTR1(fmstr), vl);  // it's safe to use v*printf to print va_list
        
        return;
    }

 //simple log   
#define sLog1(format, ...) do { _sLog(stdout, 0, -1, NULL, format, ##__VA_ARGS__); } while(0)
#define sLog2(fp, format, ...) do { _sLog(fp, 0, -1, NULL, format, ##__VA_ARGS__); } while(0)
#define sLog3(fp, lvl, format, ...) do { _sLog(fp, lvl, -1, NULL, format, ##__VA_ARGS__); } while(0)
 //detailed log
#define xLog1(format, ...) do { _sLog(stdout, 0, __LINE__, __BASE_FILE__, format, ##__VA_ARGS__); } while(0)
#define xLog2(fp, format, ...) do { _sLog(fp, 0, __LINE__, __BASE_FILE__, format, ##__VA_ARGS__); } while(0)
#define xLog3(fp, lvl, format, ...) do { _sLog(fp, lvl, __LINE__, __BASE_FILE__, format, ##__VA_ARGS__); } while(0)

    class LogBase {
        virtual void log( int, const char*, std::string, ...) = 0;
        virtual void log( int, int, const char*, std::string, ...) = 0;
    };

    class NoLog : public LogBase {
    public:
        void log(int , const char*, std::string fmt, ...) { }
        void log(int , int, const char*, int lvl, std::string fmt, ...) { }
    };

    class QLog : public LogBase {
        public:
            
        private:
            FILE*           fp_;
            std::string     first_msg_;
    };

    class Screen : public LogBase {
        public:
            void log(int line, const char * file, std::string fmt, ...) { 
                va_list vl;
                va_start(vl, fmt);
                _sLog(stdout, LOG_INF, line, file, fmt.c_str(), vl);
                va_end(vl);
            }
            void log(int line, const char * file, int lvl, std::string fmt, ...) { 
                va_list vl;
                va_start(vl, fmt);
                _sLog(stdout, lvl, line, file, fmt.c_str(), vl);
                va_end(vl);
            }
        private:
            int     reserve_;
    };
} //  namespace quasar

#endif
