#ifndef QQUICKITEMMAPBOXGL_H
#define QQUICKITEMMAPBOXGL_H

#include <QQuickItem>
#include <QTimer>

class QQuickItemMapboxGL : public QQuickItem
{
  Q_OBJECT

public:
  QQuickItemMapboxGL(QQuickItem *parent = nullptr);
  ~QQuickItemMapboxGL();

  QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

signals:
  void startRefreshTimer();
  void stopRefreshTimer();

public slots:

private:
  QSize m_last_size;
  QTimer m_timer;
};

#endif // QQUICKITEMMAPBOXGL_H
