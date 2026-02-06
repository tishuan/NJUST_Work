#ifndef CPARAM_H
#define CPARAM_H

#include <string>
#include <map>
using namespace std;

class CParam {
private:
    map<string, map<string, int>> data; // 存储参数数据的私有成员对象
    string filepath; // 存储文件路径的私有成员对象
    static int count; // 静态成员变量，用于统计读写配置文件的次数

public:
    CParam(const string& path); // 带参构造函数，接收文件路径作为参数
    void ReadFile(); // 读取txt配置文件的方法
    void WriteFile(); // 写入txt配置文件的方法
    CParam& operator+=(const CParam& other); // 重载操作运算符，合并两个CParam对象的数据
    bool operator==(CParam& other); // 重载相等运算符，判断两个CParam是否相同
    void SetFilepath(string _filepath) { filepath = _filepath; }
    static int GetFileIOCount(); // 获取读写配置文件的次数的静态方法
};

#endif
