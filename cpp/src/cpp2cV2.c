#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************/
/*--------------------- PublicTransport_t & VirtualPublicTransport_t --------*/
/*****************************************************************************/

typedef struct PublicTransport PublicTransport_t;
typedef struct VirtualPublicTransport VirtualPublicTransport_t;

typedef void (*base_func)(PublicTransport_t*);

static int s_count_public_transport = 0;

void PublicTransportDtor(PublicTransport_t* this);
void PublicTransportDisplay(PublicTransport_t* this);

struct PublicTransport
{
    VirtualPublicTransport_t* vptr;
    int m_license_plate;
};

struct VirtualPublicTransport
{
    base_func dtor;
    base_func display;
};

static VirtualPublicTransport_t base_virtual = {PublicTransportDtor,
                                                PublicTransportDisplay};

/* public */
void PublicTransportCtor(PublicTransport_t* const this)
{
    this->m_license_plate = ++s_count_public_transport;
    this->vptr = &base_virtual;
    printf("PublicTransport::Ctor()%d\n", this->m_license_plate);
}

void PublicTransportDtor(PublicTransport_t* const this) /* virtual */
{
    this->vptr = &base_virtual;
    --s_count_public_transport;
    printf("PublicTransport::Dtor()%d\n", this->m_license_plate);
}

void PublicTransportCopyCtor(PublicTransport_t* const this,
                             const PublicTransport_t* other)
{
    this->m_license_plate = ++s_count_public_transport;
    this->vptr = &base_virtual;
    printf("PublicTransport::CCtor() %d\n", this->m_license_plate);
}

void PublicTransportDisplay(PublicTransport_t* const this) /* virtual */
{
    printf("PublicTransport::display(): %d\n", this->m_license_plate);
}

static void PublicTransportPrintCount(void)
{
    printf("s_count: %d\n", s_count_public_transport);
}

/* protected */
int PublicTransportGetID(PublicTransport_t* const this)
{
    return this->m_license_plate;
}

/* private */
PublicTransport_t* PublicTransportOperatorEqual(PublicTransport_t* const this,
                                                PublicTransport_t* const other)
{
    return NULL;
}

/*****************************************************************************/
/*------------------------- Minibus_t & VirtualMinibus_t --------------------*/
/*****************************************************************************/
typedef struct Minibus Minibus_t;
typedef struct VirtualMinibus VirtualMinibus_t;

typedef void (*derived_func)(Minibus_t* const, int);

void MinibusDtor(Minibus_t* const this);
void MinibusDisplay(Minibus_t* const this);
void MinibusWash(Minibus_t* const this, int minutes);

struct Minibus
{
    PublicTransport_t base;
    int m_numSeats;
};

struct VirtualMinibus
{
    VirtualPublicTransport_t base_virt;
    derived_func wash;
};

static VirtualMinibus_t derived1_virtual = {
    {(base_func)MinibusDtor, (base_func)MinibusDisplay}, MinibusWash};

/* public */
void MinibusCtor(Minibus_t* const this)
{
    PublicTransportCtor((PublicTransport_t* const)this);
    this->base.vptr = (VirtualPublicTransport_t*)(&derived1_virtual);
    this->m_numSeats = 20;
    printf("Minibus::Ctor()\n");
}

void MinibusCopyCtor(Minibus_t* const this, Minibus_t* const other)
{
    PublicTransportCopyCtor((PublicTransport_t* const)this,
                            (PublicTransport_t* const)other);
    this->base.vptr = (VirtualPublicTransport_t*)(&derived1_virtual);
    this->m_numSeats = other->m_numSeats;
    printf("Minibus::CCtor()\n");
}

void MinibusDtor(Minibus_t* const this) /* virtual */
{
    this->base.vptr = (VirtualPublicTransport_t*)(&derived1_virtual);
    printf("Minibus::Dtor()\n");
    PublicTransportDtor((PublicTransport_t* const)this);
}

void MinibusDisplay(Minibus_t* const this) /* virtual */
{
    printf("Minibus::display() ID:%d",
           PublicTransportGetID((PublicTransport_t* const)this));
    printf(" num seats:%d\n", this->m_numSeats);
}

