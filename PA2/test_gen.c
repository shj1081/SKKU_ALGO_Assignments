// code for random string consist of A,T,G,C

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// function to generate random string of length n with A,T,G,C
void generate(int n)
{
    int i;
    char str[121];
    for (i = 0; i < n; i++)
    {
        int num = rand() % 4;
        if (num == 0)
            str[i] = 'A';
        else if (num == 1)
            str[i] = 'T';
        else if (num == 2)
            str[i] = 'G';
        else
            str[i] = 'C';
    }
    printf("%s\n", str);
}

int main()
{
    srand(time(0));
    printf("%d\n$\n",5);
    generate(120);
    generate(120);
    generate(120);
    generate(120);
    generate(120);
    return 0;
}
