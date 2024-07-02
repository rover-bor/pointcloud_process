#include "utils.h"


void gradientPointCloudColorMap(Eigen::MatrixXd points, Eigen::MatrixXd& colors) {
    colors.setZero();
    // Eigen::ArrayXf dist = (points.array().col(0).square() + points.array().col(1).square()).sqrt();
    // // 调整渐变半径
    // dist /= 51.2;
    // dist *= 127.+255.*3;

    // 减R (127->0)
    // int clr_1 = 127;
    // Eigen::ArrayXf dy_r = 127 - dist;
    // Eigen::ArrayXXf tmp((dist<clr_1).cast<int>().sum(), 3);
    // tmp.setZero();
    // tmp.col(0).select(dist<clr_1, dy_r);
    // tmp.col(1) = 0;
    // tmp.col(2) = 255;
    // colors.array().select(dist<clr_1, tmp);

    // 加G (0->255)
    // int clr_2 = 127+255;
    // Eigen::ArrayXf dy_g = dist - clr_1;
    // Eigen::ArrayXXf tmp((dist<clr_1).cast<int>().sum(), 3);
    // tmp.setZero();
    // tmp.col(0).select(dist<clr_1, dy_r);
    // tmp.col(1) = 0;
    // tmp.col(2) = 255;
    // colors.array().select(dist<clr_1, tmp);
}