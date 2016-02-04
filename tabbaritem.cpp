#include "tabbaritem.h"
#include "tabbar.h"
#include <QDebug>
#include <QPainter>

TabBarItem::TabBarItem(TabBar * tabBar, const QString & text, int index)
{
    m_tabBar = tabBar;
    m_index = index;
    setText(text);
    setStyleSheet("font: 13px;min-width: 20px; margin-left: 5px;margin-right: 5px");
    setMargin(0);
}

void TabBarItem::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit m_tabBar->tabBarClicked(m_index);
}

void TabBarItem::paintEvent(QPaintEvent *event)
{
    qDebug() << "paint tab";
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    double w = width();
    double h = height();
    QPolygon polygon;
    polygon << QPoint(0, 0);
    polygon << QPoint(w -5, 0);
    polygon << QPoint(w, h/2);
    polygon << QPoint(w -5, h);
    polygon << QPoint(0, h);
    polygon << QPoint(0 + 5, h/2);
    QPen pen(m_borderColor);
    pen.setWidthF(1.2);
    painter.setPen(pen);
    painter.setBrush(m_backgroundColor);
    painter.drawPolygon(polygon);
    painter.end();
    QLabel::paintEvent(event);
}

void TabBarItem::setBackgroundColor(QColor color)
{
    m_backgroundColor = color;
}

void TabBarItem::setBorderColor(QColor color)
{
    m_borderColor = color;
}

