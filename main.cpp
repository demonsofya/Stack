#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include "Txlib.h"
#include "stack.h"

int main()
{
    atexit(ExitFunction);

    Stack_t stk = {};

    StackCtor(&stk, 3);

    StackPush(&stk, 1);
    StackPush(&stk, 2);
    StackPush(&stk, 3);


    int first_value = 0;
    int second_value = 0;
    int third_value = 0;

    //for (int counter = -1; counter <= stk.capacity; counter++)
    //    stk.data[counter] = 0;

    StackPop(&stk, &first_value);
    StackPop(&stk, &second_value);
    StackPop(&stk, &third_value);


    printf("\nFirst = %d, second = %d, third = %d\n", first_value, second_value, third_value);

    StackDtor(&stk);

    return 0;
}
