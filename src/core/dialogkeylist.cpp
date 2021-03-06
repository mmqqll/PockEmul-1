#include <QPainter>
#include <QCloseEvent>
#include <QDebug>

#include "common.h"
#include "pcxxxx.h"
#include "Keyb.h"
#include "dialogkeylist.h"
#include "Log.h"
//


DialogKeyList::DialogKeyList(CPObject * parent, Qt::WindowFlags f)
	: QDialog(parent, f)
{
	setupUi(this);
	
	keyFound = false;
    connect(lwKeys,SIGNAL(itemSelectionChanged()),this,SLOT(slotSelectKey()));
	connect(pbInit,SIGNAL(clicked()),this,SLOT(slotInitSize()));
	connect(pbDel,SIGNAL(clicked()),this,SLOT(slotDelKey()));
    connect(pbApply,SIGNAL(clicked()),this,SLOT(slotApplySize()));

    connect(pbUA,SIGNAL(clicked()),this,SLOT(moveUp()));
    connect(pbDA,SIGNAL(clicked()),this,SLOT(moveDown()));
    connect(pbLA,SIGNAL(clicked()),this,SLOT(moveLeft()));
    connect(pbRA,SIGNAL(clicked()),this,SLOT(moveRight()));

    connect(pbShowAll,SIGNAL(clicked()),this,SLOT(showAll()));

	pPC = parent;
	// populate lvKeys
	InsertKeys();
}
//

void DialogKeyList::InsertKeys(void)
{
	QListWidgetItem *item;	

    for (int i=0; i < pPC->pKEYB->Keys.count(); i++)
 	{
        item = new QListWidgetItem(pPC->pKEYB->Keys.at(i).Description, lwKeys);
        item->setData( Qt::UserRole, qVariantFromValue( pPC->pKEYB->Keys.at(i).ScanCode ) );
        if (pPC->pKEYB->Keys.at(i).MasterScanCode != 0 )
            item->setHidden(true);
    }
}

void DialogKeyList::InsertRelKeys(int masterScanCode)
{
    relKeys->clear();

    QListWidgetItem *item;

    for (int i=0; i < pPC->pKEYB->Keys.count(); i++)
    {
        if (pPC->pKEYB->Keys.at(i).MasterScanCode == masterScanCode )
        {
            item = new QListWidgetItem(pPC->pKEYB->Keys.at(i).Description, relKeys);
            item->setData( Qt::UserRole, qVariantFromValue( pPC->pKEYB->Keys.at(i).MasterScanCode ) );
        }
    }
}

void DialogKeyList::slotDelKey()
{
    return;
    //TODO: Manage list

    keyIter = pPC->pKEYB->Keys.erase( keyIter );
	keyFound = false;
	pPC->pKEYB->modified = true;
	delete lwKeys->currentItem();

}

void DialogKeyList::slotApplySize()
{
    for (int i=0;i<lwKeys->count();i++) {
        if (lwKeys->item(i)->isSelected()) {
            pPC->pKEYB->Keys[i].Rect.setWidth(sbHor->value());
            pPC->pKEYB->Keys[i].Rect.setHeight(sbVer->value());
        }
    }
    pPC->pKEYB->modified = true;
    pPC->Refresh_Display = true;
    pPC->update();
}

void DialogKeyList::slotInitSize()
{
    for (int i=0;i<lwKeys->count();i++) {
        if (lwKeys->item(i)->isSelected()) {
            pPC->pKEYB->Keys[i].Rect.moveTo(0,0);
        }
    }
	pPC->pKEYB->modified = true;
    pPC->Refresh_Display = true;
    pPC->update();
}

void DialogKeyList::moveUp()
{
    int _step = QGuiApplication::queryKeyboardModifiers()==Qt::ShiftModifier ? 10 : 1;
    for (int i=0;i<lwKeys->count();i++) {
        if (lwKeys->item(i)->isSelected()) {
            pPC->pKEYB->Keys[i].Rect.adjust(0,-_step,0,-_step);
            pPC->pKEYB->modified = true;
        }
    }
    pPC->Refresh_Display = true;
    pPC->update();
}

void DialogKeyList::moveDown()
{
    int _step = QGuiApplication::queryKeyboardModifiers()==Qt::ShiftModifier ? 10 : 1;
    for (int i=0;i<lwKeys->count();i++) {
        if (lwKeys->item(i)->isSelected()) {
            pPC->pKEYB->Keys[i].Rect.adjust(0,_step,0,_step);
            pPC->pKEYB->modified = true;
        }
    }
    pPC->Refresh_Display = true;
    pPC->update();
}

void DialogKeyList::moveLeft()
{
    int _step = QGuiApplication::queryKeyboardModifiers()==Qt::ShiftModifier ? 10 : 1;
    for (int i=0;i<lwKeys->count();i++) {
        if (lwKeys->item(i)->isSelected()) {
            pPC->pKEYB->Keys[i].Rect.adjust(-_step,0,-_step,0);
            pPC->pKEYB->modified = true;
        }
    }
    pPC->Refresh_Display = true;
    pPC->update();
}

void DialogKeyList::moveRight()
{
    int _step = QGuiApplication::queryKeyboardModifiers()==Qt::ShiftModifier ? 10 : 1;
    for (int i=0;i<lwKeys->count();i++) {
        if (lwKeys->item(i)->isSelected()) {
            pPC->pKEYB->Keys[i].Rect.adjust(_step,0,_step,0);
            pPC->pKEYB->modified = true;
        }
    }
    pPC->Refresh_Display = true;
    pPC->update();
}


void DialogKeyList::showAll()
{
    for (keyIter = pPC->pKEYB->Keys.begin(); keyIter != pPC->pKEYB->Keys.end(); ++keyIter)
    {
        listRect.append(keyIter->Rect);
    }
    pPC->Refresh_Display = true;
    pPC->update();
}

CKey DialogKeyList::findKey(QString desc) {
    for (int i=0; i < pPC->pKEYB->Keys.count();i++)
    {
        if (pPC->pKEYB->Keys.at(i).Description == desc)
        {
            return pPC->pKEYB->Keys.at(i);
        }
    }
    return CKey();
}

void DialogKeyList::slotSelectKey()
{
	// Find the correct Keys in List
    // Draw the Boundary
    listRect.clear();;

    for (int i=0;i<lwKeys->count();i++) {
        if (lwKeys->item(i)->isSelected()) {
            sbHor->setValue(pPC->pKEYB->Keys.at(i).Rect.width());
            sbVer->setValue(pPC->pKEYB->Keys.at(i).Rect.height());

            InsertRelKeys(pPC->pKEYB->Keys.at(i).ScanCode);
        }
    }
    pPC->Refresh_Display = true;
    pPC->update();
}

QList<QRect> DialogKeyList::getkeyFoundRect(void)
{
    return listRect;
}

void DialogKeyList::closeEvent(QCloseEvent *event)
{
	pPC->dialogkeylist=0;
	event->accept();
}
