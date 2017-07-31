#ifndef STEAPPINSTANCE_H
#define STEAPPINSTANCE_H

#include <QObject>

#include <IVI-LauncherSDK/qtstelaunchersdkglobal.h>

QT_BEGIN_NAMESPACE

class STEApp;
class STEShellSurface_wl;

class Q_STE_LAUNCHERSDK_EXPORT STEAppInstance: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(STEAppInstance)

public:
    virtual ~STEAppInstance();

    enum SizeState : uint32_t {
        sizeState_normal = 0,
        sizeState_minimal = 1,
        sizeState_maximal = 2
    };
    Q_ENUM(SizeState)

    STEApp* getApp() const;
    virtual quint64 getPID() const;

    const QList<STEShellSurface_wl*>& getShellsurfaces() { return shellsurfaces; }

    static STEAppInstance* fromPID(qint64 PID);

signals:
    void shellSurfaceAdded(STEShellSurface_wl* shellSurface);
    void resizeRequest(SizeState sizeState);

private slots:
    void removeShellSurface();

protected:
    STEAppInstance(STEApp* app, QObject* parent = nullptr);

private:
    STEApp* app;
    QList<STEShellSurface_wl*> shellsurfaces;

    void addShellSurface(STEShellSurface_wl* shellsurface);
    friend class STECompositor;
};

QT_END_NAMESPACE

#endif // STEAPPINSTANCE_H
