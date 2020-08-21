#ifndef FLECS_OS_API_H
#define FLECS_OS_API_H

#include <stdarg.h>

#if defined(_MSC_VER) || defined(__MINGW32__)
#include <malloc.h>
#else
#include <alloca.h>
#endif

#if defined(_WIN32)
#define ECS_OS_WINDOWS
#elif defined(__linux__)
#define ECS_OS_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
#define ECS_OS_DARWIN
#else
/* Unknown OS */
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ecs_time_t {
    uint32_t sec;
    uint32_t nanosec;
} ecs_time_t;

/* Allocation counters (not thread safe) */
extern int64_t ecs_os_api_malloc_count;
extern int64_t ecs_os_api_realloc_count;
extern int64_t ecs_os_api_calloc_count;
extern int64_t ecs_os_api_free_count;

/* Use handle types that _at least_ can store pointers */
typedef uintptr_t ecs_os_thread_t;
typedef uintptr_t ecs_os_cond_t;
typedef uintptr_t ecs_os_mutex_t;
typedef uintptr_t ecs_os_dl_t;

/* Generic function pointer type */
typedef void (*ecs_os_proc_t)(void);

/* OS API init */
typedef 
void (*ecs_os_api_init_t)(void);

/* OS API deinit */
typedef 
void (*ecs_os_api_fini_t)(void);

/* Memory management */
typedef 
void* (*ecs_os_api_malloc_t)(
    ecs_size_t size);

typedef 
void (*ecs_os_api_free_t)(
    void *ptr);

typedef
void* (*ecs_os_api_realloc_t)(
    void *ptr, 
    ecs_size_t size);

typedef
void* (*ecs_os_api_calloc_t)(
    ecs_size_t size);

typedef
char* (*ecs_os_api_strdup_t)(
    const char *str);

/* Threads */
typedef
void* (*ecs_os_thread_callback_t)(
    void*);

typedef
ecs_os_thread_t (*ecs_os_api_thread_new_t)(
    ecs_os_thread_callback_t callback,
    void *param);

typedef
void* (*ecs_os_api_thread_join_t)(
    ecs_os_thread_t thread);


/* Atomic increment / decrement */
typedef
int (*ecs_os_api_ainc_t)(
    int32_t *value);


/* Mutex */
typedef
ecs_os_mutex_t (*ecs_os_api_mutex_new_t)(
    void);

typedef
void (*ecs_os_api_mutex_lock_t)(
    ecs_os_mutex_t mutex);

typedef
void (*ecs_os_api_mutex_unlock_t)(
    ecs_os_mutex_t mutex);

typedef
void (*ecs_os_api_mutex_free_t)(
    ecs_os_mutex_t mutex);

/* Condition variable */
typedef
ecs_os_cond_t (*ecs_os_api_cond_new_t)(
    void);

typedef
void (*ecs_os_api_cond_free_t)(
    ecs_os_cond_t cond);

typedef
void (*ecs_os_api_cond_signal_t)(
    ecs_os_cond_t cond);

typedef
void (*ecs_os_api_cond_broadcast_t)(
    ecs_os_cond_t cond);

typedef
void (*ecs_os_api_cond_wait_t)(
    ecs_os_cond_t cond,
    ecs_os_mutex_t mutex);

typedef 
void (*ecs_os_api_sleep_t)(
    int32_t sec,
    int32_t nanosec);

typedef
void (*ecs_os_api_get_time_t)(
    ecs_time_t *time_out);

/* Logging */
typedef
void (*ecs_os_api_log_t)(
    const char *fmt,
    va_list args);

/* Application termination */
typedef
void (*ecs_os_api_abort_t)(
    void);

/* Dynamic libraries */
typedef
ecs_os_dl_t (*ecs_os_api_dlopen_t)(
    const char *libname);

typedef
ecs_os_proc_t (*ecs_os_api_dlproc_t)(
    ecs_os_dl_t lib,
    const char *procname);

typedef
void (*ecs_os_api_dlclose_t)(
    ecs_os_dl_t lib);

typedef
char* (*ecs_os_api_module_to_path_t)(
    const char *module_id);

/* Prefix members of struct with '_' as some system headers may define macro's
 * for functions like "strdup" or "log" */

