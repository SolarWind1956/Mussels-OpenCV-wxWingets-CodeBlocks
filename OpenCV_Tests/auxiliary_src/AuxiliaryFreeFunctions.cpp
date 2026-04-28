//  AuxiliaryFreeFunctions.cpp
/*
 **************************************************************
 * Name:      AuxiliaryFreeFunctions.cpp
 * Purpose:   Code for Application
 * Author:    Sergej E. Heckel (Solar_Wind1956@yahoo.com)
 *              with insightful support from Gemini (AI)
 * 	                "Дорогу осилит идущий, а вдвоем идти спорее"
 * Created:   2026-06-21
 * Copyright: Sergej E. Heckel (www.Solar_Wind1956.com)
 * License:
 **************************************************************
*/
#include "AuxiliaryFreeFunctions.h"
//  Вспомогательные функции
/*
    ----------------------------------------------------------------------------------------------------
    Функции открытия файла и получения соответствующей матрицы изображения
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
cv::Mat     getOriginalImage(){
    cv::Mat original_img = cv::Mat::zeros(400, 400, CV_8UC3);

    // По желанию: рисуем на нем текст, чтобы было понятно, что файл не загружен
    cv::putText (   original_img
                ,   "No Image Loaded"
                ,   cv::Point(50, 200)
                ,   cv::FONT_HERSHEY_SIMPLEX
                ,   1.0
                ,   cv::Scalar(255, 255, 255) // Белый цвет
                ,   2
                );
    //  Выбираем файл с изображением для считывания
    wxString    fullFileName;               //  Переменная для получения полноо имени файла

    if (!getFullFileName(fullFileName)){
        cout << bright_red << "Ошибка получения полного имени файла. Завершение работы." << reset << endl;
    } else {
        // 	Считываем изображение из файла
        cv::Mat  temp_RGB_or_BGR = cv::imread(std::string(fullFileName), cv::IMREAD_COLOR);

        #if 0
        if(tempRGB.empty()){
            cout << bright_red << std::string(fullFileName) + "Ошибка загрузки изображения. Завершение работы." << reset << endl;
		} else {
            cv::cvtColor(tempRGB, original_img, cv::COLOR_BGR2RGB); //
		}
		#endif // 0

        if (!temp_RGB_or_BGR.empty()) {
            cv::cvtColor(temp_RGB_or_BGR, original_img, cv::COLOR_BGR2RGB);
        };
	};
	return original_img.clone();
}
/*
    ----------------------------------------------------------------------------------------------------
    Функция отображения исследуемог изображения в прокруточных окнах
*/
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

        bitmap              = wxBitmap(wx_img_copy.Rescale  (   mat_image.cols * zoom
                                                            ,   mat_image.rows * zoom
                                                            ,   wxIMAGE_QUALITY_HIGH)
                                                            );


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
        staticBitmap->Refresh();
        staticBitmap->Update();
        scrolled_window->Layout();      //  Говорим окну: "Твоё содержимое обновилось, пересчитай размеры"
        scrolled_window->Refresh();     //  Говорим системе: "Перерисуй окно прямо сейчас"
}
/*
    ----------------------------------------------------------------------------------------------------
    Функция масштабирования изображения для предварительного просмотра
*/
wxBitmap    GetScaledBitmap(const wxBitmap& original, int targetW, int targetH)
{
    wxImage img = original.ConvertToImage();

    //  Хитрость №2: Сохранение пропорций
    double aspect = (double)original.GetWidth() / original.GetHeight();

    if (targetW / aspect <= targetH) {
        targetH = targetW / aspect;
    } else {
        targetW = targetH * aspect;
    }

    //  Используем качественный алгоритм масштабирования (wxIMAGE_QUALITY_HIGH)
    return wxBitmap(img.Scale(targetW, targetH, wxIMAGE_QUALITY_HIGH));
}
