#include <iostream>
#include <stdio.h>
#include <stdlib.h> //malloc, free

/* ============== phase 1 start: ==============
a. what is the CFront Compiler:
A: its the first and original compiler of c++ language, his job was to translate
the c++ code to c and then compile the c as usual, there for he wasnt "normal
compiler" he was Transpiler.

b.
Q: how did the Cfront compiler treat C code(that is, code without any
C++ features):
A: all the the things he could leave it as is he did, but still
have things that he change, like:
1. Name Mangling - still did it for each function(becuase in compile time he
cant know what happned in linker - that maybe will have 1 more file that have
same function that in c its not ok but in c++ its ok becuase ovverloading..).

2. explicit casting - C++ is more hard in his rules, for examplse in c its ok to
write (*void) and when cfront see it he was tranltae it to explicit relevant
cast and if cant understand what cast it is so its compile time error.

Q: Was the resulttant executable efficient(Hint: What is the zero overhead
principle):
A: yes it was very efficient becuase it was 100% C language code at the end.

c. why is it importent for us to understand how it worked:
1. its explain why c++ have backwards compatibillity to C language(its literly
C..).

2. in RTOS/Embedded with that understasing we can understand the overhead that
the c++ language will add/not add to us, and understand how write more efficient
code.


============== phase 1 end ==============*/

/* ============== phase 2 start: ==============*/
struct A
{
};

struct B
{
    int x;
    int y;
};

void Question2(void) { printf("%lu\n", sizeof(struct B)); }

/*
a.
Q: can sizeof C struct be 0:
A:
1. in C no, the standart of c89 struct most have atleast 1 element in him, but
some compiler (like gcc) allowed it as exection that not the standart, and in
that case the sizeof will return 0.

2. in C++ its most be minimum 1-> there for if compile in c++ the compiler will
put in this 1 char and it will return 1.

Q: why:
A: in c++ each object most have uniqe memory adress or else it was break the
pointer arhthmetic and we wont be able to understand the diffrent between 2
difrrent objects.

b. what are the sizeof of struct A and struct B:
1. in C: the sizeof A its not compile with gcc pedantic error.
but without the pedantic error its un-speciped(in clang its not compile and in
gcc its will with size 0).

2. in C++: will be 1.

and sizeof B is 8 in both languages.

c. diffreent ways an instance of a C struct can be used:
* passed to functions - by val, by ref.
* init - if not init its garbge values, if init it will be the first values.
* in struct - then when create the wrap struct need to init it as the init say
here above.

d. how cFront was tranlte that struct into C? he was change something?
Yes he was change, he will translate it like this:
struct A
{
    char dumy;
};

and struct B was the same.

============== phase 2 end ==============*/

/* ============== phase 3 Start: ==============*/

class Base3
{
    int dum;
};

union Uni3
{
    double value;
    int hey;
    int Foo3();
};

void Question3(void)
{
    Uni3 h = {3};
    (void)h;
}

/*
a. what new features did C++ introduce into classes wich did not exist in C
structs: A: in C struct was passive colletion of variables, in c++ its become
active objects that can mange himself with that featres:

1. member function: can defention function in the struct that will be able to do
things on the private parameters(work with hidden this pointer).

2. Access modifiers: in c++ the struct will get defult public and in class is
private.

3. Ctors & Dtors: mehanicem that auto call to Ctor while the object is create
and auto call to Dtor when he is delete.

4. Inheritance & Polymorphism: that class can work on base of other class and
the virtual mehanicem that work behind the scence with V-Tables.


b.
1. what is aggregate initialization:
its use of values in {} to init the variables in struct without call to any
function -> "Point p = {1, 3};"

2. What is the diffrence between C++ class vs C structs in regard to that:
- in C can do it to any struct.
- in C++ can do it only if all this rules are true:
* no Ctor declare by user.
* no private/protected parameters.
* no virtual function.
* no base claess(the class dont inhirit from anyone).

if ill add Ctor to class ill wont be able to agraget init him with {1,3} it will
do compile error UNLESS i made relevant Ctor(that will get 2 paramerets type
numeric in that case).

3. is Unions can be agraget:
yes, both c and c++ can do it but in c++ it with the rules i mention in 2.

a. try inheriting from a uinion:
A: compile error "a class cannot derive from union".

b. try to make a uinion inherit from a class:
A: compile error "a union is not allowed to have base class".

c. try adding a method to a uinion:
A: not like C, here in C++ its work.

d. try initializtion a union instance using an aggregate initialization:
A: work, BUT in c++98 use aggregate initialization on union will allways
explicit init only the first variable in the union.

============== phase 3 end ==============*/

