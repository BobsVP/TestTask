// Создайте класс, который читает формат BMP (24 или 32 бита) и выводит в консоль двумя разными символами два цвета: 
// черный и белый. Для чтения из файла используйте библиотеку <fstream>. 
// Ваш класс должен инкапсулировать все необходимые данные по открытию и отображению картинки, а также 
// 3 главные функции приложения: openBMP(const string & fileName), displayBMP(), closeBMP()

#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>

struct BmpHead
{
    std::uint16_t type;                // Тип файла
    std::uint32_t size;                // Размер файла в байтах
    std::uint16_t reserved1;           // Зарезервировано
    std::uint16_t reserved2;           // Зарезервировано
    std::uint32_t offset_data;         // Смещение до начала данных изображения
    std::uint32_t size_hed;            // Размер заголовка
    std::int32_t width;                // Ширина изображения
    std::int32_t height;               // Высота изображения
    std::uint16_t planes;              // Количество цветовых плоскостей
    std::uint16_t bits_per_pixel;      // Биты на пиксель
    std::uint32_t compression;         // Тип сжатия
    std::uint32_t size_image;          // Размер изображения
    std::int32_t x_pixels_per_meter;   // Горизонтальное разрешение
    std::int32_t y_pixels_per_meter;   // Вертикальное разрешение
    std::uint32_t colors_used;         // Количество используемых цветов
    std::uint32_t colors_important;    // Количество важных цветов
    std::ifstream file;                // Открытый файл
};

class drawBmp {
public:
    void openBMP(const std::string& fileName) {             //Открытие файла BMP и чтение заголовка
        file = std::ifstream(fileName, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << fileName << std::endl;
            return;
        }

        read_data(head.type);
        if (head.type != 0x4D42) {                          // BM 
            std::cerr << "Error: '" << fileName << "' is not BMP file." << std::endl;
            return;
        }

        read_data(head.size);
        read_data(head.reserved1);
        read_data(head.reserved2);
        read_data(head.offset_data);
        read_data(head.size_hed);
        read_data(head.width);
        read_data(head.height);
        read_data(head.planes);
        read_data(head.bits_per_pixel);

        std::cout << "BMP size: " << head.size << " bytes" << std::endl;
        std::cout << "Width: " << head.width << " pixels" << std::endl;
        std::cout << "Height: " << head.height << " pixels" << std::endl;
        std::cout << "Bits per pixel: " << head.bits_per_pixel << std::endl;

        if (head.bits_per_pixel != 24 && head.bits_per_pixel != 32) {
            std::cerr << "It's not 24-bit or 32-bit BMP." << std::endl;
            return;
        }

        read_data(head.compression);
        read_data(head.size_image);
        read_data(head.x_pixels_per_meter);
        read_data(head.y_pixels_per_meter);
        read_data(head.colors_used);
        read_data(head.colors_important);
    }

    void displayBMP() {
        int linePadding = ((head.width * (head.bits_per_pixel / 8)) % 4); // Количество байт в строке обязательно должно быть кратно 4,
        linePadding = (4 - linePadding) % 4;                   // в конец каждого пиксельного ряда добавляются недостающие байты
        std::uint8_t R, G, B, Al;
        std::vector<std::vector<char>> Pixels;
        Pixels.resize(head.height);
        file.seekg(head.offset_data, std::ios::beg);

        for (int32_t y = 0; y < head.height; ++y) {              // Читаем пиксели в вектор
            for (int32_t x = 0; x < head.width; ++x) {
                read_data(R);
                read_data(G);
                read_data(B);
                if (head.bits_per_pixel == 32) read_data(Al);
                if (R == 255 && G == 255 && B == 255) Pixels[y].push_back('W');
                else Pixels[y].push_back('.');
            }
            file.seekg(linePadding, std::ios_base::cur);        // Пропускаем добавленные в конец ряда байты
        }

        for (auto y = Pixels.crbegin(); y != Pixels.crend(); ++y) {        // Выводим в правильном порядке
            for (auto x = 0; x < head.width; ++x) {
                std::cout << (*y)[x];
            }
            std::cout << std::endl;
        }
    }

    void closeBMP() {
        if (file.is_open()) {
            file.close();
        }
    }

    virtual ~drawBmp() {
        if (file.is_open()) {
            file.close();
        }
    }
private:
    BmpHead head;                       // Заголовок bmp файла
    std::ifstream file;                 // Открытый файл

    template <typename T>
    void read_data(T& value) {
        file.read((char*)(&value), sizeof(value));
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "The path to the BMP file should be used as an argument" << std::endl;
        return 1;
    }

    std::string filepath = argv[1];

    drawBmp OpenAndDraw;
    OpenAndDraw.openBMP(filepath);
    OpenAndDraw.displayBMP();
    OpenAndDraw.closeBMP();

    return 0;
}