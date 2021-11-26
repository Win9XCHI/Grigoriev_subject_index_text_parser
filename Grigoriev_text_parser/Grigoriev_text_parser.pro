QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        classes/account_unit.cpp \
        classes/assembly.cpp \
        classes/category.cpp \
        classes/content.cpp \
        classes/county.cpp \
        classes/graphs.cpp \
        classes/navigation.cpp \
        classes/pages.cpp \
        classes/person.cpp \
        classes/pointer.cpp \
        classes/province.cpp \
        classes/work.cpp \
        filemanager.cpp \
        main.cpp \
        parser.cpp \
        reader.cpp \
        unparser.cpp \
        writer.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    classes/account_unit.h \
    classes/assembly.h \
    classes/category.h \
    classes/content.h \
    classes/county.h \
    classes/graphs.h \
    classes/navigation.h \
    classes/pages.h \
    classes/person.h \
    classes/pointer.h \
    classes/province.h \
    classes/work.h \
    constants.h \
    filemanager.h \
    parser.h \
    reader.h \
    unparser.h \
    writer.h
