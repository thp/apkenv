#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <netdb.h>
#include <sched.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <poll.h>
#include <pwd.h>
#include <arpa/inet.h>
#include <setjmp.h>
#include <wctype.h>
#include <wchar.h>
#include <dirent.h>

#include <zlib.h>

#include "../apkenv.h"

void
my_abort() SOFTFP;
double
my_acos(double __x) SOFTFP;
double
my_asin(double __x) SOFTFP;
double
my_atan(double __x) SOFTFP;
double
my_atan2(double __y, double __x) SOFTFP;
double
my_ceil(double __x) SOFTFP;
clock_t
my_clock() SOFTFP;
int
my_close(int __fd) SOFTFP;
int
my_connect(int __fd, __const struct sockaddr *__addr, socklen_t __len) SOFTFP;
double
my_cos(double __x) SOFTFP;
double
my_cosh(double __x) SOFTFP;
uLong
my_crc32(uLong crc, const Bytef *buf, uInt len) SOFTFP;
double
my_difftime(time_t __time1, time_t __time0) SOFTFP;
void
my_exit(int __status) SOFTFP;
double
my_exp(double __x) SOFTFP;
int
my_fclose(FILE *__stream) SOFTFP;
char *
my_fgets(char *__restrict __s, int __n, FILE *__restrict __stream) SOFTFP;
int
my_fprintf(FILE *stream, const char *format, ...);
int
my_vfprintf(FILE *stream, const char *format, va_list ap) SOFTFP;
int
my_fflush(FILE *stream) SOFTFP;
double
my_floor(double __x) SOFTFP;
double
my_fmod(double __x, double __y) SOFTFP;
FILE *
my_fopen(__const char *__restrict __filename, __const char *__restrict __modes) SOFTFP;
int
my_fputc(int __c, FILE *__stream) SOFTFP;
int
my_fputs(__const char *__restrict __s, FILE *__restrict __stream) SOFTFP;
size_t
my_fread(void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream) SOFTFP;
void
my_free(void *__ptr) SOFTFP;
void
my_freeaddrinfo(struct addrinfo *__ai) SOFTFP;
FILE *
my_freopen(__const char *__restrict __filename, __const char *__restrict __modes, FILE *__restrict __stream) SOFTFP;
double
my_frexp(double __x, int *__exponent) SOFTFP;
int
my_fseek(FILE *__stream, long int __off, int __whence) SOFTFP;
int
my_fsync(int __fd) SOFTFP;
long int
my_ftell(FILE *__stream) SOFTFP;
size_t
my_fwrite(__const void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __s) SOFTFP;
int
my_getaddrinfo(__const char *__restrict __name, __const char *__restrict __service, __const struct addrinfo *__restrict __req, struct addrinfo **__restrict __pai) SOFTFP;
char *
my_getenv(__const char *__name) SOFTFP;
int
my_gettimeofday(struct timeval *__restrict __tv, __timezone_ptr_t __tz) SOFTFP;
struct tm *
my_gmtime(__const time_t *__timer) SOFTFP;
int
my_inflate(z_streamp strm, int flush) SOFTFP;
int
my_inflateEnd(z_streamp strm) SOFTFP;
int
my_inflateInit_(z_streamp strm, const char *version, int stream_size) SOFTFP;
int
my_inflateInit2_(z_streamp strm, int windowBits, const char *version, int stream_size) SOFTFP;
int
my_inflateReset(z_streamp strm) SOFTFP;
double
my_ldexp(double __x, int __exponent) SOFTFP;
struct tm *
my_localtime(__const time_t *__timer) SOFTFP;
double
my_log(double __x) SOFTFP;
double
my_log10(double __x) SOFTFP;
void
my_longjmp(struct __jmp_buf_tag __env[1], int __val) SOFTFP;
long int
my_lrand48() SOFTFP;
__off_t
my_lseek(int __fd, __off_t __offset, int __whence) SOFTFP;
void *
my_malloc(size_t __size) SOFTFP;
void *
my_memchr(__const void *__s, int __c, size_t __n) SOFTFP;
int
my_memcmp(__const void *__s1, __const void *__s2, size_t __n) SOFTFP;
void *
my_memcpy(void *__restrict __dest, __const void *__restrict __src, size_t __n) SOFTFP;
void *
my_memmove(void *__dest, __const void *__src, size_t __n) SOFTFP;
void *
my_memset(void *__s, int __c, size_t __n) SOFTFP;
time_t
my_mktime(struct tm *__tp) SOFTFP;
double
my_modf(double __x, double *__iptr) SOFTFP;
double
my_pow(double __x, double __y) SOFTFP;
int
my_puts(__const char *__s) SOFTFP;
int
my_raise(int __sig) SOFTFP;
ssize_t
my_read(int __fd, void *__buf, size_t __nbytes) SOFTFP;
void *
my_realloc(void *__ptr, size_t __size) SOFTFP;
ssize_t
my_recv(int __fd, void *__buf, size_t __n, int __flags) SOFTFP;
int
my_remove(__const char *__filename) SOFTFP;
int
my_rename(__const char *__old, __const char *__new) SOFTFP;
int
my_sched_yield() SOFTFP;
int
my_select(int __nfds, fd_set *__restrict __readfds, fd_set *__restrict __writefds, fd_set *__restrict __exceptfds, struct timeval *__restrict __timeout) SOFTFP;
ssize_t
my_send(int __fd, __const void *__buf, size_t __n, int __flags) SOFTFP;
int
my_setjmp(jmp_buf __env) SOFTFP;
char *
my_setlocale(int __category, __const char *__locale) SOFTFP;
int
my_setsockopt(int __fd, int __level, int __optname, __const void *__optval, socklen_t __optlen) SOFTFP;
int
my_setvbuf(FILE *__restrict __stream, char *__restrict __buf, int __modes, size_t __n) SOFTFP;
double
my_sin(double __x) SOFTFP;
double
my_sinh(double __x) SOFTFP;
int
my_socket(int __domain, int __type, int __protocol) SOFTFP;
double
my_sqrt(double __x) SOFTFP;
void
my_srand48(long int __seedval) SOFTFP;
int
my_strcasecmp(__const char *__s1, __const char *__s2) SOFTFP;
char *
my_strcat(char *__restrict __dest, __const char *__restrict __src) SOFTFP;
char *
my_strchr(__const char *__s, int __c) SOFTFP;
int
my_strcmp(__const char *__s1, __const char *__s2) SOFTFP;
int
my_strcoll(__const char *__s1, __const char *__s2) SOFTFP;
char *
my_strcpy(char *__restrict __dest, __const char *__restrict __src) SOFTFP;
size_t
my_strcspn(__const char *__s, __const char *__reject) SOFTFP;
char *
my_strerror(int __errnum) SOFTFP;
size_t
my_strftime(char *__restrict __s, size_t __maxsize, __const char *__restrict __format, __const struct tm *__restrict __tp) SOFTFP;
size_t
my_strlen(__const char *__s) SOFTFP;
char *
my_strncat(char *__restrict __dest, __const char *__restrict __src, size_t __n) SOFTFP;
int
my_strncmp(__const char *__s1, __const char *__s2, size_t __n) SOFTFP;
char *
my_strncpy(char *__restrict __dest, __const char *__restrict __src, size_t __n) SOFTFP;
char *
my_strpbrk(__const char *__s, __const char *__accept) SOFTFP;
char *
my_strrchr(__const char *__s, int __c) SOFTFP;
char *
my_strstr(__const char *__haystack, __const char *__needle) SOFTFP;
double
my_strtod(__const char *__restrict __nptr, char **__restrict __endptr) SOFTFP;
char *
my_strtok(char *__restrict __s, __const char *__restrict __delim) SOFTFP;
long int
my_strtol(__const char *__restrict __nptr, char **__restrict __endptr, int __base) SOFTFP;
unsigned long int
my_strtoul(__const char *__restrict __nptr, char **__restrict __endptr, int __base) SOFTFP;
int
my_system(__const char *__command) SOFTFP;
double
my_tan(double __x) SOFTFP;
double
my_tanh(double __x) SOFTFP;
time_t
my_time(time_t *__timer) SOFTFP;
FILE *
my_tmpfile() SOFTFP;
char *
my_tmpnam(char *__s) SOFTFP;
int
my_ungetc(int __c, FILE *__stream) SOFTFP;
int
my_usleep(__useconds_t __useconds) SOFTFP;
ssize_t
my_write(int __fd, __const void *__buf, size_t __n) SOFTFP;

int my_sprintf(char *str, const char *format, ...) SOFTFP;
int my_snprintf(char *str, size_t size, const char *format, ...) SOFTFP;
int my_vsnprintf(char *str, size_t size, const char *format, va_list ap) SOFTFP;

int my_stat(const char *path, void *buf);
int my_fstat(int fd, void *buf);
int my_lstat(const char *path, void *buf);
int my_fstatat(int dirfd, const char *pathname, void *buf, int flags);

struct a_dirent;

struct a_dirent *my_readdir(DIR *dirp);
int my_readdir_r(DIR *dirp, struct a_dirent *entry, struct a_dirent **result);

// workaround for unity?
int my_munmap(void *__addr, size_t __len);

void libc_wrappers_init(void);
