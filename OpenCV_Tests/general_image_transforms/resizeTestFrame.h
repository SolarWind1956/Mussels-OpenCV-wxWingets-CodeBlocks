#ifndef RESIZE_H
#define RESIZE_H
//  resizeTestFrame.h
/*
 ***************************************************************
 * Name:      resizeTestFrame.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-25
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
class resizeTestFrame : public wxFrame
{
 public :
    resizeTestFrame(wxWindow* parent, const wxString& title);

    wxPanel             *   m_informPanel;                //  Информация о тестировании
    wxPanel             *   m_transforming_namePanel;        //  Вид фильтрации
    wxPanel             *   m_signaturePanel;             //  Сигнатуры фильтров
    wxPanel             *   m_debugPanel;                 //  Панель для отладочного вывода

    wxStaticText        *   m_transform_name_text;
    wxStaticText        *   m_signature_name_text;
    std::string             m_signature_text;

    wxTextCtrl          *   m_debugInfo;

    wxPanel             *   m_previewPanel;               //  Предварительный просмотр исходного изображения
    wxScrolledWindow    *   m_scrolled_wind_original;     //  Масштабируемоое исходное изображение
    wxScrolledWindow    *   m_scrolled_wind_resized;      //  Масштабируемое трансформированное изображения


    wxBitmap                m_previewBitmap;
    wxBitmap                m_originalBitmap;
    wxBitmap                m_resizedBitmap;

    wxStaticBitmap      *   m_staticPreviewBitmap = nullptr;
    wxStaticBitmap      *   m_staticOriginalBitmap = nullptr;
    wxStaticBitmap      *   m_staticResizedBitmap = nullptr;

    //  Матрицы изображений для вывода на экран и правильных преобразований
    cv::Mat                 m_cv_original_img;      //  Оригинальное изображение
    cv::Mat                 m_cv_resized_img;       //  Изображение после изменения размера

    //  Изображение для отобржения
    wxImage                 m_wx_img;

    //  1.
    ZoomSliderCtrl      *   m_zoom_orig_slider_ctrl;
    ZoomSliderCtrl      *   m_zoom_resz_slider_ctrl;

    //  2.  Новое значение для ширины изображеия
    wxSpinCtrl          *   m_spin_newWidth_ctrl;
    int                     m_newWidth                  = 0;

    //  3.  Новое значение для высоты изображеия
    wxSpinCtrl          *   m_spin_newHeight_ctrl;
    int                     m_newHeight                 = 0;

    //  4.  Коэффициент масштабирования по ширине изображения (по оси X)
    wxSpinCtrlDouble    *   m_spin_X_scaling_ctrl;
    double                  m_X_scaling                 = 1.0;

    //  5.  Коэффициент масштабирования по высоте изображения (по оси Y)
    wxSpinCtrlDouble    *   m_spin_Y_scaling_ctrl;
    double                  m_Y_scaling                 = 1.0;

    //  6.  Метод интерполяции
    wxChoice            *   m_interpolation_chois_ctrl;
    int                     m_interpolation             =  cv::INTER_LINEAR;


    void OnResize(wxSizeEvent& event);

    //  1.
    void OnZoomOrigSliderChanged(wxCommandEvent& event);
    void OnZoomReszSliderChanged(wxCommandEvent& event);

    //  2.
    void OnNewWidthChanged(wxSpinEvent& event);
    //  3.
    void OnNewHeightChanged(wxSpinEvent& event);
    //  4.
    void OnX_scalingChanged(wxSpinDoubleEvent& event);
    //  5.
    void OnY_scalingChanged(wxSpinDoubleEvent& event);
    //  6.
    void OnInterpolationChanged(wxCommandEvent& event);

    void UpdateAllViews();
 private:
    void            ApplyTransformation();
    std::string     getSignatureText();
};

#endif // RESIZE_H
