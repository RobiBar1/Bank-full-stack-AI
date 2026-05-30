/*
Writer:  Robi
Checker: Shimon
Date:    31.05.2026
*/

/*
#define TEMPLATE_MAX(T)                                                        \
    T Max_##T(T a, T b) { return a > b ? a : b; }

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
*/

#include <stdio.h>  /* printf */
#include <stdlib.h> /* malloc */

#define UNUSED(x) ((void)(x))
#define TEMPLATE_MAX(T)                                                        \
    T Max_##T(T a, T b) { return a > b ? a : b; }

TEMPLATE_MAX(int)

/***************************************************************************/
/* ====================== PublicTransport & BaseVirtual_t ================ */
/***************************************************************************/
typedef struct PublicTransport PublicTransport_t;
typedef void (*NonParameter)(PublicTransport_t* const);

typedef struct BaseVirtual
{
    NonParameter dtor;
    NonParameter display;
} BaseVirtual_t;

struct PublicTransport
{
    BaseVirtual_t* vptr;
    int m_license_plate;
};

void Z_PublicTransportvDtor(PublicTransport_t* const this);
void Z_PublicTransportDisplay(PublicTransport_t* const this);

static BaseVirtual_t s_base_vtable = {Z_PublicTransportvDtor,
                                      Z_PublicTransportDisplay};
static int s_public_transport_t_count = 0;

/*------public: */
void Z_PublicTransportCtor(PublicTransport_t* const this)
{
    this->vptr = &s_base_vtable;
    this->m_license_plate = ++s_public_transport_t_count;
    printf("PublicTransport::Ctor()%d\n", this->m_license_plate);
}

void Z_PublicTransportvDtor(PublicTransport_t* const this) /* virtual */
{
    --s_public_transport_t_count;
    printf("PublicTransport::Dtor()%d\n", this->m_license_plate);
}

void Z_PublicTransportCopyCtor(PublicTransport_t* const this,
                               PublicTransport_t* other)
{
    UNUSED(other);
    this->vptr = &s_base_vtable;
    this->m_license_plate = ++s_public_transport_t_count;
    printf("PublicTransport::CCtor() %d\n", this->m_license_plate);
}

void Z_PublicTransportDisplay(PublicTransport_t* const this) /* virtual */
{
    printf("PublicTransport::display(): %d\n", this->m_license_plate);
}

static void Z_PublicTransportPrintCount()
{
    printf("s_count: %d\n", s_public_transport_t_count);
}

/*------protected:*/
int Z_PublicTransportGet_ID(PublicTransport_t* const this)
{
    return this->m_license_plate;
}

/*------private:*/
PublicTransport_t*
Z_PublicTransportOperatorEqual(PublicTransport_t* const this,
                               const PublicTransport_t* const other)
{
    UNUSED(this);
    UNUSED(other);

    return NULL;
}

/***************************************************************************/
/* ====================== Minibus & MinibusVirtual_t ================ */
/***************************************************************************/
typedef struct Minibus Minibus_t;

typedef void (*ThisIntParameterMinibus)(Minibus_t* const, int);

typedef struct MinibusVirtual
{
    BaseVirtual_t base;
    ThisIntParameterMinibus wash_mini;
} MinibusVirtual_t;

struct Minibus
{
    PublicTransport_t m_base;
    int m_numSeats;
};

void Z_MinibusDtor(Minibus_t* const this);
void Z_MinibusDisplay(Minibus_t* const this);
void Z_MinibusWash(Minibus_t* const this, int minutes);

static MinibusVirtual_t s_mini_bus_vtable = {{
                                                 (NonParameter)Z_MinibusDtor,
                                                 (NonParameter)Z_MinibusDisplay,
                                             },
                                             Z_MinibusWash};

/*------public:*/
void Z_MinibusCtor(Minibus_t* const this)
{
    Z_PublicTransportCtor((PublicTransport_t*)this);
    this->m_base.vptr = (BaseVirtual_t*)(&s_mini_bus_vtable);
    this->m_numSeats = 20;
    printf("Minibus::Ctor()\n");
}

void Z_MinibusCopyCtor(Minibus_t* const this, Minibus_t* other)
{
    Z_PublicTransportCopyCtor((PublicTransport_t*)this,
                              (PublicTransport_t*)other);
    this->m_base.vptr = (BaseVirtual_t*)(&s_mini_bus_vtable);
    this->m_numSeats = other->m_numSeats;
    printf("Minibus::CCtor()\n");
}

