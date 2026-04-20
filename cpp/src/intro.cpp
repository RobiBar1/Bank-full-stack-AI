#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>
#include <iostream>
#include <typeinfo>

using namespace std;

typedef struct xy
{
    int x;
} x;

static void FirstQuestion(void)
{
    const int i = 5;
    int const i2 = 3;
    int const* i3 = NULL;
    x* const s = {0};
    x const s1 = {0};
    const x* s2 = {0};
    const x** const s3 = {0};
    int const* (*const s4)(int*) = {0};

    cout << typeid(i).name() << endl;
    cout << typeid(i2).name() << endl;
    cout << typeid(i3).name() << endl;
    cout << typeid(s).name() << endl;
    cout << typeid(s1).name() << endl;
    cout << typeid(s2).name() << endl;
    cout << typeid(s3).name() << endl;
    cout << typeid(s4).name() << endl;
}

enum E
{
    AA,
    BB,
    CC,
    DD
};
enum E2
{
    TT,
    KK
};

static void SecondQuestion(void)
{
    /*
    a.

    in c enum its int and in c++ its type from its own.

    enum E e = 2;
    enum E2 e2 = AA;
    //  E2 e22 = KK;
    ++e;
    int i = e;

    cout << e << endl;
    cout << e2 << endl;


    /*
    b.in c its will get garbge val and in c++ its will be 0(
         problly active default constartcor that init it)
    */

    enum E e;
    enum E2 e2;
    //  E2 e22 = KK;
    //++e;
    // int i = e;

    cout << e << endl;
    cout << e2 << endl;

    /*
    c.
    1. in c if print val that didnt init i will get garbge.
    2. in c++ its the same.

    so need to init it before use for wont get random output.


    */
}

static void QuestionThree(void)
{
    /*
        a. its 1 byte. and this is the standart but not guaranteed.
        b. its garbge val.
        c. the implications is that need init bool var before use him for not
       get random val.
       d.
       int into char: will take the byte(left or right byte difnce on liitle/big
       endian). int into bool: will ask if that int is == 0, and only if all 4
       bytes is 0 its will put 0, else it will put 1
    */
    bool b1 = false;
    bool b2;
    bool b3;
    int i = b1;
    i = 7;
    b2 = i;
    char a = i;
    printf("%d\n", b3);
}

typedef struct List List_t;
typedef struct Person Person_t;

struct List
{
    int x;
    void* a;
};

struct nothing
{
    int age;
    int key;
    long x;
};

struct Person
{
    int age;
    int key;
    long x;
};

static void* ll_find(List_t* ll, int key) { return ll->a; }

void QuestionFourA(List_t* ll, int key, int age)
{
    void* vv = ll_find(ll, key);
    Person_t* p = (Person_t*)ll;
    p->age = age;
    p->x = 50;
    printf("%d\n", p->age);
}

void QuestionFourC(List_t* ll, int key, int age)
{
    void* vv = ll_find(ll, key);
    Person_t* p = static_cast<Person_t*>(vv);
    p->age = age;
    printf("%d\n", p->age);
}

static void ActiveQuestionFour(void)
{
    Person_t p = {10, 20, 40};
    struct nothing a = {10, 20, 40};
    List_t ll = {30, &p};

    /*QuestionFourA(&ll, 0, 15);*/
    /*
    question 4:
    a. so even we enter nothing struct into the list it keep overide on his
    address and may curapt his memory.

    b. becuase we do explicit cast and he "trust" us we know what we are doing.
    */

    QuestionFourC(&ll, 0, 15);
    /*
    c. we change the code.

    d. static_cast its opertaror and its doing static cast in compile time and
    check if cast is allowed and ok, how he will know if allowd?
    1. will check if A is son/father of B of not its not allowed.
    2. Built-in Conversions from premetive types.

    e.
    1. const_cast - it for make const varible to not const.
    2. reinterpret_cast:
        * cast pointer to num of his adress for do math on the adress itself.
        * casting pointer from 1 type to another(e.g *dog to *student).
    f.
    static_cast:
    type - the type of what we casting to.
    value - return the val that we casting to.
    side-effect -
    1. data lost, if i cast double to int..
    2. if we ovveride the casting opertator, it will active that function that
    can do maybe print for exmaple so the print is side effect

    (cast_to)x:
    type - the type of what we casting to.
    value - return the val that we casting to.
    side-effect -
    1. data lost, if i cast double to int..

    the diffrent between them is that static_cast give me 100% the type i want
    and the other one is may be runtime error.

    g.yes we can, becuase its support legecy and in this cases:
    1. when we want to do our code more readable(in real world almost never use
    it).

    h. what is function like cast? -> int(x); and will you use it:
    mostly in function parameters if i want to imporve readable.


    */
}

