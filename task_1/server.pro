QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = server

SOURCES += main.cpp \
           mytcpserver.cpp

HEADERS += mytcpserver.h