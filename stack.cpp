#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

#include "stack.h"

const Canary = 0xDEADBABE;

int StackVerify(Stack_t *stk) {

    int error = 0;

    if (stk == NULL)
        return Pointer_Error;

    if (stk->capacity <= 0)
        error |= Capacity_Error;

    if (stk->stack_size < 0)
        error |= Size_Error;

    if (stk->data == NULL)
        error |= Buffer_Error;

    return error;
}

int StackCtor(Stack_t *stk, int capacity) {

    if (stk == NULL)
        return Pointer_Error;

    stk->data = (int *) calloc(1, capacity*sizeof(int));
    stk->stack_size = 0;
    stk->capacity = capacity;

    Return_Error(stk);
}

int StackPush(Stack_t *stk, int value) {

    Return_If_Error(stk);

    if (stk->capacity + 1 <= stk->stack_size) {
        stk->data = (int *) realloc(stk->data, stk->stack_size*2*sizeof(int));
        stk->capacity = stk->stack_size*2;
    }

    stk->data[stk->stack_size++] = value;
//printf("value is %d\n\n", stk->data[stk->stack_size-1]);   // Кладется нормальное

    Return_Error(stk);

}

int StackPop(Stack_t *stk, int *value) {

    Return_If_Error(stk);

    if (value == NULL)
        return Pointer_Error;

    if (stk->stack_size == 0)
        return Size_Error;

//printf("old value is %d\n", *value);
//printf("stack_size is %d, data ptr [%p], capacity is %d\n", stk->stack_size, stk->data, stk->capacity);
//printf("thing that must be value is %d\n", stk->data[stk->stack_size - 1]);
    *value = stk->data[--stk->stack_size];
//printf("new value is %d\n\n", *value);

    if (stk->capacity >= 3 * stk->stack_size && stk->stack_size != 0) {
        stk->data = (int *) realloc(stk->data, stk->stack_size * 2 * sizeof(int));
        stk->capacity = stk->stack_size * 2;
    }

    Return_Error(stk);
}

int StackDtor(Stack_t *stk) {

    Return_If_Error(stk);
                            //TODO: garbage in stk memory
    free(stk->data);

    return 0;
}

void StackDump(Stack_t *stk, const char *file_name, const char *function_name, int line_number) {

    int error = StackVerify(stk);

    printf("StackDump() from %s at %s:%d:\n", file_name, function_name, line_number);

    if (error & Pointer_Error || stk == NULL) {      // Same variants, just to be sure
        printf("ERROR! Stack pointer is wrong. Pointer is [%p]\n", stk);
        return;
    }

    printf("Stack [%p]\n", stk);

    if (error & Buffer_Error) {
        printf("ERROR! Buffer is wrong. Buffer pointer is [%p]\n", stk->data);    // Working
        return;
    }

    printf("Buffer [%p]\n", stk->data);

    if (error & Size_Error)
        printf("ERROR! Size is wrong, size is %d\n", stk->stack_size);
    if (error & Capacity_Error)
        printf("ERROR! Capacity is wrong, capacity is %d\n", stk->capacity);
    if (!(error & Size_Error) && !(error & Capacity_Error))
        printf("Size is %d, capacity is %d\n", stk->stack_size, stk->capacity);

    int curr_element = 0;

    while (curr_element < stk->stack_size && curr_element < stk->capacity) {
        printf("*[%d] = %d\n", curr_element, stk->data[curr_element]);
        curr_element++;
    }

    while (curr_element < stk->capacity) {
        printf("[%d] = %d - after size\n", curr_element, stk->data[curr_element]);
        curr_element++;
    }

}

