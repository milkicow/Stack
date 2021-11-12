#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>

void StackCtor(struct Stack* stk, int capacity);

void StackPush(Stack* stk, double value);

void StackDtor(Stack* stk);

void StackPop(Stack* stk);

void StackResize(Stack* stk);

void StackDump(Stack* stk);

int StackVerify(Stack* stk);

void StackFDump(Stack* stk);

void ClearFile();

void StackOk(Stack* stk);

void DumpERrors(Stack* stk, FILE* fdump);

struct Stack
{   
    int canary1;
    double* data;
    int size;
    int capacity;
    int canary2;
    int error_code;
};

enum StackERrors
{
    STACK_OVERFLOW   = 1 << 1,
    SIZE_LESS_ZERO   = 1 << 2,
    CAP_LESS_ZERO    = 1 << 3,
    DATA_PTR_NULL    = 1 << 4,
    CANARY1_DIED     = 1 << 5,
    CANARY2_DIED     = 1 << 6,
    CANARY3_DIED     = 1 << 7,
    CANARY4_DIED     = 1 << 8,
};

/*
if (error)
    status |= error_code;

000100101
|
110011110
=
110111111
*/

int main()
{
    Stack stk = {};

    ClearFile();

    StackCtor(&stk, 1);

    StackPush(&stk, 1);

    StackPush(&stk, 2);

    StackPush(&stk, 3);

    StackPop(&stk);

    StackPop(&stk);

    StackPop(&stk);

    StackPop(&stk);

    StackPop(&stk);

    StackPop(&stk);

    StackDtor(&stk);

    return 0;
}

void StackCtor(struct Stack* stk, int capacity)
{
    assert(stk);

    stk -> error_code = 0;

    stk -> data = (double*) calloc(capacity + 2, sizeof(*(stk -> data)));
    if(stk -> data == NULL) stk -> error_code |= DATA_PTR_NULL;

    stk -> capacity = capacity;
    if(stk -> capacity <= 0) stk -> error_code |= CAP_LESS_ZERO;

    stk -> size = 0; 

    stk -> canary1 = 0xBE31AB;
    stk -> canary2 = 0xBE31AB;

    *(int*)(stk -> data) = 0xBE31AB;
    *(int*)((char*)stk -> data + sizeof(*(stk -> data)) * (stk -> capacity + 1)) = 0xBE31AB;


}
// Разобраться полностью со структурами!!! vv

void StackPush(Stack* stk, double value)
{   
    assert(stk);
    StackOk(stk);

    if ((stk -> size) >= (stk -> capacity))
    {
        StackResize(stk);
    }

    stk -> data[++stk -> size] = value;
    StackOk(stk);
}

void StackDtor(Stack* stk)
{
    assert(stk);
    free(stk -> data);
}

void StackPop(Stack* stk)
{
    assert(stk);
    StackOk(stk);

    /*if(stk -> size <= 0)
    {
        //печатать ошибку ?
        stk -> size = ERRPTR; - создать enum и выводить коды ошибок -> нужно будет убрать assert // прочитать про enum
    }*/
    stk -> data[stk -> size--] = 0;
    StackOk(stk);
}

void StackResize(Stack* stk)
{   
    assert(stk);
    StackOk(stk);

    *(int*)((char*)stk -> data + sizeof(*(stk -> data)) * (stk -> capacity + 1)) = 0;

    stk -> data = (double*) realloc((double*) stk -> data, (2 * stk -> capacity + 2)* sizeof(*(stk -> data)));
    stk -> capacity = 2 * stk -> capacity;
 
    *(int*)((char*)stk -> data + sizeof(*(stk -> data)) * (stk -> capacity + 1)) = 0xBE31AB;

    StackOk(stk);
}

void StackDump(Stack* stk)
{   
    assert(stk);

    printf("\n------------------------------------------\n");

    printf("canary1 = %X\n", stk -> canary1);
    printf("canary2 = %X\n", stk -> canary2);

    printf("data = %p\n", stk -> data);
    printf("size = %d\n", stk -> size);
    printf("capacity = %d\n\n", stk -> capacity);

    printf("canary3 = %X\n", *(int*)(stk -> data));
    for(int i = 1; i <= (stk -> size); i++)
    {   
        printf("*[%d] = %.2lf\n", i, stk -> data[i]);
    }

    for(int i = stk -> size + 1; i <= (stk -> capacity); i++)
    {   
        printf(" [%d] = %.2lf\n", i, stk -> data[i]);
    }
    printf("canary4 = %X\n", *(int*)((char*)stk -> data + sizeof(*(stk -> data)) * (stk -> capacity + 1)));
    printf("------------------------------------------\n");
}

