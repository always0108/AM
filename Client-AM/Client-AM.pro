QT       += core gui
QT       += network
QT       += widgets
QT       += opengl
QT       += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += qwt
DEFINES += QT_DLL QWT_DLL

#LIBS += -L"D:\Software\Qt\5.12.9\mingw73_32\lib" -lqwt
#INCLUDEPATH += D:\Software\Qt\5.12.9\mingw73_32\include\Qwt
#LIBS   += -lopengl32
#LIBS   += -lglu32
#LIBS   += -lglut32

LIBS += -L"C:\Qt\Qt5.12.0\5.12.0\mingw73_64" -lqwt
INCLUDEPATH += C:\Qt\Qt5.12.0\5.12.0\mingw73_64\include\Qwt
LIBS += -L"C:\Qt\Qt5.12.0\5.12.0\mingw73_64\lib"  -llibfreeglut -lopengl32

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    printsettings.cpp \
    recvfile.cpp \
    sendfile.cpp \
    serversettings.cpp \
    showpath.cpp \
    showwidget.cpp \
    tcpclient.cpp \
    glwindow.cpp \
    model.cpp \
    showmodel.cpp \
    pathprogress.cpp \
    modellist.cpp

HEADERS += \
    mainwindow.h \
    printsettings.h \
    recvfile.h \
    sendfile.h \
    serversettings.h \
    showpath.h \
    showwidget.h \
    tcpclient.h \
    glwindow.h \
    model.h \
    showmodel.h \
    pathprogress.h \
    pathprogress.h \
    modellist.h

FORMS += \
    showmodel.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
