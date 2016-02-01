#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QTreeView>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QFileInfo>

class TreeFilterProxyModel;
class ViewFilterProxyModel;
class Model;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void treeViewSelectionChanged(QModelIndex current, QModelIndex previous);
    void listViewPressed(QModelIndex current);
    void openFile();
    void executeFile(QModelIndex index, bool run);
    void directoryLoaded();
private:
    QListView * m_listView;
    QTreeView * m_treeView;
    QTreeView * m_detailTreeView;
    QStackedWidget * m_stackedWidget;

    QDockWidget * m_dockTree;
    QVBoxLayout * m_vBoxLayout;

    TreeFilterProxyModel * m_treeModel;
    ViewFilterProxyModel * m_viewModel;

    Model * m_model;

    QItemSelectionModel *m_treeViewSelectionModel;
    QItemSelectionModel *m_listViewSelectionModel;

    QFileInfo m_currentIndex;

    QString m_startPath;
};

#endif // MAINWINDOW_H