/* ============== phase 4 C++ classes vs structs Start: ==============
Q: what are the differences between a C++ class and a C++ struct:
A:
1. Members Default Access - in struct its public and in class its private.

2. Default Inheritance:
* when class inhirite from X the defualt make X private.
* when struct inhirite from X the defualt make X public.
============== phase 4 end ==============*/

/* ============== phase 5 C++ Classes Start: ==============*/

class Point5
{
  public:
    Point5();
    Point5(int x, int y);

  private:
    int m_x;
    int m_y;
};

Point5::Point5() {}
Point5::Point5(int x, int y) : m_x(0), m_y(0)
{
    (void)x;
    (void)y;
}

class A5
{
};

class B5
{
  private:
    int x;
    int y;
    Point5 p;
};

int Question5Cpp(void)
{
    A5 a;
    (void)a;
    B5 b;
    B5 b2(b);
    b2 = b;

    return 0;
}

//=============== after Cfront ======================:
// Point5 -> Point5 in C:
struct Point5C
{
    int m_x;
    int m_y;
};

void Point5C_Point5C(
    struct Point5C* const this1 /*will be this but here its save word*/)
{
    (void)this1;
}

void Point5C_Point5C_int_int(struct Point5C* const this1, int x, int y)
{
    this1->m_x = x;
    this1->m_y = y;
}
void Point5C_destructor(struct Point5C* const this1) { (void)this1; }
void Point5C_copy_constructor(struct Point5C* const this1,
                              const struct Point5C* other)
{
    this1->m_x = other->m_x;
    this1->m_y = other->m_y;
}

struct Point5C* Point5C_assignment_operator(struct Point5C* const this1,
                                            const struct Point5C* other)
{
    if (this1 != other)
    {
        this1->m_x = other->m_x;
        this1->m_y = other->m_y;
    }

    return this1;
}

// A5 -> A5 in C:
struct A5C
{
    char dumy;
};

void A5C_A5C(struct A5C* const this1) { this1->dumy = '\0'; }

void A5C_destructor(struct A5C* const this1) { (void)this1; }

void A5C_copy_constructor(struct A5C* const this1, const struct A5C* other)
{
    this1->dumy = other->dumy;
}

struct A5C* A5C_assignment_operator(struct A5C* const this1,
                                    const struct A5C* other)
{
    if (this1 != other)
    {
        this1->dumy = other->dumy;
    }

    return this1;
}

// B5 -> B5 in C:

struct B5C
{
    int x;
    int y;
    struct Point5C p;
};

void B5C_B5C(struct B5C* const this1) { Point5C_Point5C(&(this1->p)); }

void B5C_Dtor(struct B5C* const this1) { Point5C_destructor(&(this1->p)); }

void B5C_copy_Ctor(struct B5C* const this1, const struct B5C* other)
{
    this1->x = other->x;
    this1->y = other->y;
    Point5C_copy_constructor(&(this1->p), &(other->p));
}

struct B5C* B5C_assignment_operator(struct B5C* const this1,
                                    const struct B5C* other)
{
    if (this1 != other)
    {
        this1->x = other->x;
        this1->y = other->y;
        Point5C_assignment_operator(&(this1->p), &(other->p));
    }

    return this1;
}