void MinibusWash(Minibus_t* const this, int minutes) /* virtual */
{
    printf("Minibus::wash(%d) ID:%d\n", minutes,
           PublicTransportGetID((PublicTransport_t* const)this));
}

/*****************************************************************************/
/*------------------- ArmyMinibus_t & VirtualArmyMinibus_t ------------------*/
/*****************************************************************************/
typedef struct ArmyMinibus ArmyMinibus_t;
typedef struct VirtualArmyMinibus VirtualArmyMinibus_t;

void ArmyMinibusDtor(ArmyMinibus_t* const this);

struct ArmyMinibus
{
    Minibus_t derived1;
};

struct VirtualArmyMinibus
{
    VirtualMinibus_t derived_virt;
};

static VirtualArmyMinibus_t derived2Public_virtual = {
    {{(base_func)ArmyMinibusDtor, (base_func)MinibusDisplay},
     (derived_func)MinibusWash}};

/* public */
void ArmyMinibusCtor(ArmyMinibus_t* const this)
{
    MinibusCtor((Minibus_t* const)this);
    this->derived1.base.vptr =
        (VirtualPublicTransport_t*)(&derived2Public_virtual);
    printf("ArmyMinibus::Ctor()\n");
}

void ArmyMinibusCopyCtor(ArmyMinibus_t* const this, ArmyMinibus_t* const other)
{
    MinibusCopyCtor((Minibus_t* const)this, (Minibus_t* const)other);
    this->derived1.base.vptr =
        (VirtualPublicTransport_t*)(&derived2Public_virtual);
    printf("ArmyMinibus::CCtor()\n");
}

void ArmyMinibusDtor(ArmyMinibus_t* const this) /* virtual */
{
    this->derived1.base.vptr =
        (VirtualPublicTransport_t*)(&derived2Public_virtual);
    printf("ArmyMinibus::Dtor()\n");
    MinibusDtor((Minibus_t* const)this);
}

/*****************************************************************************/
/*---------------------------- Taxi_t & VirtualTaxi_t -----------------------*/
/*****************************************************************************/

typedef struct Taxi Taxi_t;
typedef struct VirtualTaxi VirtualTaxi_t;

void TaxiDtor(Taxi_t* const this);
void TaxiDisplay(Taxi_t* const this);

struct Taxi
{
    PublicTransport_t base;
};

struct VirtualTaxi
{
    VirtualPublicTransport_t derived_virt;
};

static VirtualTaxi_t derived1TaxiPublic_virtual = {
    {(base_func)TaxiDtor, (base_func)TaxiDisplay}};

/* public */
void TaxiCtor(Taxi_t* const this)
{
    PublicTransportCtor((PublicTransport_t* const)this);
    this->base.vptr = (VirtualPublicTransport_t*)(&derived1TaxiPublic_virtual);
    printf("Taxi::Ctor()\n");
}

void TaxiCopyCtor(Taxi_t* const this, Taxi_t* const other)
{
    PublicTransportCopyCtor((PublicTransport_t* const)this,
                            (PublicTransport_t* const)other);
    this->base.vptr = (VirtualPublicTransport_t*)(&derived1TaxiPublic_virtual);
    printf("Taxi::CCtor()\n");
}

void TaxiDtor(Taxi_t* const this) /* virtual */
{
    this->base.vptr = (VirtualPublicTransport_t*)(&derived1TaxiPublic_virtual);
    printf("Taxi::Dtor()\n");
    PublicTransportDtor((PublicTransport_t* const)this);
}

void TaxiDisplay(Taxi_t* const this) /* virtual */
{
    printf("Taxi::display() ID:%d\n",
           PublicTransportGetID((PublicTransport_t* const)this));
}

/*****************************************************************************/
/*----------------------- SpecialTaxi_t & VirtualSpecialTaxi_t --------------*/
/*****************************************************************************/

typedef struct SpecialTaxi SpecialTaxi_t;
typedef struct VirtualSpecialTaxi VirtualSpecialTaxi_t;

void SpecialTaxiDtor(SpecialTaxi_t* const this);
void SpecialTaxiDisplay(SpecialTaxi_t* const this);

struct SpecialTaxi
{
    Taxi_t derived1;
};

struct VirtualSpecialTaxi
{
    VirtualTaxi_t derived1_virt;
};

