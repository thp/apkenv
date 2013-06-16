#include "libc_wrappers.h"
#include "hooks.h"

#include <sys/syscall.h>
#include <assert.h>

#ifdef APKENV_DEBUG
#  define WRAPPERS_DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#  define WRAPPERS_DEBUG_PRINTF(...)
#endif

static FILE *stdio_files[3];

#define IS_STDIO_FILE(_f) \
    ((size_t)((void *)(_f) - (void *)&my___sF) < sizeof(my___sF))
#define TO_STDIO_FILE(_f) \
    stdio_files[(size_t)((void *)(_f) - (void *)&my___sF) / SIZEOF_SF]

static inline void swap(void **a, void **b)
{
    void *tmp = *a;
    *a = *b;
    *b = tmp;
}

int my___isthreaded = 1;
void
my_abort()
{
    WRAPPERS_DEBUG_PRINTF("abort()\n");
    abort();
}
double
my_acos(double __x)
{
    WRAPPERS_DEBUG_PRINTF("acos()\n", __x);
    return acos(__x);
}
double
my_asin(double __x)
{
    WRAPPERS_DEBUG_PRINTF("asin()\n", __x);
    return asin(__x);
}
double
my_atan(double __x)
{
    WRAPPERS_DEBUG_PRINTF("atan()\n", __x);
    return atan(__x);
}
double
my_atan2(double __y, double __x)
{
    WRAPPERS_DEBUG_PRINTF("atan2()\n", __y, __x);
    return atan2(__y, __x);
}
double
my_ceil(double __x)
{
    WRAPPERS_DEBUG_PRINTF("ceil()\n", __x);
    return ceil(__x);
}
clock_t
my_clock()
{
    WRAPPERS_DEBUG_PRINTF("clock()\n");
    return clock();
}
int
my_close(int __fd)
{
    WRAPPERS_DEBUG_PRINTF("close()\n", __fd);
    return close(__fd);
}
int
my_connect(int __fd, __const struct sockaddr *__addr, socklen_t __len)
{
    WRAPPERS_DEBUG_PRINTF("connect()\n", __fd, __addr, __len);
    return connect(__fd, __addr, __len);
}
double
my_cos(double __x)
{
    WRAPPERS_DEBUG_PRINTF("cos()\n", __x);
    return cos(__x);
}
double
my_cosh(double __x)
{
    WRAPPERS_DEBUG_PRINTF("cosh()\n", __x);
    return cosh(__x);
}
uLong
my_crc32(uLong crc, const Bytef *buf, uInt len)
{
    WRAPPERS_DEBUG_PRINTF("crc32()\n", crc, buf, len);
    return crc32(crc, buf, len);
}
double
my_difftime(time_t __time1, time_t __time0)
{
    WRAPPERS_DEBUG_PRINTF("difftime()\n", __time1, __time0);
    return difftime(__time1, __time0);
}
void
my_exit(int __status)
{
    WRAPPERS_DEBUG_PRINTF("exit()\n", __status);
    exit(__status);
}
double
my_exp(double __x)
{
    WRAPPERS_DEBUG_PRINTF("exp()\n", __x);
    return exp(__x);
}
int
my_fclose(FILE *__stream)
{
    WRAPPERS_DEBUG_PRINTF("fclose(%x, %x)\n", __stream, &my___sF);
    if (IS_STDIO_FILE(__stream))
        return fclose(TO_STDIO_FILE(__stream));
    else
        return fclose(__stream);
}
int
my_fcntl(int __fd, int __cmd, ...)
{
    WRAPPERS_DEBUG_PRINTF("fcntl()\n", __fd, __cmd);
    return fcntl(__fd, __cmd);
}
char *
my_fgets(char *__restrict __s, int __n, FILE *__restrict __stream)
{
    WRAPPERS_DEBUG_PRINTF("fgets()\n", __s, __n, __stream);
    if (IS_STDIO_FILE(__stream))
        return fgets(__s, __n, TO_STDIO_FILE(__stream));
    else
        return fgets(__s, __n, __stream);
}
double
my_floor(double __x)
{
    WRAPPERS_DEBUG_PRINTF("floor()\n", __x);
    return floor(__x);
}
double
my_fmod(double __x, double __y)
{
    WRAPPERS_DEBUG_PRINTF("fmod()\n", __x, __y);
    return fmod(__x, __y);
}
FILE *
my_fopen(__const char *__restrict __filename, __const char *__restrict __modes)
{
    WRAPPERS_DEBUG_PRINTF("fopen(%s, %s)\n", __filename, __modes);
    FILE  *f = fopen(__filename,__modes);
    WRAPPERS_DEBUG_PRINTF("fopen(%s, %s) -> %x\n", __filename, __modes, f);
    return f;
}
int
my_fputc(int __c, FILE *__stream)
{
    WRAPPERS_DEBUG_PRINTF("fputc()\n", __c, __stream);
    if (IS_STDIO_FILE(__stream))
        return fputc(__c, TO_STDIO_FILE(__stream));
    else
        return fputc(__c, __stream);
}
int
my_fputs(__const char *__restrict __s, FILE *__restrict __stream)
{
    WRAPPERS_DEBUG_PRINTF("fputs()\n", __s, __stream);
    if (IS_STDIO_FILE(__stream))
        return fputs(__s, TO_STDIO_FILE(__stream));
    else
        return fputs(__s, __stream);
}
size_t
my_fread(void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream)
{
    WRAPPERS_DEBUG_PRINTF("fread(%x, %d, %d, %x)\n", __ptr, __size, __n, __stream);
    size_t result = fread(__ptr, __size, __n, __stream);
    WRAPPERS_DEBUG_PRINTF("fread done: %d\n", result);
    return result;
}
void
my_free(void *__ptr)
{
    WRAPPERS_DEBUG_PRINTF("free()\n", __ptr);
    free(__ptr);
}
void
my_freeaddrinfo(struct addrinfo *__ai)
{
    WRAPPERS_DEBUG_PRINTF("freeaddrinfo()\n", __ai);
    swap((void**)&(__ai->ai_canonname), (void**)&(__ai->ai_addr));
    freeaddrinfo(__ai);
}
FILE *
my_freopen(__const char *__restrict __filename, __const char *__restrict __modes, FILE *__restrict __stream)
{
    WRAPPERS_DEBUG_PRINTF("freopen()\n", __filename, __modes, __stream);
    if (IS_STDIO_FILE(__stream)) {
        printf("IGNORING freopen\n");
        return NULL;
    }
    return freopen(__filename, __modes, __stream);
}
double
my_frexp(double __x, int *__exponent)
{
    WRAPPERS_DEBUG_PRINTF("frexp()\n", __x, __exponent);
    return frexp(__x, __exponent);
}
int
my_fscanf(FILE *__restrict __stream, __const char *__restrict __format, ...)
{
    WRAPPERS_DEBUG_PRINTF("fscanf()\n", __stream, __format);
    return fscanf(__stream, __format);
}
int
my_fseek(FILE *__stream, long int __off, int __whence)
{
    WRAPPERS_DEBUG_PRINTF("fseek(%x, %d, %d)\n", __stream, __off, __whence);
    return fseek(__stream, __off, __whence);
}
int
my_fsync(int __fd)
{
    WRAPPERS_DEBUG_PRINTF("fsync()\n", __fd);
    return fsync(__fd);
}
long int
my_ftell(FILE *__stream)
{
    WRAPPERS_DEBUG_PRINTF("ftell()\n", __stream);
    return ftell(__stream);
}
size_t
my_fwrite(__const void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __s)
{
    WRAPPERS_DEBUG_PRINTF("fwrite()\n", __ptr, __size, __n, __s);
    return fwrite(__ptr, __size, __n, __s);
}
int
my_getaddrinfo(const char *hostname, const char *servname,
    const struct addrinfo *hints, struct addrinfo **res)
{
    WRAPPERS_DEBUG_PRINTF("getaddrinfo(%s,%s,%p,%p)\n", hostname, servname, hints, res);
    // make a local copy of hints
    struct addrinfo *fixed_hints = (struct addrinfo*)malloc(sizeof(struct addrinfo));
    memcpy(fixed_hints, hints, sizeof(struct addrinfo));
    // fix bionic -> glibc missmatch
    swap((void**)&(fixed_hints->ai_canonname), (void**)&(fixed_hints->ai_addr));
    // do glibc getaddrinfo
    int result = getaddrinfo(hostname, servname, fixed_hints, res);
    // release the copy of hints
    free(fixed_hints);
    // fix bionic <- glibc missmatch
    struct addrinfo *it = *res;
    while(NULL != it)
    {
        swap((void**)&(it->ai_canonname), (void**)&(it->ai_addr));
        it = it->ai_next;
    }
    return result;
}
char *
my_getenv(__const char *__name)
{
    WRAPPERS_DEBUG_PRINTF("getenv(%s)\n", __name);
    return getenv(__name);
}
int
my_gettimeofday(struct timeval *__restrict __tv, __timezone_ptr_t __tz)
{
    WRAPPERS_DEBUG_PRINTF("gettimeofday()\n", __tv, __tz);
    return gettimeofday(__tv, __tz);
}
struct tm *
my_gmtime(__const time_t *__timer)
{
    WRAPPERS_DEBUG_PRINTF("gmtime()\n", __timer);
    return gmtime(__timer);
}
int
my_inflate(z_streamp strm, int flush)
{
    WRAPPERS_DEBUG_PRINTF("inflate()\n", strm, flush);
    return inflate(strm, flush);
}
int
my_inflateEnd(z_streamp strm)
{
    WRAPPERS_DEBUG_PRINTF("inflateEnd()\n", strm);
    return inflateEnd(strm);
}
int
my_inflateInit_(z_streamp strm, const char *version, int stream_size)
{
    WRAPPERS_DEBUG_PRINTF("inflateInit_()\n", strm, version, stream_size);
    return inflateInit_(strm, version, stream_size);
}
int
my_inflateInit2_(z_streamp strm, int windowBits, const char *version, int stream_size)
{
    WRAPPERS_DEBUG_PRINTF("inflateInit2_()\n", strm, windowBits, version, stream_size);
    return inflateInit2_(strm, windowBits, version, stream_size);
}
int
my_inflateReset(z_streamp strm)
{
    WRAPPERS_DEBUG_PRINTF("inflateReset()\n", strm);
    return inflateReset(strm);
}
double
my_ldexp(double __x, int __exponent)
{
    WRAPPERS_DEBUG_PRINTF("ldexp()\n", __x, __exponent);
    return ldexp(__x, __exponent);
}
struct tm *
my_localtime(__const time_t *__timer)
{
    WRAPPERS_DEBUG_PRINTF("localtime()\n", __timer);
    return localtime(__timer);
}
double
my_log(double __x)
{
    WRAPPERS_DEBUG_PRINTF("log()\n", __x);
    return log(__x);
}
double
my_log10(double __x)
{
    WRAPPERS_DEBUG_PRINTF("log10()\n", __x);
    return log10(__x);
}
void
my_longjmp(struct __jmp_buf_tag __env[1], int __val)
{
    WRAPPERS_DEBUG_PRINTF("longjmp()\n", __env, __val);
    longjmp(__env, __val);
}
long int
my_lrand48()
{
    WRAPPERS_DEBUG_PRINTF("lrand48()\n");
    return lrand48();
}
__off_t
my_lseek(int __fd, __off_t __offset, int __whence)
{
    WRAPPERS_DEBUG_PRINTF("lseek()\n", __fd, __offset, __whence);
    return lseek(__fd, __offset, __whence);
}
void *
my_malloc(size_t __size)
{
    WRAPPERS_DEBUG_PRINTF("malloc()\n", __size);
    return malloc(__size);
}
void *
my_memchr(__const void *__s, int __c, size_t __n)
{
    WRAPPERS_DEBUG_PRINTF("memchr()\n", __s, __c, __n);
    return memchr(__s, __c, __n);
}
int
my_memcmp(__const void *__s1, __const void *__s2, size_t __n)
{
    WRAPPERS_DEBUG_PRINTF("memcmp()\n", __s1, __s2, __n);
    return memcmp(__s1, __s2, __n);
}
void *
my_memcpy(void *__restrict __dest, __const void *__restrict __src, size_t __n)
{
    WRAPPERS_DEBUG_PRINTF("memcpy() -> memmove!\n", __dest, __src, __n);
    return memmove(__dest, __src, __n);
}
void *
my_memmove(void *__dest, __const void *__src, size_t __n)
{
    WRAPPERS_DEBUG_PRINTF("memmove()\n", __dest, __src, __n);
    return memmove(__dest, __src, __n);
}
void *
my_memset(void *__s, int __c, size_t __n)
{
    WRAPPERS_DEBUG_PRINTF("memset()\n", __s, __c, __n);
    return memset(__s, __c, __n);
}
time_t
my_mktime(struct tm *__tp)
{
    WRAPPERS_DEBUG_PRINTF("mktime()\n", __tp);
    return mktime(__tp);
}
double
my_modf(double __x, double *__iptr)
{
    WRAPPERS_DEBUG_PRINTF("modf()\n", __x, __iptr);
    return modf(__x, __iptr);
}
int
my_open(__const char *__file, int __oflag, ...)
{
    WRAPPERS_DEBUG_PRINTF("open()\n", __file, __oflag);
    return open(__file, __oflag);
}
double
my_pow(double __x, double __y)
{
    WRAPPERS_DEBUG_PRINTF("pow()\n", __x, __y);
    return pow(__x, __y);
}
int
my_printf(__const char *__restrict __format, ...)
{
    WRAPPERS_DEBUG_PRINTF("WRAPPERS_DEBUG_PRINTF()\n", __format);
    return WRAPPERS_DEBUG_PRINTF(__format);
}
int
my_puts(__const char *__s)
{
    WRAPPERS_DEBUG_PRINTF("puts()\n", __s);
    return puts(__s);
}
int
my_raise(int __sig)
{
    WRAPPERS_DEBUG_PRINTF("raise()\n", __sig);
    return raise(__sig);
}
ssize_t
my_read(int __fd, void *__buf, size_t __nbytes)
{
    WRAPPERS_DEBUG_PRINTF("read()\n", __fd, __buf, __nbytes);
    return read(__fd, __buf, __nbytes);
}
void *
my_realloc(void *__ptr, size_t __size)
{
    WRAPPERS_DEBUG_PRINTF("realloc()\n", __ptr, __size);
    return realloc(__ptr, __size);
}
ssize_t
my_recv(int __fd, void *__buf, size_t __n, int __flags)
{
    WRAPPERS_DEBUG_PRINTF("recv()\n", __fd, __buf, __n, __flags);
    return recv(__fd, __buf, __n, __flags);
}
int
my_remove(__const char *__filename)
{
    WRAPPERS_DEBUG_PRINTF("remove()\n", __filename);
    return remove(__filename);
}
int
my_rename(__const char *__old, __const char *__new)
{
    WRAPPERS_DEBUG_PRINTF("rename()\n", __old, __new);
    return rename(__old, __new);
}
int
my_sched_yield()
{
    WRAPPERS_DEBUG_PRINTF("sched_yield()\n");
    return sched_yield();
}
int
my_select(int __nfds, fd_set *__restrict __readfds, fd_set *__restrict __writefds, fd_set *__restrict __exceptfds, struct timeval *__restrict __timeout)
{
    WRAPPERS_DEBUG_PRINTF("select()\n", __nfds, __readfds, __writefds, __exceptfds, __timeout);
    return select(__nfds, __readfds, __writefds, __exceptfds, __timeout);
}
ssize_t
my_send(int __fd, __const void *__buf, size_t __n, int __flags)
{
    WRAPPERS_DEBUG_PRINTF("send()\n", __fd, __buf, __n, __flags);
    return send(__fd, __buf, __n, __flags);
}
int
my_setjmp(jmp_buf __env)
{
    WRAPPERS_DEBUG_PRINTF("setjmp()\n", __env);
    return setjmp(__env);
}
char *
my_setlocale(int __category, __const char *__locale)
{
    WRAPPERS_DEBUG_PRINTF("setlocale()\n", __category, __locale);
    return setlocale(__category, __locale);
}
int
my_setsockopt(int __fd, int __level, int __optname, __const void *__optval, socklen_t __optlen)
{
    WRAPPERS_DEBUG_PRINTF("setsockopt()\n", __fd, __level, __optname, __optval, __optlen);
    return setsockopt(__fd, __level, __optname, __optval, __optlen);
}
int
my_setvbuf(FILE *__restrict __stream, char *__restrict __buf, int __modes, size_t __n)
{
    WRAPPERS_DEBUG_PRINTF("setvbuf()\n", __stream, __buf, __modes, __n);
    if (IS_STDIO_FILE(__stream)) {
        printf("IGNORING setvbuf\n");
        return 0;
    }
    return setvbuf(__stream, __buf, __modes, __n);
}
double
my_sin(double __x)
{
    WRAPPERS_DEBUG_PRINTF("sin(%lf)\n", __x);
    return sin(__x);
}
double
my_sinh(double __x)
{
    WRAPPERS_DEBUG_PRINTF("sinh()\n", __x);
    return sinh(__x);
}
int
my_socket(int __domain, int __type, int __protocol)
{
    WRAPPERS_DEBUG_PRINTF("socket()\n", __domain, __type, __protocol);
    return socket(__domain, __type, __protocol);
}
double
my_sqrt(double __x)
{
    WRAPPERS_DEBUG_PRINTF("sqrt()\n", __x);
    return sqrt(__x);
}
void
my_srand48(long int __seedval)
{
    WRAPPERS_DEBUG_PRINTF("srand48()\n", __seedval);
    srand48(__seedval);
}
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
char *
my_strcat(char *__restrict __dest, __const char *__restrict __src)
{
    WRAPPERS_DEBUG_PRINTF("strcat()\n", __dest, __src);
    return strcat(__dest, __src);
}
char *
my_strchr(__const char *__s, int __c)
{
    WRAPPERS_DEBUG_PRINTF("strchr()\n", __s, __c);
    return strchr(__s, __c);
}
int
my_strcmp(__const char *__s1, __const char *__s2)
{
    WRAPPERS_DEBUG_PRINTF("strcmp()\n", __s1, __s2);
    return strcmp(__s1, __s2);
}
int
my_strcoll(__const char *__s1, __const char *__s2)
{
    WRAPPERS_DEBUG_PRINTF("strcoll()\n", __s1, __s2);
    return strcoll(__s1, __s2);
}
char *
my_strcpy(char *__restrict __dest, __const char *__restrict __src)
{
    WRAPPERS_DEBUG_PRINTF("strcpy()\n", __dest, __src);
    return strcpy(__dest, __src);
}
size_t
my_strcspn(__const char *__s, __const char *__reject)
{
    WRAPPERS_DEBUG_PRINTF("strcspn()\n", __s, __reject);
    return strcspn(__s, __reject);
}
char *
my_strerror(int __errnum)
{
    WRAPPERS_DEBUG_PRINTF("strerror()\n", __errnum);
    return strerror(__errnum);
}
size_t
my_strftime(char *__restrict __s, size_t __maxsize, __const char *__restrict __format, __const struct tm *__restrict __tp)
{
    WRAPPERS_DEBUG_PRINTF("strftime()\n", __s, __maxsize, __format, __tp);
    return strftime(__s, __maxsize, __format, __tp);
}
size_t
my_strlen(__const char *__s)
{
    WRAPPERS_DEBUG_PRINTF("strlen()\n", __s);
    return strlen(__s);
}
char *
my_strncat(char *__restrict __dest, __const char *__restrict __src, size_t __n)
{
    WRAPPERS_DEBUG_PRINTF("strncat()\n", __dest, __src, __n);
    return strncat(__dest, __src, __n);
}
int
my_strncmp(__const char *__s1, __const char *__s2, size_t __n)
{
    WRAPPERS_DEBUG_PRINTF("strncmp()\n", __s1, __s2, __n);
    return strncmp(__s1, __s2, __n);
}
char *
my_strncpy(char *__restrict __dest, __const char *__restrict __src, size_t __n)
{
    WRAPPERS_DEBUG_PRINTF("strncpy()\n", __dest, __src, __n);
    return strncpy(__dest, __src, __n);
}
char *
my_strpbrk(__const char *__s, __const char *__accept)
{
    WRAPPERS_DEBUG_PRINTF("strpbrk()\n", __s, __accept);
    return strpbrk(__s, __accept);
}
char *
my_strrchr(__const char *__s, int __c)
{
    WRAPPERS_DEBUG_PRINTF("strrchr()\n", __s, __c);
    return strrchr(__s, __c);
}
char *
my_strstr(__const char *__haystack, __const char *__needle)
{
    WRAPPERS_DEBUG_PRINTF("strstr()\n", __haystack, __needle);
    return strstr(__haystack, __needle);
}
double
my_strtod(__const char *__restrict __nptr, char **__restrict __endptr)
{
    WRAPPERS_DEBUG_PRINTF("strtod()\n", __nptr, __endptr);
    return strtod(__nptr, __endptr);
}
char *
my_strtok(char *__restrict __s, __const char *__restrict __delim)
{
    WRAPPERS_DEBUG_PRINTF("strtok()\n", __s, __delim);
    return strtok(__s, __delim);
}
long int
my_strtol(__const char *__restrict __nptr, char **__restrict __endptr, int __base)
{
    WRAPPERS_DEBUG_PRINTF("strtol()\n", __nptr, __endptr, __base);
    return strtol(__nptr, __endptr, __base);
}
unsigned long int
my_strtoul(__const char *__restrict __nptr, char **__restrict __endptr, int __base)
{
    WRAPPERS_DEBUG_PRINTF("strtoul()\n", __nptr, __endptr, __base);
    return strtoul(__nptr, __endptr, __base);
}
int
my_system(__const char *__command)
{
    WRAPPERS_DEBUG_PRINTF("system()\n", __command);
    return system(__command);
}
double
my_tan(double __x)
{
    WRAPPERS_DEBUG_PRINTF("tan()\n", __x);
    return tan(__x);
}
double
my_tanh(double __x)
{
    WRAPPERS_DEBUG_PRINTF("tanh()\n", __x);
    return tanh(__x);
}
time_t
my_time(time_t *__timer)
{
    WRAPPERS_DEBUG_PRINTF("time()\n", __timer);
    return time(__timer);
}
FILE *
my_tmpfile()
{
    WRAPPERS_DEBUG_PRINTF("tmpfile()\n");
    return tmpfile();
}
char *
my_tmpnam(char *__s)
{
    WRAPPERS_DEBUG_PRINTF("tmpnam()\n", __s);
    //return tmpnam(__s);
    return NULL;
}
int
my_ungetc(int __c, FILE *__stream)
{
    WRAPPERS_DEBUG_PRINTF("ungetc()\n", __c, __stream);
    return ungetc(__c, __stream);
}
int
my_usleep(__useconds_t __useconds)
{
    WRAPPERS_DEBUG_PRINTF("usleep()\n", __useconds);
    return usleep(__useconds);
}
ssize_t
my_write(int __fd, __const void *__buf, size_t __n)
{
    WRAPPERS_DEBUG_PRINTF("write()\n", __fd, __buf, __n);
    return write(__fd, __buf, __n);
}