void StackFDump(Stack* stk)
{   
    FILE* fdump = NULL;
    fdump = fopen("FDUMP.txt", "a");


    fprintf(fdump, "\n------------------------------------------\n");

    DumpERrors(stk, fdump);

    fprintf(fdump, "ERror_status = %d\n", stk -> error_code);

    fprintf(fdump, "canary1 = %X\n", stk -> canary1);
    fprintf(fdump, "canary2 = %X\n", stk -> canary2);

    fprintf(fdump, "data = %p\n", stk -> data);
    fprintf(fdump, "size = %d\n", stk -> size);
    fprintf(fdump, "capacity = %d\n\n", stk -> capacity);


    fprintf(fdump, "canary3 = %X\n", *(int*)(stk -> data));
    for(int i = 1; i <= (stk -> size); i++)
    {   
        fprintf(fdump, "*[%d] = %.2lf\n", i, stk -> data[i]);
    }

    for(int i = stk -> size + 1; i <= (stk -> capacity); i++)
    {   
        fprintf(fdump, " [%d] = %.2lf\n", i, stk -> data[i]);
    }
    fprintf(fdump, "canary4 = %X\n", *(int*)((char*)stk -> data + sizeof(*(stk -> data)) * (stk -> capacity + 1)));
    fprintf(fdump, "------------------------------------------\n");

    fclose(fdump);
}

void ClearFile()
{
    FILE* fdump  = NULL;
    fdump = fopen("FDUMP.txt", "w");
    fclose(fdump);
}

int StackVerify(Stack* stk)
{
    if(stk -> canary1 != 0xBE31AB)       stk -> error_code |= CANARY1_DIED;
    if(stk -> canary2 != 0xBE31AB)       stk -> error_code |= CANARY2_DIED;
    if(stk -> data == NULL)              stk -> error_code |= DATA_PTR_NULL;
    if(stk -> size < 0)                  stk -> error_code |= SIZE_LESS_ZERO;
    if(stk -> capacity < 0)              stk -> error_code |= CAP_LESS_ZERO;
    if(stk -> size > stk -> capacity)    stk -> error_code |= STACK_OVERFLOW;
    if(*(int*)(stk -> data) != 0xBE31AB) stk -> error_code |= CANARY3_DIED;
    if(*(int*)((char*)stk -> data + sizeof(*(stk -> data)) * (stk -> capacity + 1)) != 0xBE31AB) stk -> error_code |= CANARY4_DIED;

    return(stk -> error_code);
}

void StackOk(Stack* stk)
{
    if (StackVerify(stk))
    {
        StackFDump(stk);
        exit(EXIT_FAILURE);
    }
}

void DumpERrors(Stack* stk, FILE* fdump)
{
    if(stk -> error_code & STACK_OVERFLOW)
    {
        fprintf(fdump, "STACK_OVERFLOW\n");
    }
    if(stk -> error_code & SIZE_LESS_ZERO)
    {
        fprintf(fdump, "SIZE_LESS_ZERO\n");
    }
    if(stk -> error_code & CAP_LESS_ZERO)
    {
        fprintf(fdump, "CAP_LESS_ZERO\n");
    }
    if(stk -> error_code & DATA_PTR_NULL)
    {
        fprintf(fdump, "DATA_PTR_NULL\n");
    }
    if(stk -> error_code & CANARY1_DIED)
    {
        fprintf(fdump, "CANARY1_DIED\n");
    }
    if(stk -> error_code & CANARY2_DIED)
    {
        fprintf(fdump, "CANARY2_DIED\n");
    }
    if(stk -> error_code & CANARY3_DIED)
    {
        fprintf(fdump, "CANARY3_DIED\n");
    }
    if(stk -> error_code & CANARY4_DIED)
    {
        fprintf(fdump, "CANARY4_DIED\n");
    }
}




//плохо работает realloc
//...
//очень плохо // оказалось, что изменение памяти плохо работает YA_YBLAN там было указано в байтах -> надо было умножить еще на sizeof

// прочитать еще раз про realloc действительно vv



// написать dump - вывод данных о стеке vv

// прочитать про size_t vv
// прочитать про typedef vv
// прочитать про realloc  vv
// прочитать про enum vv


// StackOK() !!

// канарейки !!

// хэш !!