#include "tabbar.h"
#include "tabbaritem.h"
#include <QDebug>

TabBar::TabBar()
{

}

void TabBar::addTab(const QString &text)
{
    deSelectAll();
    TabBarItem * item = new TabBarItem(this, text, m_items.size());
    m_items.append(item);
    item->setBackgroundColor(Qt::lightGray);
    item->setBorderColor(Qt::gray);
    item->setParent(this);
    item->show();
    doLayout();
}

void TabBar::removeTab(int index)
{
    TabBarItem * item = m_items.at(index);
    m_items.removeAt(index);
    delete item;
    doLayout();
}

int TabBar::count()
{
    return m_items.size();
}

void TabBar::doLayout()
{
    double growWidth = 0;
    double h = height();
    for(int i = 0; i < count(); i++)
    {
        TabBarItem * item = m_items.at(i);
        double w = item->width();
        item->setGeometry(growWidth, 0, w, h);
        growWidth += w - 6;
    }
}

void TabBar::deSelectAll()
{
    for(int i = 0; i < count(); i++)
    {
        TabBarItem * item = m_items.at(i);
        item->setBackgroundColor(Qt::white);
    }
}



