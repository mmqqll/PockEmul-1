#include <QtGui>
#include <QString>

#include "common.h"
#include "cc40.h"
#include "tms7000/tms7000.h"
#include "hd44780.h"
#include "Lcdc_cc40.h"
#include "Inter.h"
#include "Keyb.h"
#include "cextension.h"

#include "Connect.h"
#include "dialoganalog.h"

#include "Log.h"

/*
CC-40 MEMORY MAP1:


    DECIMAL    HEX     DESCRIPTION         SIZE
   -------------------------------------------------------------------------
       0      0000
                      REGISTER FILE       128 BYTES PROCESSOR RAM
     127      007F
   -------------------------------------------------------------------------
     128      0080
                      UNUSED              128 BYTES
     255      00FF
   -------------------------------------------------------------------------
     256      0100
                      PERIPHERAL FILE     256 BYTES MEMORY MAP I/O
     511      01FF
   -------------------------------------------------------------------------
     512      0200
                      UNUSED              1.5K
    2047      07FF
   -------------------------------------------------------------------------
    2048      0800
                      SYSTEM RAM          2K INSTALLED
    4095      0FFF
   -------------------------------------------------------------------------
    4096      1000
                      SYSTEM RAM          2K INSTALLED
    6143      17FF
   -------------------------------------------------------------------------
    6144      1800
                      SYSTEM RAM          6K NOT INSTALLED
   12287      2FFF
   -------------------------------------------------------------------------
   12288      3000
                      SYSTEM RAM          2K INSTALLED
   14335      37FF
   -------------------------------------------------------------------------
   14336      3800
                      SYSTEM RAM          6K NOT INSTALLED
   20479      4FFF
   -------------------------------------------------------------------------
   20480      5000
                      CARTRIDE PORT       32K
   53247      CFFF
   -------------------------------------------------------------------------
   53248      D000
                      SYSTEM ROM          8K
   61439      EFFF
   -------------------------------------------------------------------------
   61440      F000
                      UNUSED              2K
   63487      F7FF
   -------------------------------------------------------------------------
   63488      F800
                      PROCESSOR ROM       2K
   65536      FFFF
   -------------------------------------------------------------------------

============================================================================

   CC-40 MEMORY MAP: AFTER 12K RAM ADDED:

    DECIMAL    HEX     DESCRIPTION         SIZE
   -------------------------------------------------------------------------
       0      0000
                      REGISTER FILE       128 BYTES PROCESSOR RAM
     127      007F
   -------------------------------------------------------------------------
     128      0080
                      UNUSED              128 BYTES
     255      00FF
   -------------------------------------------------------------------------
     256      0100
                      PERIPHERAL FILE     256 BYTES MEMORY MAP I/O
     511      01FF
   -------------------------------------------------------------------------
     512      0200
                      UNUSED              1.5K
    2047      07FF
   -------------------------------------------------------------------------
    2048      0800
                      SYSTEM RAM          2K INSTALLED
    4095      0FFF
   -------------------------------------------------------------------------
    4096      1000
                      SYSTEM RAM          8K INSTALLED
   12287      2FFF
   -------------------------------------------------------------------------
   12288      3000
                      SYSTEM RAM          8K INSTALLED
   20479      4FFF
   -------------------------------------------------------------------------
   20480      5000
                      CARTRIDE PORT       32K
   53247      CFFF
   -------------------------------------------------------------------------
   53248      D000
                      SYSTEM ROM          8K
   61439      EFFF
   -------------------------------------------------------------------------
   61440      F000
                      UNUSED              2K
   63487      F7FF
   -------------------------------------------------------------------------
   63488      F800
                      PROCESSOR ROM       2K
   65536      FFFF
   -------------------------------------------------------------------------

===========================================================================
    */

#define STROBE_TIMER 5

