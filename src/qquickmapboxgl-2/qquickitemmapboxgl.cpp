#include "qquickitemmapboxgl.h"
#include "qsgmapboxglnode.h"

#include <QDebug>

QQuickItemMapboxGL::QQuickItemMapboxGL(QQuickItem *parent):
  QQuickItem(parent)
{
  qDebug() << "QQuickItemMapboxGL::QQuickItemMapboxGL";

  setFlag(ItemHasContents);

  m_timer.setInterval(250);
  connect(&m_timer, &QTimer::timeout, this, &QQuickItemMapboxGL::update);
  connect(&m_timer, &QTimer::timeout, [=](){ qDebug() << "T";});
  connect(this, SIGNAL(startRefreshTimer()), &m_timer, SLOT(start()));
  connect(this, &QQuickItemMapboxGL::stopRefreshTimer, &m_timer, &QTimer::stop);
}

QQuickItemMapboxGL::~QQuickItemMapboxGL()
{
  qDebug() << "QQuickItemMapboxGL::~QQuickItemMapboxGL";
}

QSGNode* QQuickItemMapboxGL::updatePaintNode(QSGNode *node, UpdatePaintNodeData *)
{
  QSGMapboxGLTextureNode *n = static_cast<QSGMapboxGLTextureNode *>(node);
  QSize sz(width(), height());

  if (!n)
    {
      QMapboxGLSettings settings;
      settings.setAccessToken(qgetenv("MAPBOX_ACCESS_TOKEN"));
      settings.setCacheDatabasePath("/tmp/mbgl-cache.db");
      settings.setCacheDatabaseMaximumSize(20 * 1024 * 1024);
      settings.setViewportMode(QMapboxGLSettings::DefaultViewport);

      n = new QSGMapboxGLTextureNode(settings, sz, window()->devicePixelRatio(), this);
    }

  if (sz != m_last_size)
    {
      n->resize(sz, window()->devicePixelRatio());
      m_last_size = sz;
    }

  bool loaded = n->render(window());
  if (!loaded && !m_timer.isActive())
    {
      qDebug() << "Start timer";
      emit startRefreshTimer();
    }
  else if (loaded && m_timer.isActive())
    {
      qDebug() << "Stop timer";
      emit stopRefreshTimer();
    }

  return n;
}

