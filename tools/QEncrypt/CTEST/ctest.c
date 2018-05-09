#include <stdio.h>
#include <libqencrypt.h>

int main() {
    int ret;
    int i;
    void *ptr;
    char x;
    ret = qaf_initialize();
    
    qaf_handle_t *ff = OpenEmptyAppendFile("abc.txt");
    
    ptr = AllocBuffer(ff);
    x = 'a';
    for(i = 0; i < 22; i++) {
        ((char*)ptr)[i] = x;
        x++;
    }
    printf("write(plain): %s\n", (char*)ptr);
    ret = SecureAppend(ff, ptr);
    printf("ret: %d\n", ret);

    ret = Sync(ff);
    printf("ret: %d\n", ret);
    
    printf("write(secured): %s\n", (char*)ptr);
    CloseFile(ff);
    
    ff = OpenNonEmptyReadFile("abc.txt");
    ret = SecureRead(ff);
    printf("ret: %d\n", ret);
    
    printf("read:\n");
    int bc = BlockCount(ff);
    for(i = 0; i < bc; i++){
        printf("i: %d, %s", i, Block(ff, i));
    }
    CloseFile(ff);




    ret = qaf_deinitialize();
    return 0;
}

