#ifndef TEMP_H
#define TEMP_H

inline void log1(int line, const char *file) {
    printf("%s %d\n", file, line);
}

#define Log1() log1(__LINE__, __BASE_FILE__)

#endif
