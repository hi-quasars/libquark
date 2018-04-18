#ifndef quasar_TEST_UTILS_H
#define quasar_TEST_UTILS_H

#include <vector>

#include <common.h>

namespace quasar {

    typedef int (* TestFunc)();
    struct TestObject {
        TestFunc fp;
        const char *name;
        TestObject(TestFunc fp_, const char* n_) : fp(fp_), name(n_){ }
    };
    class RunTest {
    public:
        void add_test(TestFunc fp, const char *n);
        void Run();
    private:
        typedef std::vector<TestObject>::iterator titr;
        std::vector<TestObject> tobjs;
    };
    
    #define ADDTest(rt, func) do {              \
            TYPE_CHECK(rt, quasar::RunTest);    \
            TYPE_CHECK(func, quasar::TestFunc); \
            rt.add_test(func, #func);           \
        } while(0)
} // namespace quasar

#endif
