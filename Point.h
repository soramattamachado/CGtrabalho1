/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Point.h
 * Author: renato
 *
 * Created on 14 de Agosto de 2016, 12:19
 */

#ifndef POINT_H
#define POINT_H

class Point {
public:
    Point(int x, int y);
    Point();
    virtual ~Point();
    int getX();
    int getY();
private:
    int _x;
    int _y;
};

#endif /* POINT_H */
