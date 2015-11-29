#include "captureface.h"
#include "ui_captureface.h"
#include "common_cv.h"

using namespace std;

CaptureFace::CaptureFace(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CaptureFace)
{
    ui->setupUi(this);
    m_capture_done = true;

    m_pthread = new QThread();
    m_getcamframe = new getcamframe();

    m_getcamframe->Dosetup(m_pthread);
    MaximumFace2Becaptured = 10;

    connect(m_getcamframe,SIGNAL(imageQueued()),this,SLOT(captureface()));
}

CaptureFace::~CaptureFace()
{
    delete ui;
}

void CaptureFace::closeEvent(QCloseEvent * )
{
    emit captureface_closed();
}

void CaptureFace::on_pbtBack_clicked()
{
    close();
}

// Get access to the webcam.
//void initWebcam(VideoCapture &videoCapture, int cameraNumber)
//{
//    // Get access to the default camera.
//    try {   // Surround the OpenCV call by a try/catch block so we can give a useful error message!
//        videoCapture.open(cameraNumber);
//    } catch (cv::Exception &e) {}
//    if ( !videoCapture.isOpened() ) {
//        cerr << "ERROR: Could not access the camera!" << endl;
//        exit(1);
//    }
//    cout << "Loaded camera " << cameraNumber << "." << endl;
//}

// Load the face and 1 or 2 eye detection XML classifiers.
void initDetectors(CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2)
{
    // Load the Face Detection cascade classifier xml file.
    try {   // Surround the OpenCV call by a try/catch block so we can give a useful error message!
        faceCascade.load(faceCascadeFilename);
    } catch (cv::Exception &e) {}
    if ( faceCascade.empty() ) {
        cerr << "ERROR: Could not load Face Detection cascade classifier [" << faceCascadeFilename << "]!" << endl;
        cerr << "Copy the file from your OpenCV data folder (eg: 'C:\\OpenCV\\data\\lbpcascades') into this WebcamFaceRec folder." << endl;
        exit(1);
    }
    cout << "Loaded the Face Detection cascade classifier [" << faceCascadeFilename << "]." << endl;

    // Load the Eye Detection cascade classifier xml file.
    try {   // Surround the OpenCV call by a try/catch block so we can give a useful error message!
        eyeCascade1.load(eyeCascadeFilename1);
    } catch (cv::Exception &e) {}
    if ( eyeCascade1.empty() ) {
        cerr << "ERROR: Could not load 1st Eye Detection cascade classifier [" << eyeCascadeFilename1 << "]!" << endl;
        cerr << "Copy the file from your OpenCV data folder (eg: 'C:\\OpenCV\\data\\haarcascades') into this WebcamFaceRec folder." << endl;
        exit(1);
    }
    cout << "Loaded the 1st Eye Detection cascade classifier [" << eyeCascadeFilename1 << "]." << endl;

    // Load the Eye Detection cascade classifier xml file.
    try {   // Surround the OpenCV call by a try/catch block so we can give a useful error message!
        eyeCascade2.load(eyeCascadeFilename2);
    } catch (cv::Exception &e) {}
    if ( eyeCascade2.empty() ) {
        cerr << "Could not load 2nd Eye Detection cascade classifier [" << eyeCascadeFilename2 << "]." << endl;
        // Dont exit if the 2nd eye detector did not load, because we have the 1st eye detector at least.
        //exit(1);
    }
    else
        cout << "Loaded the 2nd Eye Detection cascade classifier [" << eyeCascadeFilename2 << "]." << endl;
}