/*
Question 5:
a. diffrent in behiver is:
- in c the compiler see that have no foo() function but its ok for him and then
the linker will draw an link error.
- in c++ the compiler see that have no foo() function AND its stop it, he say "i
want to know about all functions"

b. explain each word in the warning got from c
"implicit declaration of function ‘foo’":
- implicit: mean i dont see where is it.
- declaration: the declare line/.

c.its safe to use this calling undeclared functions:
1. when we use "base" language librerris and the linker know where to serch them
2. when we use librires/ shared librures and mark the linker(static/dynamic)
wherer to find the functions.

d. write 2 examples that prove the danger of alling undeclared functions:
1. if i call function name foo() and i want he will run foo() from home/ds/foo()
then linker go serach and find other foo() in home/foo() and think its my
function.
2. if i call to function in shared library and she exist then i update the
library and delete the function from there then the exute will run and when try
to dlopen(explicit call) the function wont work and the sevirce wont work.
*/

/*
Question 6:
a. what are advantages of be able to defined variables anywhere in a block:
1. Read-ability: can see it near the place i use it.
2. we can declare about it in for scope and it will auto delete when we go out
of scope.

b. what bugs can we prevent and never see again thnks to this change:
1. uninitialized variables.

c. declare about them near as posible to the use of them.

d. devide it to 2:
1. with compilre optimize its even better.
2. with-out compilre optimize its a bit slower.

anyway its more maintainabily, and bug prevent.
*/

/*====================== Start Question 7 ======================*/

static void QuestionSeven(void)
{
    int* ptr = (int*)malloc(1000 * sizeof(int));
    delete ptr;
    int* ptr2 = new int[1000];
    free(ptr2);
}
/*
Question 7:
a. how new/delete is opertators prevent bugs(common with malloc/free):
1. MALLOC return void* then i need to do casting, if i do wrong casting the
compiler wont tell me about it.
so NEW auto return the pointer to type he just create.

2. you dont need to give size to NEW so wont be size bug.

3. malloc wont call constructor and NEW will.

b.
type - will give pointer as the type we want.
value - address to the start of the allocate memory.
side-effect:
1. alloc memory
2. call the constructor relevant type.
3. if he fail draws expetions?

c. yes its safe. and good if we want del object that is null so we dont need to
check if he is null or not.

d. yes its safe to do free to 0 or to NULL.

e. undefined behaver, if do it on Objects its will probbly crash(becuase will
active destructor and didnt active constructor)

*/

/*====================== End Question 7 ======================*/

/*====================== Start Question 12 ======================*/
static void Question12(void)
{
    /*
    void Foo() {}

    void Foo(int i) {};
    void Foo(char i) {};

    unsigned char y = 5;
    float x = 5;
    float* p = &x;
    int i = 0;
    short s = 0;
    Foo(&i);
    Foo(s);
    Foo(x);
    Foo(y);
    Foo(p);
    Foo(NULL);
    */

    void* handler = dlopen("./shaerd.so", RTLD_LAZY);
    int (*hey)() = NULL;
    *(void**)&hey = dlsym(handler, "_Z3heyv");
    hey();
}

/*
b. what is good for: more readable, flexable.
what are the dangers: type ambiguity we can call function foo() and pass int,
but have 2 foo declare functions 1 expect to get double and 1 expect to get long
so we dont know what the implicit convert will be into and what will active
becuase of it.

c. its cant convert its to anyone this is the problems:
float: can be 2 of them(int&char) so he cant deside where to go.
NULL: can be 2 of them(int&char) so he cant deside where to go.
pointer: cant be anyone of them.

e. in c++ when create shaerd object have problem to explicitly load it, becuase
the linker search the name i gave him(hey) and the the symbol table have other
names(e.g Z3heyi)..

g. how we solve the problem:
I saw the real name of then function in symbol table with nm and called her with
dlsym.

better solve it will be with factory desing pattern but even there we will use
the exetrn c for the entery point function
*/

/*====================== End Question 12 ======================*/

