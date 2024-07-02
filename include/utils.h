#include <iostream>
#include <stdio.h>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <map>
#include <Eigen/Dense>
#include "open3d/Open3D.h"

using namespace std;
using namespace Eigen;


// class DataProcess {
// public:
//     string src_dir;
//     // DataProcess() {

//     // }
//     DataProcess(const char* src_dir) : src_dir((string)src_dir) {}

//     string img_dir_name = "image_2";
//     string img_dir = src_dir + img_dir_name + "/";
//     string pc_dir = src_dir + "velodyne/";
//     string image_format = "png";
//     string pc_format = "bin";
//     string calib_format = "txt";

// private:
//     void getPath(std::vector<string>& files, const char* dir_path);
//     void loadPointCloud(string pc_path, void **data, unsigned int *length);
//     void getCalibData(string calib_path);
// };

// data process
void getPath(std::vector<string>& files, const char* dir_path);
void loadPointCloud(string pc_path, void **data, unsigned int *length);
void getCalibData(string calib_path, map<string, MatrixXd>& matrices);

// point cloud
void pointCloudVisualize(float* points_data, size_t points_size, int elem_num);
void changePCtoImage(float* points_data, Eigen::MatrixXd& points_uv, size_t points_size, int elem_num, map<string, MatrixXd> matrices);