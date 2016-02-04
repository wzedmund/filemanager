#ifndef TABBARITEM_H
#define TABBARITEM_H

#include <QLabel>

class TabBar;

class TabBarItem : public QLabel
{
public:
    TabBarItem(TabBar * tabBar, const QString & text, int index);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void setBackgroundColor(QColor color);
    void setBorderColor(QColor color);
private:
    TabBar * m_tabBar;
    int m_index;
    QColor m_backgroundColor;
    QColor m_borderColor;
};

#endif // TABBARITEM_H
