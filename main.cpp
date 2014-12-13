#include "facerecognitionmain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FaceRecognitionMain w;
    w.show();

    return a.exec();
}
