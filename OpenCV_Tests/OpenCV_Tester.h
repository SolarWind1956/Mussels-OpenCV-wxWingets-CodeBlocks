//  OpenCV_Tester.h
/*
 ***************************************************************
 * Name:      OpenCV_Tester.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-21
 * Copyright: Sergej E. Heckel (https://sites.google.com/view/sergej-heckel)
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
#include <wx/simplebook.h>

#include "color_output.hpp"
using namespace color_output;

#include "opencv_filters_src/BlurTestFrame.h"
#include "opencv_filters_src/medianBlurTestFrame.h"
#include "opencv_filters_src/GaussianBlurTestFrame.h"
#include "opencv_filters_src/thresholdTestFrame.h"

#include "opencv_morphology_src/erodeTestFrame.h"
#include "opencv_morphology_src/dilateTestFrame.h"
#include "opencv_morphology_src/erodeAfterThresholdTestFrame.h"
#include "opencv_morphology_src/erodeAfterAdaptiveTestFrame.h"

#include "general_image_transforms/resizeTestFrame.h"
#include "general_image_transforms/warpAffineTestFrame.h"
#include "general_image_transforms/warpPerspectiveTestFrame.h"
/*
    --------------------------------------------------------------------------------------------------------------------
    Каркассный класс для тестирования библиотеки OpenCV

    Класс главного фрейма теста
*/
class OpenCV_Tester_Window : public wxFrame
{
 public:
    OpenCV_Tester_Window(const wxString& title);

    wxPanel         *	m_frame_panel;
    wxBoxSizer      *   vbox = nullptr;
    //  ---------------------------------------------------------------------------------------------------
    wxMenuBar       *   main_menu_bar;
    //  ---------------------------------------------------------------------------------------------------
    wxMenu          *   menu_Filters;
    wxMenu          *   menu_Morphology;
    wxMenu          *   menu_GeneralTransforms;

    //  ---------------------------------------------------------------------------------------------------
    wxMenuItem      *   menu_item_Blur;
    wxMenuItem      *   menu_item_medianBlur;
    wxMenuItem      *   menu_item_GaussianBlur;
    wxMenuItem      *   menu_item_threshold;

    //  ---------------------------------------------------------------------------------------------------
    wxMenuItem      *   menu_item_erode;
    wxMenuItem      *   menu_item_dilate;
    wxMenuItem      *   menu_item_erodeAfterThreshold;
    wxMenuItem      *   menu_item_erodeAfterAdaptive;

    //  ---------------------------------------------------------------------------------------------------
    wxMenuItem      *   menu_item_resize;
    wxMenuItem      *   menu_item_warpAffine;
    wxMenuItem      *   menu_item_warpPerspective;

    //  Filters and Convolution   -   Ядра, фильтры и свертки
    BlurTestFrame                       *   m_BlurPage;
    medianBlurTestFrame                 *   m_medianBlurPage;
    GaussianBlurTestFrame               *   m_GaussianBlurPage;
    thresholdTestFrame                  *   m_thresholdPage;

    void On_Blur(wxCommandEvent& event);
    void On_medianBlur(wxCommandEvent& event);
    void On_GaussianBlur(wxCommandEvent& event);
    void On_threshold(wxCommandEvent& event);

    //  Morphological transformations - морфологические преобразования
    erodeTestFrame                      *   m_erodePage;
    dilateTestFrame                     *   m_dilatePage;
    erodeAfterThresholdTestFrame        *   m_erodeAfterThresholdPage;
    erodeAfterAdaptiveTestFrame         *   m_erodeAfterAdaptivePage;

    void On_erode(wxCommandEvent& event);
    void On_dilate(wxCommandEvent& event);
    void On_erodeAfterThreshold(wxCommandEvent& event);
    void On_erodeAfterAdaptive(wxCommandEvent& event);

    //  General Image Transforms    -   Преобразования изображений общего вида
    resizeTestFrame                     *   m_resizePage;
    warpAffineTestFrame                 *   m_warpAffinePage;
    warpPerspectiveTestFrame            *   m_warpPerspectivePage;

    void On_resize(wxCommandEvent& event);
    void On_warpAffine(wxCommandEvent& event);
    void On_warpPerspective(wxCommandEvent& event);
};

#endif  // OpenCV_Tester_Window_H_INCLUDED


