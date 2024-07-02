#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>

#include "utils.h"

using namespace cv;


int main(int argc, char** argv){
    const char* src_dir = nullptr;
    int opt = 0;
    while((opt = getopt(argc, argv, "hp:")) != 1) {
        switch(opt) {
            case 'h':
                cout << "Usage: ./pc_demo -p <img_path or images folder>" << endl;
            case 'p':
                src_dir = optarg;
        }
        break;
    }

    string path_str = (string)src_dir;
    string img_dir_name = "image_2";
    string img_dir = path_str + img_dir_name + "/";
    string pc_dir = path_str + "velodyne/";
    string image_format = "png";
    string pc_format = "bin";
    string calib_format = "txt";

    vector<string> img_names;
    getPath(img_names, (char*)img_dir.c_str());
    cout << "files size: " << img_names.size() << endl;
    for (auto img_name : img_names) {
        if (img_name != "000031.png")
            continue;
        string img_path = img_dir + img_name;
        size_t pos = img_path.find(img_dir_name);
        string pc_dir_name = "velodyne";
        string calib_dir_name = "calib";
        string pc_path = img_path;
        pc_path.replace(pos, img_dir_name.length(), pc_dir_name);
        size_t pos_format_pc = pc_path.find(image_format);
        pc_path.replace(pos_format_pc, image_format.length(), pc_format);
        string calib_path = img_path;
        calib_path.replace(pos, img_dir_name.length(), calib_dir_name);
        size_t pos_format_calib = calib_path.find(image_format);
        calib_path.replace(pos_format_calib, image_format.length(), calib_format);

        cout << "image path: " << img_path << endl;
        cout << "point cloud path: " << pc_path << endl;
        cout << "calib path: " << calib_path << endl;

        // image process
        Mat img = imread(img_path);
        int width = img.rows;
        int height = img.cols;
        int channel = img.channels();
        // imshow("image", img);
        // waitKey(0);

        // point cloud process
        unsigned int length = 0;
        void* data = nullptr;
        shared_ptr<char> buffer((char*)data, default_delete<char[]>());
        loadPointCloud(pc_path.c_str(), &data, &length);
        buffer.reset((char*)data);
        float* points = (float*)buffer.get();
        size_t points_size = length/sizeof(float)/4;
        cout << "points size: " << points_size << endl;
        for (size_t j=0; j<5; ++j) {
            float x = points[j*4 + 0];
            float y = points[j*4 + 1];
            float z = points[j*4 + 2];
            float intensity = points[j*4 + 3];
            cout << "Point" << j << ": " << x << " " << y << " " << z << " " << intensity << endl;
        }

        // calibration process
        map<string, MatrixXd> matrices;
        getCalibData(calib_path, matrices);
        cout << "P2:" << endl;
        cout << matrices["P2"] << endl;
        cout << "P3" << endl;
        cout << matrices["P3"] << endl;
        cout << "R0_rect: " << endl;
        cout << matrices["R0_rect"] << endl;
        cout << "Tr_velo_to_cam: " << endl;
        cout << matrices["Tr_velo_to_cam"] << endl;
        cout << "Tr_imu_to_velo:" << endl;
        cout << matrices["Tr_imu_to_velo"] << endl;

        // point cloud visualize
        // pointCloudVisualize(points, points_size, 4);

        // point cloud on image
        Eigen::MatrixXd points_uv(points_size, 2);
        changePCtoImage(points, points_uv, points_size, 4, matrices);
        Scalar color(255, 0, 0);
        for (int i=0; i<points_size; i++) {
            Point center(points_uv(i, 0), points_uv(i, 1));
            circle(img, center, 1, color, -1);
        }
        imshow("img", img);
        waitKey(0);

        // break;
    }
    
    return 0;
}