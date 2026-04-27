//  ZoomSliderCtrl.h
/*
 **************************************************************
 * Name:      OpenCV_Tester.cpp
 * Purpose:   Code for Application Class
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-04-21
 * Copyright: Sergej E. Heckel (https://sites.google.com/view/sergej-heckel)
 * License:
 **************************************************************
*/
#ifndef ZOOMSLIDERCTRL_H
#define ZOOMSLIDERCTRL_H
/*
    --------------------------------------------------------------------------------------------------------------------
    Класс компонента выбора масштаба отображения изображений
*/
class ZoomSliderCtrl : public wxControl{
 public:
    ZoomSliderCtrl(wxWindow * parent, int max_zoom = 5);

    void OnScroll(wxCommandEvent& event);

    wxSlider *      zoom_slider;
    int     m_max_zoom  =   5;
    int     m_zoom      =   1;
};
//  ------------------------------------------------------------------------------------------------------
//#include "ZoomSliderCtrl.cpp"
#endif // ZOOMSLIDERCTRL_H
