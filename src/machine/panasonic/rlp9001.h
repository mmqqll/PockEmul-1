#ifndef RLP9001_H
#define RLP9001_H

#include "pobject.h"

#include "modelids.h"

class Crlp9001:public CPObject {
    Q_OBJECT
public:
    const char*	GetClassName(){ return("Crlp9001");}

    Cconnector	*pCONNECTOR;


    bool run(void);
    bool init(void);				//initialize
    bool exit(void);				//end
    virtual bool    SaveSession_File(QXmlStreamWriter *xmlOut);
    virtual bool	LoadSession_File(QXmlStreamReader *xmlIn);
    virtual bool	InitDisplay(void);
    virtual bool UpdateFinalImage();
    virtual void ComputeKey(KEYEVENT ke = KEY_PRESSED,int scancode=0,QMouseEvent *event=0);

    Crlp9001(CPObject *parent = 0, Models mod=RLP9001);
    virtual ~Crlp9001();

protected:
    void paintEvent(QPaintEvent *);
protected slots:
    void contextMenuEvent ( QContextMenuEvent * );
    void Rotate();
    void ROMSwitch();
    void exportROM();
    void addModule(QString item, CPObject *pPC);

private:

    bool rotate;
    bool romSwitch;
    int  romoffset;
    Models model;
    quint8 bank;
    int currentSlot;
    bool slotChanged;

};

#endif // RLP9001_H
