#include <opencv2/opencv.hpp>
#include <semcv/semcv.hpp>
#include <iostream>
#include <vector>

// Обёртки над функциями библиотеки semcv
inline cv::Mat create_target_image(int a, int b, int c) {
    return gen_tgtimg00(a, b, c);
}

inline cv::Mat apply_gaussian_noise(const cv::Mat& img, int sigma) {
    return add_noise_gau(img, sigma);
}

// Модифицированная функция генерации гистограммы
inline cv::Mat generate_histogram(const cv::Mat& img, const cv::Scalar& color) {
    // Конвертируем изображение в grayscale, если оно цветное
    cv::Mat gray_img;
    if (img.channels() == 3) {
        cv::cvtColor(img, gray_img, cv::COLOR_BGR2GRAY);
    } else {
        gray_img = img.clone();
    }

    // Создаем белый фон (3 канала)
    cv::Scalar bg_color(255, 255, 255);
    
    // Преобразуем однокомпонентный цвет в трехкомпонентный серый
    cv::Scalar hist_color = color;
    if (color.rows == 1 && color.cols == 1) {
        hist_color = cv::Scalar(color[0], color[0], color[0]);
    }
    
    return make_hist(gray_img, bg_color, hist_color);
}

// Функция стандартизации изображений
cv::Mat standardize_image(const cv::Mat& img, int target_width = 300) {
    cv::Mat result = img.clone();
    
    // Изменение размера с сохранением пропорций
    if (result.cols != target_width) {
        double aspect_ratio = static_cast<double>(result.rows) / result.cols;
        int new_height = static_cast<int>(target_width * aspect_ratio);
        cv::resize(result, result, cv::Size(target_width, new_height));
    }
    
    // Преобразование в цветное изображение (только для визуализации)
    if (result.channels() == 1) {
        cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
    }
    
    // Нормализация типа
    if (result.depth() != CV_8U) {
        result.convertTo(result, CV_8U);
    }
    
    return result;
}

// Построение вертикальной "ленты" из изображений и гистограмм
cv::Mat create_histogram_display(cv::Mat& base_img, 
                                cv::Mat& noise_img1, 
                                cv::Mat& noise_img2, 
                                cv::Mat& noise_img3, 
                                cv::Scalar hist_color1, 
                                cv::Scalar hist_color2) 
{
    // Стандартизируем все изображения
    cv::Mat std_base = standardize_image(base_img);
    cv::Mat std_noise1 = standardize_image(noise_img1);
    cv::Mat std_noise2 = standardize_image(noise_img2);
    cv::Mat std_noise3 = standardize_image(noise_img3);
    
    // Генерируем гистограммы (передаем оригинальные grayscale изображения)
    cv::Mat hist_base = generate_histogram(base_img, hist_color1);  // Используем оригинальное grayscale
    cv::Mat hist_noise1 = generate_histogram(noise_img1, hist_color2);
    cv::Mat hist_noise2 = generate_histogram(noise_img2, hist_color1);
    cv::Mat hist_noise3 = generate_histogram(noise_img3, hist_color2);
    
    // Стандартизируем гистограммы для конкатенации
    hist_base = standardize_image(hist_base);
    hist_noise1 = standardize_image(hist_noise1);
    hist_noise2 = standardize_image(hist_noise2);
    hist_noise3 = standardize_image(hist_noise3);
    
    // Собираем вертикальную композицию
    std::vector<cv::Mat> vertical_components = {
        std_base, hist_base,
        std_noise1, hist_noise1,
        std_noise2, hist_noise2,
        std_noise3, hist_noise3
    };
    
    cv::Mat composite;
    cv::vconcat(vertical_components, composite);
    
    return composite;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <output_path>" << std::endl;
        return 1;
    }

    // Явное определение цветов в BGR формате
    const cv::Scalar LIGHT_GRAY(195, 195, 195);  // Светло-серый
    const cv::Scalar DARK_GRAY(160, 160, 160);   // Темно-серый

    // Базовые изображения с различными уровнями
    std::vector<cv::Mat> base_images = {
        create_target_image(0, 127, 255),
        create_target_image(20, 127, 235),
        create_target_image(55, 127, 200),
        create_target_image(90, 127, 165)
    };

    const std::vector<int> noise_levels = {3, 7, 15};

    // Генерация зашумленных изображений
    std::vector<std::vector<cv::Mat>> noise_sets;
    for (const auto& img : base_images) {
        std::vector<cv::Mat> noisy_set;
        for (int sigma : noise_levels) {
            noisy_set.push_back(apply_gaussian_noise(img, sigma));
        }
        noise_sets.push_back(noisy_set);
    }

    // Создание гистограммных полос
    std::vector<cv::Mat> image_rows;
    const std::vector<cv::Scalar> color_pattern = {LIGHT_GRAY, DARK_GRAY};

    for (size_t i = 0; i < base_images.size(); ++i) {
        image_rows.push_back(create_histogram_display(
            base_images[i],
            noise_sets[i][0],
            noise_sets[i][1],
            noise_sets[i][2],
            color_pattern[i % 2],
            color_pattern[(i + 1) % 2]
        ));
    }

    // Выравнивание высоты для горизонтальной конкатенации
    int max_height = 0;
    for (const auto& img : image_rows) {
        max_height = std::max(max_height, img.rows);
    }
    
    for (auto& img : image_rows) {
        if (img.rows < max_height) {
            cv::copyMakeBorder(img, img, 
                              0, max_height - img.rows, 
                              0, 0, 
                              cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
        }
    }

    // Создание финального изображения
    cv::Mat result;
    cv::hconcat(image_rows, result);

    // Сохранение результата
    try {
        cv::imwrite(argv[1], result);
        std::cout << "Successfully saved result to: " << argv[1] << std::endl;
    } catch (const cv::Exception& e) {
        std::cerr << "Error saving image: " << e.what() << std::endl;
        return 2;
    }

    return 0;
}