/*
====================== Start Question 13: =====================
*/

namespace useless
{
unsigned int g_wasteful[400];
extern int g_notthere;
void Foo() {}

namespace wasteoftyme
{
void Foo() {}
} // namespace wasteoftyme
} // namespace useless

namespace stupid
{
void Foo() {}
void Bar() {}
void DoNothing(unsigned int) {}
} // namespace stupid

namespace useless
{
void DoNothing(int) {}
} // namespace useless

using namespace useless;

void DoStuff()
{
    stupid::Bar();
    Foo();
    using stupid::Foo;
    Foo();
    DoNothing(g_wasteful[3] + 1);
}

void Foo() {}

using namespace stupid;

namespace comeon = useless::wasteoftyme;

void DoMoreStuff()
{
    comeon::Foo();
    // Foo(); //try uncommenting this line, solve the error
    Bar(); // why doesn't this line create an error?
    DoNothing(g_wasteful[3] + 1);
}

namespace useless
{
void DoUselessStuff() { DoNothing(g_wasteful[3] + 1); }
} // namespace useless

/*
namespace - space that in him all the names are uniqs and are acceable from out
side.

a. in the block from the line you declere on him.
b.
* labels - goto.
* tags: names of structs, enums, union.
* members of tags: the varibles inside the tags.
* all the rest: local varibels in functions, globals varibles in file, names of
functions.

c. when give same name for varibles in my scope and outer scope.
name space help solve it by be able to call specific x from "full path".

d. give options to use elements of namespace without explicitly writing the
space-name each time.

e. never do it! becuase the pre-proccessor will paste in the the client code and
will force him to use it.

f - i: take the closest name-space. but if the close namespace fuction will get
void and i pass int it will serch farther if have function that get int, and if
find will the take the first he find.

j. never do define in namespaces for pre-proccessor wont replace it with other
user same name varible and will make undefind behavor.

======================= End Question 13 =====================
*/

/*
======================= Start Question 14: =====================
*/

static void Question14(void)
{
    const int i = 3;
    int* ip = (int*)&i;
    *ip = 5;
    int arr[i];

    printf("%d %d, %d\n", i, *ip, sizeof(arr));
}

/*

a. c++ see const as define and go and copy 3 to where he see i, then its prints
3,5 in first print. but if i make i to "const volatile int i = 3;" so the
compiler wont do optimize on the i and will read it in run time so the print
will be 5, 5.

b-c. so if we get the const i decalre to global its cant be change, it will dorp
seg fualt becuase he will sit on row data.
and if we either declare nomral global varible and then do nm we will see:
* the const global will be the static symbol table and he will get mangeling.
but he wont apper in dynamic symblo table becuase c++ make its static (if we
want to see it there we will do exetrn to this but then we will see that he dont
get mangeling).
* and the normal global will be in the symbol table the static and the dynamic
and will not get mangeling.

d. in c we cant do it becuase he dont know the i size in compile time but in c++
we can do it because in compile time he will rpelace the i with the literal(as
define) so he know the size of arr in comppile time so its allowd.

e. stop use define becuase:
1. able to debug.
2. type checking.
3. live only in the scope you decalre(read-able, more safe etc..).
======================= End Question 14 =====================
*/

/*
======================= Start Question 15: =====================
*/

inline void foo15()
{
    cout << "Hello";
    cout << "Hello";
}

static void Question15(void) { foo15(); }

/*
======================= End Question 15 =====================
*/

/*
======================= Start Question 16: =====================
*/

template <typename T> T Max(T t1, T t2) { (t1 > t2) ? t1 : t2; }

void Question16()
{
    int arr[12];

    cout << Max<int>(3, 5) << " " << Max<double>(3.7, 12.9) << " ";
    cout << Max<int*>(arr, arr + 8) << endl;
    cout << Max('a', 'b') << endl;
}

/*
a. if not call its not exist in symbole table.
b. if call its in the symbol table while do ovverloading for each diffrent call
type.

c. will be weak symbol in the table so will take 1 of them.

d.he will try do implicit recognize wich funcion to call only if have
decide-able wich function to call it will call it, else it compile error.
then we make this Max('a', 'b') to expliti -> Max<int>('a', 'b'), now he will
try to convert the chars to int if he will success its ok else its compile time.

e.
will compile(its do syntax but not semntic). but if we call the function in
compile time he will inject in the last line of function exeption("Illegal
instruction (core dumped)") that will run in run time when we will be on this
line.

f. need to support the relevants operators we use in the function on the T
parameters.

g. compiler check it and act accorindly if we call it or not.
======================= End Question 16 =====================
*/