Ccc40::Ccc40(CPObject *parent)	: CpcXXXX(parent)
{								//[constructor]
    setfrequency( (int) 2500000);
    setcfgfname(QString("cc40"));

    SessionHeader	= "CC40PKM";
    Initial_Session_Fname ="cc40.pkm";

    BackGroundFname	= P_RES(":/cc40/cc40.png");
    LcdFname		= P_RES(":/cc40/cc40lcd.png");
    SymbFname		= "";

//    TopFname    = P_RES(":/fp200/fp200Top.png");

    memsize		= 0x20000;
    InitMemValue	= 0x00;

    SlotList.clear();
    SlotList.append(CSlot(52  , 0x0000 ,	""                  , ""	, CSlot::RAM , "RAM"));
    SlotList.append(CSlot(2  , 0xF800 ,	P_RES(":/cc40/cc40_2krom.bin")  , ""	, CSlot::ROM , "ROM cpu"));
    SlotList.append(CSlot(32 , 0x10000,	P_RES(":/cc40/cc40.bin")        , ""	, CSlot::ROM , "ROM"));


    setDXmm(236);
    setDYmm(147);
    setDZmm(24);

    setDX(845);
    setDY(525);

    Lcd_X		= 50;
    Lcd_Y		= 180;
    Lcd_DX		= 186;
    Lcd_DY		= 9;
    Lcd_ratio_X	= 2.7;
    Lcd_ratio_Y	= 2.7;

    pLCDC		= new Clcdc_cc40(this);
    pCPU		= new Ctms70c20(this);
    pTIMER		= new Ctimer(this);
    pKEYB		= new Ckeyb(this,"cc40.map");
    pHD44780    = new CHD44780(P_RES(":/cc40/hd44780_a00.bin"),this);


    ioFreq = 0;             // Mandatory for Centronics synchronization
    ptms7000cpu = (Ctms70c20*)pCPU;
}

Ccc40::~Ccc40() {
}

void Ccc40::power_w(UINT8 data)
{
    // d0: power-on hold latch
    m_power = data & 1;

    // stop running
    if (!m_power)
        ptms7000cpu->set_input_line(INPUT_LINE_RESET, ASSERT_LINE);
}

bool Ccc40::Chk_Adr(UINT32 *d, UINT32 data)
{
    Q_UNUSED(data)


    if ( (*d>=0x0100) && (*d<=0x010F) )	{ ptms7000cpu->pf_write(*d-0x100,data); return false;	}  // CPU RAM
    if (*d==0x0111) { power_w(data); return false; }
    if (*d==0x0115) { fillSoundBuffer((data & 1) ? 0x7f : 0); return false; }
    if (*d==0x0119) {
        RomBank = data & 0x03; /*qWarning()<<"romBank:"<<RomBank;*/
        RamBank = (data >> 2 & 3);
        m_banks = data & 0x0f;
        return true;
    }
    if (*d==0x011A) { clock_w(data); return false; }
    if (*d==0x011E) {
//        qWarning()<<"pHD44780->control_write:"<<data;
        pHD44780->control_write(data);
        pLCDC->redraw = true;
        return false;
    }
    if (*d==0x011F) {
//        qWarning()<<"pHD44780->data_write:"<<data;
        pHD44780->data_write(data);
        pLCDC->redraw = true;
        return false;
    }
    if ( (*d>=0x0000) && (*d<=0xCFFF) )	{ return true;	}  // CPU RAM
    if ( (*d>=0xD000) && (*d<=0xEFFF) )	{ *d += 0x3000 + ( RomBank * 0x2000 );	return false; } // system ROM
    if ( (*d>=0xF800) && (*d<=0xFFFF) )	{ return false;	}                                       // CPU ROM


    return true;
}

bool Ccc40::Chk_Adr_R(UINT32 *d, UINT32 *data)
{
    Q_UNUSED(d)
    Q_UNUSED(data)

    if ( (*d>=0x0100) && (*d<=0x010F) )	{ *data = ptms7000cpu->pf_read(*d-0x100); return false;	}  // CPU RAM

    if (*d==0x0116) { *data = 1; return false; }
    if (*d==0x0119) { *data = m_banks; return false; }
    if (*d==0x011A) { *data = clock_r(); return false; }
    if (*d==0x011E) { *data = pHD44780->control_read(); return false; }
    if (*d==0x011F) { *data = pHD44780->data_read(); return false; }
    if ( (*d>=0xD000) && (*d<=0xEFFF) )	{ *d += 0x3000 + ( RomBank * 0x2000 );	return true; } // system ROM

    return true;
}



UINT8 Ccc40::in(UINT8 Port)
{
    UINT8 Value=0;

    switch (Port) {
    case TMS7000_PORTA: // keyboard read
        return getKey();
        break;
    case TMS7000_PORTB: break;
    case TMS7000_PORTC: break;
    case TMS7000_PORTD: break;
    default :break;
    }

     return (Value);
}

