#include <cstdio>
#include <test_utls.h>

namespace quasar {
    void RunTest::add_test(TestFunc fp, const char *n) {
        tobjs.push_back(TestObject(fp, n));
    }
    void RunTest::Run() {
        int ret;
        for ( titr i = tobjs.begin(); i != tobjs.end(); ++i ) {
            printf("[ %s ] Running ---------\n", i->name);
            ret = (*i).fp();
            printf("[ %s ] Ret: %d\n", i->name, ret);
        }
    }
    
    
}