int Question5InC(void) // c89, if was c98 i could mimic 100% of cpp flow.
{
    struct A5C a;
    struct B5C b;
    struct B5C b2;

    A5C_A5C(&a);
    B5C_B5C(&b);
    B5C_copy_Ctor(&b2, &b);
    B5C_assignment_operator(&b2, &b);

    B5C_Dtor(&b2);
    B5C_Dtor(&b);
    A5C_destructor(&a);

    return 0;
}

/*
a. what are the 4 conditions for generated functions(Ctor, Dtor, Copy Ctor,
assignemnt opertator) to be genereated:
1. that the user didnt implement them.

2. that someone use them in code.

3. base class support - if A inhirt from B so if B Dtor will be private the
compiler wont able to create Dtor to A and there for on one will be able to
create A in code.

4. Composition variables suuport that funcion either - need ALL data members
will support that operation or the compiler wont create it.

b.
1. translate the code from C++ to C(did out of comment above^^).
2. what implicit argument did you declare in order for the generated methods to
work:
A: "this1" -> in normal c it was called "this" as we get in the cpp.
============== phase 5 end ==============*/

/* ============== phase 6 C++ Class methods Start: ==============*/
/*
a. how does the sizeof() of instance change when we add methods:
A: non virtual functions dont effect the sizeof instance.

b. when do the private public keywords take effects(compile time or run time):
A: only in compile time - each private is in the struct and public
methods its function that get "this" parameter. in run time wont be any reminder
to this modifers.

c. make main code in c++ that the Cfront wont compole due to a vilation of
access modifiers rules(e.g try to get private member from main).
"
int main()
{
    B b;
    b.x = 5;
    return 0;
}
"

d.
Q: what coding patterns were used in order to implement "encapsulation" and
"methods" in C:
1. "methods" - normal functions that get pointer to the struct as first
argument.

2. "encapsulation" - as we did typedef in h. file, so they know the name but not
what in him(and the implement is in the c file..).

Q: how are this patterns implemented in c++ as language build-in features:
1. "methods" - the compiler put public methods as global functions and implicit
put the "this" in left argument.

2. "encapsulation" - the compiler check the c++ guard
words(public/protected/private) in his sybmol table for each method/variable in
the class and then for each call to element(variable/method) he will check in
wich scope the call happned(e.g main so its not in the class) then check if its
mebmber function if its called not from class.
============== phase 6 end ==============*/

/* ============== phase 7 Start: ==============
a. Translate the the new lines in main to C:
*/

void Question7InCpp()
{
    //...
    B b;
    (void)b;
    // b.SetX(5);
    // b.SetY(6);
    B* p = new B;
    // p->SetX(5);
    // p->SetY(6);
    delete p;
}

void Question7C()
{
    //...
    struct B5C b;
    B5C_B5C(&b);

    // b.SetX(5); -> B5C_SetX(&b, 5);
    // b.SetY(6); -> B5C_SetY(&b, 6);

    /*B* p = new B;*/
    struct B5C* p = (struct B5C*)malloc(sizeof(struct B5C));
    if (NULL != p)
    {
        B5C_B5C(p);
    }

    // p->SetX(5); -> B5C_SetX(p, 5);
    // p->SetY(6); -> B5C_SetY(p, 6);

    /*delete p;*/
    if (NULL != p)
    {
        B5C_Dtor(p);
        free(p);
    }

    B5C_Dtor(&b);
}

/*

============== phase 7 end ==============*/

/* ============== phase 8 Start: ==============*/
void Foo8Cpp()
{
    B5C array1[100];
    (void)array1;
    // ...
    B5C* array2 = new B5C[100];
    // ...
    delete[] array2;
}

void Foo8CppTranslateToC()
{
    /*B5C array1[100];*/
    struct B5C array1[100];
    int i = 0;

    for (; i < 100; ++i)
    {
        B5C_B5C(&(array1[i]));
    }

    /*B5C* array2 = new B5C[100];*/
    struct B5C* array2 = (struct B5C*)malloc(sizeof(struct B5C) * 100);
    if (NULL != array2)
    {
        for (i = 0; i < 100; ++i)
        {
            B5C_B5C(&(array2[i]));
        }
    }
    // ...
    /*delete[] array2;*/
    if (NULL != array2)
    {
        for (; i >= 0; --i)
        {
            B5C_Dtor(&(array2[i]));
        }

        free(array2);
    }

    for (i = 99; i >= 0; --i)
    {
        B5C_Dtor(&(array1[i]));
    }
}

