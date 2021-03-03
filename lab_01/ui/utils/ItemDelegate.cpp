//
// Created by gregory on 13.02.2021.
//

#include <QLineEdit>
#include <QIntValidator>
#include "ItemDelegate.h"

ItemDelegate::ItemDelegate(QObject *parent) :
        QItemDelegate(parent) {
}

QWidget *ItemDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const {
    QLocale locale(QLocale::C);
    locale.setNumberOptions(QLocale::RejectGroupSeparator);

    auto validator = new QDoubleValidator;
    validator->setLocale(locale);

    auto *editor = new QLineEdit(parent);
    editor->setValidator(validator);
    return editor;
}


void ItemDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const {
    QString value = index.model()->data(index, Qt::EditRole).toString();
    auto *line = dynamic_cast<QLineEdit *>(editor);
    line->setText(value);
}


void ItemDelegate::setModelData(QWidget *editor,
                                QAbstractItemModel *model,
                                const QModelIndex &index) const {
    auto *line = dynamic_cast<QLineEdit *>(editor);
    QString value = line->text();
    model->setData(index, value);
}


void ItemDelegate::updateEditorGeometry(QWidget *editor,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}