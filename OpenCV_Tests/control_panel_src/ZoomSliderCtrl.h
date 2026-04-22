//  ZoomSliderCtrl.h
#ifndef ZOOMSLIDERCTRL_H
#define ZOOMSLIDERCTRL_H
/*
    --------------------------------------------------------------------------------------------------------------------
    Класс компонента выбора масштаба отображения изображений
*/
class ZoomSliderCtrl : public wxControl{
 public:
    ZoomSliderCtrl(wxWindow * parent) ;

    void OnScroll(wxCommandEvent& event);

    wxSlider *      zoom_slider;

    int     m_zoom    =   1;
};
//  ------------------------------------------------------------------------------------------------------
//#include "ZoomSliderCtrl.cpp"
#endif // ZOOMSLIDERCTRL_H