static VirtualSpecialTaxi_t derived2SpecialTaxiPublic_virtual = {
    {{(base_func)SpecialTaxiDtor, (base_func)SpecialTaxiDisplay}}};

/* public */
void SpecialTaxiCtor(SpecialTaxi_t* const this)
{
    TaxiCtor((Taxi_t* const)this);
    this->derived1.base.vptr =
        (VirtualPublicTransport_t*)(&derived2SpecialTaxiPublic_virtual);
    printf("SpecialTaxi::Ctor()\n");
}

void SpecialTaxiCopyCtor(SpecialTaxi_t* const this, SpecialTaxi_t* const other)
{
    TaxiCopyCtor((Taxi_t* const)this, (Taxi_t* const)other);
    this->derived1.base.vptr =
        (VirtualPublicTransport_t*)(&derived2SpecialTaxiPublic_virtual);
    printf("SpecialTaxi::CCtor()\n");
}

void SpecialTaxiDtor(SpecialTaxi_t* const this) /* virtual */
{
    this->derived1.base.vptr =
        (VirtualPublicTransport_t*)(&derived2SpecialTaxiPublic_virtual);
    printf("SpecialTaxi::Dtor()\n");
    TaxiDtor((Taxi_t* const)this);
}

void SpecialTaxiDisplay(SpecialTaxi_t* const this) /* virtual */
{
    printf("SpecialTaxi::display() ID:%d\n",
           PublicTransportGetID((PublicTransport_t* const)this));
}

/*****************************************************************************/
/*----------------- PublicConvoy_t & VirtualPublicConvoy_t ------------------*/
/*****************************************************************************/
typedef struct Convoy Convoy_t;
typedef struct VirtualConvoy VirtualConvoy_t;

void ConvoyDtor(Convoy_t* const this);
void ConvoyDisplay(Convoy_t* const this);

struct Convoy
{
    PublicTransport_t derived1;
    PublicTransport_t* m_pt1;
    PublicTransport_t* m_pt2;
    Minibus_t m_m;
    Taxi_t m_t;
};

struct VirtualConvoy
{
    VirtualPublicTransport_t derived1_virt;
};

static VirtualConvoy_t s_convoy_virtual = {
    {(base_func)ConvoyDtor, (base_func)ConvoyDisplay}};

/* public */
void ConvoyCtor(Convoy_t* const this)
{
    PublicTransportCtor((PublicTransport_t* const)this);
    this->derived1.vptr = (VirtualPublicTransport_t*)(&s_convoy_virtual);

    this->m_pt1 = (PublicTransport_t*)malloc(sizeof(Minibus_t));
    MinibusCtor((Minibus_t* const)this->m_pt1);

    this->m_pt2 = (PublicTransport_t*)malloc(sizeof(Taxi_t));
    TaxiCtor((Taxi_t* const)this->m_pt2);

    MinibusCtor((Minibus_t* const)(&this->m_m));
    TaxiCtor((Taxi_t* const)(&this->m_t));
}

void ConvoyCopyCtor(Convoy_t* const this, Convoy_t* const other)
{
    PublicTransportCopyCtor((PublicTransport_t* const)this,
                            (PublicTransport_t* const)other);

    this->derived1.vptr = (VirtualPublicTransport_t*)(&s_convoy_virtual);

    this->m_pt1 = (PublicTransport_t*)malloc(sizeof(Minibus_t));
    MinibusCopyCtor((Minibus_t* const)this->m_pt1,
                    (Minibus_t* const)other->m_pt1);

    this->m_pt2 = (PublicTransport_t*)malloc(sizeof(Taxi_t));
    TaxiCopyCtor((Taxi_t* const)this->m_pt2, (Taxi_t* const)other->m_pt2);

    MinibusCopyCtor((Minibus_t* const)(&this->m_m),
                    (Minibus_t* const)(&other->m_m));
    TaxiCopyCtor((Taxi_t* const)(&this->m_t), (Taxi_t* const)(&other->m_t));
}

