/* main.h */
#define TEMPLATE_MAX(T)                                                        \
    T Max_##T(T a, T b) { return a > b ? a : b; }

/* main.c */

/* Instantiating the functions for specific types */
TEMPLATE_MAX(int)
TEMPLATE_MAX(double)

int Question21C()
{
    int a = Max_int(5, 6);
    float b = Max_double(5.5, 6.5);
    (void)a;
    (void)b;

    return 0;
}

int main()
{
    Question21C();
    return 0;
}
