#ifndef QSGTEXTUREPLAIN_H
#define QSGTEXTUREPLAIN_H

#include <QSGTexture>
#include <QImage>

class QSGTexturePlain : public QSGTexture
{
    Q_OBJECT
public:
    QSGTexturePlain();
    virtual ~QSGTexturePlain();

    void setOwnsTexture(bool owns) { m_owns_texture = owns; }
    bool ownsTexture() const { return m_owns_texture; }

    void setTextureId(int id);
    int textureId() const override;
    void setTextureSize(const QSize &size) { m_texture_size = size; }
    QSize textureSize() const override { return m_texture_size; }

    void setHasAlphaChannel(bool alpha) { m_has_alpha = alpha; }
    bool hasAlphaChannel() const override { return m_has_alpha; }

    bool hasMipmaps() const override { return mipmapFiltering() != QSGTexture::None; }

    void setImage(const QImage &image);
    const QImage &image() { return m_image; }

    void bind() override;

    static QSGTexturePlain *fromImage(const QImage &image) {
        QSGTexturePlain *t = new QSGTexturePlain();
        t->setImage(image);
        return t;
    }

protected:
    QImage m_image;

    uint m_texture_id;
    QSize m_texture_size;
    QRectF m_texture_rect;

    uint m_has_alpha : 1;
    uint m_dirty_texture : 1;
    uint m_dirty_bind_options : 1;
    uint m_owns_texture : 1;
    uint m_mipmaps_generated : 1;
    uint m_retain_image: 1;
};

#endif // QSGTEXTUREPLAIN_H
