#ifndef GAUSSIANBLUR_H
#define GAUSSIANBLUR_H
//  GaussianBlurTestFrame.h
/*
 ***************************************************************
 * Name:      GaussianBlurTestFrame.cpp
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
class GaussianBlurTestFrame : public wxPanel
{
 public :
    GaussianBlurTestFrame(wxWindow* parent);

    wxPanel          *  m_informPanel;                //  Панель для вывода информации о тестировании
    wxPanel          *  m_filtering_namePanel;        //  Панель для вывода информации о тестировании
    wxPanel          *  m_signaturePanel;             //  Панель для вывода информации о тестировании

    wxStaticText     *  m_filtering_name_text;
    wxStaticText     *  m_signature_name_text;
    std::string         m_signature_text;

    wxPanel          *  m_previewPanel;               //  Панель для предварительного просмотра исходного изображения
    wxScrolledWindow *  m_scrolled_wind_original;     //  Окно для просмотра масштабируемого исходного изображения
    wxScrolledWindow *  m_scrolled_wind_filtered;     //  Окно для просмотра масштабируемого отфильтрованного изображения

    wxBitmap            m_previewBitmap;
    wxBitmap            m_originalBitmap;
    wxBitmap            m_filteredBitmap;

    wxStaticBitmap *    m_staticPreviewBitmap = nullptr;
    wxStaticBitmap *    m_staticOriginalBitmap;
    wxStaticBitmap *    m_staticFilteredBitmap;


    cv::Mat             m_cv_original_img;
    cv::Mat             m_cv_filtered_img;

    wxImage             m_wx_img;


    ZoomSliderCtrl   *  m_zoom_slider_ctrl;

    KernelMatrixCtrl *  m_kernel_width_ctrl;
    int                 m_kernel_width = 3;

    wxSpinCtrlDouble *  m_spin_sigmaX_ctrl;
    double              m_sigmaX;

    wxSpinCtrlDouble *  m_spin_sigmaY_ctrl;
    double              m_sigmaY;

    wxChoice         *  m_border_chois_ctrl;
    int                 m_border_extrapolation  = cv::BORDER_CONSTANT;  //  default constant

    void OnResize(wxSizeEvent& event);

    void OnZoomSliderChanged(wxCommandEvent& event);
    void OnKernelWidthChanged(wxCommandEvent& event);
    void OnSigmaXChanged(wxCommandEvent& event);
    void OnSigmaYChanged(wxCommandEvent& event);
    void OnBorderExtrapolChanged(wxCommandEvent& event);

    void UpdateAllViews();
 private:
    void            ApplyFiltering();
    std::string     getSignatureText();
};

#endif // GAUSSIANBLUR_H