void Z_MinibusDtor(Minibus_t* const this) /* virtual */
{
    printf("Minibus::Dtor()\n");
    Z_PublicTransportvDtor((PublicTransport_t*)this);
}

void Z_MinibusDisplay(Minibus_t* const this) /* virtual */
{
    printf("Minibus::display() ID:%d",
           Z_PublicTransportGet_ID((PublicTransport_t*)this));
    printf(" num seats:%d\n", this->m_numSeats);
}

void Z_MinibusWash(Minibus_t* const this, int minutes) /* virtual */
{
    printf("Minibus::wash(%d) ID:%d\n", minutes,
           Z_PublicTransportGet_ID((PublicTransport_t*)this));
}

/***************************************************************************/
/* ====================== ArmyMinibus & ArmyMinibusVirtual_t ============= */
/***************************************************************************/
typedef struct ArmyMinibus ArmyMinibus_t;
typedef void (*ThisParameterArmyMinibus)(ArmyMinibus_t* const);

typedef struct ArmyMinibusVirtual
{
    MinibusVirtual_t derived;
} ArmyMinibusVirtual_t;

struct ArmyMinibus
{
    Minibus_t m_dervied;
};

void Z_ArmyMinibusDtor(ArmyMinibus_t* const this);

static ArmyMinibusVirtual_t s_army_mini_bus_vtable = {
    {{(NonParameter)Z_ArmyMinibusDtor, (NonParameter)Z_MinibusDisplay},
     (ThisIntParameterMinibus)Z_MinibusWash},
};

/*------public:*/
void Z_ArmyMinibusCtor(ArmyMinibus_t* const this)
{
    Z_MinibusCtor((Minibus_t* const)this);
    this->m_dervied.m_base.vptr = (BaseVirtual_t*)(&s_army_mini_bus_vtable);
    printf("ArmyMinibus::Ctor()\n");
}

void Z_ArmyMinibusCopyCtor(ArmyMinibus_t* const this, ArmyMinibus_t* other)
{
    Z_MinibusCopyCtor((Minibus_t* const)this, (Minibus_t* const)other);
    this->m_dervied.m_base.vptr = (BaseVirtual_t*)(&s_army_mini_bus_vtable);
    printf("ArmyMinibus::CCtor()\n");
}

void Z_ArmyMinibusDtor(ArmyMinibus_t* const this) /* virtual */
{
    UNUSED(this);
    printf("ArmyMinibus::Dtor()\n");
    Z_MinibusDtor((Minibus_t* const)this);
}

/***************************************************************************/
/* ====================== Taxi & TaxiVirtual_t ============= */
/***************************************************************************/
typedef struct Taxi Taxi_t;

typedef struct TaxiVirtual
{
    BaseVirtual_t base;
} TaxiVirtual_t;

struct Taxi
{
    PublicTransport_t m_base;
};

void Z_TaxiDtor(Taxi_t* const this);
void Z_TaxiDisplay(Taxi_t* const this);

static TaxiVirtual_t s_taxi_vtable = {
    {(NonParameter)Z_TaxiDtor, (NonParameter)Z_TaxiDisplay}};

/*------public:*/
void Z_TaxiCtor(Taxi_t* const this)
{
    Z_PublicTransportCtor((PublicTransport_t* const)this);
    this->m_base.vptr = (BaseVirtual_t*)(&s_taxi_vtable);
    printf("Taxi::Ctor()\n");
}

void Z_TaxiCopyCtor(Taxi_t* const this, Taxi_t* other)
{
    Z_PublicTransportCopyCtor((PublicTransport_t* const)this,
                              (PublicTransport_t* const)other);
    this->m_base.vptr = (BaseVirtual_t*)(&s_taxi_vtable);
    printf("Taxi::CCtor()\n");
}

void Z_TaxiDtor(Taxi_t* const this) /* virtual */
{
    printf("Taxi::Dtor()\n");
    Z_PublicTransportvDtor((PublicTransport_t* const)this);
}

void Z_TaxiDisplay(Taxi_t* const this) /* virtual */
{
    printf("Taxi::display() ID:%d\n",
           Z_PublicTransportGet_ID((PublicTransport_t* const)this));
}

/***************************************************************************/
/* ====================== SpecialTaxi & SpecialTaxiVirtual_t ============= */
/***************************************************************************/
typedef struct SpecialTaxi SpecialTaxi_t;