/*
======================= Start Question 17: =====================
*/

template <typename T> void foo(T t)
{
    std::cout << t << " Generic foo for " << typeid(t).name() << std::endl;
}

template <> void foo<int>(int t)
{
    std::cout << t << " Specialized foo (for int param)\n";
}

void foo(int t) { std::cout << t << " test dummy\n"; }

void Question17()
{
    foo(4.2);
    foo<int>(5);
    foo(5);
}

/*


======================= End Question 17 =====================
*/

/*
======================= Start Question 18: =====================
*/

struct X18
{
    int x;
    int& y;
};

int& foo18(int& i)
{
    int j = 11;
    int& ip = j;
    i = 10;
    return ip;
}

void Question18()
{
    int i = 0;
    int& ip = i;

    ip = 9;
    int& ip2 = foo18(i);

    struct X18 some = {0, i};
    struct X18& somep = some;

    cout << i << endl;
    cout << ip2 << endl;
    cout << sizeof(ip) << endl;
    cout << &i << " " << &ip << endl;
    cout << some.x << " " << some.y << endl;
}

/*
======================= End Question 18 =====================
*/

/*
======================= Start Question 19 =====================
*/

void Foo19(const double& d)
{
    cout << d << endl;
    cout << &d << endl;
}
void Bar19(double& d)
{

    cout << d << endl;
    cout << &d << endl;
}

static void Question19(void)
{
    int a = 5;
    Foo19(a);
    Bar19(reinterpret_cast<double&>(a));

    cout << a << endl;
    cout << &a << endl;
}

/*
======================= End Question 19 =====================
*/

/*
======================= Start Question 20 =====================
*/

struct X
{
    explicit X(); // Initialization function without parameters is named
                  // default
    //  constructor (often abbreviated as: default ctor). */
    explicit X(int a_, int b_ = 8); // Regular (non default) Ctor.
    ~X(); // Deinitialization function is called destructor (Dtor).
    X(const X& other_); // Copy initialization function is called copy
                        // constructor or CCtor.
    X& operator=(const X& other_); // Assignment operator.

    int m_a;
    const int m_b;
};

X::X()
    : m_a(3), m_b(4) // Implementation of the default Ctor. Why the X::X? The
                     // code after the colon is called an initialization list.
{
    // m_a = 3;
    // m_b = 4;
    std::cout << "this: " << this << " X default Ctor. m_a: " << m_a
              << " m_b: " << m_b << std::endl;
}

X::X(int a_, int b_) : m_a(a_), m_b(b_) // Implementation of the second Ctor.
{
    std::cout << "this: " << this << " X int int Ctor. m_a: " << m_a
              << " m_b: " << m_b << std::endl;
}

X::X(const X& other_)
    : m_a(other_.m_a), m_b(other_.m_b) // Implementation of the copy Ctor.
{
    std::cout << "this: " << this << " X copy Ctor. m_a: " << m_a
              << " m_b: " << m_b << std::endl;
}

X& X::operator=(const X& other_) ///
{
    m_a = other_.m_a;
    // m_b = other_.m_b;
    std::cout << "this: " << this << " X assignment operator. m_a: " << m_a
              << " does not change m_b: " << m_b << std::endl;
    return *this;
}

X::~X()
{
    std::cout << "this: " << this << " X Dtor. m_a: " << m_a << " m_b: " << m_b
              << std::endl;
}

void Question20()
{
    /*X   x1;
    X x2(7);
    X* px = new X(x2);
    X x3(9, 10);
    X x4(x1);

    x1 = x3;

    delete px;
    px = 0; */
}

/*

explicit mean that i most call this constractor explitily or i ask demend the
compiler to not call it implicit.
======================= End Question 20 =====================
*/

/*
======================= Start Question 21: =====================
*/

void Question21()
{
    /*  X x1;
     X x2(7);
     X* px = new X(x2);
     X x3(9, 10);
     X x4(x1);

     x1 = x3;

     delete px;
     px = 0;
     cout << "21" << endl; */

    /*  X* xp = new X[10];
     delete[] xp; */
}

