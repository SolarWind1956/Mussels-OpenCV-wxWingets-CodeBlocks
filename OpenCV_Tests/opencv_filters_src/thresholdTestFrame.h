#ifndef THRESHOLD_H
#define THRESHOLD_H
//  thresholdTestFrame.h
/*
 ***************************************************************
 * Name:      thresholdTestFrame.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-24
 * Copyright: Sergej E. Heckel (https://sites.google.com/view/sergej-heckel)
 * License:
 *************************************************************
*/
#include <iostream>
using std::cout; using std::endl; using std::cerr;  using std::boolalpha;
#include <functional>
#include <windows.h>
#include <string> // обязательно для to_wstring

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
#include <wx/spinctrl.h>
#include <wx/radiobox.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "color_output.hpp"
using namespace color_output;

#include "../auxiliary_src/AuxiliaryFreeFunctions.h"

#include "../control_panel_src/KernelMatrixCtrl.h"
#include "../control_panel_src/ZoomSliderCtrl.h"

/*
    --------------------------------------------------------------------------------------------------------------------
    Класс тестового фрейма для изображений и компонетнов управления
*/
class thresholdTestFrame : public wxPanel
{
 public :
    thresholdTestFrame(wxWindow* parent);

    wxPanel             *   m_informPanel;                //  Информация о тестировании
    wxPanel             *   m_filtering_namePanel;        //  Вид фильтрации
    wxPanel             *   m_signaturePanel;             //  Сигнатуры фильтров
    wxPanel             *   m_debugPanel;                 //  Панель для отладочного вывода

    wxStaticText        *   m_filtering_name_text;
    wxStaticText        *   m_signature_name_text;
    std::string             m_signature_text;

    wxTextCtrl          *   m_debugInfo;

    wxPanel             *   m_previewPanel;               //  Предварительный просмотр исходного изображения
    wxScrolledWindow    *   m_scrolled_wind_original;     //  Масштабируемоое исходное изображение
    wxScrolledWindow    *   m_scrolled_wind_filtered_1;   //  Масштабируемое отфильтрованное изображения
    wxScrolledWindow    *   m_scrolled_wind_filtered_2;   //  Масштабируемое отфильтрованное изображения

    wxBitmap                m_previewBitmap;
    wxBitmap                m_originalBitmap;
    wxBitmap                m_filteredBitmap_1;
    wxBitmap                m_filteredBitmap_2;

    wxStaticBitmap      *   m_staticPreviewBitmap = nullptr;
    wxStaticBitmap      *   m_staticOriginalBitmap = nullptr;
    wxStaticBitmap      *   m_staticFilteredBitmap_1 = nullptr;
    wxStaticBitmap      *   m_staticFilteredBitmap_2 = nullptr;

    cv::Mat                 m_cv_original_img;
    cv::Mat                 m_cv_filtered_img_1;
    cv::Mat                 m_cv_filtered_img_2;

    cv::Mat                 out1_for_display;
    cv::Mat                 out2_for_display;

    wxImage                 m_wx_img;
    //  1.
    ZoomSliderCtrl      *   m_zoom_slider_ctrl;
    //  2.
    wxSpinCtrlDouble    *   m_spin_thresh_ctrl;
    double                  m_thresh            = 1.0;
    //  3.
    wxSpinCtrlDouble    *   m_spin_maxValue_1_ctrl;
    double                  m_maxValue_1        = 1.0;
    //  4.
    wxSpinCtrlDouble    *   m_spin_maxValue_2_ctrl;
    double                  m_maxValue_2        = 15.0;
    //  5.
    wxChoice            *   m_choice_threshold_type_1_ctrl;
    int                     m_threshold_type_1  = cv::THRESH_TRUNC;
    //  6.
    wxChoice            *   m_choice_threshold_type_2_ctrl;
    int                     m_threshold_type_2  = cv::THRESH_BINARY_INV;
    //  7.
    wxChoice            *   m_choice_AdaptiveThresholdTypes_ctrl;
    int                     m_adaptiveMethod    = cv::ADAPTIVE_THRESH_GAUSSIAN_C;
    //  8.
    wxSpinCtrl          *   m_spin_blockSize_ctrl;
    int                     m_blockSize         = 71;
    //  9.
    wxSpinCtrlDouble    *   m_spin_Offset_ctrl;
    double                  m_Offset          = 15.0;      //  Какое-то смещение

    void OnResize(wxSizeEvent& event);

    //  1.
    void OnZoomSliderChanged(wxCommandEvent& event);
    //  2.
    void OnThreshChanged(wxSpinDoubleEvent& event);
    //  3.
    void OnMaxValue_1Changed(wxSpinDoubleEvent& event);
    //  4.
    void OnMaxValue_2Changed(wxSpinDoubleEvent& event);
    //  5.
    void OnThreshold_type_1Changed(wxCommandEvent& event);
    //  6.
    void OnThreshold_type_2Changed(wxCommandEvent& event);
    //  7.
    void OnAdaptiveThresholdTypesChanged(wxCommandEvent& event);
    //  8.
    void OnBlockSizeChanged(wxSpinEvent& event);
    //  9.
    void OnOffsetChanged(wxSpinDoubleEvent& event);

    void UpdateAllViews();
 private:
    void            ApplyFiltering();
    std::string     getSignatureText();
};

#endif // THRESHOLD_H
