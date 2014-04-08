#include "linux_wrappers.h"

int
my_strcasecmp(__const char *__s1, __const char *__s2)
{
#define TOLOWER(x) ((((x) >= 'A') && ((x) <= 'Z')) ? ((x) - 'A' + 'a') : (x))
    while(('\0' != *__s1) && ('\0' != *__s2))
    {
        char c1 = TOLOWER(*__s1), c2 = TOLOWER(*__s2);
        if(c1 != c2) return (c1 - c2);
        __s1++;
        __s2++;
    }
#undef TOLOWER
    return 0;
}
