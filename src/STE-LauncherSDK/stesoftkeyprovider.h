#ifndef STESOFTKEYPROVIDER_H
#define STESOFTKEYPROVIDER_H

#include <STE-LauncherSDK/qtstelaunchersdkglobal.h>
#include <QOpenGLContext>

QT_BEGIN_NAMESPACE

class QWaylandCompositor;
class STESoftKey;
class QQuickItem;

class Q_STE_LAUNCHERSDK_EXPORT STESoftKeyProvider : public QObject
{
    Q_OBJECT

public:
    STESoftKeyProvider();

    virtual QList<STESoftKey*> createSoftKeys() = 0;
    virtual QQuickItem* createSoftKeyVisualization();
};

QT_END_NAMESPACE

#endif // STESOFTKEYPROVIDER_H
