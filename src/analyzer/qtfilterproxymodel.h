#ifndef QTFILTERPROXYMODEL_H
#define QTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

#include <boost/optional.hpp>

#include <regex>

class QTFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    boost::optional<std::regex> filter_pattern;

    void update_parents_values();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

public:
    QTFilterProxyModel(QObject *parent = 0);
    void setFilterRegExp(const boost::optional<std::regex> &filter);
};

#endif // QTFILTERPROXYMODEL_H
