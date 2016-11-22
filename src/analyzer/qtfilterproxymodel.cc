#include "qtfilterproxymodel.h"

#include "analyzercontroller.h"
#include "wtcommon/logger.h"
#include "wtcommon/datetimeutils.h"

#include <QStandardItemModel>

QTFilterProxyModel::QTFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    this->setSourceModel(new QStandardItemModel());
}

bool QTFilterProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    auto this_index = sourceModel()->index(sourceRow, 1, sourceParent);
    auto parent_index = sourceModel()->index(this_index.parent().row(), 1);

    return !parent_index.isValid()
            || !filter_pattern
            || std::regex_search(sourceModel()->data(this_index).toString().toStdString(), filter_pattern.get())
            || std::regex_search(sourceModel()->data(parent_index).toString().toStdString(), filter_pattern.get());

}

bool QTFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    int left_row  = left.row();
    int right_row = right.row();
    int compared_column = 2;

    QModelIndex left_idx = sourceModel()->index(left_row, compared_column, left.parent());
    QModelIndex right_idx = sourceModel()->index(right_row, compared_column, right.parent());

    qlonglong leftData = sourceModel()->data(left_idx).toLongLong();
    qlonglong rightData = sourceModel()->data(right_idx).toLongLong();

    return leftData < rightData;
}

void QTFilterProxyModel::setFilterRegExp(const boost::optional<std::regex> &filter)
{
    filter_pattern = filter;

    invalidateFilter();

    update_parents_values();
}


void QTFilterProxyModel::update_parents_values()
{
    total_time = 0;
    for (int r = 0; r < rowCount(); r++)
    {
        QModelIndex parent_index = index(r, 0);
        qlonglong total = 0;

        for (int child_row = 0; child_row < rowCount(parent_index); child_row++)
        {
            QVariant v = data(index(child_row, 2, parent_index));
            total += v.toLongLong();
        }

        total_time += total;

        setData(index(r, 2), QVariant(total));
        setData(index(r, 0), QVariant(QString::fromStdString(WT::time_to_display(std::chrono::seconds(total)))));
    }
}
