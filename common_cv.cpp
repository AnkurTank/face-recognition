#include "common_cv.h"

// The Face Recognition algorithm can be one of these and perhaps more, depending on your version of OpenCV, which must be atleast v2.4.1:
//    "FaceRecognizer.Eigenfaces":  Eigenfaces, also referred to as PCA (Turk and Pentland, 1991).
//    "FaceRecognizer.Fisherfaces": Fisherfaces, also referred to as LDA (Belhumeur et al, 1997).
//    "FaceRecognizer.LBPH":        Local Binary Pattern Histograms (Ahonen et al, 2006).
//const char *facerecAlgorithm = "FaceRecognizer.Fisherfaces";
const char *facerecAlgorithm = "FaceRecognizer.Eigenfaces";


// Sets how confident the Face Verification algorithm should be to decide if it is an unknown person or a known person.
// A value roughly around 0.5 seems OK for Eigenfaces or 0.7 for Fisherfaces, but you may want to adjust it for your
// conditions, and if you use a different Face Recognition algorithm.
// Note that a higher threshold value means accepting more faces as known people,
// whereas lower values mean more faces will be classified as "unknown".
const float UNKNOWN_PERSON_THRESHOLD = 0.7f;


// Cascade Classifier file, used for Face Detection.

const char *faceCascadeFilename = "/opt/FaceRecognition/bin/lbpcascade_frontalface.xml";     // LBP face detector.
//const char *faceCascadeFilename = "haarcascade_frontalface_alt_tree.xml";  // Haar face detector.
//const char *eyeCascadeFilename1 = "haarcascade_lefteye_2splits.xml";   // Best eye detector for open-or-closed eyes.
//const char *eyeCascadeFilename2 = "haarcascade_righteye_2splits.xml";   // Best eye detector for open-or-closed eyes.
//const char *eyeCascadeFilename1 = "haarcascade_mcs_lefteye.xml";       // Good eye detector for open-or-closed eyes.
//const char *eyeCascadeFilename2 = "haarcascade_mcs_righteye.xml";       // Good eye detector for open-or-closed eyes.
const char *eyeCascadeFilename1 = "/opt/FaceRecognition/bin/haarcascade_eye.xml";               // Basic eye detector for open eyes only.
const char *eyeCascadeFilename2 = "/opt/FaceRecognition/bin/haarcascade_eye_tree_eyeglasses.xml"; // Basic eye detector for open eyes if they might wear glasses.


// Set the desired face dimensions. Note that "getPreprocessedFace()" will return a square face.
const int faceWidth = 70;
const int faceHeight = faceWidth;

// Try to set the camera resolution. Note that this only works for some cameras on
// some computers and only for some drivers, so don't rely on it to work!
const int DESIRED_CAMERA_WIDTH = 640;
const int DESIRED_CAMERA_HEIGHT = 480;

// Parameters controlling how often to keep new faces when collecting them. Otherwise, the training set could look to similar to each other!
const double CHANGE_IN_IMAGE_FOR_COLLECTION = 0.3;      // How much the facial image should change before collecting a new face photo for training.
const double CHANGE_IN_SECONDS_FOR_COLLECTION = 1.0;       // How much time must pass before collecting a new face photo for training.

const char *windowName = "WebcamFaceRec";   // Name shown in the GUI window.
const int BORDER = 8;  // Border between GUI elements to the edge of the image.

const bool preprocessLeftAndRightSeparately = true;   // Preprocess left & right sides of the face separately, in case there is stronger light on one side.

// Set to true if you want to see many windows created, showing various debug info. Set to 0 otherwise.
bool m_debug = false;

QT_BEGIN_NAMESPACE
/*Global variables*/
Ptr<FaceRecognizer> model;
vector<Mat> preprocessedFaces;
vector<int> faceLabels;
vector<QString> facename;
int m_selectedPerson = 1;
QString m_enteredName="";
vector<int> m_latestFaces;
QString homepath = getenv("HOME");
QString homepath2 = getenv("HOME");
QString dataStoreFile = homepath.append("/trainedData.yml");
QString faceNameStorePath = homepath2.append("/faceNames.txt");
QString g_personname;
QT_END_NAMESPACE