int
my_fprintf(FILE *stream, const char *format, ...)
{
    WRAPPERS_DEBUG_PRINTF("my_fprintf(%x, %s)\n", stream, format);
    va_list ap;
    int result;
    va_start(ap, format);
    if (IS_STDIO_FILE(stream))
        result = vfprintf(TO_STDIO_FILE(stream), format, ap);
    else
        result = vfprintf(stream, format, ap);
    va_end(ap);
    return result;
}

int
my_vfprintf(FILE *stream, const char *format, va_list ap)
{
    WRAPPERS_DEBUG_PRINTF("my_vfprintf(%x, %s)\n", stream, format);
    if (IS_STDIO_FILE(stream))
        return vfprintf(TO_STDIO_FILE(stream), format, ap);
    else
        return vfprintf(stream, format, ap);
}

int
my_fflush(FILE *stream)
{
    WRAPPERS_DEBUG_PRINTF("my_fflush(%x)\n", stream);
    if (IS_STDIO_FILE(stream))
        return fflush(TO_STDIO_FILE(stream));
    else
        return fflush(stream);
}

int my_sprintf(char *str, const char *format, ...)
{
    WRAPPERS_DEBUG_PRINTF("sprintf\n");
    va_list ap;
    va_start(ap, format);
    int result = vsprintf(str, format, ap);
    va_end(ap);
    return result;
}

