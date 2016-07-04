#include "steappcontainer.h"

#include <QWaylandQuickItem>
#include <QSGClipNode>
#include <QQmlEngine>
#include <QDebug>

#include "wayland/steshellsurface_wl_p.h"
#include "steapp.h"
#include "steappinstance.h"
#include "stecompositor.h"

static QList<STEAppContainer*> STEAppContainerList;

//*
STEAppContainer::STEAppContainer(QQuickItem* parent)
    : QQuickItem(parent)
    , app(nullptr)
{
    STEAppContainerList.append(this);
    setFlag(QQuickItem::ItemHasContents);
    setVisible(true);
}

STEAppContainer::~STEAppContainer()
{
    STEAppContainerList.removeOne(this);
}

QList<STEAppContainer*>& STEAppContainer::getSTEAppContainerList()
{
    return STEAppContainerList;
}

void STEAppContainer::setApp(STEApp* newApp)
{
    if(app == newApp)
        return;

    if(app != nullptr)
    {
        disconnect(app, &STEApp::appInstanceAdded, this, &STEAppContainer::addAppInstance);
        foreach(STEAppInstance* appInstance, app->getInstances())
        {
            removeAppInstance(appInstance);
        }
    }

    foreach(QWaylandQuickItem* waylandItem, waylandItems)
    {
        waylandItem->deleteLater();
    }

    app = newApp;

    connect(app, &STEApp::appInstanceAdded, this, &STEAppContainer::addAppInstance);
    foreach(STEAppInstance* appInstance, app->getInstances())
    {
        addAppInstance(appInstance);
    }

    emit appChanged();
}

void STEAppContainer::addAppInstance(STEAppInstance* appInstance)
{
    qDebug() << "Adding app instance" << static_cast<void*>(appInstance) << "to app container";

    connect(appInstance, &STEAppInstance::destroyed, this, (void (STEAppContainer::*)())&STEAppContainer::removeAppInstance);
    connect(appInstance, &STEAppInstance::shellSurfaceAdded, this, &STEAppContainer::addShellSurface);
    foreach(STEShellSurface_wl* shellSurface, appInstance->getShellsurfaces())
    {
        addShellSurface(shellSurface);
    }
}

void STEAppContainer::removeAppInstance()
{
    STEAppInstance* appInstance = qobject_cast<STEAppInstance*>(sender());

    if(appInstance == nullptr)
        return;

    removeAppInstance(appInstance);
}

void STEAppContainer::removeAppInstance(STEAppInstance* appInstance)
{
    qDebug() << "Removing app instance to app container";

    disconnect(appInstance, &STEAppInstance::shellSurfaceAdded, this, &STEAppContainer::addShellSurface);
    foreach(STEShellSurface_wl* shellSurface, appInstance->getShellsurfaces())
    {
        removeShellSurface(shellSurface);
    }
}

void STEAppContainer::addShellSurface(STEShellSurface_wl* shellSurface)
{
    qDebug() << "Adding shell surface to app container";

    connect(shellSurface, &QObject::destroyed, this, (void (STEAppContainer::*)())&STEAppContainer::removeShellSurface);

    QWaylandQuickItem* waylandQuickItem = new QWaylandQuickItem();

    if(waylandQuickItem == nullptr)
        return;

    connect(waylandQuickItem, &QWaylandQuickItem::widthChanged, shellSurface, &STEShellSurface_wl::updateWidth);
    connect(waylandQuickItem, &QWaylandQuickItem::heightChanged, shellSurface, &STEShellSurface_wl::updateHeight);

    waylandQuickItem->setParentItem(this);

    waylandQuickItem->setSurface(shellSurface->getSurface());
    QObject* anchors = waylandQuickItem->property("anchors").value<QObject*>();
    anchors->setProperty("fill", QVariant::fromValue<QQuickItem*>(this));

    waylandItems.append(waylandQuickItem);

    update();
}

void STEAppContainer::removeShellSurface()
{
    STEShellSurface_wl* shellSurface = qobject_cast<STEShellSurface_wl*>(sender());

    if(shellSurface == nullptr)
        return;

    removeShellSurface(shellSurface);
}

void STEAppContainer::removeShellSurface(STEShellSurface_wl* shellSurface)
{
    qDebug() << "Removing shell surface from app container";

    foreach(QWaylandQuickItem* waylandQuickItem, waylandItems)
    {
        if(waylandQuickItem->surface() == shellSurface->getSurface())
        {
            disconnect(waylandQuickItem, &QWaylandQuickItem::widthChanged, shellSurface, &STEShellSurface_wl::updateWidth);
            disconnect(waylandQuickItem, &QWaylandQuickItem::heightChanged, shellSurface, &STEShellSurface_wl::updateHeight);

            waylandQuickItem->deleteLater();
            waylandItems.removeAll(waylandQuickItem);
        }
    }
}

