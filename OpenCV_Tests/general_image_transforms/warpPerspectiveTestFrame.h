#ifndef WARPPERSPECTIVE_H
#define WARPPERSPECTIVE_H
//  warpPerspectiveTestFrame.h
/*
 ***************************************************************
 * Name:      warpPerspectiveTestFrame.cpp
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
class warpPerspectiveTestFrame : public wxFrame
{
 public :
    warpPerspectiveTestFrame(wxWindow* parent, const wxString& title);

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

    //  5.  Верхний Левый угол
    wxSpinCtrlDouble    *   m_spin_top_left_col_ctrl;
    double                  m_top_left_col             = 0.05;

    wxSpinCtrlDouble    *   m_spin_top_left_row_ctrl;
    double                  m_top_left_row             = 0.33;

    //  6.  Верхний Правый угол
    wxSpinCtrlDouble    *   m_spin_top_right_col_ctrl;
    double                  m_top_right_col             = 0.9;

    wxSpinCtrlDouble    *   m_spin_top_right_row_ctrl;
    double                  m_top_right_row             = 0.25;

    //  7.  Нижний Левый угол
    wxSpinCtrlDouble    *   m_spin_bottom_left_col_ctrl;
    double                  m_bottom_left_col           = 0.8;

    wxSpinCtrlDouble    *   m_spin_bottom_left_row_ctrl;
    double                  m_bottom_left_row           = 0.9;

    //  8.  Нижний Правый угол
    wxSpinCtrlDouble    *   m_spin_bottom_right_col_ctrl;
    double                  m_bottom_right_col          = 0.2;

    wxSpinCtrlDouble    *   m_spin_bottom_right_row_ctrl;
    double                  m_bottom_right_row          = 0.7;

    //  9.  Метод интерполяции
    wxChoice            *   m_interpolation_chois_ctrl;
    int                     m_interpolation             =  cv::INTER_LINEAR;

    //  10.  Тип  экстраполяции границы (borderType)
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
    void On_top_left_col_Changed(wxSpinDoubleEvent& event);
    void On_top_left_row_Changed(wxSpinDoubleEvent& event);
    //  6.
    void On_top_right_col_Changed(wxSpinDoubleEvent& event);
    void On_top_right_row_Changed(wxSpinDoubleEvent& event);
    //  7.
    void On_bottom_left_col_Changed(wxSpinDoubleEvent& event);
    void On_bottom_left_row_Changed(wxSpinDoubleEvent& event);
    //  8.
    void On_bottom_right_col_Changed(wxSpinDoubleEvent& event);
    void On_bottom_right_row_Changed(wxSpinDoubleEvent& event);
    //  9.
    void On_interpolationChanged(wxCommandEvent& event);
    //  10.
    void On_borderTypeChanged(wxCommandEvent& event);

    void UpdateAllViews();
 private:
    void            ApplyTransformation();
    std::string     getSignatureText();
    cv::Mat         get_PerspectiveTransfMatrix();
};

#endif // WARPPERSPECTIVE_H
