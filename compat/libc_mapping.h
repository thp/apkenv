
{"malloc", malloc},
{"valloc", valloc},
{"realloc", realloc},
{"calloc", calloc},
{"free", free},
{"strdup", strdup},

{"open", open},
{"openat", openat},
{"read", read},
{"write", write},
{"writev", writev},
{"lseek", lseek},
{"select", select},
{"close", close},

// must wrap because:
// - Android's 'struct stat' is incompatible with glibc's
// - bionic's *stat* may call errno and corrupt glibc's state
{"stat", my_stat},
{"fstat", my_fstat},
{"lstat", my_lstat},
{"fstatat", my_fstatat},

// struct dirent is also incompatible
{"opendir", opendir},
{"fdopendir", fdopendir},
{"readdir", my_readdir},
{"readdir_r", my_readdir_r},
{"closedir", closedir},
{"rewinddir", rewinddir},
{"dirfd", dirfd},

{"printf", printf},
{"sprintf", sprintf},
{"snprintf", my_snprintf},
{"vsnprintf", my_vsnprintf},
{"vasprintf", my_vasprintf},

{"fopen", my_fopen},
{"fdopen", fdopen},
{"freopen", my_freopen},
{"fflush", my_fflush},
{"fclose", my_fclose},
{"fcntl", fcntl},
{"fprintf", my_fprintf},
{"vprintf", vprintf},
{"vfprintf", my_vfprintf},
{"fputc", my_fputc},
{"fputs", my_fputs},
{"fgets", my_fgets},
{"fgetpos", fgetpos},
{"fsetpos", fsetpos},
{"fscanf", my_fscanf},
{"vscanf", vscanf},
{"vfscanf", vfscanf},
{"sscanf", sscanf},
{"fseek", my_fseek},
{"fileno", my_fileno},
{"fsync", fsync},
{"ftell", my_ftell},
{"fread", my_fread},
{"fwrite", my_fwrite},
{"puts", puts},
{"putc", my_putc},
{"putw", my_putw},
{"putchar", putchar},
{"getc", getc},
{"fgetc", fgetc},
{"ungetc", my_ungetc},
{"tmpfile", tmpfile},
{"setbuf", my_setbuf},
{"setvbuf", my_setvbuf},
{"setbuffer", my_setbuffer},
{"setlinebuf", my_setlinebuf},
{"ftello", ftello},
{"fseeko", fseeko},
{"ferror", my_ferror},
{"feof", feof},
{"rewind", rewind},

{"setenv", setenv},
{"sysconf", my_sysconf},

/* bionic's addrinfo struct differs from glibc's */
{"getaddrinfo", my_getaddrinfo},
{"freeaddrinfo", my_freeaddrinfo},

{"gethostbyname", gethostbyname},

{"__errno", my___errno},

{"gmtime", gmtime},
{"localtime", localtime},
{"localtime_r", localtime_r},
{"mktime", mktime},
{"strftime", strftime},
{"wcsftime", wcsftime},

{"setlocale", setlocale},

{"getenv", my_getenv},
{"geteuid", geteuid},
{"getpid", getpid},
{"geteuid", geteuid},

{"clearerr", my_clearerr},

{"ioctl", ioctl},
{"mmap", mmap},
{"munmap", munmap},
{"poll", poll},

{"mkdir", mkdir},
{"chmod", chmod},
{"umask", umask},
{"remove", my_remove},
{"rename", my_rename},

{"accept", accept},
{"listen", listen},
{"alarm", alarm},
{"atoi", atoi},
{"atol", atol},
{"basename", basename},
{"bind", bind},
{"clock_gettime", clock_gettime},

{"geteuid", geteuid},
{"getpeername", getpeername},
{"getpwuid", getpwuid},
{"getsockname", getsockname},
{"getsockopt", getsockopt},
{"gmtime_r", gmtime_r},
{"inet_ntop", inet_ntop},
{"inet_pton", inet_pton},
{"inet_pton", inet_pton},