typedef struct SpecialTaxiVirtual
{
    TaxiVirtual_t derived1;
} SpecialTaxiVirtual_t;

struct SpecialTaxi
{
    Taxi_t m_derived1;
};

void Z_SpecialTaxiDtor(SpecialTaxi_t* const this);
void Z_SpecialTaxiDisplay(SpecialTaxi_t* const this);

static SpecialTaxiVirtual_t s_special_taxi_vtable = {
    {{(NonParameter)Z_SpecialTaxiDtor, (NonParameter)Z_SpecialTaxiDisplay}}};

/*------public:*/
void Z_SpecialTaxiCtor(SpecialTaxi_t* const this)
{
    Z_TaxiCtor((Taxi_t* const)this);
    this->m_derived1.m_base.vptr = (BaseVirtual_t*)(&s_special_taxi_vtable);
    printf("SpecialTaxi::Ctor()\n");
}

void Z_SpecialTaxiCopyCtor(SpecialTaxi_t* const this, SpecialTaxi_t* other)
{
    Z_TaxiCopyCtor((Taxi_t* const)this, (Taxi_t* const)other);
    this->m_derived1.m_base.vptr = (BaseVirtual_t*)(&s_special_taxi_vtable);
    printf("SpecialTaxi::CCtor()\n");
}

void Z_SpecialTaxiDtor(SpecialTaxi_t* const this) /* virtual */
{
    printf("SpecialTaxi::Dtor()\n");
    Z_TaxiDtor((Taxi_t* const)this);
}

void Z_SpecialTaxiDisplay(SpecialTaxi_t* const this) /* virtual */
{
    printf("SpecialTaxi::display() ID:%d\n",
           Z_PublicTransportGet_ID((PublicTransport_t* const)this));
}

/***************************************************************************/
/* =========================== Non-Class Functions ======================= */
/***************************************************************************/
void Z_PrintInfoGetPublicTransport(PublicTransport_t* const a)
{
    a->vptr->display((PublicTransport_t* const)a);
}

void Z_PrintInfoGetVoid(void) { Z_PublicTransportPrintCount(); }

void Z_PrintInfoGetMiniBus(Minibus_t* const m)
{
    ((MinibusVirtual_t*)(m->m_base.vptr))->wash_mini(m, 3);
}

PublicTransport_t Z_PrintInfoGetInt(int i)
{
    Minibus_t ret;
    PublicTransport_t result;
    UNUSED(i);

    Z_MinibusCtor(&ret);

    printf("print_info(int i)\n");
    ((MinibusVirtual_t*)(ret.m_base.vptr))
        ->base.display((PublicTransport_t*)&ret);

    Z_PublicTransportCopyCtor(&result, (PublicTransport_t*)&ret);
    Z_MinibusDtor(&ret);

    return result;
}

void Z_TaxiDisplayGetTaxi(Taxi_t* s)
{
    s->m_base.vptr->display((PublicTransport_t*)(s));
}