/*
a. what methods are behing called within FooCpp():
A: this stages:
1. for array1 Ctor x100 times.
2. for array2 alloc memory for sizeof(100) B5C. then call 100 Ctors for them.
3. then will be 100 Dtors for the array2.
4. then will be 100 Dtors for the array1.

b. what happens if the square brackets are missing("delete array"):
A: undefind behaver(memory leak, heap currapt, etc..).

c. when would we want to omit the square brackets intentionally:
A: when we do "new X"(wihtout brackets) so we will delete either without
brackets.

d. translate "Foo()" to c as if you were the Cfront compiler.
A: did on code above^^
============== phase 8 end ==============*/

/* ============== phase 9 Pass by value&refernce Start: ==============*/

B5C Bar9Cpp(B5C b1, B5C* b2, B5C& b3)
{
    (void)b2;
    (void)b3;
    return b1;
}

void Bar9CppTranslateToC(struct B5C* result, struct B5C b1, struct B5C* b2,
                         struct B5C* const b3)
{
    (void)b2;
    (void)b3;

    // return b1;
    *result = b1;
}

/*
a. how can you pass a struct instance by refernce in C:
A: by const pointer and check before that he is not null.

b. translte the code in Bar9Cpp to c.
A: i did above^, explain on the void return and reuslt parameter:
becuase B5C have Copy Ctor/ assingment operator the compolier auto add that out
param and use 1 of them, for do deep copy.
============== phase 9 end ==============*/

/* ============== phase 10 Static methods Start: ==============*/
class B10
{
  public:
    static void Bla();

  private:
    int m_x;
};

void B10::Bla() {}

int Question10Cpp()
{
    //...
    B10::Bla();
    //..

    B10 b;
    b.Bla();

    return 0;
}

void B10C_Bla() {}

// tranlte to C:
struct B10C
{
    char dumy;
};

int Question10CppTranslateToC()
{
    //...
    /*B10C::Bla();*/
    B10C_Bla();
    //..

    B10C b;
    (void)b;
    // B10C_Ctor(&b);

    /*b.Bla();*/
    B10C_Bla();

    // B10C_Dtor(&b);
    return 0;
}

/*
a.
Q: what is the difference between static and non-static methods in your
implementation:
A: that static method wont get "this" parameter.

b. what is the difference between the two Bla() calls:
A: No diffrenece, both will call same function.

============== phase 10 end ==============*/

/* ============== phase 11 Inheritance Start: ==============*/

class A11
{
  private:
    int m_x;
};

class B11 : public A11
{
    int m_y;
};

// Cfront to C:

struct A11C
{
    int m_x;
};

struct B11C
{
    struct A11C m_x;
    int m_y;
};

//======= check memory:
class Base11
{
    int x;
    int y;
};
class Derived11 : public Base11
{
    int z;
};

int Question11Cpp()
{
    Base11 a;
    Derived11 b;

    std::cout << sizeof(a) << std::endl;
    std::cout << sizeof(b) << std::endl;

    return 0;
}

/*
a. Given a hierarchy of classes, what is that order of Ctor being executed upon
leaf-object creation:
A: the order is that the base class will be the first Ctor that active.

b. Given a hierarchy of classes, what is that order of Dtor being executed upon
leaf-object destruction:
A: the last class that his Ctor was active is the first one that his Dtor will
active.

c.
1. when you debug a call to leaf class Ctor what Ctor are you entering first?
second?
A: i enter to the leaf Ctor first the first line will call his father and so
on..

2. why:
A: for create the father that i most he will be exists for ill use him in me.

d.
Q: Tranlating C++ class inheritance to C code, how would you have an object of
derived hold an instance of the parent members:
A: he will hold him as member.

Q: why:
A: to be able to use all his members.

e. Given the following classes, draw a sketch of the memory of a "Derived":
* Base11: 8
* Derived11: 12

f.
Q: are parent membres located in memory before or after the "derived" class
members: A: before.

Q: why:
A: for polymorphism that it will sit in memory from start of memory as base
class and forward.


============== phase 11 end ==============*/

