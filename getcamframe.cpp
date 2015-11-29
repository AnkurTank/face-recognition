#include "getcamframe.h"

// Get access to the webcam.
void initWebcam(VideoCapture &videoCapture, int cameraNumber)
{
    // Get access to the default camera.
    try {   // Surround the OpenCV call by a try/catch block so we can give a useful error message!
        videoCapture.open(cameraNumber);
    } catch (cv::Exception &e) {}
    if ( !videoCapture.isOpened() ) {
        cerr << "ERROR: Could not access the camera!" << endl;
        exit(1);
    }
    cout << "Loaded camera " << cameraNumber << "." << endl;
}

getcamframe::getcamframe(QObject *parent) :
    QObject(parent)
{


}

void getcamframe::Dosetup(QThread *cThread)
{
    connect(cThread,SIGNAL(started()),this, SLOT(captureframe()));

    connect(cThread,SIGNAL(destroyed()),this,SLOT(threadDestoryed()));
    this->m_pmythread = cThread;
}

void getcamframe::captureframe()
{
    VideoCapture videoCapture;
    cv::Mat img;

    int cameraNumber = 0;   // Change this if you want to use a different camera device.

    // Get access to the webcam.
    initWebcam(videoCapture, cameraNumber);

    // Try to set the camera resolution. Note that this only works for some cameras on
    // some computers and only for some drivers, so don't rely on it to work!
    videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, DESIRED_CAMERA_WIDTH);
    videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, DESIRED_CAMERA_HEIGHT);
    videoCapture.set(CV_CAP_PROP_FPS,30);

    while (this->m_bStart == true) {
        videoCapture >> img;

        cv::cvtColor(img,img,CV_BGR2RGB);
        my_imgQueue.enqueue(img);

        //Emit to signal that image is queued.
        emit this->imageQueued();

    }

    //If you break close the camera
    if (videoCapture.isOpened()) {
        videoCapture.release();
    }

}


void getcamframe :: threadDestoryed()
{
    qDebug() << "-----------------------Thread is destroyed------------- \n";

}