{"memrchr", memrchr},
{"qsort", qsort},
{"bsearch", bsearch},
{"shutdown", shutdown},
{"sigaction", sigaction},
//{"siglongjmp", siglongjmp},
//{"sigsetjmp", _sigsetjmp},
{"strtok_r", strtok_r},
{"strncasecmp", strncasecmp},

{"strerror_r", strerror_r},
{"strerror", strerror},
{"strcspn", strcspn},
{"strsep", strsep},
{"strcoll", strcoll},
{"strcspn", my_strcspn},
{"strerror", my_strerror},
{"strpbrk", my_strpbrk},
{"system", my_system},
{"tmpnam", my_tmpnam},
{"usleep", usleep},

{"connect", my_connect},
{"recv", my_recv},
{"sched_yield", my_sched_yield},
{"send", my_send},
{"socket", my_socket},
{"setsockopt", my_setsockopt},

{"gettimeofday", my_gettimeofday},

{"abort", my_abort},
{"clock", my_clock},
{"difftime", my_difftime},
{"exit", my_exit},

{"strchr", strchr},
{"strcpy", strcpy},
{"strcat", strcat},
{"strtoul", strtoul},
{"strrchr", strrchr},
{"strncmp", strncmp},
{"strncat", strncat},
{"strlen", strlen},
{"strtod", my_strtod},
{"strtol", strtol},
{"strtok", strtok},
{"strncpy", strncpy},
{"strcmp", strcmp},
{"strstr", strstr},
{"memmove", memmove},
{"memset", memset},
{"memcpy", memcpy},
{"memchr", memchr},
{"memcmp", memcmp},
{"time", time},
{"tzset", tzset},
{"tzname", tzname},

// Android's longjumps are incompatible with glibc's
// and will corrupt stack if we use glibc ones
//{"setjmp", setjmp},
//{"longjmp", longjmp},
{"raise", raise},

{"srand48", srand48},
{"lrand48", lrand48},

{"iswalpha", iswalpha},
{"iswspace", iswspace},
{"iswlower", iswlower},
{"islower", islower},
{"isupper", isupper},
{"ispunct", ispunct},
{"iswxdigit", iswxdigit},
{"iscntrl", iscntrl},
{"iswprint", iswprint},
{"iswupper", iswupper},
{"iswcntrl", iswcntrl},
{"iswpunct", iswpunct},
{"iswdigit", iswdigit},
{"isprint", isprint},
{"isalpha", isalpha},
{"isspace", isspace},
{"isxdigit", isxdigit},

{"tolower", tolower},
{"toupper", toupper},
{"towlower", towlower},
{"towupper", towupper},
{"wcsncpy", wcsncpy},
{"wcscmp", wcscmp},
{"wmemmove", wmemmove},
{"wmemset", wmemset},
{"wmemcpy", wmemcpy},
{"wmemchr", wmemchr},
{"wmemcmp", wmemcmp},
{"wcslen", wcslen},

/* zlib */
{"inflate", inflate},
{"inflateInit_", inflateInit_},
{"inflateInit2_", inflateInit2_},
{"inflateReset", inflateReset},
{"inflateEnd", inflateEnd},
{"deflate", deflate},
{"deflateInit2_", deflateInit2_},
{"deflateEnd", deflateEnd},
{"crc32", crc32},
{"gzopen", gzopen},
{"gzclose", gzclose},
{"gzgets", gzgets},

/* grp.h */
{"__cxa_atexit", __cxa_atexit},
{"atexit", atexit},

/*
 * bionic libc's __isthreaded
 *
 * Must hook (and set to 1/true), otherwise android code might directly
 * access the bionic FILE struct which differs from the one in glibc platforms
 * and we don't wrap the FILE struct at the moment. There are other (thread
 * related) cases where this matters too.
 *
 * For details see the corresponding macro definitions of feof, ferror,
 * clearerr, fileno, getc, putc, FLOCKFILE, FUNLOCKFILE, _MALLOC_LOCK,
 * _MALLOC_UNLOCK, _MALLOC_LOCK_INIT, _ATEXIT_LOCK, _ATEXIT_UNLOCK
 * and the functions:  _tzLock, _tzUnlock, located in bionic/libc
 *
 */
{"__isthreaded", &my___isthreaded},

