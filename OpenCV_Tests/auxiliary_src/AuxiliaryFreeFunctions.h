//  AuxiliaryFreeFunctions.h
/*
 **************************************************************
 * Name:      OpenCV_Tester.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-21
 * Copyright: Sergej E. Heckel (https://sites.google.com/view/sergej-heckel)
 * License:
 **************************************************************
*/
#ifndef AUXILIARYFREEFUNCTIONS_H_INCLUDED
#define AUXILIARYFREEFUNCTIONS_H_INCLUDED

#include <iostream>
using std::cout; using std::endl; using std::cerr;  using std::boolalpha;
#include <functional>

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/dirctrl.h>
#include <wx/dir.h>
#include <wx/splitter.h>
#include <wx/dnd.h>
#include <wx/listctrl.h>
#include <wx/font.h>
#include <wx/control.h>
#include <wx/filename.h>
#include <wx/slider.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "color_output.hpp"
using namespace color_output;

//  Вспомогательные функции
/*
    ----------------------------------------------------------------------------------------------------
    Функции открытия файла и получения соответствующей матрицы изображения
*/
bool        getFullFileName(wxString &  fullFileName);
cv::Mat     getOriginalImage();

/*
    ----------------------------------------------------------------------------------------------------
    Функция отображения исследуемог изображения в прокруточных окнах
*/
void UpdateDisplay  (   wxImage &           wx_img
                    ,   cv::Mat &           mat_image
                    ,   wxBitmap &          bitmap
                    ,   wxStaticBitmap*&    staticBitmap
                    ,   wxScrolledWindow*&  scrolled_window
                    ,   int                 zoom
                    );
/*
    ----------------------------------------------------------------------------------------------------
    Функция масштабирования изображения для предварительного просмотра
*/
// Пример функции для получения масштабированного битмапа
wxBitmap    GetScaledBitmap(const wxBitmap& original, int targetW, int targetH);


#endif // AUXILIARYFREEFUNCTIONS_H_INCLUDED
