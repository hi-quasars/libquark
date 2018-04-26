#include <cstring>
#include <iostream>
#include "QEncry.h"

int test_01()
{
    Quark::QEncryObject qo("./test.txt");
    char buffer[Quark::QEBlockSize];
    memset(buffer, 0, sizeof(buffer));
    int ret;

    while(1){
        ret = qo.ReadBlock(buffer);
        printf("ret: %d\n----------------\n%s\n", ret, buffer);
        if (ret != Quark::rOK){
            break;
        }
    }
    return 0;
}

int test_02()
{
    int ret;
    Quark::QEncryObject q0("./before1.txt");
    
    ret = q0.ReadBlock(NULL);
    
    q0.PrintUchar(q0.InternalBuffer(0), q0.InternalBSize(0));
    std::cout << "----------------" << std::endl;

    q0.QkEncript(q0.InternalBuffer(0), q0.InternalBSize(0));
    q0.PrintUchar(q0.InternalBuffer(2), q0.InternalBSize(2));
    std::cout << "----------------" << std::endl;
    


    Quark::QEncryObject q1("./after.txt");
    ret = q1.WriteBlock(q0.InternalBuffer(2), q0.InternalBSize(2));

    ret = q1.ReadBlock(NULL);
    q1.QkDecript(q1.InternalBuffer(0), q1.InternalBSize(0));
    std::cout << "----------------" << std::endl;
    q0.PrintUchar(q1.InternalBuffer(2), q1.InternalBSize(2));


}

int main() {
    return test_02();
}