typedef struct ecs_os_api_t {
    /* API init / deinit */
    ecs_os_api_init_t _init;
    ecs_os_api_fini_t _fini;

    /* Memory management */
    ecs_os_api_malloc_t _malloc;
    ecs_os_api_realloc_t _realloc;
    ecs_os_api_calloc_t _calloc;
    ecs_os_api_free_t _free;

    /* Strings */
    ecs_os_api_strdup_t _strdup;

    /* Threads */
    ecs_os_api_thread_new_t _thread_new;
    ecs_os_api_thread_join_t _thread_join;

    /* Atomic incremenet / decrement */
    ecs_os_api_ainc_t _ainc;
    ecs_os_api_ainc_t _adec;

    /* Mutex */
    ecs_os_api_mutex_new_t _mutex_new;
    ecs_os_api_mutex_free_t _mutex_free;
    ecs_os_api_mutex_lock_t _mutex_lock;
    ecs_os_api_mutex_lock_t _mutex_unlock;

    /* Condition variable */
    ecs_os_api_cond_new_t _cond_new;
    ecs_os_api_cond_free_t _cond_free;
    ecs_os_api_cond_signal_t _cond_signal;
    ecs_os_api_cond_broadcast_t _cond_broadcast;
    ecs_os_api_cond_wait_t _cond_wait;

    /* Time */
    ecs_os_api_sleep_t _sleep;
    ecs_os_api_get_time_t _get_time;

    /* Logging */
    ecs_os_api_log_t _log;
    ecs_os_api_log_t _log_error;
    ecs_os_api_log_t _log_debug;
    ecs_os_api_log_t _log_warning;

    /* Application termination */
    ecs_os_api_abort_t _abort;

    /* Dynamic library loading */
    ecs_os_api_dlopen_t _dlopen;
    ecs_os_api_dlproc_t _dlproc;
    ecs_os_api_dlclose_t _dlclose;

    /* Overridable function that translates from a logical module id to a
     * shared library filename */
    ecs_os_api_module_to_path_t _module_to_dl;

    /* Overridable function that translates from a logical module id to a
     * path that contains module-specif resources or assets */
    ecs_os_api_module_to_path_t _module_to_etc;    
} ecs_os_api_t;

FLECS_EXPORT
extern ecs_os_api_t ecs_os_api;

FLECS_EXPORT
void ecs_os_init(void);

FLECS_EXPORT
void ecs_os_fini(void);

FLECS_EXPORT
void ecs_os_set_api(
    ecs_os_api_t *os_api);

FLECS_EXPORT
void ecs_os_set_api_defaults(void);

/* Memory management */
#define ecs_os_malloc(size) ecs_os_api._malloc(size);
#define ecs_os_free(ptr) ecs_os_api._free(ptr);
#define ecs_os_realloc(ptr, size) ecs_os_api._realloc(ptr, size)
#define ecs_os_calloc(size) ecs_os_api._calloc(size)
#if defined(_MSC_VER) || defined(__MINGW32__)
#define ecs_os_alloca(size) _alloca((size_t)(size))
#else
#define ecs_os_alloca(size) alloca((size_t)(size))
#endif

/* Strings */
#define ecs_os_strdup(str) ecs_os_api._strdup(str)
#define ecs_os_strlen(str) (ecs_size_t)strlen(str)
#define ecs_os_strcmp(str1, str2) strcmp(str1, str2)
#define ecs_os_strncmp(str1, str2, num) strncmp(str1, str2, (size_t)(num))
#define ecs_os_memcmp(ptr1, ptr2, num) memcmp(ptr1, ptr2, (size_t)(num))
#define ecs_os_memcpy(ptr1, ptr2, num) memcpy(ptr1, ptr2, (size_t)(num))
#define ecs_os_memset(ptr, value, num) memset(ptr, value, (size_t)(num))

