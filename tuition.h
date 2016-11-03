#ifndef tuition_h
#define tuition_h

#include "nonPropertySqr.h"

const int TuitPay = 300;
const double TuitPortion = 0.1;

class Tuition : public nonPropertySqr{
public:
    Tuition();
    void notify(Player* p) override;//make p pay 10% of all wealth or $300
};

#endif

