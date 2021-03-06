#include <QPainter>

#include "ti/cc40.h"
#include "Lcdc_cc40.h"
#include "Lcdc_symb.h"

#define BIT(x,n) (((x)>>(n))&1)




Clcdc_cc40::Clcdc_cc40(CPObject *parent, QRect _lcdRect, QRect _symbRect, QString _lcdfname, QString _symbfname):
    Clcdc(parent,_lcdRect,_symbRect,_lcdfname,_symbfname){						//[constructor]

    internalSize = QSize(186,10);
    pixelSize = 4;
    pixelGap = 1;
}


void Clcdc_cc40::disp_symb(void)
{
    Refresh = true;

    Clcdc::disp_symb();


}

#undef COLOR
#define COLOR(b)	( ( (b) ) ? Color_On : Color_Off)
#if 1
HD44780_PIXEL_UPDATE(Ccc40_update_pixel_symb)
{

    if (line == 1 && pos == 15)
    {
        // the last char is used to control lcd indicators
        // reference _________________...
        // output#  |10  11     12     13     14      0      1      2      3   4
        // above    | <  SHIFT  CTL    FN     DEG    RAD    GRAD   I/O    UCL  >
        // ---- raw lcd screen here ----
        // under    |    ERROR   v      v      v      v      v      v    _LOW
        // output#  |    60     61     62     63     50     51     52     53
        int id = y*10+x;
        switch (id) {
        case 0:  Clcdc::disp_one_symb(painter,S_RAD, COLOR(state),	140,	0); break;
        case 1:  Clcdc::disp_one_symb(painter,S_G,   COLOR(state),	160,	0);
                 Clcdc::disp_one_symb(painter,S_RAD, COLOR(state),	165,	0); break;
        case 2:  Clcdc::disp_one_symb(painter,S_IO,  COLOR(state),	190,	0); break;
        case 3:  Clcdc::disp_one_symb(painter,S_UCL, COLOR(state),	215,	0); break;
        case 4:  Clcdc::disp_one_symb(painter,S_SUP, COLOR(state),	240,	0); break;
        case 10: Clcdc::disp_one_symb(painter,S_INF, COLOR(state),	  0,	0); break;
        case 11: Clcdc::disp_one_symb(painter,S_SHIFT, COLOR(state), 20,	0); break;
        case 12: Clcdc::disp_one_symb(painter,S_CTL, COLOR(state),	 50,	0); break;
        case 13: Clcdc::disp_one_symb(painter,S_FN,  COLOR(state),	 80,	0); break;
        case 14: Clcdc::disp_one_symb(painter,S_DEG, COLOR(state),	110,	0); break;

        case 50: Clcdc::disp_one_symb(painter,S_DA, COLOR(state),	140,	25); break;
        case 51: Clcdc::disp_one_symb(painter,S_DA, COLOR(state),	165,	25); break;
        case 52: Clcdc::disp_one_symb(painter,S_DA, COLOR(state),	190,	25); break;
        case 53: Clcdc::disp_one_symb(painter,S_LOW, COLOR(state),	215,	25); break;
        case 60: Clcdc::disp_one_symb(painter,S_ERROR, COLOR(state), 20,	25); break;
        case 61: Clcdc::disp_one_symb(painter,S_DA, COLOR(state),	 50,	25); break;
        case 62: Clcdc::disp_one_symb(painter,S_DA, COLOR(state),	 80,	25); break;
        case 63: Clcdc::disp_one_symb(painter,S_DA, COLOR(state),	110,	25); break;
        }
    }
}
HD44780_PIXEL_UPDATE(Ccc40_update_pixel)
{
    // char size is 5x7 + cursor
    if (x > 4 || y > 7)
        return;
    if (line == 1 && pos == 15)
    {
    }
    else if (line < 2 && pos < 16)
    {
        // internal: 2*16, external: 1*31 + indicators
        if (y == 7) y++;
//        painter->setPen(COLOR(state));
//        painter->drawPoint( 1 + line*16*6 + pos*6 + x, 1 + y );
        plcd->drawPixel(painter, 1 + line*16*6 + pos*6 + x, 1 + y ,COLOR(state));
    }
}
#endif

void Clcdc_cc40::disp(void)
{
    CHD44780 * pHD44780 = ((Ccc40*) pPC)->pHD44780;
    if (!ready) return;
    if (!pHD44780 ) return;
    if (!redraw) {
        if (!updated) return;
    }

    lock.lock();

    redraw = false;
    Refresh = true;
    info = pHD44780->getInfo();
//    disp_symb();


    QPainter painter(LcdImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    info->m_lines = 2;
    info->m_chars = 16;
    pHD44780->set_pixel_update_cb(&Ccc40_update_pixel);
    pHD44780->screen_update(this,&painter,Color_On,Color_Off);
    painter.end();


    QPainter painterSymb(SymbImage);
    painterSymb.setCompositionMode(QPainter::CompositionMode_Source);
    info->m_lines = 2;
    info->m_chars = 16;
    pHD44780->set_pixel_update_cb(&Ccc40_update_pixel_symb);
    pHD44780->screen_update(this,&painterSymb,Color_On,Color_Off);
    painterSymb.end();


    updated = false;

    lock.unlock();

}


