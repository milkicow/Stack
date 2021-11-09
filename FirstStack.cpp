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

bool StackOk(Stack* stk);

void StackFDump(Stack* stk);

struct Stack
{   
    int canary1;
    double* data;
    int size;
    int capacity;
    int canary2;
};

enum StackERrors
{
    STACK_PTR_NULL  = 1 << 1,
    STACK_OVERFLOW  = 1 << 2,
    STACK_UNDERFLOW = 1 << 3,
    SIZE_BIGGER_CAP = 1 << 4,
    SIZE_LESS_ZERO  = 1 << 5,
    CAP_LESS_ZERO   = 1 << 6,
    DATA_PTR_NULL   = 1 << 7,
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

    StackCtor(&stk, 1);

    StackFDump(&stk);

    StackPush(&stk, 1);
    StackFDump(&stk);

    StackPush(&stk, 2);
    StackFDump(&stk);

    StackPush(&stk, 3);
    StackFDump(&stk);

    StackPop(&stk);
    StackFDump(&stk);

    StackPop(&stk);
    StackFDump(&stk);

    StackPop(&stk);
    StackFDump(&stk);



    StackDtor(&stk);

    return 0;
}

void StackCtor(struct Stack* stk, int capacity)
{
    assert(stk);
    assert(capacity);

    stk -> data = (double*) calloc(capacity + 2, sizeof(*(stk -> data)));
    stk -> capacity = capacity;
    stk -> size = 0; //проверить // указывает на первую пустую ячейку или на последнюю заполненную ?? if num of last - rewrite >= 1 func 

    stk -> canary1 = 0xBE31AB;
    stk -> canary2 = 0xBE31AB;

    *(int*)(stk -> data) = 0xBE31AB;
    *(int*)((char*)stk -> data + sizeof(*(stk -> data)) * (stk -> capacity + 1)) = 0xBE31AB;
}
// Разобраться полностью со структурами!!! vv

void StackPush(Stack* stk, double value)
{   
    assert(stk);

    if ((stk -> size) >= (stk -> capacity))
    {
        StackResize(stk);
    }

    stk -> data[++stk -> size] = value;
}

void StackDtor(Stack* stk)
{
    assert(stk);
    free(stk -> data);
    //stk -> data = ERRPTR; - создать enum и выводить коды ошибок // прочитать про enum
}

void StackPop(Stack* stk)
{
    assert(stk);
    assert(stk -> size);

    /*if(stk -> size <= 0)
    {
        //печатать ошибку ?
        stk -> size = ERRPTR; - создать enum и выводить коды ошибок -> нужно будет убрать assert // прочитать про enum
    }*/
    stk -> data[stk -> size--] = 0;
}

void StackResize(Stack* stk)
{
    assert(stk);

    *(int*)((char*)stk -> data + sizeof(*(stk -> data)) * (stk -> capacity + 1)) = 0;

    stk -> data = (double*) realloc((double*) stk -> data, (2 * stk -> capacity + 2)* sizeof(*(stk -> data)));
    stk -> capacity = 2 * stk -> capacity;
 
    *(int*)((char*)stk -> data + sizeof(*(stk -> data)) * (stk -> capacity + 1)) = 0xBE31AB;

    assert(stk -> data);
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

bool StackOk(Stack* stk)
{

}

void StackFDump(Stack* stk)
{   
    
    FILE* fdump = NULL;
    fdump = fopen("FDUMP.txt", "a");

    fprintf(fdump, "\n------------------------------------------\n");

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