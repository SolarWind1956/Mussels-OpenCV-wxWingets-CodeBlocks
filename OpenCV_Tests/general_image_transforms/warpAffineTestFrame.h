#ifndef WARPAFFINE_H
#define WARPAFFINE_H
//  warpAffineTestFrame.h
/*
 ***************************************************************
 * Name:      warpAffineTestFrame.cpp
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
class warpAffineTestFrame : public wxFrame
{
 public :
    warpAffineTestFrame(wxWindow* parent, const wxString& title);

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
    wxScrolledWindow    *   m_scrolled_wind_transformed;  //  Масштабируемое трансформированное изображения


    wxBitmap                m_previewBitmap;
    wxBitmap                m_originalBitmap;
    wxBitmap                m_transformedBitmap;

    wxStaticBitmap      *   m_staticPreviewBitmap = nullptr;
    wxStaticBitmap      *   m_staticOriginalBitmap = nullptr;
    wxStaticBitmap      *   m_staticTransformedBitmap = nullptr;

    //  Матрицы изображений для вывода на экран и правильных преобразований
    cv::Mat                 m_cv_original_img;      //  Оригинальное изображение
    cv::Mat                 m_cv_transformed_img;   //  Изображение после преобразования
    cv::Mat                 src;
    cv::Mat 		        warp_mat;

    //  Изображение для отобржения
    wxImage                 m_wx_img;

    //  1.
    ZoomSliderCtrl      *   m_zoom_orig_slider_ctrl;

    //  2.
    ZoomSliderCtrl      *   m_zoom_transf_slider_ctrl;

    //  3.  Новое значение для ширины изображеия
    wxSpinCtrl          *   m_spin_newWidth_ctrl;
    int                     m_newWidth                  = 0;

    //  4.  Новое значение для высоты изображеия
    wxSpinCtrl          *   m_spin_newHeight_ctrl;
    int                     m_newHeight                 = 0;

    //  5.  Угол поворота alpha (вокруг оси Z)
    wxSlider                *   m_alpha_slider_ctrl;
    int                     m_alpha                     = 0;

    //  6.  Угол поворота beta (вокруг оси X)
    wxSlider                *   m_beta_slider_ctrl;
    int                     m_beta                      = 0;

    //  7.  Угол поворота gamma (вокруг оси Y)
    wxSlider                *   m_gamma_slider_ctrl;
    int                     m_gamma                     = 0;

    //  8.  Метод интерполяции
    wxChoice            *   m_interpolation_chois_ctrl;
    int                     m_interpolation             =  cv::INTER_LINEAR;

    //  9.  Тип  экстраполяции границы (borderType)
    wxChoice            *   m_borderType_chois_ctrl;
    int                     m_borderType                =  cv::BORDER_CONSTANT;

    void OnResize(wxSizeEvent& event);

    //  1.
    void On_zoomOrigSliderChanged(wxCommandEvent& event);
    //  2.
    void On_zoomTransfSliderChanged(wxCommandEvent& event);
    //  3.
    void On_newWidthChanged(wxSpinEvent& event);
    //  4.
    void On_newHeightChanged(wxSpinEvent& event);
    //  5.
    void On_alphaChanged(wxCommandEvent& event);
    //  6.
    void On_betaChanged(wxCommandEvent& event);
    //  7.
    void On_gammaChanged(wxCommandEvent& event);
    //  8.
    void On_interpolationChanged(wxCommandEvent& event);
    //  9.
    void On_borderTypeChanged(wxCommandEvent& event);

    void UpdateAllViews();
 private:
    void            ApplyTransformation();
    std::string     getSignatureText();
    cv::Mat         get_M(int, int, int);
};

#endif // WARPAFFINE_H
