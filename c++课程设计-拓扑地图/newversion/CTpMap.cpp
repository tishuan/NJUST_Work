#include "CTpMap.h"
#include <limits>
#include <iostream>

MyVertex::MyVertex(int i, const string& n, double posX, double posY)
    : index(i), name(n), x(posX), y(posY), nextVertex(NULL), firstEdge(NULL) {}

MyEdge::MyEdge(double w, MyVertex* v)
    : weight(w), vertex(v), nextEdge(NULL) {}

CTpMap::CTpMap()
    : firstVertex(NULL) {}

CTpMap::~CTpMap() {
    RemoveAll();
}

void CTpMap::AddVertex(int index, const string& name, double x, double y) {
    // 创建新顶点
    MyVertex* newVertex = new MyVertex(index, name, x, y);
    newVertex->nextVertex = firstVertex;//与上一个顶点连接
    firstVertex = newVertex;//first变成当前顶点
    idToVer[index] = newVertex;
}

void CTpMap::AddDirectedEdge(int fromIndex, int toIndex, double weight) {
    // 查找起点和终点
    MyVertex* fromVertex = firstVertex;
    MyVertex* toVertex = firstVertex;

    while (fromVertex != NULL && fromVertex->index != fromIndex)
        fromVertex = fromVertex->nextVertex;

    while (toVertex != NULL && toVertex->index != toIndex)
        toVertex = toVertex->nextVertex;

    if (fromVertex == NULL || toVertex == NULL)
        return;

    // 创建新边
    MyEdge* newEdge = new MyEdge(weight, toVertex);
    newEdge->nextEdge = fromVertex->firstEdge;
    fromVertex->firstEdge = newEdge;
}

void CTpMap::RemoveDirectedEdge(int fromIndex, int toIndex) {
    // 查找起点
    MyVertex* fromVertex = firstVertex;
    while (fromVertex != NULL && fromVertex->index != fromIndex)
        fromVertex = fromVertex->nextVertex;

    if (fromVertex == NULL)
        return;

    // 查找要删除的边
    MyEdge* prevEdge = NULL;
    MyEdge* currentEdge = fromVertex->firstEdge;

    while (currentEdge != NULL && currentEdge->vertex->index != toIndex) {
        prevEdge = currentEdge;
        currentEdge = currentEdge->nextEdge;
    }

    if (currentEdge == NULL)
        return;

    // 删除边
    if (prevEdge == NULL)
        fromVertex->firstEdge = currentEdge->nextEdge;
    else
        prevEdge->nextEdge = currentEdge->nextEdge;

    delete currentEdge;
}

void CTpMap::AddEdge(int uIndex, int vIndex, double weight)
{
    AddDirectedEdge(uIndex, vIndex, weight);
    AddDirectedEdge(vIndex, uIndex, weight);
}

void CTpMap::RemoveEdge(int uIndex, int vIndex)
{
    RemoveDirectedEdge(uIndex, vIndex);
    RemoveDirectedEdge(vIndex, uIndex);
}

void CTpMap::RemoveVertex(int index) {
    idToVer.erase(index);
    // 查找要删除的顶点
    MyVertex* prevVertex = NULL;
    MyVertex* currentVertex = firstVertex;

    while (currentVertex != NULL && currentVertex->index != index) {
        prevVertex = currentVertex;
        currentVertex = currentVertex->nextVertex;
    }

    if (currentVertex == NULL)
        return;

    // 删除与顶点相关的边
    MyEdge* currentEdge = currentVertex->firstEdge;
    while (currentEdge != NULL) {
        MyEdge* nextEdge = currentEdge->nextEdge;
        int toIndex = currentEdge->vertex->index;
        RemoveEdge(index, toIndex);
        currentEdge = nextEdge;
    }

    // 删除顶点
    if (prevVertex == NULL)
        firstVertex = currentVertex->nextVertex;
    else
        prevVertex->nextVertex = currentVertex->nextVertex;

    delete currentVertex;
}

