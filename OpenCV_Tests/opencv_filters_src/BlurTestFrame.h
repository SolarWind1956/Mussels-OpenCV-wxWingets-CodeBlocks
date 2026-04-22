//  BlurTestFrame.h
/*
 ***************************************************************
 * Name:      BlurTestFrame.h
 * Purpose:   Code for Application Class
 * Author:    Sergi E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-22
 * Copyright: Sergi E. Heckel (www.Solar_Wind1956.com)
 * License:
 *************************************************************
*/
#ifndef BLURTESTFRAME_H
#define BLURTESTFRAME_H

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

#include "../auxiliary_src/AuxiliaryFreeFunctions.h"

#include "../control_panel_src/KernelMatrixCtrl.h"
#include "../control_panel_src/ZoomSliderCtrl.h"
/*
    --------------------------------------------------------------------------------------------------------------------
    Класс тестового фрейма для изображений и компонетнов управления
*/
class BlurTestFrame : public wxPanel
{
 public :
    BlurTestFrame(wxWindow* parent);

    wxPanel          *  informPanel;                //  Панель для вывода информации о тестировании
    wxStaticText     *  m_filter_name_text;

    wxPanel          *  previewPanel;               //  Панель для предварительного просмотра исходного изображения
    wxScrolledWindow *  scrolled_wind_original;     //  Окно для просмотра масштабируемого исходного изображения
    wxScrolledWindow *  scrolled_wind_filtered;     //  Окно для просмотра масштабируемого отфильтрованного изображения

    wxBitmap            previewBitmap;
    wxBitmap            originalBitmap;
    wxBitmap            filteredBitmap;

    wxStaticBitmap *    staticPreviewBitmap = nullptr;
    wxStaticBitmap *    staticOriginalBitmap;
    wxStaticBitmap *    staticFilteredBitmap;


    cv::Mat             m_cv_original_img;
    cv::Mat             m_cv_filtered_img;

    wxImage             m_wx_img;


    ZoomSliderCtrl   *  zoom_slider_ctrl;

    KernelMatrixCtrl *  kernel_matrix_ctrl;
    int                 m_kernel_size = 7;

    void OnResize(wxSizeEvent& event);

    void OnZoomSlider(wxCommandEvent& event);
    void OnKernelChanged(wxCommandEvent& event);
    void UpdateAllViews();
 private:
    void ApplyFilters();
};

#endif // BLURTESTFRAME_H
