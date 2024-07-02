#include "utils.h"


void loadPointCloud(string file, void **data, unsigned int* length) {
    fstream dataFile(file, ios::in | ios::binary);
    if (!dataFile.is_open()) {
        cerr << "Can't open files: " << file << endl;
        return;
    }

    // get length of file
    unsigned int len = 0;
    dataFile.seekg(0, dataFile.end);    // 将seekg 函数有两个参数：偏移量和起始位置. 文件指针（读取位置）设置到文件末尾，从而可以获取文件的大小。在执行完这行代码后，文件指针位于文件的末尾。
    len = dataFile.tellg();     // 获取文件指针的位置，即文件大小
    dataFile.seekg(0, dataFile.beg);

    // allocate memory
    char* buffer = new(nothrow) char[len];  // 用于指示 new 操作符在分配内存失败时不要抛出异常，而是返回一个空指针（nullptr）
    if (buffer==NULL) {
        cerr << "Can't malloc buffer." << endl;
        dataFile.close();
        exit(-1);
    }

    // read data as a black
    dataFile.read(buffer, len);
    if (!dataFile) {
        cerr << "Error reading file: " << file << endl;
        delete[] buffer;
    }
    dataFile.close();

    *data = (void*)buffer;
    *length = len;
}


void printPointCloud(open3d::geometry::PointCloud &pointcloud) {
    using namespace open3d;

    bool pointcloud_has_normal = pointcloud.HasNormals();
    utility::LogInfo("Pointcloud has %d points.",
                     (int)pointcloud.points_.size());

    Eigen::Vector3d min_bound = pointcloud.GetMinBound();
    Eigen::Vector3d max_bound = pointcloud.GetMaxBound();
    utility::LogInfo(
            "Bounding box is: ({:.4f}, {:.4f}, {:.4f}) - ({:.4f}, {:.4f}, "
            "{:.4f})",
            min_bound(0), min_bound(1), min_bound(2), max_bound(0),
            max_bound(1), max_bound(2));

    for (size_t i = 0; i < pointcloud.points_.size(); i++) {
        if (pointcloud_has_normal) {
            const Eigen::Vector3d &point = pointcloud.points_[i];
            const Eigen::Vector3d &normal = pointcloud.normals_[i];
            utility::LogInfo("{:.6f} {:.6f} {:.6f} {:.6f} {:.6f} {:.6f}",
                             point(0), point(1), point(2), normal(0), normal(1),
                             normal(2));
        } else {
            const Eigen::Vector3d &point = pointcloud.points_[i];
            utility::LogInfo("{:.6f} {:.6f} {:.6f}", point(0), point(1),
                             point(2));
        }
    }
    utility::LogInfo("End of the list.");
}


void pointCloudVisualize(float* points_data, size_t points_size, int elem_num) {
    auto point_cloud = make_shared<open3d::geometry::PointCloud>();
    point_cloud->points_.resize(points_size);
    // Eigen::MatrixXd points(points_size, 3);
    for (int i=0; i<points_size; i++) {
        Eigen::Vector3d point;
        point[0] = points_data[i * elem_num];
        point[1] = points_data[i * elem_num + 1];
        point[2] = points_data[i * elem_num + 2];
        point_cloud->points_[i] = point;
    }
    // printPointCloud(point_cloud);
    
    // 创建窗口并添加点云
    auto vis = make_shared<open3d::visualization::Visualizer>();
    vis->CreateVisualizerWindow("Open3D Point Cloud Visualization", 1280*2, 720*2);
    vis->AddGeometry(point_cloud);

    auto axes = std::make_shared<open3d::geometry::LineSet>();
    axes->points_ = {
        {0.0, 0.0, 0.0}, // Origin
        {1.0, 0.0, 0.0}, // X axis
        {0.0, 1.0, 0.0}, // Y axis
        {0.0, 0.0, 1.0}  // Z axis
    };
    axes->lines_ = {
        Eigen::Vector2i(0, 1), // X axis
        Eigen::Vector2i(0, 2), // Y axis
        Eigen::Vector2i(0, 3)  // Z axis
    };
    axes->colors_ = {
        {1.0, 0.0, 0.0}, // Red for X axis
        {0.0, 1.0, 0.0}, // Green for Y axis
        {0.0, 0.0, 1.0}  // Blue for Z axis
    };

    // Add the LineSet to the visualizer
    vis->AddGeometry(axes);

    vis->GetRenderOption().point_size_ = 1.0;
    vis->GetRenderOption().background_color_ = Eigen::Vector3d(0, 0, 0);

    vis->Run();
    vis->DestroyVisualizerWindow();

    // open3d::visualization::DrawGeometries({point_cloud}, "PointCloud", 1600, 900);
}


void changePCtoImage(float* points_data, Eigen::MatrixXd& points_uv, size_t points_size, int elem_num, map<string, MatrixXd> matrices) {
    Eigen::MatrixXd point_cloud(points_size, 3);
    for (int i=0; i<points_size; i++) {
        point_cloud(i, 0) = points_data[i*elem_num];
        point_cloud(i, 1) = points_data[i*elem_num+1];
        point_cloud(i, 2) = points_data[i*elem_num+2];
    }

    Eigen::MatrixXd pc_expanded(points_size, 4);
    pc_expanded.block(0, 0, points_size, 3) = point_cloud;
    pc_expanded.col(3).setOnes();

    Eigen::MatrixXd homography = matrices["R0_rect"] * matrices["Tr_velo_to_cam"];
    Eigen::MatrixXd homography_exp = Eigen::MatrixXd::Zero(4, 4);
    homography_exp.block(0, 0, 3, 4) = homography;
    homography_exp(3, 3) = 1;
    Eigen::MatrixXd H = matrices["P2"] * homography;

    Eigen::MatrixXd transform_points = (H * pc_expanded.transpose()).transpose();
    transform_points.array().colwise() /= transform_points.col(2).array(); 

    points_uv = transform_points.block(0, 0, points_size, 2);
}