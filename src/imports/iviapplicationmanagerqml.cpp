#include "iviapplicationmanagerqml.h"

#include <IVI-Launcher/private/iviapplicationmanager_p.h>
#include <IVI-Launcher/private/ivisingletonmanager_p.h>

#include <QVariant>

#include <vector>
#include <algorithm>

namespace {
static IVIApplicationListModel& getGlobalApplicationList() {
    static IVIApplicationListModel applicationListModel;
    return applicationListModel;
}
enum Roles : int {
    Application,
    Name,
    Description,
    Icon,
    Website,
    Categories,
    Executable,
    Arguments,
    WorkingDirectory,
    RunningState
};
}

IVIApplicationListModel::IVIApplicationListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    connect(&IVIApplicationManager::getInstance(), &IVIApplicationManager::applicationListChanged, this, &IVIApplicationListModel::handleApplicationListChanged);
    {
        auto applicationsList = IVIApplicationManager::getInstance().getApplicationList();
        applications.reserve(applicationsList.size());
        foreach(auto application, applicationsList)
            applications.push_back(application);
    }
}

int IVIApplicationListModel::rowCount(const QModelIndex& parent) const {
    return applications.size();
}

QVariant IVIApplicationListModel::data(const QModelIndex& index, int role) const {
    IVIApplication* application = applications[index.row()];
    switch(role) {
    case Roles::Application:
        return QVariant::fromValue(application);
    case Roles::Name:
        return QVariant::fromValue(application->getName());
    case Roles::Description:
        return QVariant::fromValue(application->getDescription());
    case Roles::Icon:
        return QVariant::fromValue(application->getIcon());
    case Roles::Website:
        return QVariant::fromValue(application->getWebsite());
    case Roles::Categories:
        return QVariant::fromValue(application->getCategories());
    case Roles::Executable:
        return QVariant::fromValue(application->getExecutable());
    case Roles::Arguments:
        return QVariant::fromValue(application->getArguments());
    case Roles::WorkingDirectory:
        return QVariant::fromValue(application->getWorkingDirectory());
    case Roles::RunningState:
        return QVariant::fromValue(application->getRunningState());
    default:
        return QVariant::Invalid;
    }
}

QVariant IVIApplicationListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if(orientation != Qt::Horizontal)
        return QAbstractListModel::headerData(section, orientation, role);

    switch(role) {
    case Roles::Application:
        return QVariant::fromValue(QStringLiteral("Application"));
    case Roles::Name:
        return QVariant::fromValue(QStringLiteral("Name"));
    case Roles::Description:
        return QVariant::fromValue(QStringLiteral("Description"));
    case Roles::Icon:
        return QVariant::fromValue(QStringLiteral("Icon"));
    case Roles::Website:
        return QVariant::fromValue(QStringLiteral("Website"));
    case Roles::Categories:
        return QVariant::fromValue(QStringLiteral("Categories"));
    case Roles::Executable:
        return QVariant::fromValue(QStringLiteral("Executable"));
    case Roles::Arguments:
        return QVariant::fromValue(QStringLiteral("Arguments"));
    case Roles::WorkingDirectory:
        return QVariant::fromValue(QStringLiteral("WorkingDirectory"));
    case Roles::RunningState:
        return QVariant::fromValue(QStringLiteral("RunningState"));
    default:
        return QVariant::Invalid;
    }
}

QHash<int, QByteArray> IVIApplicationListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles.insert(Roles::Application, "application");
    roles.insert(Roles::Name, "name");
    roles.insert(Roles::Description, "description");
    roles.insert(Roles::Icon, "icon");
    roles.insert(Roles::Website, "website");
    roles.insert(Roles::Categories, "categories");
    roles.insert(Roles::Executable, "executable");
    roles.insert(Roles::Arguments, "arguments");
    roles.insert(Roles::WorkingDirectory, "workingDirectory");
    roles.insert(Roles::RunningState, "runningState");
    return roles;
}

void IVIApplicationListModel::handleApplicationListChanged() {
    auto newApplicationsList = IVIApplicationManager::getInstance().getApplicationList();

    std::vector<int> foundIndex;
    std::vector<IVIApplication*> appendList;
    for(IVIApplication* application : newApplicationsList) {
        auto i = getApplicationIndex(application);
        if(0 <= i) {
            foundIndex.push_back(i);
        } else {
            appendList.push_back(application);
        }
    }
    std::sort(foundIndex.begin(), foundIndex.end());

    int lastIndex = 0;
    for(auto i : foundIndex) {
        if(lastIndex < i) {
            beginRemoveRows(QModelIndex(), lastIndex, i-1);
            applications.erase(applications.begin() + lastIndex, applications.begin() + i-1);
            endRemoveRows();
        }
        lastIndex = i + 1;
    }
    if(lastIndex != applications.size()) {
        beginRemoveRows(QModelIndex(), lastIndex, applications.size()-1);
        applications.erase(applications.begin() + lastIndex, applications.end());
        endRemoveRows();
    }

    if(0 < appendList.size()) {
        beginInsertRows(QModelIndex(), applications.size(), applications.size() + appendList.size()-1);
        applications.insert(applications.end(), appendList.begin(), appendList.end());
        endInsertRows();
    }
}

int IVIApplicationListModel::getApplicationIndex(IVIApplication* application) {
    for(int i = 0; i < applications.size(); i++) {
        if(applications[i] == application)
            return i;
    }
    return -1;
}


IVIApplicationManagerQML::IVIApplicationManagerQML(QObject *parent)
    : QObject(parent)
{
    IVISingletonManager::createQmlCompositor(nullptr);
    connect(&IVIApplicationManager::getInstance(), &IVIApplicationManager::applicationListChanged, this, &IVIApplicationManagerQML::applicationListChanged);
}

namespace {
static int CountFunction(QQmlListProperty<IVIApplication>*) {
    return IVIApplicationManagerPrivate::getApplications().count();
}

static IVIApplication* AtFunction(QQmlListProperty<IVIApplication>*, int index) {
    return IVIApplicationManagerPrivate::getApplications().at(index);
}
}

IVIApplicationListModel* IVIApplicationManagerQML::getApplicationList() {
    return &getGlobalApplicationList();
}
/*
QQmlListProperty<IVIApplication> IVIApplicationManagerQML::getApplicationList() const {
    return QQmlListProperty<IVIApplication>(&IVIApplicationManager::getInstance(), nullptr, CountFunction, AtFunction);
}
*/