int my_snprintf(char *str, size_t size, const char *format, ...)
{
    WRAPPERS_DEBUG_PRINTF("snprintf\n");
    va_list ap;
    va_start(ap, format);
    int result = vsnprintf(str, size, format, ap);
    va_end(ap);
    return result;
}

int my_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    WRAPPERS_DEBUG_PRINTF("vsnprintf '%s'\n", format);
    return vsnprintf(str, size, format, ap);
}

/* android uses kernel's stat64 as 'struct stat', so do direct syscalls */
int my_stat(const char *path, void *buf)
{
    return syscall(__NR_stat64, path, buf);
}

int my_fstat(int fd, void *buf)
{
    return syscall(__NR_fstat64, fd, buf);
}

int my_lstat(const char *path, void *buf)
{
    return syscall(__NR_lstat64, path, buf);
}

int my_fstatat(int dirfd, const char *pathname, void *buf, int flags)
{
    return syscall(__NR_fstatat64, dirfd, pathname, buf, flags);
}

struct a_dirent {
    uint64_t         d_ino;
    int64_t          d_off;
    unsigned short   d_reclen;
    unsigned char    d_type;
    char             d_name[256];
};

static void convert_dirent(struct a_dirent *ade, const struct dirent *de)
{
    ade->d_ino = de->d_ino;
    ade->d_off = de->d_off;
    ade->d_reclen = de->d_reclen;
    ade->d_type = de->d_type;
    strncpy(ade->d_name, de->d_name, sizeof(ade->d_name));
}

struct a_dirent *my_readdir(DIR *dirp)
{
    static struct a_dirent ade;
    struct dirent *de;
    if ((de = readdir(dirp)) == NULL)
        return NULL;
    convert_dirent(&ade, de);
    return &ade;
}

int my_readdir_r(DIR *dirp, struct a_dirent *entry, struct a_dirent **result)
{
    struct dirent de, *de_ret = NULL;
    int ret;
    ret = readdir_r(dirp, &de, &de_ret);
    if (ret != 0)
        return ret;
    if (de_ret == NULL) {
        *result = NULL;
        return 0;
    }
    convert_dirent(entry, de_ret);
    *result = entry;
    return 0;
}

void libc_wrappers_init(void)
{
    stdio_files[0] = stdin;
    stdio_files[1] = stdout;
    stdio_files[2] = stderr;
}