/* ============== phase 12 Object Ctor Order Start: ==============*/
class Base12
{
  public:
    Base12(int inx, int iny);

  private:
    int x;
    int y;
};

Base12::Base12(int inx, int iny) : x(inx), y(iny) {}

class Derived12 : public Base12
{
  public:
    Derived12(int inx, int iny, int inz);

  private:
    int z;
};

Derived12::Derived12(int inx, int iny, int inz) : Base12(inx, iny), z(inz) {}

// Cfront to C:

struct Base12C
{
    int x;
    int y;
};

void Base12C_int_int(struct Base12C* const this1, int inx, int iny)
{
    this1->x = inx;
    this1->y = iny;
}

struct Derived12C
{
    struct Base12C b;
    int z;
};

void Derived12c_int_int_int(struct Derived12C* const this1, int inx, int iny,
                            int inz)
{
    Base12C_int_int(&(this1->b), inx, iny);
    this1->z = inz;
}

/*
a. what is the order of object Ctor refer to the base class Ctor call,
own member init list(MIL), own Ctor body execution:
A: Father, Mil, my body Ctor.

b.
Q: what is the order of init of members within the MIL:
A: in order they declare in class definition.

Q: why:
A: becuase Dtor most destory it execly in reverse order to what called to Ctor,
so if mil change the order the compiler wont be able to know who called when.

c. why is it dangerous to call a member function(virtual or non-virtual) from
within the MIL:
A: becuase she will get "this" parameter that didnt init yet..

d. tranlstae the code to C:
A: did above^^.
============== phase 12 end ==============*/

/* ============== phase 13 Polymorphism Start: ==============*/

class Base13
{
  public:
    virtual ~Base13();
    virtual void Foo();
};

class Derived13 : public Base13
{
    //..
};

/*
a. where is VPTR located:
A: by most compilers it will be the first member of the base class that use
virtual.

b. why would we NOT want VPTR to be the first member?(Hint: Think about
backward-compatibility to C).
A: when put its first its give offset to all other members then its dont
backward compatibility with C.

c. why would we want VPTR to be the first member?(Hint: Where about would one
expect it to be?)
A: for simplifies dynamic dispatch (polymorphism) and improves performance, when
call virtual function compiler know for 100% that the VPTR is in offset 0, else
he would need to save alot more metadata for each object(becuase he need what
the sizeof the object itself we cant anymore use him polomorpysem and allways go
to offset 0) and in run time go and calc for each object where is the vptr is
hold.

d. why do we actually not care about breaking back-compatibility of C++ classes
to C struct(Hint: when does a programmer defintely know that a calss instance
should not be delivered to a C function as a struct with thte same fields?)

A:if a class has virtual functions, its behavior relies on C++ runtime
mechanics. Passing such an object to a C function implies the C function will
try to manipulate it as "Plain Old Data" (POD), which fundamentally breaks the
object-oriented design. Since a polymorphic object is inherently incompatible
with C logic, breaking its memory layout compatibility with C is irrelevant.

e. is there a VPTR(Virtual Table Pointer) in every class derived from another?
A: If Base introduces virtual methods, it gets a VPTR. Derived inherits this
layout. Derived does not get a second VPTR, but in
============== phase 13  end ==============*/

/* ============== phase 14 Polymorphism - Virtual Dtor Start: ==============*/

/*
a.
Q: never public-inherit from a class wich has no virtual Dtor, why:
A: when delete object that inhirit from pointer to the base class its undefind
behavor in c++. the compiler will make static link(becuase its not virtual..)
and active only the Dtor of the base class.

b. are there any execptions for this rule?
A: Yes, in 2 general cases:
1. when the Dtor of base class is non virtual PROTECTED -> in that case if try
delete base ptr to derived class it will make compile error, that design pattern
lead to safe inhirit from that perspective.

2. if in progrem promse to us that all the classes on the stack or all the
pointers will be delete while they point to there real derived, its will be ok
either.
============== phase 14 end ==============*/

