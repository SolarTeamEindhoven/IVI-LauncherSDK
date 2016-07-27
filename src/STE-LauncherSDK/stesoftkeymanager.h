#ifndef STESOFTKEYMANAGER_H
#define STESOFTKEYMANAGER_H

#include <QObject>

#include <STE-LauncherSDK/qtstelaunchersdkglobal.h>

#include <QQuickItem>
#include <QOpenGLContext>

QT_BEGIN_NAMESPACE

class QWaylandCompositor;
class STESoftKeyProvider;
class STESoftKey;
class STEAppInstance;

class Q_STE_LAUNCHERSDK_EXPORT STESoftKeyManager : public QObject
{
    Q_OBJECT

public:
    const QList<STESoftKey*>& getSoftkeyList();
    STESoftKey* fromID(uint32_t ID);
    uint32_t getUniqueID();
    QQuickItem* item();

    static STESoftKeyManager* instance();

private:
    explicit STESoftKeyManager(QObject *parent = 0);

    QQuickItem visualizer;
    bool pluginsLoaded;
    QList<STESoftKeyProvider*> providers;
    QList<STESoftKey*> softkeys;
    uint32_t maxID;

    void changeActiveApp(STEAppInstance* appInstance);
    void loadProviders();
};

QT_END_NAMESPACE

#endif // STESOFTKEYMANAGER_H
