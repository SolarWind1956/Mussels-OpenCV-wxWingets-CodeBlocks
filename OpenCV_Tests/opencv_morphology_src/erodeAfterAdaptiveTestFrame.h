#ifndef ERODEAFTERTADAPTIVE_H
#define ERODEAFTERTADAPTIVE_H
//  erodeAfterAdaptiveTestFrame.h
/*
 ***************************************************************
 * Name:      erodeAfterAdaptiveTestFrame.cpp
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
class erodeAfterAdaptiveTestFrame : public wxFrame
{
 public :
    erodeAfterAdaptiveTestFrame(wxWindow* parent, const wxString& title);

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
    wxScrolledWindow    *   m_scrolled_wind_filtered;   //  Масштабируемое отфильтрованное изображения
    wxScrolledWindow    *   m_scrolled_wind_transformed;   //  Масштабируемое отфильтрованное изображения

    wxBitmap                m_previewBitmap;
    wxBitmap                m_originalBitmap;
    wxBitmap                m_filteredBitmap;
    wxBitmap                m_transformedBitmap;

    wxStaticBitmap      *   m_staticPreviewBitmap = nullptr;
    wxStaticBitmap      *   m_staticOriginalBitmap = nullptr;
    wxStaticBitmap      *   m_staticFilteredBitmap = nullptr;
    wxStaticBitmap      *   m_staticTransformeddBitmap = nullptr;

    //  Матрицы изображений для вывода на экран и правильных преобразований
    cv::Mat                 m_cv_original_img;      //  Оригинальное изображение
    cv::Mat                 m_cv_filtered_img;      //  Изображение после адаптивной пороговой фильтрации
    cv::Mat                 m_cv_transformed_img;   //  Изображение после морфологической трансфоормации

    cv::Mat                 m_filtered_img_for_display;
    cv::Mat                 m_image_for_transform;
    cv::Mat                 m_transformed_img_for_display;
    cv::Mat                 m_binary_filtered_img;
    cv::Mat                 m_gray_original_img;
    //  Изображение для отобржения
    wxImage                 m_wx_img;

    //  1.
    ZoomSliderCtrl      *   m_zoom_slider_ctrl;
    //  2.
    wxSpinCtrlDouble    *   m_spin_maxValue_ctrl;
    double                  m_maxValue                      = 250.0;
    //  3.
    wxChoice            *   m_choice_adaptiveMethod_ctrl;
    int                     m_adaptiveMethod                = cv::ADAPTIVE_THRESH_GAUSSIAN_C;
    //  4.
    wxChoice            *   m_choice_adaptiveType_ctrl;
    int                     m_adaptiveType                  = cv::THRESH_BINARY;
    //  5.
    wxSpinCtrl          *   m_spin_blockSize_ctrl;
    int                     m_blockSize                     = 21;
    //  6.
    wxSpinCtrlDouble    *   m_spin_Offset_ctrl;
    double                  m_Offset                        = -3.0;      //  Какое-то смещение
    //  7.
    KernelMatrixCtrl    *   m_kernel_width_ctrl;
    int                     m_kernel_width                  = 3;
    KernelMatrixCtrl    *   m_kernel_height_ctrl;
    int                     m_kernel_height                 = 3;
    //  8.
    wxRadioBox          *   m_kernel_shape_radiobox_ctrl;
    int                     m_kernel_shape = cv::MORPH_RECT;
    //  9.
    wxSpinCtrl          *   m_iterations_spin_ctrl;
    int                     m_iterations = 1;
    //  10.
    wxChoice            *   m_border_chois_ctrl;
    int                     m_border_extrapolation          = cv::BORDER_CONSTANT;
    //  11.
    //  Флаг переключения типа морфологической трансформации "Полутона" / "Цвет"
    wxRadioBox          *   m_gray_color_radiobox_ctrl;
    int                     m_gray_color_idx                = 0;   //  Gray

    void OnResize(wxSizeEvent& event);

    //  1.
    void OnZoomSliderChanged(wxCommandEvent& event);
    //  2.
    void OnMaxValueChanged(wxSpinDoubleEvent& event);
    //  3.
    void OnAdaptiveMethodChanged(wxCommandEvent& event);
    //  4.
    void OnAdaptiveTypeChanged(wxCommandEvent& event);
    //  5.
    void OnBlockSizeChanged(wxSpinEvent& event);
    //  6.
    void OnOffsetChanged(wxSpinDoubleEvent& event);
    //  7.
    void OnKernelWidthChanged(wxCommandEvent& event);
    void OnKernelHeightChanged(wxCommandEvent& event);
    //  8.
    void OnKernelShapeChanged(wxCommandEvent& event);
    //  9.
    void OnIterationsChanged(wxCommandEvent& event);
    //  10.
    void OnBorderExtrapolChanged(wxCommandEvent& event);
    //  11.
    void OnGrayOrColorChanged(wxCommandEvent& event);


    void UpdateAllViews();
 private:
    void            ApplyMixedTransformation();
    std::string     getSignatureText();
    wxArrayString   getKernelShape();
    int             GetSelectedMorphShape();
    wxArrayString   getGrayOrColor();
};

#endif // ERODEAFTERTADAPTIVE_H