/*

a. from the Dctor its from the end to start as allways.

b. wont work and cuppart the heap.

c. no, its wont call the Ctor and Dtor. its ok its work like this becuase its
premetive variables and no Ctor or Dtor to active. but we cant trust it because
its undefinde behavor in c++ its may work or may be memoery leak so even of new
double arr do delete[] x.

d. i can manage without the new and delete without the []-> allwyas use [].
the disadvnace of use [1] will be more overhead in meta data of count how much
Dtor to active in end.

e. the defulat constarctor.
======================= End Question 21 =====================
*/

/*
======================= Start Question 22: =====================
*/

struct Y
{
    X m_x;
    int m_i;
};

void Question22(void)
{
    /*
    X x1;
    X x2(7);
    X* px = new X(x2);
    X x3(9, 10);
    X x4(x1);

    // x1 = x3;

    delete px;
    px = 0;

    cout << "21" << endl;
    X* xp = new X[10];
    delete[] xp;

    cout << "22" << endl;
    Y y1;
    y1.m_x.m_a = 250;
    Y y2(y1);
    Y y3;
    */

    // y3 = y1;
}

/*
a - b. we get (7 but 3 not for now)4 things:
1. defult Ctor. (C1&C2)
2. Cpy Ctor. (C1&C2)
3. Assingmet operator.
4. Dctor. (C1&C2)

c. lazy creation, create the function by demend.

d. by demend(if call y somewhere so defult constactors, if copy y1 to y2 so copy
constractor).

e. X now dont have defult cCtor becuase we implement Ctor to X that override the
defulat Ctor of X.

f. two options to make it work without our impeelmts for the duaflt X Ctor:
1. override the Y defult Ctor to call the other Ctor of X that we implement:
Y::Y() : m_x(3, 4) {}

2. to un implement the other Ctor we make in X that the compiler will create
default Ctor in compile time.

g. the problem begin in this line "y3 = y1;" where he serach the Y '=' operator
then he create and use his defult operator wich called X::X= operator thenn
becuase we deleted our implemention of X= operator the compiler create defult
operator function and the in run time he call her.
then becuase the X struct have const variable that already get value when we
used defualt Ctor so we cant now change him with "x_m = something" that happned
in the defult opertator function and this is error.

h. yes, we will create this:
"Y& Y::operator=(const Y& other_)
{
    m_x.m_a = other_.m_x.m_a;
    m_i = other_.m_i;

    std::cout << "done opertato" << std::endl;

    return *this;
}"

but if I should use it? the answer is no, becuase:
1. maybe I dont know how X is implement for go into his private varibles.
2. even if i know its bad-practive and may have bugs.

======================= End Question 22 =====================
*/

/*
======================= Start Question 23: =====================
*/

static void Question23(void) {}

/*
a.
1. defult Ctor.
2. Cpy Ctor.
3. this operators:
*  '='
4. Dtor.

advance, 2 more functions is able to be overloading, operator:
1. '&'.
2. ','.

c. yes, all of them, if we dont use them.

d. in each file that have use them.

e. in the linker can be multiple defention and then it draw link error.
- how this problems will be solved: they get Weak symbol and the linker choose
random function.
======================= End Question 23 =====================
*/

/*
======================= Start Question 24 =====================
*/

struct X24
{
    X24(int a_, int b_);

    void Inc(); /*equal to-> void (*Inc)(X24* me); but member function is static
                   and dont take place on memory*/
    void Dec();

    int m_a;
    int m_b;
};

X24::X24(int a_, int b_) : m_a(a_), m_b(b_) {}

void X24::Inc()
{
    ++m_a;
    ++m_b;
    cout << "this: " << this << endl;
}

void X24::Dec()
{
    --m_a;
    --m_b;
    cout << "this: " << this << endl;
}

void Inc() {}

static void Print(int a, int b) { cout << a << " and " << b << endl; }

static void Question24(void)
{
    X24 x1(7, -55);
    X24 x2(x1);

    Inc();
    x1.Inc();
    x1.Inc();
    cout << "x1 " << &x1 << endl;
    x2.Dec();
    cout << "x2 " << &x2 << endl;
}