#if defined(_MSC_VER)
#define ecs_os_strcat(str1, str2) strcat_s(str1, INT_MAX, str2)
#define ecs_os_sprintf(ptr, ...) sprintf_s(ptr, INT_MAX, __VA_ARGS__)
#define ecs_os_vsprintf(ptr, fmt, args) vsprintf_s(ptr, INT_MAX, fmt, args)
#define ecs_os_strcpy(str1, str2) strcpy_s(str1, INT_MAX, str2)
#define ecs_os_strncpy(str1, str2, num) strncpy_s(str1, INT_MAX, str2, (size_t)(num))
#else
#define ecs_os_strcat(str1, str2) strcat(str1, str2)
#define ecs_os_sprintf(ptr, ...) sprintf(ptr, __VA_ARGS__)
#define ecs_os_vsprintf(ptr, fmt, args) vsprintf(ptr, fmt, args)
#define ecs_os_strcpy(str1, str2) strcpy(str1, str2)
#define ecs_os_strncpy(str1, str2, num) strncpy(str1, str2, (size_t)(num))
#endif


/* Threads */
#define ecs_os_thread_new(callback, param) ecs_os_api._thread_new(callback, param)
#define ecs_os_thread_join(thread) ecs_os_api._thread_join(thread)

/* Atomic increment / decrement */
#define ecs_os_ainc(value) ecs_os_api._ainc(value)
#define ecs_os_adec(value) ecs_os_api._adec(value)

/* Mutex */
#define ecs_os_mutex_new() ecs_os_api._mutex_new()
#define ecs_os_mutex_free(mutex) ecs_os_api._mutex_free(mutex)
#define ecs_os_mutex_lock(mutex) ecs_os_api._mutex_lock(mutex)
#define ecs_os_mutex_unlock(mutex) ecs_os_api._mutex_unlock(mutex)

/* Condition variable */
#define ecs_os_cond_new() ecs_os_api._cond_new()
#define ecs_os_cond_free(cond) ecs_os_api._cond_free(cond)
#define ecs_os_cond_signal(cond) ecs_os_api._cond_signal(cond)
#define ecs_os_cond_broadcast(cond) ecs_os_api._cond_broadcast(cond)
#define ecs_os_cond_wait(cond, mutex) ecs_os_api._cond_wait(cond, mutex)

/* Time */
#define ecs_os_sleep(sec, nanosec) ecs_os_api._sleep(sec, nanosec)
#define ecs_os_get_time(time_out) ecs_os_api._get_time(time_out)

/* Logging (use functions to avoid using variadic macro arguments) */
FLECS_EXPORT
void ecs_os_log(const char *fmt, ...);

FLECS_EXPORT
void ecs_os_warn(const char *fmt, ...);

FLECS_EXPORT
void ecs_os_err(const char *fmt, ...);

FLECS_EXPORT
void ecs_os_dbg(const char *fmt, ...);

/* Application termination */
#define ecs_os_abort() ecs_os_api._abort()

/* Dynamic libraries */
#define ecs_os_dlopen(libname) ecs_os_api._dlopen(libname)
#define ecs_os_dlproc(lib, procname) ecs_os_api._dlproc(lib, procname)
#define ecs_os_dlclose(lib) ecs_os_api._dlclose(lib)

/* Module id translation */
#define ecs_os_module_to_dl(lib) ecs_os_api._module_to_dl(lib)
#define ecs_os_module_to_etc(lib) ecs_os_api._module_to_etc(lib)

/* Sleep with floating point time */
FLECS_EXPORT
void ecs_sleepf(
    double t);

/* Measure time since provided timestamp */
FLECS_EXPORT
double ecs_time_measure(
    ecs_time_t *start);

/* Calculate difference between two timestamps */
FLECS_EXPORT
ecs_time_t ecs_time_sub(
    ecs_time_t t1,
    ecs_time_t t2);

/* Convert time value to a double */
FLECS_EXPORT
double ecs_time_to_double(
    ecs_time_t t);

FLECS_EXPORT
void* ecs_os_memdup(
    const void *src, 
    ecs_size_t size);

/** Are heap functions available? */
FLECS_EXPORT
bool ecs_os_has_heap(void);

/** Are threading functions available? */
FLECS_EXPORT
bool ecs_os_has_threading(void);

/** Are time functions available? */
FLECS_EXPORT
bool ecs_os_has_time(void);

/** Are logging functions available? */
FLECS_EXPORT
bool ecs_os_has_logging(void);

/** Are dynamic library functions available? */
FLECS_EXPORT
bool ecs_os_has_dl(void);

/** Are module path functions available? */
FLECS_EXPORT
bool ecs_os_has_modules(void);

#ifdef __cplusplus
}
#endif

#endif