//*/

/*
#include "steappcontainer.h"

#include <QWaylandQuickSurface>
#include <QWaylandInputMethodControl>
#include <QWaylandTextInput>
#include <QWaylandCompositor>
#include <qwaylandinput.h>
//#include <QtWaylandCompositor/private/qwlclientbufferintegration_p.h>

#include <QKeyEvent>
#include <QGuiApplication>
#include <QScreen>

#include <QSGSimpleTextureNode>
#include <QQuickWindow>

#include <QMutexLocker>
#include <QMutex>

#include <wayland-server.h>
#include <QThread>

#include <QOpenGLFunctions>

#ifndef GL_TEXTURE_EXTERNAL_OES
#define GL_TEXTURE_EXTERNAL_OES 0x8D65
#endif

class QWaylandBufferMaterialShader : public QSGMaterialShader
{
public:
    QWaylandBufferMaterialShader(QWaylandBufferRef::BufferFormatEgl format);

    void updateState(const RenderState &state, QSGMaterial *newEffect, QSGMaterial *oldEffect) Q_DECL_OVERRIDE;
    char const *const *attributeNames() const Q_DECL_OVERRIDE;

protected:
    void initialize() Q_DECL_OVERRIDE;

private:
    const QWaylandBufferRef::BufferFormatEgl m_format;
    int m_id_matrix;
    int m_id_opacity;
    QVarLengthArray<int, 3> m_id_tex;
};

class QWaylandBufferMaterial : public QSGMaterial
{
public:
    QWaylandBufferMaterial(QWaylandBufferRef::BufferFormatEgl format);
    ~QWaylandBufferMaterial();

    void setTextureForPlane(int plane, uint texture);

    void bind();

    QSGMaterialType *type() const Q_DECL_OVERRIDE;
    QSGMaterialShader *createShader() const Q_DECL_OVERRIDE;

private:
    void setTextureParameters(GLenum target);
    void ensureTextures(int count);

    const QWaylandBufferRef::BufferFormatEgl m_format;
    QVarLengthArray<GLuint, 3> m_textures;
};

static const struct {
    const char * const vertexShaderSourceFile;
    const char * const fragmentShaderSourceFile;
    GLenum textureTarget;
    int planeCount;
    bool canProvideTexture;
    QSGMaterial::Flags materialFlags;
    QSGMaterialType materialType;
} bufferTypes[] = {
    // BufferFormatEgl_Null
    { "", "", 0, 0, false, 0, {} },

    // BufferFormatEgl_RGB
    {
        ":/qt-project.org/wayland/compositor/shaders/surface.vert",
        ":/qt-project.org/wayland/compositor/shaders/surface_rgbx.frag",
        GL_TEXTURE_2D, 1, true,
        QSGMaterial::Blending,
        {}
    },

    // BufferFormatEgl_RGBA
    {
        ":/qt-project.org/wayland/compositor/shaders/surface.vert",
        ":/qt-project.org/wayland/compositor/shaders/surface_rgba.frag",
        GL_TEXTURE_2D, 1, true,
        QSGMaterial::Blending,
        {}
    },

    // BufferFormatEgl_EXTERNAL_OES
    {
        ":/qt-project.org/wayland/compositor/shaders/surface.vert",
        ":/qt-project.org/wayland/compositor/shaders/surface_oes_external.frag",
        GL_TEXTURE_EXTERNAL_OES, 1, false,
        QSGMaterial::Blending,
        {}
    },

    // BufferFormatEgl_Y_U_V
    {
        ":/qt-project.org/wayland/compositor/shaders/surface.vert",
        ":/qt-project.org/wayland/compositor/shaders/surface_y_u_v.frag",
        GL_TEXTURE_2D, 3, false,
        QSGMaterial::Blending,
        {}
    },

    // BufferFormatEgl_Y_UV
    {
        ":/qt-project.org/wayland/compositor/shaders/surface.vert",
        ":/qt-project.org/wayland/compositor/shaders/surface_y_uv.frag",
        GL_TEXTURE_2D, 2, false,
        QSGMaterial::Blending,
        {}
    },

    // BufferFormatEgl_Y_XUXV
    {
        ":/qt-project.org/wayland/compositor/shaders/surface.vert",
        ":/qt-project.org/wayland/compositor/shaders/surface_y_xuxv.frag",
        GL_TEXTURE_2D, 2, false,
        QSGMaterial::Blending,
        {}
    }
};

QWaylandBufferMaterialShader::QWaylandBufferMaterialShader(QWaylandBufferRef::BufferFormatEgl format)
    : QSGMaterialShader()
    , m_format(format)
{
    setShaderSourceFile(QOpenGLShader::Vertex, QString::fromLatin1(bufferTypes[format].vertexShaderSourceFile));
    setShaderSourceFile(QOpenGLShader::Fragment, QString::fromLatin1(bufferTypes[format].fragmentShaderSourceFile));
}

void QWaylandBufferMaterialShader::updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newEffect, QSGMaterial *oldEffect)
{
    QSGMaterialShader::updateState(state, newEffect, oldEffect);

    QWaylandBufferMaterial *material = static_cast<QWaylandBufferMaterial *>(newEffect);
    material->bind();

    if (state.isMatrixDirty())
        program()->setUniformValue(m_id_matrix, state.combinedMatrix());

    if (state.isOpacityDirty())
        program()->setUniformValue(m_id_opacity, state.opacity());
}

const char * const *QWaylandBufferMaterialShader::attributeNames() const
{
    static char const *const attr[] = { "qt_VertexPosition", "qt_VertexTexCoord", 0 };
    return attr;
}

void QWaylandBufferMaterialShader::initialize()
{
    QSGMaterialShader::initialize();

    m_id_matrix = program()->uniformLocation("qt_Matrix");
    m_id_opacity = program()->uniformLocation("qt_Opacity");

    for (int i = 0; i < bufferTypes[m_format].planeCount; i++) {
        m_id_tex << program()->uniformLocation("tex" + QByteArray::number(i));
        program()->setUniformValue(m_id_tex[i], i);
    }

    Q_ASSERT(m_id_tex.size() == bufferTypes[m_format].planeCount);
}

QWaylandBufferMaterial::QWaylandBufferMaterial(QWaylandBufferRef::BufferFormatEgl format)
    : QSGMaterial()
    , m_format(format)
{
    QOpenGLFunctions *gl = QOpenGLContext::currentContext()->functions();

    gl->glBindTexture(bufferTypes[m_format].textureTarget, 0);
    setFlag(bufferTypes[m_format].materialFlags);
}

QWaylandBufferMaterial::~QWaylandBufferMaterial()
{
    QOpenGLFunctions *gl = QOpenGLContext::currentContext()->functions();

    for (GLuint texture : m_textures)
        gl->glDeleteTextures(1, &texture);
}

void QWaylandBufferMaterial::setTextureForPlane(int plane, uint texture)
{
    if (plane < 0 || plane >= bufferTypes[m_format].planeCount) {
        qWarning("plane index is out of range");
        return;
    }

    QOpenGLFunctions *gl = QOpenGLContext::currentContext()->functions();
    const GLenum target = bufferTypes[m_format].textureTarget;

    gl->glBindTexture(target, texture);
    setTextureParameters(target);

    ensureTextures(plane - 1);

    if (m_textures.size() <= plane) {
        m_textures << texture;
    } else {
        std::swap(m_textures[plane], texture);
        gl->glDeleteTextures(1, &texture);
    }
}

void QWaylandBufferMaterial::bind()
{
    QOpenGLFunctions *gl = QOpenGLContext::currentContext()->functions();
    const GLenum target = bufferTypes[m_format].textureTarget;

    ensureTextures(bufferTypes[m_format].planeCount);

    switch (m_textures.size()) {
    case 3:
        gl->glActiveTexture(GL_TEXTURE2);
        gl->glBindTexture(target, m_textures[2]);
    case 2:
        gl->glActiveTexture(GL_TEXTURE1);
        gl->glBindTexture(target, m_textures[1]);
    case 1:
        gl->glActiveTexture(GL_TEXTURE0);
        gl->glBindTexture(target, m_textures[0]);
    }
}

QSGMaterialType *QWaylandBufferMaterial::type() const
{
    return const_cast<QSGMaterialType *>(&bufferTypes[m_format].materialType);
}

QSGMaterialShader *QWaylandBufferMaterial::createShader() const
{
    return new QWaylandBufferMaterialShader(m_format);
}


void QWaylandBufferMaterial::setTextureParameters(GLenum target)
{
    QOpenGLFunctions *gl = QOpenGLContext::currentContext()->functions();
    gl->glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl->glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl->glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl->glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

//TODO move this into a separate centralized texture management class
void QWaylandBufferMaterial::ensureTextures(int count)
{
    QOpenGLFunctions *gl = QOpenGLContext::currentContext()->functions();
    const GLenum target = bufferTypes[m_format].textureTarget;
    GLuint texture;

    for (int plane = m_textures.size(); plane < count; plane++) {
        gl->glGenTextures(1, &texture);
        gl->glBindTexture(target, texture);
        setTextureParameters(target);
        m_textures << texture;
    }
}

QMutex *STEAppContainer::mutex = 0;

class QWaylandSurfaceTextureProvider : public QSGTextureProvider
{
public:
    QWaylandSurfaceTextureProvider()
        : m_smooth(false)
        , m_sgTex(0)
    {
    }

    ~QWaylandSurfaceTextureProvider()
    {
        if (m_sgTex)
            m_sgTex->deleteLater();
    }

    void setBufferRef(STEAppContainer *surfaceItem, const QWaylandBufferRef &buffer)
    {
        Q_ASSERT(QThread::currentThread() == thread());
        m_ref = buffer;
        delete m_sgTex;
        m_sgTex = 0;
        if (m_ref.hasBuffer()) {
            if (buffer.isShm()) {
                m_sgTex = surfaceItem->window()->createTextureFromImage(buffer.image());
                if (m_sgTex) {
                    m_sgTex->bind();
                }
            } else {
                QQuickWindow::CreateTextureOptions opt = QQuickWindow::TextureOwnsGLTexture;
                QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface *>(surfaceItem->surface());
                if (surface && surface->useTextureAlpha()) {
                    opt |= QQuickWindow::TextureHasAlphaChannel;
                }

                GLuint texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                buffer.bindToTexture();
                m_sgTex = surfaceItem->window()->createTextureFromId(texture , QSize(surfaceItem->width(), surfaceItem->height()), opt);
            }
        }
        emit textureChanged();
    }

    QSGTexture *texture() const Q_DECL_OVERRIDE
    {
        if (m_sgTex)
            m_sgTex->setFiltering(m_smooth ? QSGTexture::Linear : QSGTexture::Nearest);
        return m_sgTex;
    }

    void setSmooth(bool smooth) { m_smooth = smooth; }
private:
    bool m_smooth;
    QSGTexture *m_sgTex;
    QWaylandBufferRef m_ref;
};


STEAppContainer::STEAppContainer(QQuickItem *parent)
    : QQuickItem(parent)
    , m_view(Q_NULLPTR)
    , m_oldSurface(Q_NULLPTR)
    , m_provider(Q_NULLPTR)
    , m_paintEnabled(true)
    , m_touchEventsEnabled(false)
    , m_inputEventsEnabled(true)
    , m_newTexture(false)
    , m_focusOnClick(true)
    , m_sizeFollowsSurface(true)
    , m_connectedWindow(Q_NULLPTR)
    , m_origin(QWaylandSurface::OriginTopLeft)
{
    if (!mutex)
        mutex = new QMutex;

    m_view.reset(new QWaylandView(q));
    setFlag(QQuickItem::ItemHasContents);

    update();

    setSmooth(true);

    setInputEventsEnabled(true);
    QObject::connect(this, &QQuickItem::windowChanged, q, &QWaylandQuickItem::updateWindow);
    QObject::connect(view.data(), &QWaylandView::surfaceChanged, q, &QWaylandQuickItem::surfaceChanged);
    QObject::connect(view.data(), &QWaylandView::surfaceChanged, q, &QWaylandQuickItem::handleSurfaceChanged);
    QObject::connect(view.data(), &QWaylandView::surfaceDestroyed, q, &QWaylandQuickItem::surfaceDestroyed);
}

STEAppContainer::~STEAppContainer()
{
    Q_D(STEAppContainer);
    disconnect(this, &QQuickItem::windowChanged, this, &STEAppContainer::updateWindow);
    QMutexLocker locker(d->mutex);
    if (d->provider)
        d->provider->deleteLater();
}

QWaylandCompositor *STEAppContainer::compositor() const
{
    Q_D(const STEAppContainer);
    return d->view->surface() ? d->view->surface()->compositor() : Q_NULLPTR;
}

QWaylandView *STEAppContainer::view() const
{
    Q_D(const STEAppContainer);
    return d->view.data();
}

QWaylandSurface *STEAppContainer::surface() const
{
    Q_D(const STEAppContainer);
    return d->view->surface();
}

void STEAppContainer::setSurface(QWaylandSurface *surface)
{
    Q_D(STEAppContainer);
    d->view->setSurface(surface);
    update();
}

QWaylandSurface::Origin STEAppContainer::origin() const
{
    Q_D(const STEAppContainer);
    return d->origin;
}

bool STEAppContainer::isTextureProvider() const
{
    Q_D(const STEAppContainer);
    return QQuickItem::isTextureProvider() || d->provider;
}

QSGTextureProvider *STEAppContainer::textureProvider() const
{
    Q_D(const STEAppContainer);

    if (QQuickItem::isTextureProvider())
        return QQuickItem::textureProvider();

    return d->provider;
}

void STEAppContainer::mousePressEvent(QMouseEvent *event)
{
    Q_D(STEAppContainer);
    if (!d->shouldSendInputEvents()) {
        event->ignore();
        return;
    }

    if (!inputRegionContains(event->pos())) {
        event->ignore();
        return;
    }

    QWaylandInputDevice *inputDevice = compositor()->inputDeviceFor(event);

    if (d->focusOnClick)
        takeFocus(inputDevice);

    inputDevice->sendMouseMoveEvent(d->view.data(), event->localPos() / d->scaleFactor(), event->windowPos());
    inputDevice->sendMousePressEvent(event->button());
}

void STEAppContainer::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(STEAppContainer);
    if (d->shouldSendInputEvents()) {
        QWaylandInputDevice *inputDevice = compositor()->inputDeviceFor(event);
        inputDevice->sendMouseMoveEvent(d->view.data(), event->localPos() / d->scaleFactor(), event->windowPos());
    } else {
        emit mouseMove(event->windowPos());
        event->ignore();
    }
}

void STEAppContainer::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(STEAppContainer);
    if (d->shouldSendInputEvents()) {
        QWaylandInputDevice *inputDevice = compositor()->inputDeviceFor(event);
        inputDevice->sendMouseReleaseEvent(event->button());
    } else {
        emit mouseRelease();
        event->ignore();
    }
}

void STEAppContainer::hoverEnterEvent(QHoverEvent *event)
{
    Q_D(STEAppContainer);
    if (!inputRegionContains(event->pos())) {
        event->ignore();
        return;
    }
    if (d->shouldSendInputEvents()) {
        QWaylandInputDevice *inputDevice = compositor()->inputDeviceFor(event);
        inputDevice->sendMouseMoveEvent(d->view.data(), event->pos(), mapToScene(event->pos()));
    } else {
        event->ignore();
    }
}

void STEAppContainer::hoverMoveEvent(QHoverEvent *event)
{
    Q_D(STEAppContainer);
    if (surface()) {
        if (!inputRegionContains(event->pos())) {
            event->ignore();
            return;
        }
    }
    if (d->shouldSendInputEvents()) {
        QWaylandInputDevice *inputDevice = compositor()->inputDeviceFor(event);
        inputDevice->sendMouseMoveEvent(d->view.data(), event->pos() / d->scaleFactor(), mapToScene(event->pos()));
    } else {
        event->ignore();
    }
}

void STEAppContainer::hoverLeaveEvent(QHoverEvent *event)
{
    Q_D(STEAppContainer);
    if (d->shouldSendInputEvents()) {
        QWaylandInputDevice *inputDevice = compositor()->inputDeviceFor(event);
        inputDevice->setMouseFocus(Q_NULLPTR);
    } else {
        event->ignore();
    }
}

void STEAppContainer::wheelEvent(QWheelEvent *event)
{
    Q_D(STEAppContainer);
    if (d->shouldSendInputEvents()) {
        if (!inputRegionContains(event->pos())) {
            event->ignore();
            return;
        }

        QWaylandInputDevice *inputDevice = compositor()->inputDeviceFor(event);
        inputDevice->sendMouseWheelEvent(event->orientation(), event->delta());
    } else {
        event->ignore();
    }
}

void STEAppContainer::keyPressEvent(QKeyEvent *event)
{
    Q_D(STEAppContainer);
    if (d->shouldSendInputEvents()) {
        QWaylandInputDevice *inputDevice = compositor()->inputDeviceFor(event);
        inputDevice->sendFullKeyEvent(event);
    } else {
        event->ignore();
    }
}

void STEAppContainer::keyReleaseEvent(QKeyEvent *event)
{
    Q_D(STEAppContainer);
    if (d->shouldSendInputEvents() && hasFocus()) {
        QWaylandInputDevice *inputDevice = compositor()->inputDeviceFor(event);
        inputDevice->sendFullKeyEvent(event);
    } else {
        event->ignore();
    }
}

void STEAppContainer::touchEvent(QTouchEvent *event)
{
    Q_D(STEAppContainer);
    if (d->shouldSendInputEvents() && d->touchEventsEnabled) {
        QWaylandInputDevice *inputDevice = compositor()->inputDeviceFor(event);

        if (event->type() == QEvent::TouchBegin) {
            QQuickItem *grabber = window()->mouseGrabberItem();
            if (grabber != this)
                grabMouse();
        }

        QPoint pointPos;
        const QList<QTouchEvent::TouchPoint> &points = event->touchPoints();
        if (!points.isEmpty())
            pointPos = points.at(0).pos().toPoint();

        if (event->type() == QEvent::TouchBegin && !inputRegionContains(pointPos)) {
            event->ignore();
            return;
        }

        event->accept();
        if (inputDevice->mouseFocus() != d->view.data()) {
            inputDevice->sendMouseMoveEvent(d->view.data(), pointPos, mapToScene(pointPos));
        }
        inputDevice->sendFullTouchEvent(event);
    } else {
        event->ignore();
    }
}

void STEAppContainer::mouseUngrabEvent()
{
    if (surface()) {
        QTouchEvent e(QEvent::TouchCancel);
        touchEvent(&e);
    }
}

#ifndef QT_NO_IM
void STEAppContainer::inputMethodEvent(QInputMethodEvent *event)
{
    Q_D(STEAppContainer);
    if (d->shouldSendInputEvents()) {
        d->oldSurface->inputMethodControl()->inputMethodEvent(event);
    } else {
        event->ignore();
    }
}
#endif

void STEAppContainer::surfaceChangedEvent(QWaylandSurface *newSurface, QWaylandSurface *oldSurface)
{
    Q_UNUSED(newSurface);
    Q_UNUSED(oldSurface);
}

void STEAppContainer::handleSubsurfaceAdded(QWaylandSurface *childSurface)
{
    Q_D(STEAppContainer);
    if (d->subsurfaceHandler.isNull()) {
        STEAppContainer *childItem = new STEAppContainer;
        childItem->setSurface(childSurface);
        childItem->setVisible(true);
        childItem->setParentItem(this);
        connect(childSurface, &QWaylandSurface::subsurfacePositionChanged, childItem, &STEAppContainer::handleSubsurfacePosition);
    } else {
        bool success = QMetaObject::invokeMethod(d->subsurfaceHandler, "handleSubsurfaceAdded", Q_ARG(QWaylandSurface *, childSurface));
        if (!success)
            qWarning("STEAppContainer: subsurfaceHandler does not implement handleSubsurfaceAdded()");
    }
}

QObject *STEAppContainer::subsurfaceHandler() const
{
    Q_D(const STEAppContainer);
    return d->subsurfaceHandler.data();
}

void STEAppContainer::setSubsurfaceHandler(QObject *handler)
{
    Q_D(STEAppContainer);
    if (d->subsurfaceHandler.data() != handler) {
        d->subsurfaceHandler = handler;
        emit subsurfaceHandlerChanged();
    }
}

void STEAppContainer::handleSurfaceChanged()
{
    Q_D(STEAppContainer);
    if (d->oldSurface) {
        disconnect(d->oldSurface, &QWaylandSurface::mappedChanged, this, &STEAppContainer::surfaceMappedChanged);
        disconnect(d->oldSurface, &QWaylandSurface::parentChanged, this, &STEAppContainer::parentChanged);
        disconnect(d->oldSurface, &QWaylandSurface::sizeChanged, this, &STEAppContainer::updateSize);
        disconnect(d->oldSurface, &QWaylandSurface::bufferScaleChanged, this, &STEAppContainer::updateSize);
        disconnect(d->oldSurface, &QWaylandSurface::configure, this, &STEAppContainer::updateBuffer);
        disconnect(d->oldSurface, &QWaylandSurface::redraw, this, &QQuickItem::update);
        disconnect(d->oldSurface, &QWaylandSurface::childAdded, this, &STEAppContainer::handleSubsurfaceAdded);
#ifndef QT_NO_IM
        disconnect(d->oldSurface->inputMethodControl(), &QWaylandInputMethodControl::updateInputMethod, this, &STEAppContainer::updateInputMethod);
#endif
    }
    if (QWaylandSurface *newSurface = d->view->surface()) {
        connect(newSurface, &QWaylandSurface::mappedChanged, this, &STEAppContainer::surfaceMappedChanged);
        connect(newSurface, &QWaylandSurface::parentChanged, this, &STEAppContainer::parentChanged);
        connect(newSurface, &QWaylandSurface::sizeChanged, this, &STEAppContainer::updateSize);
        connect(newSurface, &QWaylandSurface::bufferScaleChanged, this, &STEAppContainer::updateSize);
        connect(newSurface, &QWaylandSurface::configure, this, &STEAppContainer::updateBuffer);
        connect(newSurface, &QWaylandSurface::redraw, this, &QQuickItem::update);
        connect(newSurface, &QWaylandSurface::childAdded, this, &STEAppContainer::handleSubsurfaceAdded);
#ifndef QT_NO_IM
        connect(newSurface->inputMethodControl(), &QWaylandInputMethodControl::updateInputMethod, this, &STEAppContainer::updateInputMethod);
#endif

        if (newSurface->origin() != d->origin) {
            d->origin = newSurface->origin();
            emit originChanged();
        }
        if (window()) {
            QWaylandOutput *output = newSurface->compositor()->outputFor(window());
            d->view->setOutput(output);
        }

        updateSize();
    }
    surfaceChangedEvent(d->view->surface(), d->oldSurface);
    d->oldSurface = d->view->surface();
#ifndef QT_NO_IM
    updateInputMethod(Qt::ImQueryInput);
#endif
}

void STEAppContainer::takeFocus(QWaylandInputDevice *device)
{
    forceActiveFocus();

    if (!surface())
        return;

    QWaylandInputDevice *target = device;
    if (!target) {
        target = compositor()->defaultInputDevice();
    }
    target->setKeyboardFocus(surface());
    QWaylandTextInput *textInput = QWaylandTextInput::findIn(target);
    if (textInput)
        textInput->setFocus(surface());
}

void STEAppContainer::surfaceMappedChanged()
{
    update();
}

void STEAppContainer::parentChanged(QWaylandSurface *newParent, QWaylandSurface *oldParent)
{
    Q_UNUSED(oldParent);

    if (newParent) {
        setPaintEnabled(true);
        setVisible(true);
        setOpacity(1);
        setEnabled(true);
    }
}

void STEAppContainer::updateSize()
{
    Q_D(STEAppContainer);
    if (d->sizeFollowsSurface && surface()) {
        setSize(surface()->size() * (d->scaleFactor() / surface()->bufferScale()));
    }
}

bool STEAppContainer::focusOnClick() const
{
    Q_D(const STEAppContainer);
    return d->focusOnClick;
}

void STEAppContainer::setFocusOnClick(bool focus)
{
    Q_D(STEAppContainer);
    if (d->focusOnClick == focus)
        return;

    d->focusOnClick = focus;
    emit focusOnClickChanged();
}

bool STEAppContainer::inputRegionContains(const QPointF &localPosition)
{
    Q_D(STEAppContainer);
    if (QWaylandSurface *s = surface())
        return s->inputRegionContains(localPosition.toPoint() / d->scaleFactor());
    return false;
}

bool STEAppContainer::sizeFollowsSurface() const
{
    Q_D(const STEAppContainer);
    return d->sizeFollowsSurface;
}

void STEAppContainer::setSizeFollowsSurface(bool sizeFollowsSurface)
{
    Q_D(STEAppContainer);
    if (d->sizeFollowsSurface == sizeFollowsSurface)
        return;
    d->sizeFollowsSurface = sizeFollowsSurface;
    emit sizeFollowsSurfaceChanged();
}

#ifndef QT_NO_IM
QVariant STEAppContainer::inputMethodQuery(Qt::InputMethodQuery query) const
{
    return inputMethodQuery(query, QVariant());
}

QVariant STEAppContainer::inputMethodQuery(Qt::InputMethodQuery query, QVariant argument) const
{
    Q_D(const STEAppContainer);

    if (query == Qt::ImEnabled)
        return QVariant((flags() & ItemAcceptsInputMethod) != 0);

    if (d->oldSurface)
        return d->oldSurface->inputMethodControl()->inputMethodQuery(query, argument);

    return QVariant();
}
#endif

bool STEAppContainer::paintEnabled() const
{
    Q_D(const STEAppContainer);
    return d->paintEnabled;
}

void STEAppContainer::setPaintEnabled(bool enabled)
{
    Q_D(STEAppContainer);
    d->paintEnabled = enabled;
    update();
}

bool STEAppContainer::touchEventsEnabled() const
{
    Q_D(const STEAppContainer);
    return d->touchEventsEnabled;
}

void STEAppContainer::updateBuffer(bool hasBuffer)
{
    Q_D(STEAppContainer);
    Q_UNUSED(hasBuffer);
    if (d->origin != surface()->origin()) {
        d->origin = surface()->origin();
        emit originChanged();
    }
}

void STEAppContainer::updateWindow()
{
    Q_D(STEAppContainer);
    if (d->connectedWindow) {
        disconnect(d->connectedWindow, &QQuickWindow::beforeSynchronizing, this, &STEAppContainer::beforeSync);
    }

    d->connectedWindow = window();

    if (d->connectedWindow) {
        connect(d->connectedWindow, &QQuickWindow::beforeSynchronizing, this, &STEAppContainer::beforeSync, Qt::DirectConnection);
    }

    if (compositor() && d->connectedWindow) {
        QWaylandOutput *output = compositor()->outputFor(d->connectedWindow);
        Q_ASSERT(output);
        d->view->setOutput(output);
    }
}

void STEAppContainer::beforeSync()
{
    Q_D(STEAppContainer);
    if (d->view->advance()) {
        d->newTexture = true;
        update();
    }
}

#ifndef QT_NO_IM
void STEAppContainer::updateInputMethod(Qt::InputMethodQueries queries)
{
    Q_D(STEAppContainer);

    setFlag(QQuickItem::ItemAcceptsInputMethod,
            d->oldSurface ? d->oldSurface->inputMethodControl()->enabled() : false);
    QQuickItem::updateInputMethod(queries | Qt::ImEnabled);
}
#endif

QSGNode *STEAppContainer::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    Q_D(STEAppContainer);
    const bool mapped = surface() && surface()->isMapped() && d->view->currentBuffer().hasBuffer();

    if (!mapped || !d->paintEnabled) {
        delete oldNode;
        return 0;
    }

    QWaylandBufferRef ref = d->view->currentBuffer();
    const bool invertY = ref.origin() == QWaylandSurface::OriginBottomLeft;
    const QRectF rect = invertY ? QRectF(0, height(), width(), -height())
                                : QRectF(0, 0, width(), height());

    if (ref.isShm() || bufferTypes[ref.bufferFormatEgl()].canProvideTexture) {
        QSGSimpleTextureNode *node = static_cast<QSGSimpleTextureNode *>(oldNode);

        if (!node)
            node = new QSGSimpleTextureNode();

        if (!d->provider)
            d->provider = new QWaylandSurfaceTextureProvider();

        if (d->newTexture) {
            d->newTexture = false;
            d->provider->setBufferRef(this, ref);
            node->setTexture(d->provider->texture());
        }

        d->provider->setSmooth(smooth());
        node->setRect(rect);

        return node;
    } else {
        Q_ASSERT(!d->provider);

        QSGGeometryNode *node = static_cast<QSGGeometryNode *>(oldNode);

        if (!node)
            node = new QSGGeometryNode;

        QSGGeometry *geometry = node->geometry();
        QWaylandBufferMaterial *material = static_cast<QWaylandBufferMaterial *>(node->material());

        if (!geometry)
            geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4);

        if (!material)
            material = new QWaylandBufferMaterial(ref.bufferFormatEgl());

        if (d->newTexture) {
            d->newTexture = false;
            for (int plane = 0; plane < bufferTypes[ref.bufferFormatEgl()].planeCount; plane++)
                if (uint texture = ref.textureForPlane(plane))
                    material->setTextureForPlane(plane, texture);
            material->bind();
            ref.bindToTexture();
        }

        ref.updateTexture();
        QSGGeometry::updateTexturedRectGeometry(geometry, rect, QRectF(0, 0, 1, 1));

        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry, true);

        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial, true);

        return node;
    }

    Q_UNREACHABLE();
}

void STEAppContainer::setTouchEventsEnabled(bool enabled)
{
    Q_D(STEAppContainer);
    if (d->touchEventsEnabled != enabled) {
        d->touchEventsEnabled = enabled;
        emit touchEventsEnabledChanged();
    }
}

bool STEAppContainer::inputEventsEnabled() const
{
    Q_D(const STEAppContainer);
    return d->inputEventsEnabled;
}

void STEAppContainer::setInputEventsEnabled(bool enabled)
{
    Q_D(STEAppContainer);
    if (d->inputEventsEnabled != enabled) {
        d->setInputEventsEnabled(enabled);
        emit inputEventsEnabledChanged();
    }
}

void STEAppContainer::lower()
{
    QQuickItem *parent = parentItem();
    Q_ASSERT(parent);
    QQuickItem *bottom = parent->childItems().first();
    if (this != bottom)
        stackBefore(bottom);
}

void STEAppContainer::raise()
{
    QQuickItem *parent = parentItem();
    Q_ASSERT(parent);
    QQuickItem *top = parent->childItems().last();
    if (this != top)
        stackAfter(top);
}

void STEAppContainer::handleSubsurfacePosition(const QPoint &pos)
{
    Q_D(STEAppContainer);
    QQuickItem::setPosition(pos * d->scaleFactor());
}
//*/
