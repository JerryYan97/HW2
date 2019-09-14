#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_2_Core>
#include <QTimer>


class OpenGLContext
    : public QOpenGLWidget,
      public QOpenGLFunctions_3_2_Core
{
    Q_OBJECT

private:
    /// Timer for drawing new frames
    QTimer timer;

protected:
    /*** AUTOMATIC TESTING: DO NOT MODIFY ***/
    /*** If true, save a test image and exit */
    /***/ bool autotesting;

public:
    OpenGLContext(QWidget *parent);
    ~OpenGLContext();

    void debugContextVersion();
    void printGLErrorLog();
    void printLinkInfoLog(int prog);
    void printShaderInfoLog(int shader);

private slots:
    /*** AUTOMATIC TESTING: DO NOT MODIFY ***/
    /***/ void saveImageAndQuit();

    /// Slot that gets called ~60 times per second
    void timerUpdate();
};
