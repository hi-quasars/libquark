#include <cstdio>

class Base {

public:
    virtual ~Base() {
        printf("Base ~\n");
    }
    virtual void DoX() {
        printf ("Base X\n");
    }
    static int NewX(Base **ptr);
};

#define __IMPL1

#ifdef __IMPL1
class Impl1 : public Base {
public:
    virtual void DoX() {
        printf("Impl1 X\n");
    }
};
int Base::NewX(Base **ptr) {
    int rc = 0;
    printf("NewX 1\n");
    *ptr = new Impl1();
    return rc;
}

#else
class Impl2 : public Base {
public:
    virtual void DoX() {
        printf("Impl2 X\n");
    }
};
int Base::NewX(Base **ptr) {
    int rc = 0;
    *ptr = new Impl2();
    return rc;
}

#endif


int test1()
{
    Base* t1;
    if (0 != Base::NewX(&t1)){
        return -1;
    }
    t1->DoX();
    return 0;
}


class Base1 {
public:
    Base1() {
        init(1);
    }
    virtual void init() { };
    int init(int ) {
        printf("base init\n");
        return 0;
    }

    virtual void pp() = 0;
    static void test() {
        printf("pp\n");
    }
} ;

class derive : public Base1 {
public:
    virtual void init() {
        printf("derive init\n");
    }
    virtual void pp();
} ;
void derive::pp() {
    typedef void (*fp)();
    fp p1 = &Base1::test;
    p1();
}


int test2()
{
    derive d1;
    Base1 *ptr = &d1;
    ptr->init();
    ptr->pp();

    return 0;
}


int main()
{
    return test2();
}