UINT8 Ccc40::out(UINT8 Port, UINT8 Value)
{
    switch (Port) {
    case TMS7000_PORTA: break;
    case TMS7000_PORTB: // Keyboard strobe
//        qWarning()<<"strobe:"<<Value;
        ks = Value;
        break;
    case TMS7000_PORTC: break;
    case TMS7000_PORTD: break;
    default :break;
    }

    return 0;
}

UINT8 Ccc40::clock_r()
{
    return m_clock_control;
}

void Ccc40::clock_w(UINT8 data)
{
    // d3: enable clock divider
    if (data & 8)
    {
        if (m_clock_control != (data & 0x0f))
        {
            // d0-d2: clock divider (2.5MHz /3 to /17 in steps of 2)
            double div = (~data & 7) * 2 + 1;
//            m_maincpu->set_clock_scale(1 / div);
        }
    }
    else if (m_clock_control & 8)
    {
        // high to low
//        m_maincpu->set_clock_scale(1);
    }

    m_clock_control = data & 0x0f;
}

bool Ccc40::init()
{
    pCPU->logsw = false;
#ifndef QT_NO_DEBUG
    pCPU->logsw = false;
#endif
    CpcXXXX::init();
    pHD44780->init();
    initExtension();
    Reset();

    return true;
}

void	Ccc40::initExtension(void)
{
    AddLog(LOG_MASTER,"INIT EXT CC-40");
    // initialise ext_MemSlot1
//    ext_MemSlot1 = new CExtensionArray("RAM Slot 1","Add RAM Module");
//    ext_MemSlot2 = new CExtensionArray("RAM Slot 2","Add RAM Module");
//    ext_MemSlot3 = new CExtensionArray("RAM/ROM Slot 3","Add RAM or ROM Module");
//    ext_MemSlot1->setAvailable(ID_FP201,true); ext_MemSlot1->setChecked(ID_FP201,false);
//    ext_MemSlot2->setAvailable(ID_FP201,true); ext_MemSlot2->setChecked(ID_FP201,false);
//    ext_MemSlot3->setAvailable(ID_FP201,true); ext_MemSlot3->setChecked(ID_FP201,false);
//    ext_MemSlot3->setAvailable(ID_FP205,true);
//    ext_MemSlot3->setAvailable(ID_FP231CE,true);

//    addExtMenu(ext_MemSlot1);
//    addExtMenu(ext_MemSlot2);
//    addExtMenu(ext_MemSlot3);
//    extensionArray[0] = ext_MemSlot1;
//    extensionArray[1] = ext_MemSlot2;
//    extensionArray[2] = ext_MemSlot3;
}

bool Ccc40::run()
{
    CpcXXXX::run();

    return true;
}

void Ccc40::Reset()
{
    CpcXXXX::Reset();

    m_clock_control = 0;
    m_power = 1;

    pHD44780->Reset();
}

void Ccc40::TurnON()
{
    CpcXXXX::TurnON();
//    pCPU->Reset();
}

void Ccc40::TurnOFF()
{
    mainwindow->saveAll = YES;
    CpcXXXX::TurnOFF();
    mainwindow->saveAll = ASK;
}

bool Ccc40::SaveConfig(QXmlStreamWriter *xmlOut)
{
    Q_UNUSED(xmlOut)

    return true;
}

bool Ccc40::LoadConfig(QXmlStreamReader *xmlIn)
{
    Q_UNUSED(xmlIn)

    return true;
}


#define KEY(c)	( pKEYB->keyPressedList.contains(TOUPPER(c)) || pKEYB->keyPressedList.contains(c) || pKEYB->keyPressedList.contains(TOLOWER(c)))

