#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include "Txlib.h"
#include "stack.h"

int main()
{
    Stack_t stk = {};

    StackCtor(&stk, 3);

    StackPush(&stk, 1);
    printf("1 byte hash = %lo\n", Reconut_Byte_Hash(&stk));
    StackPush(&stk, 2);
    printf("2 byte hash = %lo\n", Reconut_Byte_Hash(&stk));
    StackPush(&stk, 3);
    printf("3 byte hash = %lo\n", Reconut_Byte_Hash(&stk));
StackPush(&stk, 1);
printf("4 byte hash = %lo\n", Reconut_Byte_Hash(&stk));
StackPush(&stk, 2);
printf("5 byte hash = %lo\n", Reconut_Byte_Hash(&stk));
StackPush(&stk, 3);
printf("6 byte hash = %lo\n", Reconut_Byte_Hash(&stk));
StackPush(&stk, 1);
printf("7 byte hash = %lo\n", Reconut_Byte_Hash(&stk));
StackPush(&stk, 2);
printf("8 byte hash = %lo\n", Reconut_Byte_Hash(&stk));
StackPush(&stk, 3);
printf("9 byte hash = %lo\n", Reconut_Byte_Hash(&stk));
StackPush(&stk, 1);
printf("10 byte hash = %lo\n", Reconut_Byte_Hash(&stk));
StackPush(&stk, 2);
printf("11 byte hash = %lo\n", Reconut_Byte_Hash(&stk));
StackPush(&stk, 3);
printf("12 byte hash = %lo\n\n\n", Reconut_Byte_Hash(&stk));

    int first_value = 0;
    int second_value = 0;
    int third_value = 0;

    //for (int counter = -1; counter <= stk.capacity; counter++)
    //    stk.data[counter] = 0;

    StackPop(&stk, &first_value);
    printf("1 byte hash = %lo\n\n", Reconut_Byte_Hash(&stk));
    StackPop(&stk, &second_value);
    printf("2 byte hash = %lo\n\n", Reconut_Byte_Hash(&stk));
    StackPop(&stk, &third_value);
printf("3 byte hash = %lo\n\n", Reconut_Byte_Hash(&stk));
StackPop(&stk, &first_value);
printf("4 byte hash = %lo\n\n", Reconut_Byte_Hash(&stk));
StackPop(&stk, &second_value);
printf("5 byte hash = %lo\n\n", Reconut_Byte_Hash(&stk));
StackPop(&stk, &third_value);
printf("6 byte hash = %lo\n\n", Reconut_Byte_Hash(&stk));
StackPop(&stk, &first_value);
printf("7 byte hash = %lo\n\n", Reconut_Byte_Hash(&stk));
StackPop(&stk, &second_value);
printf("8 byte hash = %lo\n\n", Reconut_Byte_Hash(&stk));
StackPop(&stk, &third_value);
printf("9 byte hash = %lo\n\n", Reconut_Byte_Hash(&stk));
StackPop(&stk, &first_value);
printf("10 byte hash = %lo\n\n", Reconut_Byte_Hash(&stk));
StackPop(&stk, &second_value);
printf("11 byte hash = %lo\n\n", Reconut_Byte_Hash(&stk));
StackPop(&stk, &third_value);
printf("12 byte hash = %lo\n\n", Reconut_Byte_Hash(&stk));

    printf("\nFirst = %d, second = %d, third = %d\n", first_value, second_value, third_value);

    StackDtor(&stk);

    return 0;
}
