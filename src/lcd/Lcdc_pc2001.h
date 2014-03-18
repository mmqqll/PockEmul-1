#ifndef LCDC_PC2001_H
#define LCDC_PC2001_H

#include "Lcdc.h"

class Clcdc_pc2001:public Clcdc{
public:
    void disp(void);				//display LCDC data to screen
    void disp_symb(void);
    const char*	GetClassName(){ return("Clcdc_pc2001");}

    Clcdc_pc2001(CPObject *parent = 0);
    virtual ~Clcdc_pc2001()
    {						//[constructor]
    }

};

#endif // LCDC_PC2001_H
