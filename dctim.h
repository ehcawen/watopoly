#ifndef dctim_h
#define dctim_h

#include "nonPropertySqr.h"

class DcTim : public nonPropertySqr{
public:
    DcTim();
    void leaveDC(Player *p, int point);
    void notify(Player* p) override;
    void notify(Player* p, int roll1,int roll2);
};

#endif

