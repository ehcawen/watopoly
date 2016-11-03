#ifndef gototim_h
#define gototim_h

#include "nonPropertySqr.h"

class DcTim;
class GotoTim : public nonPropertySqr{
public:
    GotoTim();
    DcTim* dcTim;
    void notify(Player* p) override;
};

#endif