/* ==== phase 15 Polymorphism - virtual table and methods Start: ====*/

class Base15
{
  public:
    Base15(int inx, int iny);
    virtual ~Base15();
    virtual void Foo15();
    //...
  private:
    int x;
    int y;
};

Base15::Base15(int inx, int iny) : x(inx), y(iny) {}
Base15::~Base15() {}
void Base15::Foo15() {}

class Derived15 : public Base15
{
  public:
    Derived15(int inx, int iny, int inz);

  private:
    int z;
};

Derived15::Derived15(int inx, int iny, int inz) : Base15(inx, iny), z(inz) {}

int Question15Cpp()
{
    //...
    Base15* pBase = new Derived15(5, 6, 7);
    pBase->Foo15();

    return 0;
}

// translate to C:
typedef struct Base_VTable Base15C_Vtable_t;

typedef struct Base15c
{
    const Base15C_Vtable_t*
        vptr; /* VPTR MUST be the first member of the object */
    int x;
    int y;
} Base15C;

typedef struct Derived15c
{
    Base15C base;
    int z;
} Derived15C;

struct Base_VTable
{
    void (*dtor15C)(Base15C* const this1);
    void (*Foo15C)(Base15C* const this1);
};

typedef struct Derived_VTable
{
    Base15C_Vtable_t base; /* MUST be the first member */
} Derived15C_VTable_t;

void Base15C_dtor_impl(Base15C* const this1)
{
    (void)this1;
    printf("Base Destructor called\n");
}

void Base15C_Foo_impl(Base15C* const this1)
{
    (void)this1;
    printf("Base::Foo called\n");
}

void Derived15C_dtor_impl(Derived15C* const this1)
{
    printf("Derived Destructor called\n");
    Base15C_dtor_impl(&(this1->base));
}

void Derived15C_Foo_impl(Derived15C* const this1)
{
    (void)this1;
    printf("Derived::Foo called\n");
}

void Derived15C_dtor_wrapper(Base15C* const this1)
{
    Derived15C_dtor_impl((Derived15C*)this1);
}

void Derived15C_Foo_wrapper(Base15C* const this1)
{
    Derived15C_Foo_impl((Derived15C*)this1);
}

// main implement:
const Base15C_Vtable_t g_Base_VTable = {Base15C_dtor_impl, Base15C_Foo_impl};

const Derived15C_VTable_t g_Derived_VTable = {
    {Derived15C_dtor_wrapper, Derived15C_Foo_wrapper}};

void Base15C_Init(Base15C* const this1, int inX, int inY)
{
    this1->vptr = &g_Base_VTable;

    this1->x = inX;
    this1->y = inY;
}

void Derived15C_Init(Derived15C* const this1, int inX, int inY, int inZ)
{
    Base15C_Init(&(this1->base), inX, inY);
    this1->base.vptr = (const Base15C_Vtable_t*)&g_Derived_VTable;
    this1->z = inZ;
}

int Question15CppTranlateToC(void)
{
    Derived15C d;
    Base15C* pBase = (Base15C*)&d;

    Derived15C_Init(&d, 1, 2, 3);

    pBase->vptr->Foo15C(pBase);
    pBase->vptr->dtor15C(pBase);

    return 0;
}

