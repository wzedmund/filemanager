#include "searchbar.h"
#include <QKeyEvent>
#include <QDebug>
#include <QCompleter>
#include <QDirModel>
#include <QListWidgetItem>

SearchBar::SearchBar()
{
    m_lineEdit = new QLineEdit;
    m_lineEdit->installEventFilter(this);
    m_historyAction = new QAction("h",m_lineEdit);
    m_lineEdit->addAction(m_historyAction, QLineEdit::TrailingPosition);
    m_popupList = new QListWidget;
    m_popupList->setStyleSheet("QListWidget::item:hover {background-color:lightGray;}");
    m_menuLayout = new QHBoxLayout;
    m_menuLayout->addWidget(m_popupList);
    m_menuLayout->setContentsMargins(0,0,0,0);
    m_menu = new QMenu;
    m_menu->setLayout(m_menuLayout);

    m_hBoxLayout = new QHBoxLayout;
    m_hBoxLayout->addWidget(m_lineEdit);
    m_hBoxLayout->setContentsMargins(0,0,0,0);
    m_hBoxLayout->setSpacing(0);
    setLayout(m_hBoxLayout);
    connect(m_historyAction, SIGNAL(triggered(bool)), SLOT(doOpenPopupList(bool)));
    connect(m_popupList, SIGNAL(currentRowChanged(int)), this, SLOT(doCurrentRowChanged(int)));
    connect(m_lineEdit, SIGNAL(returnPressed()), this, SLOT(doReturnPressed()));
}

SearchBar::~SearchBar()
{

}

void SearchBar::keyPressEvent(QKeyEvent *event)
{
    QFrame::keyPressEvent(event);
}

void SearchBar::focusOutEvent(QFocusEvent *e)
{
    QFrame::focusOutEvent(e);
}

void SearchBar::setCompleter(QCompleter *completer)
{
    m_lineEdit->setCompleter(completer);
}

QString SearchBar::itemText(int index) const
{
    QListWidgetItem * item = m_popupList->item(index);
    if(item)
        return item->text();
    return "";
}

void SearchBar::insertItem(int index, const QString &text)
{
    m_popupList->insertItem(index, text);
}

QString SearchBar::currentText() const
{
    return m_lineEdit->text();
}

void SearchBar::setCurrentIndex(int index)
{
    m_lineEdit->setText(itemText(index));
    emit currentTextChanged(m_lineEdit->text());
    m_popupList->clearSelection();
    m_menu->hide();
}

bool SearchBar::eventFilter(QObject *object, QEvent *event)
{
    if(object == m_lineEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if(keyEvent->key() == Qt::Key_Tab)
            {
                qDebug() << m_lineEdit->completer()->currentCompletion();
                m_lineEdit->setText(m_lineEdit->completer()->currentCompletion());
                emit m_lineEdit->returnPressed();
            }
        }
    }
    return false;
}

void SearchBar::doOpenPopupList(bool v)
{
    Q_UNUSED(v);
    QRect rect = m_lineEdit->geometry();
    QPoint widgetPos = mapToGlobal(rect.bottomLeft());
    m_menu->setFixedWidth(m_lineEdit->width());
    m_menu->exec(widgetPos);
}

void SearchBar::doCurrentRowChanged(int index)
{
    qDebug() << "doCurrentRowChanged";
    setCurrentIndex(index);
}

void SearchBar::doReturnPressed()
{
    emit currentTextChanged(m_lineEdit->text());
}