/*
c. cant cos dont know the X24 struct.

d. that the member function dont need to be init, as I should do if i dont use
it as member and that member function dont incrise place in memory of struct.

e. its dont effect the size, why?
 becuase the function call if swap with the adress of function in compile time
and this is become static call so i cant point with that function to any other
function.

f. the variables of x2, becouse its implicit write:"--(this.m_a)" and same on
the other variables.

g. X24* const

h. its the adress of the x1 and x2.

i. we use "this " isnide member functions that is not static functions. we will
use it as const pointer to my strcut.

j. if i want to do things on my struct or use things inside him.

k. compiler put it there implicity for each member function that is not static.

l. no, its rvalue or const pointer.

m. in our struct its X24* const, if the member function was static so it was
const X24* const.
======================= End Question 24 =====================
*/

/*
======================= Start Question 25: =====================
*/

struct X25
{
    explicit X25(int);
    ~X25();
    void Foo();
    void Bar() const;

    int m_a;
    int* m_p;
};

X25::X25(int a_) : m_a(a_), m_p(new int(a_)) {}

X25::~X25()
{
    delete m_p;
    m_p = 0;
}

void X25::Foo()
{
    ++m_a;
    --(*m_p);
}

void X25::Bar() const
{
    std::cout << m_a << std::endl;
    std::cout << *m_p << std::endl;
    std::cout << m_p << std::endl;

    // m_a = 0; //---1---
    // m_p = 0; //---2---
    // *m_p = 0; //---3---
    // Foo(); //---5---
}

void Fifi(const X25& x_)
{
    // x_.Foo(); //---4---
    x_.Bar();
}

static void Question25(void)
{
    X25 x1(1);

    x1.Foo();
    Fifi(x1);
}

/*
a - b. its mean that all the fields in the struct are consts.

c. 2 options to const pointers in c:
1. int* const x; -> its mean that this pointer will point allways on the same
memory adress.

2. const int* x; -> that mean that we cant change the values inside the memory
adress/arr/etc...

d. ill do it line by line, in general in const struct we cant change his fields:
1. m_a = 0; -> cant be change.

2. m_p = 0; -> cant be change.

3. *m_p = 0; -> can be change, why? becouse the const is on pointer so we can
change what in it.

4. x_.Foo(); -> this call is in this function: "void Fifi(const X25& x_)", so
the const is on the X25 so its make it const, so i cant active member function
of him that isnt const.

5. Foo(); -> this call is in this function: "void X25::Bar() const", so const is
on the Bar function so i cant call in it to function that not const.

e. becuase the const is on the pointer so cant change the adress he point on but
CAN change the value isnide him.

f. its make the function const so:
1. i cant call any function inside it that isnt a const function.
2. i cant change any member of the struct that function is its function(NOTE
that i can change variable that isnt a member function, e.g if that function get
int i parameter and change the i in it so it will work).

g. const X25* const.

h. becuase it now make sure that i wont do any change of my strct(X25 in this
case).

i. if the memeber funcion/funcion isnt change anything of member variables so
make it static.

j. about pointers i dont pass pointer by value only by refernce(for wont create
2 pointers to same place in memory then if ill free 1 the second pointer will
keep point to there and it will be dengeling pointer)so we have the 3 rule that
say if i oveerside 1 of the next things i need to do it to all of them:

1. Copy Ctor - create new pointer that point to the same value but on difreent
adress.

2. Assingment operator.
3. Dctor.
======================= End Question 25 =====================
*/

/*
======================= Start Question 26: =====================
*/

class X26
{
  public:
    explicit X26(int a_);
    void Foo26();
    int GetA() const;
    void SetA(int new_a);

    unsigned char m_a;
    void Bar26();
};

X26::X26(int a_) : m_a(a_) {}

void X26::Foo26()
{ /*printf("%d\n", m_a);*/
    printf("%d\n", GetA());
    Bar26();
}

int X26::GetA() const { return m_a; }

void X26::SetA(int new_a) { m_a = new_a; }

void X26::Bar26() { printf("Shahar is something something\n"); }

void Foo26(const X26& x_) { /*printf("%d\n", x_.m_a);*/
                            printf("%d\n", x_.GetA()); }

void Question26(void)
{
    X26 x1(455);

    x1.Foo26();
    // printf("%d\n", x1.m_a);
    Foo26(x1);
}

