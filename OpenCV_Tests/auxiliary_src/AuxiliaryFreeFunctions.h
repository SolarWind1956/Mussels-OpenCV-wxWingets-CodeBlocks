//  AuxiliaryFreeFunctions.h
/*
 **************************************************************
 * Name:      AuxiliaryFreeFunctions.h
 * Purpose:   Code for Application
 * Author:    Sergi E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-06-21
 * Copyright: Sergi E. Heckel (www.Solar_Wind1956.com)
 * License:
 **************************************************************
*/
#ifndef AUXILIARYFREEFUNCTIONS_H_INCLUDED
#define AUXILIARYFREEFUNCTIONS_H_INCLUDED
/*
 **************************************************************
 * Name:      AuxiliaryFreeFunctions.h
 * Purpose:   Code for Application
 * Author:    Sergi E. Heckel (Solar_Wind1956@yahoo.com)
 * Created:   2026-06-21
 * Copyright: Sergi E. Heckel (www.Solar_Wind1956.com)
 * License:
 **************************************************************
*/
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
    Функция открытия файла изображения
*/
bool getFullFileName(wxString &  fullFileName);

void UpdateDisplay  (   wxImage &           wx_img
                    ,   cv::Mat &           mat_image
                    ,   wxBitmap &          bitmap
                    ,   wxStaticBitmap*&    staticBitmap
                    ,   wxScrolledWindow*&  scrolled_window
                    ,   int                 zoom
                    );
/*

*/
// Пример функции для получения масштабированного битмапа
wxBitmap    GetScaledBitmap(const wxBitmap& original, int targetW, int targetH);


#endif // AUXILIARYFREEFUNCTIONS_H_INCLUDED
