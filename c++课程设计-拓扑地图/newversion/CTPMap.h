#ifndef CTPMAP_H
#define CTPMAP_H
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
using namespace std;

typedef pair<int, int> PII;
struct MyEdge;

struct MyVertex {
    int index;                // 顶点索引号
    string name;              // 顶点名称
    double x, y;              // 顶点坐标
    MyVertex* nextVertex;     // 指向下一个顶点
    struct MyEdge* firstEdge; // 指向与该顶点相连接的第一条边

    MyVertex(int i, const string& n, double posX, double posY);
};

struct MyEdge {
    double weight;    // 边的权值
    MyVertex* vertex; // 指向连接的顶点
    MyEdge* nextEdge; // 指向下一条边

    MyEdge(double w, MyVertex* v);
};

class CTpMap {
private:
    MyVertex* firstVertex;                 // 指向第一个顶点
    unordered_map<int, MyVertex*> idToVer; // 根据顶点编号快速找到顶点指针
    void AddDirectedEdge(int fromIndex, int toIndex, double weight);
    void RemoveDirectedEdge(int fromIndex, int toIndex);

public:
    CTpMap();
    ~CTpMap();

    void AddVertex(int index, const string& name, double x, double y);
    void AddEdge(int uIndex, int vIndex, double weight);
    void RemoveEdge(int uIndex, int vIndex);
    void RemoveVertex(int index);
    void RemoveAll();
    int FindShortestPath(int fromIndex, int toIndex, vector<int>& path);
};

#endif
