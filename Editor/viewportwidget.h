#ifndef VIEWPORTWIDGET_H
#define VIEWPORTWIDGET_H

#include <QOpenGLWidget>

#include <string>

namespace Engine {
    class Scene;
}

class ViewportWidget : public QOpenGLWidget
{
public:
    explicit ViewportWidget(QWidget *parent = nullptr);
    ~ViewportWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
};

#endif // SCENEWIDGET_H
