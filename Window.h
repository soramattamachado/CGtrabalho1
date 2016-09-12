/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Window.h
 * Author: renato
 *
 * Created on 14 de Agosto de 2016, 22:31
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <algorithm>
#include "Point.h"

    /* +++++++++++++++++++++++++++++++ CONSTANTS ++++++++++++++++++++++++++++++++ */
    static const double X_MIN_DEFAULT = 0;
    static const double Y_MIN_DEFAULT = 0;
    static const double X_MAX_DEFAULT = 500;
    static const double Y_MAX_DEFAULT = 400;
    static const double WINDOW_MIN_VALUE = -10000;
    static const double WINDOW_MAX_VALUE = 10000;
    static const double START_ANGLE = 0;
    static const int SCN_MATRIX_SIZE = 3;
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
class Window {
public:
    Window(double xMin, double yMin, double xMax, double yMax);
    Window();
    virtual ~Window();
    void moveUp(double value);
    void moveLeft(double value);
    void moveRight(double value);
    void moveDown(double value);
    void zoomIn(double Value);
    void zoomOut(double value);
    double getXmin();
    double getYmin();
    double getXmax();
    double getYmax();
    double getHeight();
    double getWidth();
    double getAngle();
    Point getCenter();
    void generateDescriptionSCN();
private:


    void initiateMatrix(double m[][SCN_MATRIX_SIZE]);
    void translateWorld(double dx, double dy);
    void multiplyMatrixSCN(double m[][SCN_MATRIX_SIZE]);
    double distance(double a, double b);
    double setAngle(double value);
    double _xMin;
    double _yMin;
    double _xMax;
    double _yMax;
    double _angle;
    double _SCNdescriptionMatrix[SCN_MATRIX_SIZE][SCN_MATRIX_SIZE];
};

#endif /* WINDOW_H */

