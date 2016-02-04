#ifndef TABBAR_H
#define TABBAR_H

#include <QFrame>
#include <QHBoxLayout>

class TabBarItem;

class TabBar : public QFrame
{
    Q_OBJECT
public:
    TabBar();
    void addTab(const QString &text);
    void removeTab(int index);
    int count();
    void doLayout();
private:
    void deSelectAll();
    QList<TabBarItem *> m_items;
signals:
    void tabBarClicked(int index);
};

#endif // TABBAR_H
