/**
 * @file sysmem.c
 * @brief newlib _sbrk heap implementation for bare-metal ARM.
 */
#include <errno.h>
#include <stddef.h>
#include <stdint.h>

extern uint8_t _end;
extern uint8_t _estack;
extern uint32_t _Min_Stack_Size;

void* _sbrk(ptrdiff_t incr)
{
    static uint8_t* heapEnd = NULL;
    uint8_t* previousHeapEnd;
    if (heapEnd == NULL) {
        heapEnd = &_end;
    }
    if (heapEnd + incr > &_estack - (ptrdiff_t)&_Min_Stack_Size) {
        errno = ENOMEM;
        return (void*)-1;
    }
    previousHeapEnd = heapEnd;
    heapEnd += incr;
    return (void*)previousHeapEnd;
}