void ConvoyDtor(Convoy_t* const this) /* virtual */
{
    this->derived1.vptr = (VirtualPublicTransport_t*)(&s_convoy_virtual);

    this->m_pt1->vptr->dtor((PublicTransport_t* const)this->m_pt1);
    free(this->m_pt1);

    this->m_pt2->vptr->dtor((PublicTransport_t* const)this->m_pt2);
    free(this->m_pt2);

    TaxiDtor((Taxi_t* const)(&this->m_t));
    MinibusDtor((Minibus_t* const)(&this->m_m));

    PublicTransportDtor((PublicTransport_t* const)this);
}

void ConvoyDisplay(Convoy_t* const this)
{
    this->m_pt1->vptr->display((PublicTransport_t* const)this->m_pt1);
    this->m_pt2->vptr->display((PublicTransport_t* const)this->m_pt2);
    MinibusDisplay((Minibus_t* const)(&this->m_m));
    TaxiDisplay((Taxi_t* const)(&this->m_t));
}
/*****************************************************************************/
/*------------------------------- General Functions -------------------------*/
/*****************************************************************************/

void PrintInfoGetPublicTransport(PublicTransport_t* const a)
{
    PublicTransportDisplay(a);
}

void PrintInfoGetVoid(void) { PublicTransportPrintCount(); }

void PrintInfoGetMiniBus(Minibus_t* const a) { MinibusWash(a, 3); }

void PrintInfoGetInt(int i, PublicTransport_t* const out_param)
{
    Minibus_t ret;

    MinibusCtor((Minibus_t* const)&ret);
    printf("print_info(int i)\n");
    MinibusDisplay((Minibus_t* const)&ret);

    PublicTransportCopyCtor(out_param, (PublicTransport_t* const)&ret);
    MinibusDtor((Minibus_t* const)&ret);
}

void TaxiDisplayGetTaxi(Taxi_t* const s) { TaxiDisplay(s); }

