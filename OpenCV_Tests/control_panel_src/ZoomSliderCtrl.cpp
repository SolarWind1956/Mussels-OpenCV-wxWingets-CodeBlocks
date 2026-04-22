//  ZoomSliderCtrl.cpp
/*
 ***************************************************************
 * Name:      ZoomSliderCtrl.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergi E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-21
 * Copyright: Sergi E. Heckel (www.Solar_Wind1956.com)
 * License:
 *************************************************************
*/
#include "ZoomSliderCtrl.h"

/*
    Класс компонента выбора масштаба отображения изображений
*/
ZoomSliderCtrl::ZoomSliderCtrl(wxWindow *   parent)
               : wxControl              (   parent
                                        ,   wxID_ANY
                                        )
{
    zoom_slider  =   new     wxSlider   (   this
                                        ,   wxID_ANY
                                        ,   1                       //  Начальное значение
                                        ,   1                       //  Минимальное значение
                                        ,   5                       //  Максимальное значение
                                        ,   wxPoint(10, 10)
                                        ,   wxSize(100, -1)         //  горизонтальный слайдер
                                        ,   wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS
                                        );
    zoom_slider->SetTickFreq(10);

    //  Связываем событие wxEVT_SLIDER с заданным пользователем методом OnScroll():
    //Bind (wxEVT_SLIDER,   &ZoomSliderCtrl::OnScroll,  this);
}

/*
    В методе OnScroll() мы получаем текущее значение слайдера
    и вызываем метод Refresh(),
    который генерирует событие wxEVT_PAINT:
*/
void ZoomSliderCtrl::OnScroll(wxCommandEvent& event)
{
    m_zoom    =   zoom_slider->GetValue();
    Refresh();
}
