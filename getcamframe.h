#ifndef GETCAMFRAME_H
#define GETCAMFRAME_H

#include <QObject>
#include <common_cv.h>
#include <QThread>
#include <QDebug>
#include <QtCore>
#include <QImage>
#include <qqueue.h>

/*This class is to capture the images from the
  camera and put it onto the Queue. This class function
  will be moved to thread so that QT framework gets time to
  display the images onto the GUI*/

class getcamframe : public QObject
{
    Q_OBJECT
public:
    explicit getcamframe(QObject *parent = 0);

    /*This must be called after creating object of getcamframe*/
    void Dosetup(QThread * cThread);

    /*Queue of the cv::Mat */
    QQueue <cv::Mat> my_imgQueue;
    //flag to indicate that thread should keep running or break
    bool m_bStart;

signals:
    /*Signal emitted by cpaturefram slot after queueing
      the image frame*/
    void imageQueued();

public slots:
    /*Captureframe slot runs as a thread when is thread is started*/
    void captureframe();
    /*threaddestroyed slot runs to do cleanup*/
    void threadDestoryed();
private:
     QThread * m_pmythread;

};

#endif // GETCAMFRAME_H
