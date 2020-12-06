QT += core gui network widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AuthWidget.cpp \
    MainDialog.cpp \
    ProjectListWidget.cpp \
    ProjectWidget.cpp \
    main.cpp \
    qtredmine/KeyAuthenticator.cpp \
    qtredmine/Logging.cpp \
    qtredmine/PasswordAuthenticator.cpp \
    qtredmine/RedmineClient.cpp \
    qtredmine/SimpleRedmineClient.cpp

HEADERS += \
    AuthWidget.h \
    MainDialog.h \
    ProjectListWidget.h \
    ProjectWidget.h \
    qtredmine/Authenticator.h \
    qtredmine/KeyAuthenticator.h \
    qtredmine/Logging.h \
    qtredmine/PasswordAuthenticator.h \
    qtredmine/RedmineClient.h \
    qtredmine/SimpleRedmineClient.h \
    qtredmine/SimpleRedmineTypes.h

FORMS += \
    AuthWidget.ui \
    MainDialog.ui \
    ProjectListWidget.ui \
    ProjectWidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