//#define KEY(c)	( TOUPPER(pKEYB->LastKey) == TOUPPER(c) )
quint8 Ccc40::getKey()
{

    quint8 data=0;

    if ((pKEYB->LastKey>0))
    {
        if (ks & 0x01) {
            if (KEY('1'))			data|=0x01;
            if (KEY('2'))			data|=0x02;
            if (KEY('3'))			data|=0x04;
            if (KEY('4'))			data|=0x08;
            if (KEY('5'))			data|=0x10;
            if (KEY('6'))			data|=0x20;
            if (KEY('7'))			data|=0x40;
            if (KEY('8'))			data|=0x80;
        }
        if (ks & 0x02) {
            if (KEY('Q'))			data|=0x01;
            if (KEY('W'))			data|=0x02;
            if (KEY('E'))			data|=0x04;
            if (KEY('R'))			data|=0x08;
            if (KEY('T'))			data|=0x10;
            if (KEY('Y'))			data|=0x20;
            if (KEY('U'))			data|=0x40;
            if (KEY('I'))			data|=0x80;
        }
        if (ks & 0x04) {
            if (KEY('A'))			data|=0x01;
            if (KEY('S'))			data|=0x02;
            if (KEY('D'))			data|=0x04;
            if (KEY('F'))			data|=0x08;
            if (KEY('G'))			data|=0x10;
            if (KEY('H'))			data|=0x20;
            if (KEY('J'))			data|=0x40;
            if (KEY('K'))			data|=0x80;
        }
        if (ks & 0x08) {
            if (KEY('Z'))			data|=0x01;
            if (KEY('X'))			data|=0x02;
            if (KEY('C'))			data|=0x04;
            if (KEY('V'))			data|=0x08;
            if (KEY('B'))			data|=0x10;
            if (KEY('N'))			data|=0x20;
            if (KEY('M'))			data|=0x40;
            if (KEY('<'))			data|=0x80;
        }
        if (ks & 0x10) {
//            if (KEY(''))			data|=0x01;
            if (KEY(' '))			data|=0x02;
//            if (KEY(''))			data|=0x04;
            if (KEY(':'))			data|=0x08;
            if (KEY('L'))			data|=0x10;
            if (KEY('?'))			data|=0x20;
            if (KEY('P'))			data|=0x40;
            if (KEY('O'))			data|=0x80;
        }
        if (ks & 0x20) {
//            if (KEY(''))			data|=0x01;
            if (KEY(K_CLR))			data|=0x02;
            if (KEY(K_LA))			data|=0x04;
            if (KEY(K_RA))			data|=0x08;
            if (KEY(K_UA))			data|=0x10;
            if (KEY('/'))			data|=0x20;
            if (KEY(K_DA))			data|=0x40;
            if (KEY(')'))			data|=0x80;
        }
        if (ks & 0x40) {
//            if (KEY(''))			data|=0x01;
            if (KEY('>'))			data|=0x02;
            if (KEY('+'))			data|=0x04;
            if (KEY(K_RET))			data|=0x08;
            if (KEY('-'))			data|=0x10;
//            if (KEY(''))			data|=0x20;
//            if (KEY(''))			data|=0x40;
            if (KEY('*'))			data|=0x80;
        }

        if (ks & 0x80) {
            if (KEY(K_SHT))			data|=0x01;
            if (KEY(K_CTRL))			data|=0x02;
            if (KEY(K_BRK))			data|=0x04;
            if (KEY(K_RUN))			data|=0x08;
//            if (KEY(''))			data|=0x10;
            if (KEY(K_FN))			data|=0x20;
            if (KEY(K_OF))			data|=0x40;
//            if (KEY(''))			data|=0x80;
        }

//        if (fp_log) fprintf(fp_log,"Read key [%02x]: strobe=%02x result=%02x\n",pKEYB->LastKey,ks,data^0xff);

    }

    if (data>0) {

        AddLog(LOG_KEYBOARD,tr("KEY PRESSED=%1").arg(data,2,16,QChar('0')));
    }
    return data;//^0xff;

}

//void Ccc40::keyReleaseEvent(QKeyEvent *event)
//{
////if (event->isAutoRepeat()) return;

//    if (pCPU->fp_log) fprintf(pCPU->fp_log,"\nKEY RELEASED= %c\n",event->key());
//    CpcXXXX::keyReleaseEvent(event);
//}


//void Ccc40::keyPressEvent(QKeyEvent *event)
//{
////    if (event->isAutoRepeat()) return;

//    if (pCPU->fp_log) fprintf(pCPU->fp_log,"\nKEY PRESSED= %c\n",event->key());
//    CpcXXXX::keyPressEvent(event);
//}

bool Ccc40::Get_Connector(void) {


    return true;
}
bool Ccc40::Set_Connector(void) {


    return true;
}