/*
a.
1. when writing C code which emulates c++ polymorphism, what would be the VPTR
type:
A: there two optines:
* pointer to arr of generic funcion(typedef void (*VirtualFn)(void); VirtualFn*
vptr)
* or pointer to struct(const struct Base_VTable* vptr).

2. what does it point to:
A:
* struct Points to a static global instance of Base_VTable
* pointer to funcion Points to a static global array of these generic function
pointers

b. can we implement VPTR as a "void*"?
A: Yes, the VPTR can technically be implemented as a void* because a void* can
store the address of any memory location (including an array or a struct).

c. when implementing VTable using a struct of function pointers what member must
a derived class VTable struct have?
A: When implementing the VTable using a struct of function pointers, a Derived
class VTable struct must have the Base class VTable struct as its very first
member.

d. translate the code above to c.

e. which hierarchies are actully mirrored in polymor implement?
A: in vtable we create it as inhirit and put the base vTable first.

f. can vptr be const? why?
A: no, becuase each inhirit i replace the pointer adress he point to.

g. can a VTable struct instance and its members be const?
A: Yes for both

============== phase 15 end ==============*/

/* ============== phase 16 Polymorphism - Time Start: ==============

a. what is the implication of calling virtual methods with respect to runtime
performance:
A: the derefernce of vptr to the Vtable and then the calc of offset to the
relevant function in the table in runtime, mupltiple n times(for each virtual
function call).

b. if Vtable holds an instance of its parent Vtable as a member, does it mean
that in runtime theres another indirection taking place when calling a Base
class virtual method? e.g "pDerived->vptr->m_BaseTable0>Foo();"
A: No, we can do this:"pDerived->vptr>Foo();" becuase the vptr is already point
to the relevant Vtable.

c. when can you call a virtual function, knowing that there will be no
performance impact at all?
A: Yes, in two main ways:
1. if create class on stack it will static determinte wich function will be
called.
2. if explicit call to function from class "B::Foo()".


============== phase 16 end ==============*/

/* ============== phase 17 Polymorphism - Space Start: ==============

a. how many Vtable instances should exist on runtime:
A: Exactly one instance per polymorphic class

b. is it possible for two classes to use the same VTable:
A: Yes, but in realy specific terms, so -> Practically speaking, no.

============== phase 17 end ==============*/

/* ============== phase 18 Object Ctor order - 2nd time Start: ==============

a. when should Vptr be init in a Ctor with respect to the base Ctor call, MIL
and Ctor body execution?

A: the order will be Base Ctor, then VPTR point to my VTable class, then my Ctor
body execution.

Q: why:
A: becuase c++ promse that if call from Ctor to virtual function its 100% willed
called to my(the class im in his Ctor right now) function.

b. fix code did before if its not like this..(was already like this).

c.
Q: why do we need each Ctor to init the VPTR:
A: for be able to use "my" virtual functions in Ctor if i call them.. else i
would call to Base virtual function if called virtual function in Ctor.

Q: isnt it enough for the actual derive object being create to init it:
A: no, becuase what i say in start of c.

============== phase 18 end ==============*/

/* ========== phase 19 Polymorphism - virtual methods Start: ==========

a. Given the Base class, Derived class and a virtual method Foo(), what is the
type of "this" ub Bases VTable:
A: he will get the Base.

b.
Q: when Derived "Foo()" is being called what is the type of "this":
A: from type Derived - and if i didnt override the function of base ill call in
my foo to the base foo and ill pass him "(Base*)this".

Q: why:
A: for each one will be able to active his logic and get accsess to his
variables from one side and be able to active father logic from other side if
need.
 ============== phase 19 end ==============*/

/* ============== phase 20 Multiple inheritance Start: ==============

a.
Q: why didnt the Cfront compiler support multiple inheritance:
A: becuase its very complex to implement and alot of metadata to save for make
it work for each case of this.

Q: can you implement multiple inheritance in c?
A: Yes , with alot of metadata and run time calcs of ofsets and sizeof's.

 ============== phase 20 end ==============*/

/* ============== phase 21 Start: ==============*/

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
    (void)a;
    float b = Max_double(5.5, 6.5);
    (void)b;

    return 0;
}

int main()
{
    Question21C();
    return 0;
}

/*

a. what does this code do:
A: its will paste the text (int/double) in the places that have T.

b. what does ## do:
A: its concat the text that he get without spaces.

c. what is the "-E" compiler flag used for? play with it.
A: its give me the output form the pre-proccesor to the terminal, its work like
this: "gdpp98 src/cpp2c.cpp -E"

============== phase 21 end ==============*/