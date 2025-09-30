#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <assert.h>

#include "stack.h"

// TODO:
// atexit
// logfile ctor

FILE *logfile = fopen("LogFile", "w");

unsigned long int HASH = 0;

int StackVerify(Stack_t *stk) {

    int error = 0;

    if (stk == NULL)
        return Pointer_Error;

    if (stk->capacity <= 0)
        error |= Capacity_Error;

    if (stk->stack_size < 0 )
        error |= Size_Error;

    if (stk->data == NULL)
        return error |= Buffer_Error;

    if (!CanaryCheck(stk))
        error |= Canary_Error;

    if (stk->hash != CountHash(stk))
        error |= Hash_Error;

    return error;
}

bool CanaryCheck(Stack_t *stk) {

    if (*(stk->data - 1) != CANARY || stk->data[stk->capacity] != CANARY)
        return false;

    return true;
}


//-----------------------------------------------------------------------------


int StackCtor(Stack_t *stk, int capacity) {

    if (stk == NULL)
        return Pointer_Error;

    stk->data = (int *) calloc(1, (capacity+2)*sizeof(int));
    stk->data[0] = CANARY;
    stk->data++;                 // За счет этого у меня все еще data с нулевого элемента
    stk->data[capacity] = CANARY;

    stk->stack_size = 0;
    stk->capacity = capacity;

    stk->hash = CountHash(stk);

    Return_Error(stk);
}

int StackPush(Stack_t *stk, int value) {

    Return_If_Error(stk);

    if (stk->capacity <= stk->stack_size) {

        int *new_ptr = NULL;

        stk->capacity = (stk->stack_size + 1) * 2;

        stk->data = (int *) realloc(stk->data - 1, (stk->capacity + 2) * sizeof(int));

        if (stk->data == NULL)
            return Buffer_Error;

        stk->data = (stk->data + 1);
        stk->data[stk->capacity] = CANARY;
    }

    stk->data[stk->stack_size++] = value;
//printf("value is %d\n\n", stk->data[stk->stack_size-1]);   // Кладется нормальное

    stk->hash = CountHash(stk);

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

    if (stk->capacity >= 3 * stk->stack_size && stk->stack_size > 0) {

        stk->capacity = (stk->stack_size + 1) * 2;

        stk->data = (int *) realloc(stk->data - 1, (stk->capacity + 2) * sizeof(int));

        if (stk->data == NULL)
            return Buffer_Error;

        stk->data = (stk->data + 1);
        stk->data[stk->capacity] = CANARY;
    }

    stk->hash = CountHash(stk);

    Return_Error(stk);
}

int StackDtor(Stack_t *stk) {

    if (!stk)
        return Pointer_Error;

    if (!stk->data) {
        for (int counter = 0; counter < stk->capacity; counter++)
            stk->data[counter] = POISON;

        free(stk->data);
    }
                            //TODO: garbage in stk memory
    stk->capacity = POISON;
    stk->stack_size = POISON;
    stk->data = NULL;

    return 0;
}

void StackDump(Stack_t *stk, const char *file_name, const char *function_name, int line_number) {

    int error = StackVerify(stk);

    printf("\n===========DUMP===========\n\n");

    printf("StackDump() from %s at %s:%d:\n", file_name, function_name, line_number);

    if (error & Pointer_Error || stk == NULL) {      // Same variants, just to be sure

        printf("ERROR! Stack pointer is wrong. Pointer is [%p]\n\n", stk);
        return;
    }

    printf("Stack [%p]\n", stk);

    if (error & Buffer_Error) {

        printf("ERROR! Buffer is wrong. Buffer pointer is [%p]\n\n", stk->data);    // Working
        return;
    }

    printf("Buffer [%p]\n", stk->data);

    if (error & Canary_Error)
        printf("ERROR! Canary returned false. Data was changed\n true canary %X, current are %X and %X\n\n",
                CANARY, stk->data[-1], stk->data[stk->capacity]);

    long unsigned int curr_hash = 0;
    Recount_Hash(stk, &curr_hash);
    if (error & Hash_Error)
        printf("ERROR! Hash error. Data was changed. Right hash is %ld, new is %ld\n\n", HASH, curr_hash);

    if (error & Size_Error)
        printf("ERROR! Size is wrong, size is %d\n\n", stk->stack_size);

    if (error & Capacity_Error)
        printf("ERROR! Capacity is wrong, capacity is %d\n\n", stk->capacity);

    if (!(error & Size_Error) && !(error & Capacity_Error))
        printf("Size is %d, capacity is %d\n\n", stk->stack_size, stk->capacity);

    int curr_element = 0;

    printf("-----------------------\n");

    while (curr_element < stk->stack_size && curr_element < stk->capacity) {

        printf("*[%d] = %d\n", curr_element, stk->data[curr_element]);
        curr_element++;
    }

    while (curr_element < stk->capacity) {

        printf("[%d] = %d (POISON)\n", curr_element, stk->data[curr_element]);
        curr_element++;
    }

    printf("=======================\n");

}


//-----------------------------------------------------------------------------


/*unsigned long int Count_Hash(Stack_t *stk) { // На отрицательных ломается

    if(!stk)
        return 0;

    Recount_Hash(stk, &HASH);

}*/

unsigned long int CountHash(Stack_t *stk) {

    int max_el = 0;
    unsigned long int curr_hash = 0;

    for (int counter = 0; counter < stk->stack_size; counter++)
        if (max_el < stk->data[counter])
            max_el = stk->data[counter];

    for (int counter = 0; counter < stk->stack_size; counter++)
        curr_hash += stk->data[counter] * int( pow(max_el, counter) );
        //*curr_hash += stk->data[counter];

    return curr_hash;
}

unsigned long int CountByteHash(Stack_t *stk) {

    unsigned long int curr_hash = 0;

    for (int counter = 0; counter < stk->stack_size; counter++) {

        int number = stk->data[counter];
        int curr_razrad = number%10;

        while (number != 0) {
            curr_hash = (curr_hash << 3) + curr_razrad;
            number /= 10;
        }
    }

    return curr_hash;
}

void ExitFunction() {
    fclose(logfile);
}

