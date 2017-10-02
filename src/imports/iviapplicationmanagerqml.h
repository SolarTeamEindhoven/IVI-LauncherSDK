#ifndef IVIAPPLICATIONMANAGERQML_H
#define IVIAPPLICATIONMANAGERQML_H

#include <QObject>
#include <QQmlListProperty>
#include <QAbstractListModel>
#include <QHash>
#include <QMap>

#include <IVIApplication>

class IVIApplicationListModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit IVIApplicationListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<IVIApplication*> applications;

    void handleApplicationListChanged();
    int getApplicationIndex(IVIApplication*);
};

class IVIApplicationManagerQML : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractListModel* applications READ getApplicationList CONSTANT)
//    Q_PROPERTY(QQmlListProperty<IVIApplication> applications READ getApplicationList NOTIFY applicationListChanged)

public:
    explicit IVIApplicationManagerQML(QObject *parent = 0);

signals:
    void applicationListChanged();

private:
    static IVIApplicationListModel* getApplicationList();
//    QQmlListProperty<IVIApplication> getApplicationList() const;
};

#endif // IVIAPPLICATIONMANAGERQML_H
