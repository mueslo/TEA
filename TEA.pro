# #####################################################################
# Automatically generated by qmake (2.01a) Sa 22. Aug 23:17:44 2009
# #####################################################################
DEPENDPATH += .
TARGET = 
CONFIG += static
INCLUDEPATH += C:\Qwt\src

# Input
HEADERS += teaview.h \
    finddialog.h \
    coordinates.h \
    metadatadialog.h \
    db.h \
    tea.h
FORMS += finddialog.ui \
    metadatadialog.ui \
    tea.ui
SOURCES += finddialog.cpp \
    metadatadialog.cpp \
    hl_md5.cpp \
    hl_md5wrapper.cpp \
    maintea.cpp \
    main.cpp
QT += core \
    gui \
    opengl \
    sql \
    qt3support \
    network
LIBS += -lqwt5 \
    -LC:\Qwt\lib
DEFINES += QWT_DLL
RESOURCES += icons.qrc