//  OpenCV_Tester.h
/*
 ***************************************************************
 * Name:      OpenCV_Tester.h
 * Purpose:   Code for Application Class
 * Author:    Sergi E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-21
 * Copyright: Sergi E. Heckel (www.Solar_Wind1956.com)
 * License:
 *************************************************************
*/
#ifndef OpenCV_Tester_Window_H_INCLUDED
#define OpenCV_Tester_Window_H_INCLUDED

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

#include "color_output.hpp"
using namespace color_output;

#include "opencv_filters_src/BlurTestFrame.h"
#include "opencv_filters_src/medianBlurTestFrame.h"
/*
    --------------------------------------------------------------------------------------------------------------------
    Каркассный класс для тестирования библиотеки OpenCV

    Класс главного фрейма теста
*/
class OpenCV_Tester_Window : public wxFrame
{
 public:
    OpenCV_Tester_Window(const wxString& title);

    wxPanel             *	frame_panel;

    wxPanel             *	blur_frame_panel;
    wxPanel             *	median_blur_frame_panel;

    wxBoxSizer          *   vbox;
    wxBoxSizer          *   hbox1;
    wxBoxSizer          *   hbox2;


    //BlurTestFrame     *   test_frame;
    medianBlurTestFrame *   test_frame;
};

#endif  // OpenCV_Tester_Window_H_INCLUDED


