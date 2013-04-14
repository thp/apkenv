
{"malloc", malloc},
{"realloc", realloc},
{"calloc", calloc},
{"free", free},
{"strdup", strdup},

{"open", open},
{"read", read},
{"write", write},
{"writev", writev},
{"lseek", lseek},
{"select", select},
{"close", close},
{"fstat", fstat},

{"printf", printf},
{"sprintf", sprintf},
{"snprintf", my_snprintf},
//{"vsnprintf", my_vsnprintf},

{"fopen", my_fopen},
{"fdopen", fdopen},
{"freopen", freopen},
{"fflush", my_fflush},
{"fclose", my_fclose},
{"fcntl", fcntl},
{"fprintf", my_fprintf},
{"vprintf", vprintf},
{"vfprintf", my_vfprintf},
{"fputc", fputc},
{"fputs", fputs},
{"fgets", fgets},
{"fgetpos", fgetpos},
{"fsetpos", fsetpos},
{"fscanf", fscanf},
//{"sscanf", sscanf}, TODO
{"fseek", fseek},
{"fileno", fileno},
{"fsync", fsync},
{"ftell", ftell},
{"fread", fread},
{"fwrite", fwrite},
{"puts", puts},
{"putc", putc},
{"putchar", putchar},
{"getc", putc},
{"ungetc", ungetc},
{"tmpfile", tmpfile},
{"setvbuf", setvbuf},
{"ftello", ftello},
{"fseeko", fseeko},
{"ferror", ferror},

{"setenv", setenv},
{"sysconf", sysconf},

{"getaddrinfo", getaddrinfo},
{"freeaddrinfo", freeaddrinfo},

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

{"clearerr", clearerr},

{"ioctl", ioctl},
{"mmap", mmap},
{"munmap", munmap},
{"poll", poll},

{"mkdir", mkdir},
{"chmod", chmod},
{"umask", umask},
{"clearerr", clearerr},
{"remove", my_remove},
{"rename", my_rename},
{"stat", stat},

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
{"siglongjmp", siglongjmp},
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
{"strcasecmp", my_strcasecmp},
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

{"setjmp", setjmp},
{"longjmp", longjmp},
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

{"tzset", tzset},
{"tzname", tzname},

{"valloc", valloc},