/*
a.
1. printf("%d\n", x1.m_a) -> x1.m_a is privte so we cant access him directly.
2. Defined a constructor that recives argumet, no default constructrer.

b. the member is now public and can be accessed from the main.

c. Getters and setters

d. from inside the struct's methods

e. just use getters and setters for all mambers variables and then it will
influnce this.

f. need to change all the functions that use m_a (including functions not in the
struct)

g. Encapsulation

h. can achive it only for other files that try to get my inf, we used typedef in
the h file and with that we didnt expose the implemention. but in my file if i
create struct and function know that struct i cant deny the access to his
fields.

i. Becuase it is still accessible from the same file/compilation unit.

j. the members are public by default

k. put the unwanted generated function under the private access modifier and
don't implement it or cll it from inside the struct

l. to decide which functions the compiler will generate for us(so the functions
i dont want he will generate for us i will put in private).

m. nothing

n. if we remove the public keyword, the file won't compile because the default
access modifier in a class for members is "private"

o. if we remove the private keyword, nothing changes becuase it is already
private by default

p.
1. class access modifer is private by default and struct is public by default.
2. access inheritance defaults like 1

q. For backward compatibility with the C language.

r. So that other programmers using the class can immediately see the interface -
what the class can do and how to use it. The implementation details (the
information in private) are less relevant to the external user and therefore
appear at the end.

======================= End Question 26 =====================
*/

/*
====================== Start Question 27: =====================
*/

void Question27(void) {}

/*
keywords:
1. Wrapper class:
can use to boxing usully premative type but can be use either for non-premative
type. why its good:
* make easiter interface.
* make more safe interface(can do function to check things that can be failed).
* make automate resource mangemengt(becuose when you active calss its granutedd
that will active Ctor and Dtor).
disadvantges:
* overhead(for example: we create class for use premetive..).
* more complex to implement(becuase we need to make that wrap class).

2. Resource managmenent class:
its implement RAII(Resource Acquisition Is Initialization) principle the target
is to make the life cycle(create until destory) of class in our system be
nearest much as posible to the time we acctuley use it(make him create when we
start to use, and 1 line after the last use he will delete from our system).

3. Rule of three:
oned you implement one of them you most implement all of three:
* Dtor.
* Copy-Ctor.
* assingment operator.

when we will most implement this functions, when we manement any resource that
we responsable on this alloc and dealloc of memory or use(pointers, files, I/O).

-----Questions:
1. why need this implement of String and not just use char*?
becuase all the advandtges of Wrapper class(encapsuletion, more funcecanulity,
more readyablity, more safety of memory use.)

2. when should the cTor be explicit?
* as a defualt allways with 1 argunemt for will be explit when the user want to
create an class with that number and when not for wont be undefind behavor.
* i wont use it when the logic of the paramter is the logic that use in the Ctor
anyway.

3. what are the compiler-genereated functions:
* Ctor
* Copy-Ctor
* assignemt operator.
* Dtor

4. what is the difference between member and free functions:
* free functions cant access to private members unless they friends.
* members functions i get automate my class as this as the left argument.
* members functions the left operator must be the class that i put the data into
him.
* all the members functions are weak symbols and the out functions is strong(if
get imeplemtns).

5. in defualt yes, when my function dont change members varibles.
* why we want to do it: so other const types that demend to function be const
for use them(e.g other const class or other const function) to be able use that
function.

6. ?

7.why do we need to implement dtor, cctor, assingme operator and not use the
compiler generated version?
* becuase the "rule-of-tree", we have here pointer to char.

8. explain the diffrence between using strcpy vs strlen + memcpy?
* strcpy pass on the string and copy it while it pass on him.
* strlen + memcpy, will run 1 time on the string for count then run again to cpy
the bits.


API:
namespace ilrd
{
    class String
    {
    public:
        String(const char* s);
        explicit String(const String& s);
        ~String();

        std::size_t Length() const;
        const char* Cstr() const; //getter

        String& operator=(const String& other);
        bool operator==(const String& other) const;
        bool operator>(const String& other) const;
        bool operator<(const String& other) const;

        private:
            char* m_cstr;
    };

    std::ostream& operator<<(std::ostream& os, const String& str);
    bool operator==(const char* left, const String& right);

}
====================== End Question 27 =====================
*/

/*
====================== Start Question 28: =====================
*/
using namespace std;

class X28;

int operator+(const X28& x1_, const X28& x2_) { return 7; }

// ostream& operator<<(ostream& os_, const X28& x_);

class X28
{
  public:
    bool operator==(const X28& o_) const { return m_a == o_.m_a; }
    // Java style inline - not allowed in our coding standard
  private:
    friend ostream& operator<<(ostream& os_, const X28& x_);
    int m_a;
};

