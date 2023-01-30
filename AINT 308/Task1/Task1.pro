TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

#=====================OpenCV Includes=======================
INCLUDEPATH += C:\AINT308Lib\OpenCV41\release\install\include

LIBS += -LC:\AINT308Lib\OpenCV41\release\lib
LIBS +=    -lopencv_core411 \
    -lopencv_highgui411 \
    -lopencv_imgproc411 \
    -lopencv_features2d411 \
    -lopencv_calib3d411 \
    -lopencv_videoio411 \
    -lopencv_imgcodecs411 \
    -lopencv_face411 \
    -lopencv_objdetect411 \

#======================Dlib Includes========================
#QMAKE_CXXFLAGS_RELEASE += -mavx

#INCLUDEPATH+=C:\AINT308Repository\dlib19_19\include
#LIBS +=-L"C:\AINT308Repository\dlib19_19\lib"
#LIBS+=-ldlib
#LIBS += -lmingw32
#LIBS+=-lOle32
#LIBS+=-lOleaut32
#LIBS+=-lm
#LIBS+= -ldinput8
#LIBS+=-lcomctl32

#LIBS+=-ldxguid
#LIBS+= -ldxerr8
#LIBS+=-luser32
#LIBS+=-lgdi32
#LIBS+=-lwinmm
#LIBS+= -limm32
#LIBS+= -lole32
#LIBS+=-loleaut32
#LIBS+=-lshell32
#LIBS+= -lversion
#LIBS+= -luuid

#LIBS+=-lopengl32
#LIBS+=-lglu32
#LIBS+=-lgdi32
#LIBS+=-lwinmm
#LIBS += -lws2_32

#====================Project Includes======================
SOURCES += \
        main.cpp

HEADERS += \

