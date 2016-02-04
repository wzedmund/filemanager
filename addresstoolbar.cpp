#include "addresstoolbar.h"
#include "searchbar.h"
#include "tabbar.h"
#include <QDirModel>
#include <QDebug>

AddressToolBar::AddressToolBar()
{
    m_searchBar = new SearchBar;
    m_completer = new QCompleter;
    m_searchBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_completer->setModel(new QDirModel(m_completer));
    m_completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    m_completer->setMaxVisibleItems(10);
    m_searchBar->setCompleter(m_completer);
    connect(m_completer, SIGNAL(activated(QString)), this, SLOT(doActivated(QString)));
    m_tabBar = new TabBar;
    m_stackedWidget = new QStackedWidget;
    m_button = new QPushButton("S");
    m_hBoxLayout = new QHBoxLayout;

    m_stackedWidget->addWidget(m_searchBar);
    m_stackedWidget->addWidget(m_tabBar);

    m_hBoxLayout->addWidget(m_stackedWidget);
    m_hBoxLayout->addWidget(m_button);

    setLayout(m_hBoxLayout);

    connect(m_button, SIGNAL(released()), this, SLOT(addressSwitched()));
    connect(m_searchBar, SIGNAL(currentTextChanged(QString)), this, SLOT(doActivated(QString)));
    connect(m_tabBar, SIGNAL(tabBarClicked(int)), this, SLOT(doTabBarClicked(int)));
}

AddressToolBar::~AddressToolBar()
{

}

QString AddressToolBar::itemText(int index) const
{
    return m_searchBar->itemText(index);
}

void AddressToolBar::insertItem(int index, const QString &text)
{
    m_searchBar->insertItem(index, text);
}

QString AddressToolBar::currentText() const
{
    return m_searchBar->currentText();
}

void AddressToolBar::setCurrentIndex(int index)
{
    m_searchBar->setCurrentIndex(index);
}

void AddressToolBar::setTabBarText(QString text)
{
    m_currentAddress = text;
    QStringList list;
    list.append(text.split("/"));
    list.replace(0, "/");
    list.removeAll("");
    clearAllTabs();
    m_addressOffset = (list.size() < 3) ? 0 : list.size() - 3;
    for(int i = (list.size() < 3) ? 0 : list.size() - 3; i < list.size(); i++)
        m_tabBar->addTab((QString)list.at(i));
}

void AddressToolBar::addressSwitched()
{
    if(m_button->text() == "S")
    {
        m_stackedWidget->setCurrentIndex(1);
        m_button->setText("A");
    }
    else
    {
        m_stackedWidget->setCurrentIndex(0);
        m_button->setText("S");
    }
}

void AddressToolBar::doActivated(QString text)
{
    qDebug() << "emit"<<text;
    emit activated(text);
}

void AddressToolBar::doTabBarClicked(int index)
{
    QString tabText;
    QStringList list = m_currentAddress.split("/");
    list.replace(0, "/");
    list.removeAll("");

    tabText += list.at(0);
    for(int i = 1; i <= index + m_addressOffset; i++)
        tabText += list.at(i) + "/";

    if(tabText != "/")
        tabText.remove(tabText.length() -1 ,1);

    emit activated(tabText);
}

void AddressToolBar::clearAllTabs()
{
    for(int i = 0; i < m_tabBar->count();)
    {
        qDebug() << m_tabBar->count();
        m_tabBar->removeTab(i);
    }
}

