//
// Created by gregory on 13.02.2021.
//

#ifndef LAB_01_ITEMDELEGATE_H
#define LAB_01_ITEMDELEGATE_H


#include <QItemDelegate>

class ItemDelegate : public QItemDelegate
{
Q_OBJECT
public:
    explicit ItemDelegate(QObject *parent = 0);

protected:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget * editor, const QModelIndex & index) const override;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const override;
    void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const override;
};


#endif //LAB_01_ITEMDELEGATE_H
