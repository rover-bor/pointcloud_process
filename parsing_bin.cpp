#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main() {
    int width = 960;
    int height = 512;
    int image_size = width * height;

    float* buffer = new (nothrow) float[image_size];
    if (buffer == nullptr) {
        cerr << "内存分配失败" << endl;
        return -1;
    } else {
        cout << "内存分配成功" << endl;
    }

    fstream bin_file("heatmap.bin", ios::in | ios::binary);
    if (!bin_file) {
        cerr << "无法打开文件" << endl;
        delete[] buffer;
        return -1;
    }

    // check file size
    bin_file.seekg(0, ios::end);
    size_t file_size = bin_file.tellg();
    bin_file.seekg(0, ios::beg);

    if (file_size != image_size * sizeof(float)) {
        cerr << "文件大小不匹配，预期大小: " << image_size*sizeof(float) << "字节，实际大小: " << file_size << "字节" << endl;
        bin_file.close();
        delete[] buffer;
        return -1; 
    }

    bin_file.read(reinterpret_cast<char*>(buffer), image_size*sizeof(float));
    if (bin_file.fail()) {
        cerr << "读取文件失败" << endl;
        bin_file.close();
        delete[] buffer;
        return -1;
    }
    bin_file.close();

    for (int i=0; i<10; i++) {
        cout << buffer[i] << " ";
    }

    Mat image(height, width, CV_32FC1, buffer);
    Mat image_8bit;
    image.convertTo(image_8bit, CV_8UC1, 255);

    imshow("image", image_8bit);
    waitKey(0);

    delete[] buffer;
    return 0;
}