void Question28()
{
    X28 x1;
    X28 x2;

    cout << " x1+x2:" << x1 + x2 << " x1==x2:" << (x1 == x2) << " x1:" << x1
         << " x2:" << x2 << endl;
}

ostream& operator<<(ostream& os_, const X28& x_) { return os_ << x_.m_a; }

/*
a. did

b. The differences are:
1.
* Member operator: arguments (member operator gets this implicitly).
* Non Member operator Takes the full number of arguments required by the
operator. 2.
*  Member operator: The left operand MOST be the class that the info go into
him(is implicitly the object itself  (this pointer).

c. lazy

d. We declared because the + operator gets the class as a parameter and is above
the class definition

e. its for give access to other outter function to my all(public, protected,
private) fields.

f. no becuase have time he wont need access to my private variables.

g. i will use it with getters and setters.

h. its not break encapsulation here is the secret power of friend function, its
for guard the encapsulation, but if i want to do variable priavte and not ecpose
him to others with public getters/setters so i can use friendly for get access
to him without realy exopse him with public functions.

i. in that case i would do getters becuase i have no problem to exope that
infrometion, but if it was a secret key or something like that i wouldnt want to
exopse it to the others with public getter.

j. the declete in the class its can be use as follow decalre so dont need the
follow decalre above the class implement.

k. its dont metter when its declare, its will get access to all access modiferse
in the class, so we will write in when ever the company style will say, usally
all the friend functions will be togeder at the top or bottum of the class.

l. that i can do what ever i want for each operator for examplse to operator +
do that he will do first_arg - second_arg.

m. when i copy the logic of the operator functionly so ill overide the operator
him self.

n. beaucse its more easy to read and use it as we usual do.
====================== End Question 28 =====================
*/

/*
====================== Start Question 29: =====================
*/

class X29
{
  public:
    explicit X29();
    explicit X29(int a_);
    explicit X29(int a_, int b_);

    operator int() const;
    void Print() const;

  private:
    int m_a;
};

X29::X29() : m_a(0) {}
X29::X29(int a_) : m_a(a_) {}
X29::X29(int a_, int b_) : m_a(a_ + b_) {}

X29::operator int() const { return m_a; }

void X29::Print() const { std::cout << "X29::Print() " << m_a << std::endl; }

void Fifi(X29 x_)
{
    std::cout << "Fifi() " << x_ << std::endl;
    x_.Print();
}

int Question29()
{
    X29 x1(7);

    Fifi(x1);     // (1)
    Fifi(X29(7)); // (2)
    // Fifi(9);      // (3)
    Fifi(3, 4); // (4)

    return x1 * 3; // (5)
}

/*

b. explain the syntax of line2 (Fifi(X29(7)); // (2)) including
type/value/side-effect.

type: the "X29(7)" is rvalue/temporary.
value: tmp val from type X29 where m_a init to val 7.
side-effect:
1. create temporary val with the X(int) Ctor.
2. becuase Fifi get the parameter(_x) by value so Copy-Ctor is calling to do
it(in modern compilers this copy will cencel {Copy Elision/RVO}).
3. in the function will print output to screen.
4. and at the end of actions Dtor is called.

c.a. uncomment line 3 what happned, what did the writer of line3 expect would
happen?
* we get compile error("no matching function for call to ‘Fifi(int)"), and the
writer expect that will be implicit convertion and call to the Ctor with the int
of X29, but becuase have there the "explicit" its copmile error.

c.b Remove the "explicit" keyword from "explicit X(int a_);" and try again:
* now as we exepcted its work and create implicit X class with 9 in m_a and then
print it.

d.
1. will pass exist class(lvalue) then will active copy-Ctor for create the _x
into fifi.

2.expilit create rvalue and then pass him and do the copy-Ctor(modern compiler
will improve it and will create the rvalue in the function and there for wont
call the Copy Ctor) for create the _x into fifi.

3. try to emplicit convert and then fail becuase of "explicit" on the Ctor.

e.a. what happned if uncomment 4?
* compile error("no matching function for call to ‘Fifi(int, int)’"), Fifi will
search X29 type and get int, int..

e.b. can you make it work?
* yes, explicit call to Ctor "Fifi(X29(3, 4));"

f.

====================== End Question 29 =====================
*/
int main() { Question29(); }
