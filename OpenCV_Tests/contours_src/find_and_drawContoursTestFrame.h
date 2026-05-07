#ifndef find_and_drawContours_H_INCLUDED
#define find_and_drawContours_H_INCLUDED
//  find_and_drawContoursTestFrame.h
/*
 ***************************************************************
 * Name:      find_and_drawContours.h
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-05-06
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
class find_and_drawContoursTestFrame : public wxFrame
{
 public :
    find_and_drawContoursTestFrame(wxWindow* parent, const wxString& title);

    wxPanel             *   m_informPanel;                //  Информация о тестировании
    wxPanel             *   m_transforming_namePanel;     //  Вид фильтрации
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
    cv::Mat                 gray;
    cv::Mat                 binary;
    cv::Mat		            blurred;
    cv::Mat                 gray_tmp;

    //  Вектор векторов точек контуров
    std::vector<std::vector<cv::Point>> 	contours;

    //  Изображение для отобржения
    wxImage                 m_wx_img;

    //  1.
    ZoomSliderCtrl      *   m_zoom_orig_slider_ctrl;

    //  2.
    ZoomSliderCtrl      *   m_zoom_transf_slider_ctrl;

    //  3.  Размер квадратного ядра Гауссиана
    KernelMatrixCtrl    *   m_kernel_width_ctrl;
    int                     m_kernel_width              = 3;

    //  4. 5.  Сигмы распределения Гауссиана
    wxSpinCtrlDouble    *   m_sigmaX_spin_ctrl;
    double                  m_sigmaX                    = 0.0;

    wxSpinCtrlDouble    *   m_sigmaY_spin_ctrl;
    double                  m_sigmaY                    = 0.0;

    //  6.  Тип экстраполяции границ Гауссиана (borderType)
    wxChoice            *   m_borderType_chois_ctrl;
    int                     m_borderType                =  cv::BORDER_CONSTANT;

    //  7.  Пороговое значение глобального порогового фильтра
    //wxSpinCtrlDouble    *   m_thresh_spin_ctrl;
    double                  m_thresh                    = 127.0;
    wxSlider            *   m_thresh_sldr_ctrl;
    //int                     m_thresh                     = 127;

    //  8.  Тип операции для порогового фильтра
    wxChoice            *   m_threshold_type_choice_ctrl;
    int                     m_threshold_type            = cv::THRESH_TRUNC;

    //  9.  Режим возврата контуров
    wxChoice            *   m_contour_return_mode_choice_ctrl;
    int                     m_contour_return_mode       = cv::RETR_LIST;

    //  10.  Метод апроксимации при поиске контуров
    wxChoice            *   m_approximation_method_choice_ctrl;
    int                     m_approximation_method      = cv::CHAIN_APPROX_SIMPLE;

    //  11.  Толщина линии контура при прорисовке
    wxSpinCtrl          *   m_thickness_spin_ctrl;
    int                     m_thickness                 = 1;

    //  12.  Максимальный уровень иерархии maxLevel
    wxSpinCtrl          *   m_maxLevel_spin_ctrl;
    int                     m_maxLevel                  = 1;

    void OnResize(wxSizeEvent& event);

    //  1.
    void On_zoomOrigSliderChanged(wxCommandEvent& event);
    //  2.
    void On_zoomTransfSliderChanged(wxCommandEvent& event);
    //  3.
    void On_kernel_widthChanged(wxCommandEvent& event);
    //  4.
    void On_sigmaXChanged(wxSpinDoubleEvent& event);
    //  5.
    void On_sigmaYChanged(wxSpinDoubleEvent& event);
    //  6.
    void On_borderTypeChanged(wxCommandEvent& event);
    //  7.
    void On_threshChanged(wxCommandEvent& event);
    //  8.
    void On_threshold_typeChanged(wxCommandEvent& event);
    //  9.
    void On_contour_return_modeChanged(wxCommandEvent& event);
    //  10.
    void On_approximation_methodChanged(wxCommandEvent& event);
    //  11.
    void On_thicknessChanged(wxSpinEvent& event);
    //  12.
    void On_maxLevelChanged(wxSpinEvent& event);

    void UpdateAllViews();

 private:
    void            ApplyTransformation();
    std::string     getSignatureText();
};

#endif // find_and_drawContours_H_INCLUDED
