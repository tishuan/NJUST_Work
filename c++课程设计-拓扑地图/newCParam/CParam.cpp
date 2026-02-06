#include "CParam.h"
#include <iostream>
#include <fstream>

int CParam::count = 0; // 初始化静态计数器

//构造函数名与类名同名，：初始化类对象
CParam::CParam(const string& path) : filepath(path)
{
}

void CParam::ReadFile() {
    ifstream file(filepath); // 打开文件流
    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return;
    }
    string line;
    string currentParam;
    while (getline(file, line)) {
        if (!line.empty() && line[0] != '/') {
            if (line.find(' ') == string::npos) {
                currentParam = line; // 保存当前参数名
                data[currentParam] = map<string, int>();
            }
            else {
                size_t pos = line.find('=');
                if (pos != string::npos) {
                    string key = line.substr(0, pos);
                    int value = stoi(line.substr(pos + 1));
                    data[currentParam][key] = value; // 存储参数和值
                }
            }
        }
    }
    file.close(); // 关闭文件流
    count++; // 增加读写计数
}

void CParam::WriteFile() {
    ofstream file(filepath); // 打开文件流，会清空原文件内容
    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return;
    }

    for (const auto& param : data) {
        file << param.first << "\n"; // 写入参数名
        for (const auto& entry : param.second) {
            file << entry.first << " = " << entry.second << "\n"; // 写入参数和值
        }
    }
    file.close(); // 关闭文件流
    count++; // 增加读写计数
}

// 重载操作运算符，合并两个CParam对象的数据
CParam& CParam::operator+=(const CParam& other) {
    for (const auto& param : other.data) {
        //if (!data.count(param.first))data.insert(pair<string, map<string, int>>(param.first, map<string, int>()));
        for (const auto& entry : param.second) {
            data[param.first][entry.first] = entry.second;
        }
    }
    return *this;
}

// 模板函数，判断两个键值类型均为内置类型的map是否相等
template<typename K, typename V>
bool MapEqual(map<K, V>& mp1, map<K, V>& mp2)
{
    if (mp1.size() != mp2.size())return false;
    for (auto kv1 : mp1) {
        if (!mp2.count(kv1.first))return false;
        if (kv1.second != mp2[kv1.first])return false;
    }
    return true;
}
// 重载相等运算符，判断两个CParam是否相同
bool CParam::operator==(CParam& other) {
    if (data.size() != other.data.size())return false;
    for (auto kv : data) {
        if (!other.data.count(kv.first))return false;
        if (!MapEqual(data[kv.first], other.data[kv.first]))return false;
    }
    return true;
}

int CParam::GetFileIOCount() {
    return count;
}

int main() {
    CParam param1("E:/HomeWork/ComputerOrders/flyparam.txt"); // 创建CParam对象并设置文件路径
    param1.ReadFile(); // 读取配置文件

    CParam param2("E:/HomeWork/ComputerOrders/flyparam.txt"); // 创建另一个CParam对象并设置文件路径
    param2.ReadFile(); // 读取配置文件

    if (param1 == param2)cout << "二者相等" << endl;
    else cout << "二者不等" << endl;

    return 0;
}