#include "recognize.h"
#include "ui_recognize.h"
#include <QFile>
#include <QTextStream>
#include <fstream>
#include <QStringList>

Recognize::Recognize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Recognize)
{
    ui->setupUi(this);
    m_capture_done = true;

    m_pthread = new QThread();
    m_getFramForRec = new getcamframe();

    m_getFramForRec->Dosetup(m_pthread);

    connect(m_getFramForRec,SIGNAL(imageQueued()),this,SLOT(Recognize_face()));
    /*http://stackoverflow.com/questions/14356121/how-to-call-function-after-window-is-shown*/
    connect(this, SIGNAL(window_loaded()), this, SLOT(Recognize_face()), Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
    qDebug() << "Recognize constructor Done!!!"<<endl;

}

// Load the face and 1 or 2 eye detection XML classifiers.
void initrecDetectors(CascadeClassifier &faceCascade, CascadeClassifier &eyeCascade1, CascadeClassifier &eyeCascade2)
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
bool Recognize::initializeFaceRecognizer(){

    bool haveContribModule = initModule_contrib();
    bool loadedRecognizerFile = false;
    string facerecAlgorithm = facerecAlgorithm;
    if (!haveContribModule) {

        cerr << "Error : The 'Contrib' module is needed for";
        cerr << "FaceRecognizer but hasn't been loaded to opencv!" << endl;
        exit(1);
    }

    model = Algorithm::create<FaceRecognizer>(facerecAlgorithm);
    Mat labels;
    if (model.empty())
    {
        cerr << "Error: The FaceRecognizer[" << facerecAlgorithm << "] is not available in your version of opencv";
        cerr << "Please Update to openCV v2.4.1 or newer" <<endl;
        exit(1);
    }
    qDebug() << "-----initializeFaceRecognizer()----------"<<endl;
    qDebug() << "dataStoreFile" << dataStoreFile << endl;
        QFile dstfile(dataStoreFile);

// As of not now saving and loading data from file
#if 0
        if (dstfile.exists())
        {
            try {
                qDebug()<<"Inside Try catch block"<<endl;
                /*Load trainning module*/
                model->load(dataStoreFile.toStdString());
                labels = model->get<Mat>("labels");
                loadedRecognizerFile = true;
                qDebug()<<"Before readBackFaceNames"<<endl;
                readBackFaceNames(faceNameStorePath);

            } catch (cv::Exception &e) {}
            if (labels.rows <= 0  ) {
                qDebug() << "ERROR: Couldn't load trained data from "
                            "["<<dataStoreFile <<"]!" << endl;
                //exit(1);
            }
        } else {
            qDebug() << dataStoreFile <<"is not present"<<endl;
        }
#endif

    if (preprocessedFaces.size() != 0 ) {

        //Do the training from collected faces.
        model->train(preprocessedFaces, faceLabels);

        //Save training model
        qDebug() << "Let's save face recognitiion model"<<endl;
        model->save(dataStoreFile.toStdString());
        //storeFaceNames(faceNameStorePath);

    } else {

        cout << "preprocessedFaces.size() is zero" << endl;
        //If model is not loaded from saved files exit
        if (loadedRecognizerFile != true) {
            exit(1);
            cerr <<"Recognition can't be done without training!"<<endl;
        }
    }
    return true;
}

void Recognize::showEvent(QShowEvent * ev)
{
    QDialog::showEvent(ev);
    if ( m_getFramForRec->m_bStart == false) {
        m_getFramForRec->m_bStart = true;
        // Load the face and 1 or 2 eye detection XML classifiers.
        initrecDetectors(faceCascade, eyeCascade1, eyeCascade2);

        m_getFramForRec->moveToThread(m_pthread);
        /*Initialize the Contrib module and initialize facedetector*/
        initializeFaceRecognizer();
        //Start the thread
        m_pthread->start();
    }
    qDebug() << "Recognize showEvent Done!!!"<<endl;
    emit window_loaded();
}
Recognize::~Recognize()
{
    delete ui;
}

void Recognize ::closeEvent(QCloseEvent *){

    emit recognize_closed();
}

void Recognize::on_btnRecBack_clicked()
{
    m_capture_done = false;

    if (m_getFramForRec->m_bStart) {
        m_getFramForRec->m_bStart = false;

        qDebug() << "Back button clicked\n";
        if (!m_getFramForRec->my_imgQueue.isEmpty()) {
                m_getFramForRec->my_imgQueue.clear();
        } else {
            qDebug() << "Recnognize--->>>Image que is empty no need to clear it \n";
        }
        if (m_pthread != NULL) {
            m_pthread->exit(0);
        }
    }
    close();
}

void Recognize::Recognize_face()
{
    if (!m_getFramForRec->my_imgQueue.isEmpty())
    {
        Mat img(m_getFramForRec->my_imgQueue.dequeue());
        // Find a face and preprocess it to have a standard size and contrast & brightness.
        Rect faceRect;  // Position of detected face.
        Rect searchedLeftEye, searchedRightEye; // top-left and top-right regions of the face, where eyes were searched.
        Point leftEye, rightEye;    // Position of the detected eyes.
        Mat preprocessedFace = getPreprocessedFace(img, faceWidth, faceCascade, eyeCascade1, eyeCascade2, preprocessLeftAndRightSeparately, &faceRect, &leftEye, &rightEye, &searchedLeftEye, &searchedRightEye);
       // Mat old_prepreprocessedFace;

        bool gotFaceAndEyes = false;
        if (preprocessedFace.data)
            gotFaceAndEyes = true;

        // Draw an anti-aliased rectangle around the detected face.
        if (faceRect.width > 0) {
            rectangle(img, faceRect, CV_RGB(255, 255, 0), 2, CV_AA);

            // Draw light-blue anti-aliased circles for the 2 eyes.
#ifdef DISPLAY_EYE
            Scalar eyeColor = CV_RGB(0,255,255);
            if (leftEye.x >= 0) {   // Check if the eye was detected
                circle(img, Point(faceRect.x + leftEye.x, faceRect.y + leftEye.y), 6, eyeColor, 1, CV_AA);
            }
            if (rightEye.x >= 0) {   // Check if the eye was detected
                circle(img, Point(faceRect.x + rightEye.x, faceRect.y + rightEye.y), 6, eyeColor, 1, CV_AA);
            }
#endif
        }

        int identity = -1;
        //If preprocessed face is empty no need to predict face and extract name
        if (!preprocessedFace.empty()) {
            identity = model->predict(preprocessedFace);
            qDebug() << "identity = " << identity <<endl;
            //ui->lblRecName = QString(facename[identity]);
            qDebug() << "Size of facename vector : "<<facename.size()<<endl;
            qDebug() << "Size of faceLabels vector : "<<faceLabels.size()<<endl;

            // Get some required data from the FaceRecognizer model.
            Mat eigenvectors = model->get<Mat>("eigenvectors");
            Mat averageFaceRow = model->get<Mat>("mean");
            // Project the input image onto the eigenspace.
            Mat projection = subspaceProject(eigenvectors, averageFaceRow,
            preprocessedFace.reshape(1,1));
            // Generate the reconstructed face back from the eigenspace.
            Mat reconstructionRow = subspaceReconstruct(eigenvectors,
            averageFaceRow, projection);

            // Make it a rectangular shaped image instead of a single row.
            Mat reconstructionMat = reconstructionRow.reshape(1, faceHeight);
            // Convert the floating-point pixels to regular 8-bit uchar.
            Mat reconstructedFace = Mat(reconstructionMat.size(), CV_8U);
            reconstructionMat.convertTo(reconstructedFace, CV_8U, 1, 0);
            double similarity = getSimilarity(preprocessedFace, reconstructedFace);
            qDebug() << "Similarity:" <<similarity<< "Unknown person Threshold:"<< UNKNOWN_PERSON_THRESHOLD;
            if (similarity > UNKNOWN_PERSON_THRESHOLD) {
                identity = -1; // Unknown person.
            }

            if (identity > 0 ) {
                QString recname(facename.at(identity));
                qDebug() << "FaceName = "<< recname << "Identity = "<< identity;
                ui->lblRecName->setText(recname);
            } else {
                qDebug() << "Less confidence" << endl;
                ui->lblRecName->setText("");
            }
        } else {
            qDebug() << "Recognize_face():!!!Error, Empty Preprocessed face" <<endl;
            ui->lblRecName->setText("");

        }
        QImage qimg = QImage((const unsigned char*) img.data,
                             img.cols, img.rows,
                             QImage::Format_RGB888); // convert to QImage

        ui->lblRecognizemg->setPixmap(QPixmap::fromImage(qimg));
        ui->lblRecognizemg->show();
        ui->lblRecName->show();

    } else {
        qDebug() << "Queue is empty so wait";

    }

}

bool Recognize ::storeFaceNames(QString Filename){

    QFile mFile(Filename);
    qDebug()<<"Filename :"<<Filename<<endl;
    if (!mFile.open(QFile::WriteOnly | QFile::Text)) {

        qDebug() << "Could not open" << Filename <<"file for writing"<< endl;
        return 1;
    }
    QTextStream out(&mFile);
    int i = 0;
    while(i < facename.size() )
    {
        qDebug() << "name:" << facename.at(i) << endl;
        out << facename.at(i) << endl;
        i++;
    }
    mFile.flush();
    mFile.close();

    return 0;

}
bool Recognize ::readBackFaceNames(QString Filename){

     QFile mFile(Filename);
     qDebug()<< "Inside readBackFaceNames()"<<endl;
     if (!mFile.open(QFile::ReadOnly | QFile::Text)) {
         qDebug()<< "Could not open" << Filename <<"file for reading" << endl;
         return 1;
     }
     qDebug()<< Filename << "File Opened Successfully"<<endl;

     QTextStream input(&mFile);
     QStringList line;
     int i = 0;
     for( ; ! input.atEnd();)
     {
         line << input.readLine();
         facename.push_back(line.at(i));
         qDebug() << "Reading file line: " << line.at(i) << endl;
         i++;
     }
     i = 0;
     while( i < facename.size())
     {
         qDebug() << "Str =" << facename.at(i)<<endl;
         i++;
     }
     mFile.close();
    return 0;
}
