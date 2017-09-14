#include "qquickmapboxglrenderer.hpp"

#include "qmapboxgl.hpp"
#include "qquickmapboxgl.hpp"

#include <QSize>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QQuickWindow>
#include <QOpenGLFunctions>
#include <QThread>

#include <iostream>

QScopedPointer<QMapboxGL> QQuickMapboxGLRenderer::m_map;

QQuickMapboxGLRenderer::QQuickMapboxGLRenderer()
{
  std::cout << "Construct renderer" << std::endl;
  if (m_map.isNull())
    {
      std::cout << "Create map object" << std::endl;
      //QMapbox::initializeGLExtensions();

      QMapboxGLSettings settings;
      settings.setAccessToken(qgetenv("MAPBOX_ACCESS_TOKEN"));
      settings.setCacheDatabasePath("/tmp/mbgl-cache.db");
      settings.setCacheDatabaseMaximumSize(20 * 1024 * 1024);
      //settings.setViewportMode(QMapboxGLSettings::FlippedYViewport);
      settings.setViewportMode(QMapboxGLSettings::DefaultViewport);

      m_map.reset(new QMapboxGL(nullptr, settings, QSize(256, 256), 1));
    }

  std::cout << "Threads: " << QThread::currentThread() << " " << m_map->thread() << std::endl;
}

QQuickMapboxGLRenderer::~QQuickMapboxGLRenderer()
{
  std::cout << "Destroy renderer" << std::endl;
}

QOpenGLFramebufferObject* QQuickMapboxGLRenderer::createFramebufferObject(const QSize &size)
{
  std::cout << "create frame buffer" << std::endl;

  //m_map->resize(size / m_pixelRatio, size);
  m_map->resize(size, size); //*m_pixelRatio);

  QOpenGLFramebufferObjectFormat format;
  format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
  format.setSamples(4);

  QOpenGLFramebufferObject *f = new QOpenGLFramebufferObject(size, format);
  std::cout << "new fpo: " << f->handle() << std::endl;
  std::cout << "Threads for fpo: " << QThread::currentThread() << " " << m_map->thread() << std::endl;
  return f;
}

void QQuickMapboxGLRenderer::render()
{
  framebufferObject()->bind();
  std::cout << "render: " << framebufferObject()->handle() << std::endl;
  m_map->render();
  framebufferObject()->release();

//  if (!m_map->isFullyLoaded())
//    update();
//  else
//    std::cout << "Fully loaded" << "\n";

//  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions(); // window->openglContext()->functions();
//  f->glViewport(0, 0, framebufferObject()->width(), framebufferObject()->height());

//  GLint alignment;
//  f->glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);

//  framebufferObject()->bind();
//  std::cout << "render: " << framebufferObject()->handle() << std::endl;
//  std::cout << "Threads for render: " << QThread::currentThread() << " " << m_map->thread() << std::endl;
//  //m_map->setFramebufferObject(framebufferObject()->handle());
//  m_map->render();
//  framebufferObject()->release();

//  // QTBUG-62861
//  f->glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

////  window->resetOpenGLState();
////  markDirty(QSGNode::DirtyMaterial);
}

void QQuickMapboxGLRenderer::synchronize(QQuickFramebufferObject *item)
{
  std::cout << "sync" << std::endl;

  auto quickMap = qobject_cast<QQuickMapboxGL*>(item);
//  if (!m_initialized) {
//      QObject::connect(m_map.data(), &QMapboxGL::needsRendering, quickMap, &QQuickMapboxGL::update);
//      QObject::connect(m_map.data(), SIGNAL(mapChanged(QMapboxGL::MapChange)), quickMap, SLOT(onMapChanged(QMapboxGL::MapChange)));
//      QObject::connect(this, &QQuickMapboxGLRenderer::centerChanged, quickMap, &QQuickMapboxGL::setCenter);
//      m_initialized = true;
//    }

  m_pixelRatio = 3;
  //    if (auto window = quickMap->window()) {
  //        m_pixelRatio = window->devicePixelRatio();
  //    } else {
  //        m_pixelRatio = 1;
  //    }

  auto syncStatus = quickMap->m_syncState;
  quickMap->m_syncState = QQuickMapboxGL::NothingNeedsSync;

  if (syncStatus & QQuickMapboxGL::CenterNeedsSync || syncStatus & QQuickMapboxGL::ZoomNeedsSync) {
      const auto& center = quickMap->center();
      m_map->setCoordinateZoom({ center.latitude(), center.longitude() }, quickMap->zoomLevel());
    }

  if (syncStatus & QQuickMapboxGL::StyleNeedsSync && !quickMap->m_styleUrl.isEmpty()) {
      m_map->setStyleUrl(quickMap->m_styleUrl);
    }

  if (syncStatus & QQuickMapboxGL::PanNeedsSync) {
      m_map->moveBy(quickMap->m_pan);
      quickMap->m_pan = QPointF();
      emit centerChanged(QGeoCoordinate(m_map->latitude(), m_map->longitude()));
    }

  if (syncStatus & QQuickMapboxGL::BearingNeedsSync) {
      m_map->setBearing(quickMap->m_bearing);
    }

  if (syncStatus & QQuickMapboxGL::PitchNeedsSync) {
      m_map->setPitch(quickMap->m_pitch);
    }

  if (!quickMap->m_styleLoaded) {
      return;
    }

  for (const auto& change : quickMap->m_sourceChanges) {
      m_map->updateSource(change.value("id").toString(), change);
    }
  quickMap->m_sourceChanges.clear();

  for (const auto& change : quickMap->m_layerChanges) {
      m_map->addLayer(change);
    }
  quickMap->m_layerChanges.clear();

  for (const auto& change : quickMap->m_filterChanges) {
      m_map->setFilter(change.value("layer").toString(), change.value("filter"));
    }
  quickMap->m_filterChanges.clear();

  for (const auto& change : quickMap->m_imageChanges) {
      m_map->addImage(change.name, change.sprite);
    }
  quickMap->m_imageChanges.clear();

  for (const auto& change : quickMap->m_stylePropertyChanges) {
      if (change.type == QQuickMapboxGL::StyleProperty::Paint) {
          m_map->setPaintProperty(change.layer, change.property, change.value); //, change.klass);
        } else {
          m_map->setLayoutProperty(change.layer, change.property, change.value);
        }
    }
  quickMap->m_stylePropertyChanges.clear();
}
