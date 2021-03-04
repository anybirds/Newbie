#include <Engine.hpp>

#include "viewportwidget.h"

#include <QDebug>

using namespace Engine;

ViewportWidget::ViewportWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

ViewportWidget::~ViewportWidget() {

}

void ViewportWidget::initializeGL() {
    // glew init
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK) {
        // Initializing GLEW failed
        cout << '[' << __FUNCTION__ << ']' << " message: " << glewGetErrorString(glew_error) << '\n';
        exit(1);
    }
}

void ViewportWidget::paintGL() {
    if (!Scene::GetCurrentScene()) {
        return;
    }

    Camera::GetMainCamera()->Render();

    Object::Destruct();

    update();
}

void ViewportWidget::resizeGL(int w, int h) {

}