int main(int argc, char** argv, char** envp)
{
    /*Minibus m;*/
    Minibus_t m;

    PublicTransport_t tmp;
    Minibus_t tmp_minibus;
    Taxi_t tmp_taxi;
    PublicTransport_t* array[3];
    int i = 0;
    PublicTransport_t arr2[3];
    Minibus_t m2;
    Minibus_t arr3[4];
    Taxi_t* arr4 = NULL;
    SpecialTaxi_t st;
    ArmyMinibus_t* army_minibus;
    Convoy_t* ts1;
    Convoy_t* ts2;
    Convoy_t conv_tmp;

    MinibusCtor((Minibus_t* const)&m);

    /* print_info(m); */
    PrintInfoGetMiniBus((Minibus_t* const)&m);

    /* print_info(3).display(); */

    PrintInfoGetInt(3, &tmp);
    PublicTransportDisplay(&tmp);

    PublicTransportDtor(&tmp);

    /*PublicTransport* array[] = {new Minibus(), new Taxi(), new Minibus()};*/

    array[0] = (PublicTransport_t*)malloc(sizeof(Minibus_t));
    MinibusCtor((Minibus_t* const)array[0]);

    array[1] = (PublicTransport_t*)malloc(sizeof(Taxi_t));
    TaxiCtor((Taxi_t* const)array[1]);

    array[2] = (PublicTransport_t*)malloc(sizeof(Minibus_t));
    MinibusCtor((Minibus_t* const)array[2]);

    /*
        for (int i = 0; i < 3; ++i)
        {
            array[i]->display();
        }
    */

    for (i = 0; i < 3; ++i)
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

    /* PublicTransport arr2[] = { Minibus(), Taxi(), PublicTransport() }; */

    MinibusCtor(&tmp_minibus);
    PublicTransportCopyCtor((PublicTransport_t* const)&arr2[0],
                            (PublicTransport_t* const)&tmp_minibus);

    TaxiCtor(&tmp_taxi);
    PublicTransportCopyCtor((PublicTransport_t* const)&arr2[1],
                            (PublicTransport_t* const)&tmp_taxi);

    PublicTransportCtor((PublicTransport_t* const)&arr2[2]);

    TaxiDtor((Taxi_t* const)&tmp_taxi);
    MinibusDtor((Minibus_t* const)&tmp_minibus);

    /*

    for (int i = 0; i < 3; ++i) {
        arr2[i].display();
    }
    */

    for (i = 0; i < 3; ++i)
    {
        PublicTransportDisplay((PublicTransport_t* const)&arr2[i]);
    }

    /* print_info(arr2[0]); */
    PrintInfoGetPublicTransport((PublicTransport_t* const)&arr2[0]);

    /* PublicTransport::print_count(); */
    PublicTransportPrintCount();

    /* Minibus m2; */
    MinibusCtor(&m2);

    /* m2.print_count(); */
    PublicTransportPrintCount();

    /* Minibus arr3[4]; */
    for (i = 0; i < 4; ++i)
    {
        MinibusCtor(&arr3[i]);
    }

    /* Taxi *arr4 = new Taxi[4]; */
    arr4 = (Taxi_t*)malloc(sizeof(Taxi_t) * 4 + sizeof(size_t));
    *(size_t*)arr4 = 4lu;

    arr4 = (Taxi_t*)((char*)arr4 + sizeof(size_t));
    for (i = 0; i < 4; ++i)
    {
        TaxiCtor(&arr4[i]);
    }

    /* delete[] arr4; */
    for (i = (*(size_t*)((char*)arr4 - sizeof(size_t)) - 1); i >= 0; --i)
    {
        TaxiDtor(&(arr4[i]));
    }

    free((char*)arr4 - sizeof(size_t));

    /* std::cout << max_func(1, 2) << "\n"; */
    printf("%d\n", 1 > 2 ? 1 : 2);

    /* std::cout << max_func<int>(1, 2.0f)<< "\n"; */
    printf("%d\n", 1 > 2 ? 1 : 2);

    /* SpecialTaxi st; */
    SpecialTaxiCtor((SpecialTaxi_t* const)&st);

    /* taxi_display(st); */
    TaxiCopyCtor(&tmp_taxi, (Taxi_t* const)&st);
    TaxiDisplayGetTaxi((Taxi_t* const)&tmp_taxi);
    TaxiDtor(&tmp_taxi);

    /* PublicConvoy *ts1 = new PublicConvoy(); */
    ts1 = (Convoy_t*)malloc(sizeof(Convoy_t));
    ConvoyCtor((Convoy_t* const)ts1);

    /* PublicConvoy *ts2 = new PublicConvoy(*ts1); */
    ts2 = (Convoy_t*)malloc(sizeof(Convoy_t));
    ConvoyCopyCtor((Convoy_t* const)ts2, (Convoy_t* const)ts1);

    /* ts1->display(); */
    ts1->derived1.vptr->display((PublicTransport_t* const)ts1);

    /* ts2->display(); */
    ts2->derived1.vptr->display((PublicTransport_t* const)ts2);

    /* delete ts1; */
    ConvoyDtor((Convoy_t* const)ts1);
    free(ts1);

    /* ts2->display(); */
    ts2->derived1.vptr->display((PublicTransport_t* const)ts2);

    /* delete ts2; */
    ConvoyDtor((Convoy_t* const)ts2);
    free(ts2);

    /* ArmyMinibus* army_minibus = new ArmyMinibus; */
    army_minibus = (ArmyMinibus_t*)malloc(sizeof(ArmyMinibus_t));
    ArmyMinibusCtor((ArmyMinibus_t* const)army_minibus);

    /* army_minibus->display(); */
    ((VirtualPublicTransport_t*)((PublicTransport_t*)army_minibus)->vptr)
        ->display((PublicTransport_t* const)army_minibus);

    /*army_minibus->derived1.base.vptr->display(
        (PublicTransport_t* const)army_minibus);*/

    /* army_minibus->wash(5); */
    ((VirtualMinibus_t*)*((PublicTransport_t**)army_minibus))
        ->wash((Minibus_t* const)army_minibus, 5);

    /* delete army_minibus; */
    ArmyMinibusDtor((ArmyMinibus_t* const)army_minibus);
    free(army_minibus);

    /* dtors: */

    SpecialTaxiDtor(&st);
    for (i = 3; i >= 0; --i)
    {
        MinibusDtor(&arr3[i]);
    }

    MinibusDtor(&m2);
    for (i = 2; i >= 0; --i)
    {
        PublicTransportDtor((PublicTransport_t* const)&arr2[i]);
    }

    MinibusDtor((Minibus_t* const)&m);
}