int main(int argc, char** argv, char** envp)
{
    Minibus_t m;
    Minibus_t m2;
    PublicTransport_t ret_val;

    Minibus_t* arr1_1;
    Taxi_t* arr1_2;
    Minibus_t* arr1_3;
    PublicTransport_t* array[3];
    int i = 0;
    PublicTransport_t arr2[3];
    Minibus_t arr2_1;
    Taxi_t arr2_2;
    Minibus_t arr3[4];
    Taxi_t* arr4;
    SpecialTaxi_t st;
    ArmyMinibus_t* army_minibus;
    Taxi_t temp_taxi;

    UNUSED(argc);
    UNUSED(argv);
    UNUSED(envp);

    /*Minibus m;*/
    Z_MinibusCtor(&m);

    /*print_info(m);*/
    Z_PrintInfoGetMiniBus(&m);

    /*print_info(3).display();*/
    ret_val = Z_PrintInfoGetInt(3);
    ret_val.vptr->display((PublicTransport_t* const)(&ret_val));
    Z_PublicTransportvDtor(&ret_val);

    /*PublicTransport* array[] = {new Minibus(), new Taxi(), new Minibus()};*/
    arr1_1 = (Minibus_t*)malloc(sizeof(Minibus_t));
    Z_MinibusCtor(arr1_1);

    arr1_2 = (Taxi_t*)malloc(sizeof(Taxi_t));
    Z_TaxiCtor(arr1_2);

    arr1_3 = (Minibus_t*)malloc(sizeof(Minibus_t));
    Z_MinibusCtor(arr1_3);

    array[0] = (PublicTransport_t*)arr1_1;
    array[1] = (PublicTransport_t*)arr1_2;
    array[2] = (PublicTransport_t*)arr1_3;

    /*
    for (int i = 0; i < 3; ++i)
    {
         array[i]->display();
    }
    */

    for (; i < 3; ++i)
    {
        array[i]->vptr->display(array[i]);
    }

    /*
    for (int i = 0; i < 3; ++i)
    {
        delete array[i];
    }
    */
    for (i = 0; i < 3; ++i)
    {
        array[i]->vptr->dtor(array[i]);
        free(array[i]);
    }

    /* PublicTransport arr1_2[] = { Minibus(), Taxi(), PublicTransport() }; */

    Z_MinibusCtor(&arr2_1);
    Z_PublicTransportCopyCtor(&(arr2[0]), (PublicTransport_t*)&arr2_1);

    Z_TaxiCtor(&arr2_2);
    Z_PublicTransportCopyCtor(&(arr2[1]), (PublicTransport_t*)&arr2_2);

    Z_PublicTransportCtor(&(arr2[2]));

    Z_TaxiDtor(&arr2_2);
    Z_MinibusDtor(&arr2_1);

    /*
    for (int i = 0; i < 3; ++i)
    {
        arr2[i].display();
    }
    */

    for (i = 0; i < 3; ++i)
    {
        arr2[i].vptr->display(&(arr2[i]));
    }

    /*print_info(arr2[0]);*/
    Z_PrintInfoGetPublicTransport(&(arr2[0]));

    /*PublicTransport::print_count();*/
    Z_PublicTransportPrintCount();

    /*Minibus m2;*/
    Z_MinibusCtor((Minibus_t* const)&m2);

    /*m2.print_count();*/
    Z_PublicTransportPrintCount();

    /*Minibus arr3[4];*/
    for (i = 0; i < 4; ++i)
    {
        Z_MinibusCtor(&(arr3[i]));
    }

    /*Taxi *arr4 = new Taxi[4]*/
    arr4 = (Taxi_t*)malloc(sizeof(Taxi_t) * 4);
    for (i = 0; i < 4; ++i)
    {
        Z_TaxiCtor(&(arr4[i]));
    }

    /*delete[] arr4;*/
    for (--i; i >= 0; --i)
    {
        Z_TaxiDtor(&(arr4[i]));
    }

    free(arr4);

    /*std::cout << max_func(1, 2) << "\n";*/
    printf("%d\n", Max_int(1, 2));

    /*std::cout << max_func<int>(1, 2.0f)<< "\n";*/
    printf("%d\n", Max_int(1, 2));

    /*SpecialTaxi st;*/
    Z_SpecialTaxiCtor(&st);

    /*taxi_display(st);*/
    Z_TaxiCopyCtor((Taxi_t* const)(&temp_taxi), (Taxi_t* const)(&st));
    Z_TaxiDisplayGetTaxi((Taxi_t* const)(&temp_taxi));
    Z_TaxiDtor(&temp_taxi);

    /*ArmyMinibus* army_minibus = new ArmyMinibus;*/
    army_minibus = (ArmyMinibus_t*)malloc(sizeof(ArmyMinibus_t));
    Z_ArmyMinibusCtor(army_minibus);

    /*army_minibus->display();*/
    army_minibus->m_dervied.m_base.vptr->display(
        (PublicTransport_t* const)army_minibus);

    /*army_minibus->wash(5);*/
    ((MinibusVirtual_t*)(army_minibus->m_dervied.m_base.vptr))
        ->wash_mini((Minibus_t* const)army_minibus, 5);

    /*delete army_minibus;*/
    army_minibus->m_dervied.m_base.vptr->dtor((PublicTransport_t*)army_minibus);
    free(army_minibus);

    Z_SpecialTaxiDtor(&st);
    for (i = 3; i >= 0; --i)
    {
        Z_MinibusDtor(&(arr3[i]));
    }

    Z_MinibusDtor(&m2);

    Z_PublicTransportvDtor(&arr2[2]);
    Z_PublicTransportvDtor(&arr2[1]);
    Z_PublicTransportvDtor(&arr2[0]);

    Z_MinibusDtor(&m);

    return 0;
}
