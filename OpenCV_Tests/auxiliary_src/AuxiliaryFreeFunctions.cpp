//  AuxiliaryFreeFunctions.cpp
/*
 **************************************************************
 * Name:      AuxiliaryFreeFunctions.cpp
 * Purpose:   Code for Application
 * Author:    Sergi E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-06-21
 * Copyright: Sergi E. Heckel (www.Solar_Wind1956.com)
 * License:
 **************************************************************
*/
#include "AuxiliaryFreeFunctions.h"
//  Вспомогательные функции
/*
    ----------------------------------------------------------------------------------------------------
    Функция открытия файла изображения
*/
bool getFullFileName(wxString &  fullFileName){
    //  Изображение выберем в диалоговом окне
    wxString        file_type_filter   =    "Image files (*.jpg;*.png)|"
                                            "*.jpg;*.png|JPEG files (*.jpg)|"
                                            "*.jpg|PNG files (*.png)|*.png";
    //  Пользуемся стандартным диалогом открыития файлов
    wxFileDialog    openFileDialog  (   nullptr
                                    ,   "Выберите изображение *.jpg или *.png"
                                    ,   ""
                                    ,   ""
                                    ,   file_type_filter
                                    ,   wxFD_OPEN | wxFD_FILE_MUST_EXIST
                                    );

    if (openFileDialog.ShowModal() != wxID_OK) {
        wxMessageBox("Ошибка открытия файла!");
        return false;
    }
    fullFileName    = openFileDialog.GetPath();

    wxFileName  fn(fullFileName);
    wxString    ext     =   fn.GetExt().Lower();
    if (ext != "jpg" && ext != "jpeg" && ext != "png") {
        wxMessageBox("Недопустимый формат файла!");
        return false;
    }

    return  true;
}
void UpdateDisplay  (   wxImage &           wx_img
                    ,   cv::Mat &           mat_image
                    ,   wxBitmap &          bitmap
                    ,   wxStaticBitmap*&    staticBitmap
                    ,   wxScrolledWindow*&  scrolled_window
                    ,   int                 zoom
                    )
{
        wx_img 	            = 	wxImage	(	mat_image.cols
                                        ,	mat_image.rows
                                        ,	mat_image.data
                                        ,	TRUE 				// 	статические данные, не освобождать в delete()
                                        );
        wxImage     wx_img_copy =   wx_img.Copy();

        bitmap              = wxBitmap(wx_img_copy.Rescale(mat_image.cols * zoom, mat_image.rows * zoom, wxIMAGE_QUALITY_HIGH));
        //bitmap              = wxBitmap(wx_img.Copy());

        // Важное исправление, о котором я говорил ранее (защита от утечки):
        if (!staticBitmap) {
            staticBitmap = new wxStaticBitmap(scrolled_window, -1, bitmap);
        } else {
            staticBitmap->SetBitmap(bitmap);
        }

        int width           = bitmap.GetWidth() ;
        int height          = bitmap.GetHeight() ;

        // Настройка полос прокрутки
        scrolled_window->SetScrollbars(10, 10, width / 10, height / 10);

        // А вот теперь — финальный аккорд:
        scrolled_window->Layout();      //  Говорим окну: "Твоё содержимое обновилось, пересчитай размеры"
        scrolled_window->Refresh();     //  Говорим системе: "Перерисуй окно прямо сейчас"
}
/*

*/
// Пример функции для получения масштабированного битмапа
wxBitmap    GetScaledBitmap(const wxBitmap& original, int targetW, int targetH)
{
    wxImage img = original.ConvertToImage();

    // Хитрость №2: Сохранение пропорций
    double aspect = (double)original.GetWidth() / original.GetHeight();

    if (targetW / aspect <= targetH) {
        targetH = targetW / aspect;
    } else {
        targetW = targetH * aspect;
    }

    // Используем качественный алгоритм масштабирования (wxIMAGE_QUALITY_HIGH)
    return wxBitmap(img.Scale(targetW, targetH, wxIMAGE_QUALITY_HIGH));
}