void CaptureFace::captureface()
{

    /*CascadeClassifier faceCascade;
    CascadeClassifier eyeCascade1;
    CascadeClassifier eyeCascade2;
    */
    //VideoCapture videoCapture;
    Mat img(m_getcamframe->my_imgQueue.dequeue());

    //int cameraNumber = 0;   // Change this if you want to use a different camera device.

    // Get access to the webcam.
    //initWebcam(videoCapture, cameraNumber);

    // Try to set the camera resolution. Note that this only works for some cameras on
    // some computers and only for some drivers, so don't rely on it to work!
    //videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, DESIRED_CAMERA_WIDTH);
    //videoCapture.set(CV_CAP_PROP_FRAME_HEIGHT, DESIRED_CAMERA_HEIGHT);


    // Find a face and preprocess it to have a standard size and contrast & brightness.
    Rect faceRect;  // Position of detected face.
    Rect searchedLeftEye, searchedRightEye; // top-left and top-right regions of the face, where eyes were searched.
    Point leftEye, rightEye;    // Position of the detected eyes.
    Mat preprocessedFace = getPreprocessedFace(img, faceWidth, faceCascade, eyeCascade1, eyeCascade2, preprocessLeftAndRightSeparately, &faceRect, &leftEye, &rightEye, &searchedLeftEye, &searchedRightEye);
    Mat old_prepreprocessedFace;
    double old_time = 0;

    bool gotFaceAndEyes = false;
    if (preprocessedFace.data)
        gotFaceAndEyes = true;

    // Draw an anti-aliased rectangle around the detected face.
    if (faceRect.width > 0) {
        rectangle(img, faceRect, CV_RGB(255, 255, 0), 2, CV_AA);

        // Draw light-blue anti-aliased circles for the 2 eyes.
        Scalar eyeColor = CV_RGB(0,255,255);
        if (leftEye.x >= 0) {   // Check if the eye was detected
            circle(img, Point(faceRect.x + leftEye.x, faceRect.y + leftEye.y), 6, eyeColor, 1, CV_AA);
        }
        if (rightEye.x >= 0) {   // Check if the eye was detected
            circle(img, Point(faceRect.x + rightEye.x, faceRect.y + rightEye.y), 6, eyeColor, 1, CV_AA);
        }
    }

#if 1
    if (gotFaceAndEyes) {

        // Check if this face looks somewhat different from the previously collected face.
        double imageDiff = 10000000000.0;
        if (old_prepreprocessedFace.data) {
            imageDiff = getSimilarity(preprocessedFace, old_prepreprocessedFace);
        }

        // Also record when it happened.
        double current_time = (double)getTickCount();
        double timeDiff_seconds = (current_time - old_time)/getTickFrequency();



        // Only process the face if it is noticeably different from the previous frame and there has been noticeable time gap.
        if ((imageDiff > CHANGE_IN_IMAGE_FOR_COLLECTION) && (timeDiff_seconds > CHANGE_IN_SECONDS_FOR_COLLECTION)) {

           if (MaximumFace2Becaptured != 0) {
            // Also add the mirror image to the training set, so we have more training data, as well as to deal with faces looking to the left or right.
            Mat mirroredFace;
            flip(preprocessedFace, mirroredFace, 1);

            // Add the face images to the list of detected faces.
            preprocessedFaces.push_back(preprocessedFace);
            preprocessedFaces.push_back(mirroredFace);
            faceLabels.push_back(m_selectedPerson);
            faceLabels.push_back(m_selectedPerson);
            facename.push_back(m_enteredName);
            facename.push_back(m_enteredName);

#if 0
            // Keep a reference to the latest face of each person.
            m_latestFaces[m_selectedPerson] = preprocessedFaces.size() - 2;  // Point to the non-mirrored face.
            // Show the number of collected faces. But since we also store mirrored faces, just show how many the user thinks they stored.
            cout << "Saved face " << (preprocessedFaces.size()/2) << " for person " << m_selectedPerson << endl;
#endif
            // Make a white flash on the face, so the user knows a photo has been taken.
            Mat displayedFaceRegion = img(faceRect);
            displayedFaceRegion += CV_RGB(90,90,90);

            // Keep a copy of the processed face, to compare on next iteration.
            old_prepreprocessedFace = preprocessedFace;
            old_time = current_time;
             qDebug() << "InLoop =" << MaximumFace2Becaptured << endl;
            MaximumFace2Becaptured = MaximumFace2Becaptured - 1;
            }

        }

    }

#endif

    QImage qimg = QImage((const unsigned char*) img.data,
                         img.cols, img.rows,
                         QImage::Format_RGB888); // convert to QImage

    ui->lblCaptureImg->setPixmap(QPixmap::fromImage(qimg));
    ui->lblCaptureImg->show();
}

void CaptureFace::on_pbtDone_clicked()
{
    m_capture_done = false;

    if (m_getcamframe->m_bStart) {
        m_getcamframe->m_bStart = false;

        qDebug() << "Done button clicked\n";
        qDebug() << "MaximumFace2Becaptured =" << MaximumFace2Becaptured << endl;
        if (! m_getcamframe->my_imgQueue.isEmpty()) {
                m_getcamframe->my_imgQueue.clear();
        } else {
            qDebug() << "Image que is empty no need to clear it \n";
        }
        if (m_pthread != NULL) {
            m_pthread->exit(0);
        }
    }
}

void CaptureFace::on_btnStartCapture_clicked()
{

    if ( m_getcamframe->m_bStart == false) {
        m_getcamframe->m_bStart = true;
        // Load the face and 1 or 2 eye detection XML classifiers.
        initDetectors(faceCascade, eyeCascade1, eyeCascade2);

        m_getcamframe->moveToThread(m_pthread);
        //Start the thread
        m_pthread->start();
    }
    qDebug() << "Start capture button clicked\n";
}
