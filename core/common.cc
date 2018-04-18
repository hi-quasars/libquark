
#include <common.h> // from include/

#include <sys/time.h>
#include <sys/select.h>

namespace quasar {
    void check_cpp_version() {
        printf("c++ version: %s", __cplusplus);
    }
    
    std::string now_in_string() {
        time_t raw;
        struct tm *ti;
        char buffer[TBUFSIZE];
        int ret;
        if(time(&raw)){
            ti = localtime(&raw);
            ret = sprintf(buffer, "%4d-%02d-%02d %02d:%02d:%02d", 
                    ti->tm_year + 1900, ti->tm_mon + 1, ti->tm_mday, 
                    ti->tm_hour, ti->tm_min, ti->tm_sec);
            if ( ret > 0 ) {
                return std::string(buffer);
            }
        }
        return NULLSTR;
    }

    void _sleep(int s, int us) {
        struct timeval tm = {s, us};
        select(0, NULL, NULL, NULL, &tm);
    }

    void ussleep(int us) {
        if (us < US_PER_SEC) {
            _sleep(0, us);
        } else {
            _sleep(us / US_PER_SEC, us % US_PER_SEC);
        }
    }
    
    void mssleep(int ms) {
        if (ms < MS_PER_SEC) {
            _sleep(0, ms * 1000);
        } else {
            _sleep( ms / MS_PER_SEC, (ms % MS_PER_SEC)*1000 );
        }
    }

    void ssleep(int s) {
        _sleep(s, 0);
    }


} // namespace quasar
