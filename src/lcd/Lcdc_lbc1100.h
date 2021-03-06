#ifndef LCDC_LBC1100_H
#define LCDC_LBC1100_H

#include "Lcdc.h"

class Clcdc_lbc1100:public Clcdc{
public:
    void disp(void);				//display LCDC data to screen
    void disp_symb(void);
    const char*	GetClassName(){ return("Clcdc_lbc1100");}

    Clcdc_lbc1100(CPObject *parent, QRect _lcdRect, QRect _symbRect, QString _lcdfname=QString(), QString _symbfname=QString());
    virtual ~Clcdc_lbc1100()
    {						//[constructor]
    }

};

#endif // LCDC_LBC1100_H
