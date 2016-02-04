#ifndef ADDRESSTOOLBAR_H
#define ADDRESSTOOLBAR_H

#include <QFrame>
#include <QStackedWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCompleter>

class SearchBar;
class TabBar;

class AddressToolBar : public QFrame
{
    Q_OBJECT
public:
    AddressToolBar();
    ~AddressToolBar();
    QString itemText(int index) const;
    void insertItem(int index, const QString &text);
    QString	currentText() const;
    void setCurrentIndex(int index);
    void setTabBarText(QString text);
public slots:
    void addressSwitched();
    void doActivated(QString text);
    void doTabBarClicked(int index);
signals:
    void activated(QString text);
private:
    void clearAllTabs();
    QCompleter * m_completer;
    SearchBar * m_searchBar;
    TabBar * m_tabBar;
    QStackedWidget * m_stackedWidget;
    QPushButton * m_button;
    QHBoxLayout * m_hBoxLayout;
    QString m_currentAddress;
    int m_addressOffset;
};

#endif // ADDRESSTOOLBAR_H
