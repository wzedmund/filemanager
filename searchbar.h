#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QComboBox>
#include <QCompleter>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QHBoxLayout>
#include <QMenu>

class SearchBar : public QFrame
{
    Q_OBJECT
public:
    SearchBar();
    ~SearchBar();
    void keyPressEvent(QKeyEvent *event);
    void focusOutEvent(QFocusEvent *e);
    void setCompleter(QCompleter * completer);
    QString itemText(int index) const;
    void insertItem(int index, const QString &text);
    QString	currentText() const;
    void setCurrentIndex(int index);
    bool eventFilter(QObject *object, QEvent *event);
private:
    QLineEdit * m_lineEdit;
    QMenu * m_menu;
    QListWidget * m_popupList;
    QHBoxLayout * m_hBoxLayout;
    QHBoxLayout * m_menuLayout;
    QAction * m_historyAction;
signals:
    void currentTextChanged(QString text);
public slots:
    void doOpenPopupList(bool v);
    void doCurrentRowChanged(int index);
    void doReturnPressed();
};

#endif // SEARCHBAR_H
