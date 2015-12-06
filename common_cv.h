#ifndef COMMON_CV_H
#define COMMON_CV_H

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <qstring.h>

// Include OpenCV's C++ Interface
#include "opencv2/opencv.hpp"

// Include the rest of our code!
#include "detectObject.h"       // Easily detect faces or eyes (using LBP or Haar Cascades).
#include "preprocessFace.h"     // Easily preprocess face images, for face recognition.
#include "recognition.h"     // Train the face recognition system and recognize a person from an image.

#include "ImageUtils.h"      // Shervin's handy OpenCV utility functions.

// The Face Recognition algorithm can be one of these and perhaps more, depending on your version of OpenCV, which must be atleast v2.4.1:
//    "FaceRecognizer.Eigenfaces":  Eigenfaces, also referred to as PCA (Turk and Pentland, 1991).
//    "FaceRecognizer.Fisherfaces": Fisherfaces, also referred to as LDA (Belhumeur et al, 1997).
//    "FaceRecognizer.LBPH":        Local Binary Pattern Histograms (Ahonen et al, 2006).
//const char *facerecAlgorithm = "FaceRecognizer.Fisherfaces";
extern const char *facerecAlgorithm ;//= "FaceRecognizer.Eigenfaces";


// Sets how confident the Face Verification algorithm should be to decide if it is an unknown person or a known person.
// A value roughly around 0.5 seems OK for Eigenfaces or 0.7 for Fisherfaces, but you may want to adjust it for your
// conditions, and if you use a different Face Recognition algorithm.
// Note that a higher threshold value means accepting more faces as known people,
// whereas lower values mean more faces will be classified as "unknown".
extern const float UNKNOWN_PERSON_THRESHOLD ;//= 0.7f;


// Cascade Classifier file, used for Face Detection.
extern const char *faceCascadeFilename ;// = "./lbpcascade_frontalface.xml";     // LBP face detector.
//const char *faceCascadeFilename = "haarcascade_frontalface_alt_tree.xml";  // Haar face detector.
//const char *eyeCascadeFilename1 = "haarcascade_lefteye_2splits.xml";   // Best eye detector for open-or-closed eyes.
//const char *eyeCascadeFilename2 = "haarcascade_righteye_2splits.xml";   // Best eye detector for open-or-closed eyes.
//const char *eyeCascadeFilename1 = "haarcascade_mcs_lefteye.xml";       // Good eye detector for open-or-closed eyes.
//const char *eyeCascadeFilename2 = "haarcascade_mcs_righteye.xml";       // Good eye detector for open-or-closed eyes.
extern const char *eyeCascadeFilename1 ;//= "./haarcascade_eye.xml";               // Basic eye detector for open eyes only.
extern const char *eyeCascadeFilename2;// = "./haarcascade_eye_tree_eyeglasses.xml"; // Basic eye detector for open eyes if they might wear glasses.


// Set the desired face dimensions. Note that "getPreprocessedFace()" will return a square face.
extern const int faceWidth ;//= 70;
extern const int faceHeight ;//= faceWidth;

// Try to set the camera resolution. Note that this only works for some cameras on
// some computers and only for some drivers, so don't rely on it to work!
extern const int DESIRED_CAMERA_WIDTH ;//= 640;
extern const int DESIRED_CAMERA_HEIGHT;// = 480;

// Parameters controlling how often to keep new faces when collecting them. Otherwise, the training set could look to similar to each other!
extern const double CHANGE_IN_IMAGE_FOR_COLLECTION ;//= 0.3;      // How much the facial image should change before collecting a new face photo for training.
extern const double CHANGE_IN_SECONDS_FOR_COLLECTION ;//= 1.0;       // How much time must pass before collecting a new face photo for training.

extern const char *windowName ;//= "WebcamFaceRec";   // Name shown in the GUI window.
extern const int BORDER;// = 8;  // Border between GUI elements to the edge of the image.

extern const bool preprocessLeftAndRightSeparately ;//= true;   // Preprocess left & right sides of the face separately, in case there is stronger light on one side.

// Set to true if you want to see many windows created, showing various debug info. Set to 0 otherwise.
extern bool m_debug ;//= false;

QT_BEGIN_NAMESPACE
/*Global variables*/
extern Ptr<FaceRecognizer> model;
extern vector<Mat> preprocessedFaces;
extern vector<int> faceLabels;
extern vector<QString> facename;

extern QString g_personname;
extern int m_selectedPerson;
extern QString m_enteredName;
extern vector<int> m_latestFaces;
QT_END_NAMESPACE

#endif // COMMON_CV_H
