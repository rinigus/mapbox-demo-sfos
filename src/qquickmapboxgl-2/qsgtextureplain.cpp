#include "qsgtextureplain.h"
#include <qopenglfunctions.h>
#include <QtGui/qopenglcontext.h>
#include <QtGui/qopenglfunctions.h>

QSGTexturePlain::QSGTexturePlain()
    : QSGTexture()
    , m_texture_id(0)
    , m_has_alpha(false)
    , m_dirty_texture(false)
    , m_dirty_bind_options(false)
    , m_owns_texture(true)
    , m_mipmaps_generated(false)
    , m_retain_image(false)
{
}


QSGTexturePlain::~QSGTexturePlain()
{
    if (m_texture_id && m_owns_texture && QOpenGLContext::currentContext())
        QOpenGLContext::currentContext()->functions()->glDeleteTextures(1, &m_texture_id);
}

void qsg_swizzleBGRAToRGBA(QImage *image)
{
    const int width = image->width();
    const int height = image->height();
    for (int i = 0; i < height; ++i) {
        uint *p = (uint *) image->scanLine(i);
        for (int x = 0; x < width; ++x)
            p[x] = ((p[x] << 16) & 0xff0000) | ((p[x] >> 16) & 0xff) | (p[x] & 0xff00ff00);
    }
}

void QSGTexturePlain::setImage(const QImage &image)
{
    m_image = image;
    m_texture_size = image.size();
    m_has_alpha = image.hasAlphaChannel();
    m_dirty_texture = true;
    m_dirty_bind_options = true;
    m_mipmaps_generated = false;
 }

int QSGTexturePlain::textureId() const
{
    if (m_dirty_texture) {
        if (m_image.isNull()) {
            // The actual texture and id will be updated/deleted in a later bind()
            // or ~QSGTexturePlain so just keep it minimal here.
            return 0;
        } else if (m_texture_id == 0){
            // Generate a texture id for use later and return it.
            QOpenGLContext::currentContext()->functions()->glGenTextures(1, &const_cast<QSGTexturePlain *>(this)->m_texture_id);
            return m_texture_id;
        }
    }
    return m_texture_id;
}

void QSGTexturePlain::setTextureId(int id)
{
    if (m_texture_id && m_owns_texture)
        QOpenGLContext::currentContext()->functions()->glDeleteTextures(1, &m_texture_id);

    m_texture_id = id;
    m_dirty_texture = false;
    m_dirty_bind_options = true;
    m_image = QImage();
    m_mipmaps_generated = false;
}

void QSGTexturePlain::bind()
{
    QOpenGLContext *context = QOpenGLContext::currentContext();
    QOpenGLFunctions *funcs = context->functions();
    if (!m_dirty_texture) {
        funcs->glBindTexture(GL_TEXTURE_2D, m_texture_id);
        if (mipmapFiltering() != QSGTexture::None && !m_mipmaps_generated) {
            funcs->glGenerateMipmap(GL_TEXTURE_2D);
            m_mipmaps_generated = true;
        }
        updateBindOptions(m_dirty_bind_options);
        m_dirty_bind_options = false;
        return;
    }

    m_dirty_texture = false;

    if (m_image.isNull()) {
        if (m_texture_id && m_owns_texture) {
            funcs->glDeleteTextures(1, &m_texture_id);
        }
        m_texture_id = 0;
        m_texture_size = QSize();
        m_has_alpha = false;

        return;
    }
}