void CTpMap::RemoveAll() {
    while (firstVertex != NULL) {
        MyVertex* nextVertex = firstVertex->nextVertex;
        MyEdge* currentEdge = firstVertex->firstEdge;
        delete firstVertex;
        while (currentEdge != NULL) {
            MyEdge* nextEdge = currentEdge->nextEdge;
            delete currentEdge;
            currentEdge = nextEdge;
        }
        firstVertex = nextVertex;
    }
}

int CTpMap::FindShortestPath(int fromIndex, int toIndex, vector<int>& path) {
    // 堆优化的Dijkstra
    // 顶点序号未必连续，故用unordered_map而非数组
    unordered_map<int, int> dis;
    unordered_map<int, bool> vis;
    unordered_map<int, int> pre;
    MyVertex* pv = firstVertex;
    // 初始化
    while (pv != NULL) {
        dis[pv->index] = 0x3f3f3f3f;
        vis[pv->index] = false;
        pre[pv->index] = pv->index;
        pv = pv->nextVertex;
    }
    priority_queue<PII, vector<PII>, greater<PII>> q;
    dis[fromIndex] = 0;
    q.push({ 0, fromIndex });
    bool suc = false;
    while (!q.empty()) {
        PII cur = q.top();
        q.pop();
        int vId = cur.second;
        if (vId == toIndex) {
            suc = true;
            break;
        }
        if (vis[vId])continue;
        vis[vId] = true;
        MyVertex* pv = idToVer[vId];
        for (MyEdge* pe = pv->firstEdge; pe != NULL; pe = pe->nextEdge) {
            int toId = pe->vertex->index;
            if (vis[toId])continue;
            if (dis[toId] > dis[vId] + pe->weight) {
                pre[toId] = vId;
                dis[toId] = dis[vId] + pe->weight;
                q.push({ dis[toId], toId });
            }
        }
    }
    if (!suc)return -1;
    // 构造路径
    path.clear();
    int curId = toIndex;
    do {
        path.push_back(curId);
        curId = pre[curId];
    } while (curId != fromIndex);
    path.push_back(curId);
    reverse(path.begin(), path.end());
    return dis[toIndex];
}


int main() {
    CTpMap tpMap;

    // 添加顶点
    tpMap.AddVertex(1, "A", 0.0, 0.0);
    tpMap.AddVertex(2, "B", 1.0, 1.0);
    tpMap.AddVertex(3, "C", 2.0, 2.0);
    tpMap.AddVertex(4, "D", 3.0, 3.0);

    // 添加边
    tpMap.AddEdge(1, 2, 1.0);
    tpMap.AddEdge(2, 3, 1.0);
    tpMap.AddEdge(3, 4, 1.0);
    tpMap.AddEdge(1, 4, 4.0);

    // 寻找最短路径（可达情形1）
    vector<int> path;
    int dis = tpMap.FindShortestPath(1, 4, path);
    if (dis != -1) {
        cout << "最短距离：" << dis << endl;
        cout << "最短路径：";
        for (size_t i = 0; i < path.size(); ++i) {
            if (i)cout << " -> ";
            cout << path[i];
        }
        cout << endl << endl;
    }
    else {
        cout << "二者不可达" << endl << endl;
    }

    // 删除边
    tpMap.RemoveEdge(2, 3);
    // 寻找最短路径（可达情形2）
    dis = tpMap.FindShortestPath(1, 4, path);
    if (dis != -1) {
        cout << "最短距离：" << dis << endl;
        cout << "最短路径：";
        for (size_t i = 0; i < path.size(); ++i) {
            if (i)cout << " -> ";
            cout << path[i];
        }
        cout << endl << endl;
    }
    else {
        cout << "二者不可达" << endl << endl;
    }

    // 删除边
    tpMap.RemoveEdge(1, 4);
    // 寻找最短路径（不可达情形）
    dis = tpMap.FindShortestPath(1, 4, path);
    if (dis != -1) {
        cout << "最短距离：" << dis << endl;
        cout << "最短路径：";
        for (size_t i = 0; i < path.size(); ++i) {
            if (i)cout << " -> ";
            cout << path[i];
        }
        cout << endl << endl;
    }
    else {
        cout << "二者不可达" << endl << endl;
    }

    return 0;
}
