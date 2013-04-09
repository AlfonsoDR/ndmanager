#ifndef PARAMETERTREE_H
#define PARAMETERTREE_H

#include <QTreeWidget>

class ParameterTreeItem : public QTreeWidgetItem
{
public:
    ParameterTreeItem(QTreeWidget * parent);
    ParameterTreeItem(QTreeWidgetItem *parent);
    ~ParameterTreeItem();

    void setWidget(QWidget *w);
    QWidget *widget() const;
private:
    QWidget *mWidget;
};

class ParameterTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ParameterTree(QWidget *parent = 0);
    ~ParameterTree();

    QTreeWidgetItem* addPage(const QString &icon, const QString &name, QWidget *page);
    QTreeWidgetItem* addSubPage(QTreeWidgetItem *parentItem, const QString &name, QWidget *page);
Q_SIGNALS:
    void showWidgetPage(QWidget *);
private Q_SLOTS:
    void slotItemClicked(QTreeWidgetItem*,int);
};

#endif // PARAMETERTREE_H