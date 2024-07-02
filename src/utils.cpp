#include "utils.h"


int strcmp(char* str1, const char* str2) {
    int ret = 0;
    while (!(ret=*(unsigned char*)str1 - *(unsigned char*)str2) && *str1) {
        str1++;
        str2++;
    }
    if (ret < 0)
        return -1;
    else if (ret > 0)
        return 1;
    return 0;
}


void getPath(std::vector<string>& files, const char* path) {
    struct stat s;
    if (stat(path, &s) == 0) {
        if (s.st_mode & S_IFDIR) {
            std::cout << path << " is a directory" << std::endl;
            // 提取文件夹内的路径
            DIR *dir;
            struct dirent *ptr;

            if ((dir=opendir(path))==NULL) {
                std::cerr << "[Error] " << path << " can not open!" << std::endl;
                exit(0);
            }

            while((ptr=readdir(dir))!=NULL) {
                if (strcmp(ptr->d_name, ".")==0 || strcmp(ptr->d_name, "..") < 0)
                    continue;
                else if (ptr->d_type == 8) {
                    files.push_back(ptr->d_name);
                }  // file
                // else if (ptr->d_type == 4)  // dir
            }
            closedir(dir);
        }else if (s.st_mode & S_IFREG) {
            std::cout << path << " is a file" << std::endl;
            files.push_back(path);
        }else {
            std::cerr << "not file not directory" << std::endl;
        }
    }else {
        std::cerr << "[Error] " << path << " doesn't exist!" << std::endl;
    }
}

void getCalibData(string calib_path, map<string, MatrixXd>& matrices) {
    ifstream file(calib_path);

    if (!file.is_open()) {
        cerr << "Can't open: " << calib_path << endl; 
        return;
    }

    string line;
    
    while (getline(file, line)) {
        // 用string流解析每一行内容
        istringstream iss(line);
        string matrix_name;
        string tmp;
        string token;
        iss >> tmp;     // 默认以空格进行split，以队列的方式将第一个元素放到tmp上
        if (tmp.length()==0)
            continue;
        matrix_name = tmp.substr(0, tmp.length()-1);    // 去掉冒号

        vector<double> values;
        double value;
        while (iss >> value) {
            values.push_back(value);
        }
        
        size_t num_values = values.size();
        int rows, cols;
        if (matrix_name == "R0_rect") {
            rows = 3, cols = 3;
        } else {
            rows = 3, cols = 4;
        }

        if (num_values != rows*cols) {
            cerr << "Unexpected number of values for matrix " << matrix_name << endl; 
        }

        MatrixXd matrix(rows, cols);
        for (size_t i=0; i<rows; ++i) {
            for (size_t j=0; j<cols; ++j) {
                matrix(i, j) = values[i*cols + j];
            }
        }
        matrices[matrix_name] = matrix;
    }

    file.close();
}