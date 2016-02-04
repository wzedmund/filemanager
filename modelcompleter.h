#ifndef MODELCOMPLETER_H
#define MODELCOMPLETER_H

#include <QCompleter>

class ModelCompleter : public QCompleter
{
public:
    ModelCompleter();
    QStringList splitPath(const QString & path) const;
    QString pathFromIndex(const QModelIndex & index) const;
};

#endif // MODELCOMPLETER_H
