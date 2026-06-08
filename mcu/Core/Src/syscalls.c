/**
 * @file syscalls.c
 * @brief Minimal newlib system-call stubs for bare-metal ARM.
 */
#include "stm32h7xx_hal.h"
#include "usbd_cdc_if.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <time.h>

#undef errno
extern int errno;

char* __env[1] = {0};
char** environ = __env;

void initialise_monitor_handles(void)
{
}

int _getpid(void)
{
    return 1;
}

int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

void _exit(int status)
{
    _kill(status, -1);
    for (;;) {
    }
}

int _read(int file, char* ptr, int len)
{
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

int _close(int file)
{
    (void)file;
    return -1;
}

int _fstat(int file, struct stat* st)
{
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    (void)file;
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _write(int file, char* ptr, int len)
{
    (void)file;
    if (len <= 0) {
        return 0;
    }
#ifdef MCU_DEBUG_LOG
    for (uint32_t attempt = 0; attempt < 8U; ++attempt) {
        if (CDC_Transmit_FS((uint8_t*)ptr, (uint16_t)len) == USBD_OK) {
            return len;
        }
        for (volatile uint32_t wait = 0; wait < 10000U; ++wait) {
        }
    }
#else
    (void)ptr;
#endif
    return len;
}

int _open(const char* path, int flags, ...)
{
    (void)path;
    (void)flags;
    return -1;
}

int _wait(int* status)
{
    (void)status;
    errno = ECHILD;
    return -1;
}

int _unlink(const char* name)
{
    (void)name;
    errno = ENOENT;
    return -1;
}

int _times(struct tms* buf)
{
    (void)buf;
    return -1;
}

int _stat(const char* file, struct stat* st)
{
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

int _link(const char* old, const char* newPath)
{
    (void)old;
    (void)newPath;
    errno = EMLINK;
    return -1;
}

int _fork(void)
{
    errno = EAGAIN;
    return -1;
}

int _execve(const char* name, char* const* argv, char* const* env)
{
    (void)name;
    (void)argv;
    (void)env;
    errno = ENOMEM;
    return -1;
}

int _gettimeofday(struct timeval* tv, void* tz)
{
    (void)tz;
    if (tv != NULL) {
        uint32_t ms = HAL_GetTick();
        tv->tv_sec = (long)(ms / 1000UL);
        tv->tv_usec = (long)((ms % 1000UL) * 1000UL);
    }
    return 0;
}

int usleep(unsigned int usec)
{
    uint32_t ms = (usec + 999U) / 1000U;
    if (ms == 0U) {
        ms = 1U;
    }
    HAL_Delay(ms);
    return 0;
}
