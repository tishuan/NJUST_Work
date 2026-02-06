#include "AI.h"
#include "config.h"
#include "MainWidget.h"
#include <Qdebug>
#include <Windows.h>
#include <iostream>

AI::AI(QObject *parent)
    : QObject(parent)
{
}

void AI::processData()
{
    // 与主线程通讯
    ProcessDataWork = 1;
    static int Gamemap[72][72]={0};

    if(AIGame.GameFrame==0)
    {
        for(int i=0;i<72;i++){
            for(int j=0;j<72;j++){
                Gamemap[i][j]=-1;//未探明
            }
        }
    }

    for(int i=0;i<AIGame.human_n;i++)
    {
        for(int x=-2;x<3;x++)
        {
            for(int y=-2;y<3;y++)
            {
                if((AIGame.human[i].BlockL+x)<72&&(AIGame.human[i].BlockU+y)<72){
                Gamemap[AIGame.human[i].BlockL+x][AIGame.human[i].BlockU+y]=0;//已探明
                    }
            }
        }
        //Gamemap[AIGame.human[i].BlockL+3][AIGame.human[i].BlockU+3]=-1;
    }



    for(int i=0;i<AIGame.resource_n;i++)
    {
        Gamemap[AIGame.resource[i].BlockL][AIGame.resource[i].BlockU]=1;//资源
        if(AIGame.resource[i].Type==RESOURCE_ELEPHANT){
            for(int x=0;x<3;x++){
                for(int y=0;y<3;y++){
                    Gamemap[AIGame.resource[i].BlockL+x][AIGame.resource[i].BlockU+y]=1;//资源
                }
            }
        }
    }
    for(int i=0;i<AIGame.building_n;i++)
    {
        for(int x=0;x<=3;x++){
            for(int y=0;y<=3;y++){
                Gamemap[AIGame.building[i].BlockL+x][AIGame.building[i].BlockU+y]=2;//建筑
            }
        }

    }
    for(int i=1;i<AIGame.human_n;i++){
        if(i!=9){//不算探路的人的位置
            Gamemap[AIGame.human[i].BlockL][AIGame.human[i].BlockU]=4;//人物位置
        }

    }
    for(int i=0;i<AIGame.building_n;i++){
        if(AIGame.building[i].Type==BUILDING_ARROWTOWERPOSITION){
            Gamemap[AIGame.building[i].BlockL][AIGame.building[i].BlockU]=3;//箭塔
        }
    }

    QFile file("array.txt"); // 打开或创建名为"array.txt"的文件
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        for (int i = 0; i < 72; ++i)
        {
            for (int j = 0; j < 72; ++j)
            {
                stream << Gamemap[i][j] << " "; // 将二维数组元素写入文件，以空格分隔
            }
            stream << endl; // 换行
        }
        file.close();
    }
    else
    {
        qDebug() << "无法打开文件";
    }

    // 使用作弊函数获取资源
    if(AIGame.GameFrame == 0) {
//        emit cheatRes();
//        emit cheatRes();
//        emit cheatRes();
        // 也可以再初始化时更改农民上限或进入下一时代
//        emit cheatFarmerLimit();
//        emit cheatAge();
        ProcessDataWork = 0;
        return ;
    } //这些代码仅用于方便调试，模拟实际运行环境时应删除

    // dis用于存储对象和对象之间的距离，targetResSN用于存储目的资源的全局编号
    double dis = 1e6;
    int targetResSN = 0;

    // 升级时代完成后，AI停止工作
//    if(AIGame.civilizationStage == CIVILIZATION_TOOLAGE) {
//        ProcessDataWork = 0;
//        return ;
//    }

    //static bool tryBuild = false; // 令某些建筑指令只发一次，不重复
    static bool stocktry=false;
    static bool granarytry=false;
    static bool GranaryJudge = false; // 判断谷仓是否建好
    static bool StockJudge = false; // 判断仓库是否建好

    // 如果同时拥有粮仓和仓库，且食物足够、市镇中心空闲时，尝试升级到下一个时代
    if(AIGame.civilizationStage == CIVILIZATION_STONEAGE && GranaryJudge == true && StockJudge == true && AIGame.building[0].Project == BUILDING_FREE && AIGame.Meat >= BUILDING_CENTER_UPGRADE_FOOD)
        BuildingAction(AIGame.building[0].SN, BUILDING_CENTER_UPGRADE);

    // 如果食物足够、市镇中心空闲且农民数量小于5时，尝试生产一个农民，且优先级高于升级时代
    if(AIGame.building[0].Project == BUILDING_FREE && AIGame.Meat >= BUILDING_CENTER_CREATEFARMER_FOOD && AIGame.human_n <16)
        BuildingAction(AIGame.building[0].SN, BUILDING_CENTER_CREATEFARMER);

    for(int i = 0; i < AIGame.building_n; i++) {
        // 判断是否存在粮仓和仓库，此为升级时代的前置条件
        if(AIGame.building[i].Type == BUILDING_GRANARY && GranaryJudge == false) {
            emit AIDebugText("存在粮仓");
            GranaryJudge = true;
        }
        else if(AIGame.building[i].Type == BUILDING_STOCK && StockJudge == false) {
            emit AIDebugText("存在仓库");
            StockJudge = true;
        }
    }

    //未升级时代
    //找市场空地的数组
    static int pl[16]={0};
    static int pu[16]={0};
    //判断资源是否还有
    static int gazellejudge=0;
    for(int i=0;i<AIGame.resource_n;i++){
        gazellejudge=0;
        if(AIGame.resource[i].Type==RESOURCE_GAZELLE){
            gazellejudge=1;
            break;
        }
    }
    static int bushjudge=0;
    for(int i=0;i<AIGame.resource_n;i++){
        bushjudge=0;
        if(AIGame.resource[i].Type==RESOURCE_BUSH){
            bushjudge=1;
            break;
        }
    }
    //static
if(AIGame.civilizationStage==CIVILIZATION_STONEAGE){

    // 第一个人绕市镇中心一圈探路
    double L = AIGame.human[0].L;
    double U = AIGame.human[0].U;
    const static int stepn = 22;
    // 相对坐标
    static int dL[stepn] = {0, -3, 0, 12, 0, -17, 0,23,  0,-28, 0,34,  0,-41, 0,47,  0,-53, 0,62,  0,-72};
    static int dU[stepn] = {-1, 0, 11, 0, -15, 0,22, 0,-27,  0,33, 0,-40,  0,46, 0,-52,  0,60, 0,-68,  0};


    static double ListL[stepn];
    static double ListU[stepn];
    static int stepi;
    if(AIGame.GameFrame == 1)
    {
        double L1=L,U1=U;
        for(int i=0;i<stepn;i++){
            L1 += dL[i] * BLOCKSIDELENGTH;
            ListL[i] = L1;
            U1 += dU[i] * BLOCKSIDELENGTH;
            ListU[i] = U1;
        }
        stepi = 0;
        HumanMove(AIGame.human[0].SN, ListL[stepi], ListU[stepi]);
    }
    double fL = fabs(AIGame.human[0].L - ListL[stepi]);
    double fU = fabs(AIGame.human[0].U - ListU[stepi]);

    static int rl=0;
    static int ru=0;
    double fl1=fabs(AIGame.human[0].L-rl*BLOCKSIDELENGTH);
    double fu1=fabs(AIGame.human[0].U-ru*BLOCKSIDELENGTH);
    static int tep=0;
    if(AIGame.human[0].NowState==HUMAN_STATE_STOP)
    {
        tep=1;
    }
    if(tep==1&&AIGame.human[0].Blood>0){
        if((fl1<1&&fu1<1)||AIGame.human[0].NowState==HUMAN_STATE_STOP){
            rl=rand()%72;
            ru=rand()%72;
           // qDebug()<<rl<<"随机数"<<ru;
            HumanMove(AIGame.human[0].SN,rl*BLOCKSIDELENGTH,ru*BLOCKSIDELENGTH);
        }
    }
    static int beingattack=0;
    if(AIGame.human[0].NowState==HUMAN_STATE_GOTO_RESOURCE){
        beingattack=1;
        //qDebug()<<"第一个人受到攻击";
    }
    if(AIGame.human[0].NowState==HUMAN_STATE_IDLE&&beingattack==1){
        dis = 1e6;
        targetResSN = 0;
        for(int i = 0; i < AIGame.resource_n; i++) {
            if(AIGame.resource[i].Type == RESOURCE_GAZELLE) {
                double tempDistance = calDistance(AIGame.human[0].L, AIGame.human[0].U, AIGame.resource[i].L, AIGame.resource[i].U);
                if(dis > tempDistance) {
                    dis = tempDistance;
                    targetResSN = AIGame.resource[i].SN;
                }
            }
        }
        if(targetResSN != 0) {
            HumanAction(AIGame.human[0].SN, targetResSN);
            qDebug()<<"第1个村民打羚羊";
        }
    }


    if(AIGame.GameFrame > 1 && (fL < 1.1  && fU < 1.1) && stepi<stepn - 1&&tep!=1) {
        stepi++;
        HumanMove(AIGame.human[0].SN, ListL[stepi], ListU[stepi]);
    }

    // 第二个人尝试去(40, 31)的位置建立一个仓库
    static int StockFrame = 0; // 记录建造仓库时的帧数
    static int BuildingStockX = 40, BuildingStockY = 31; // 记录准备建造仓库的位置
    if(AIGame.Wood > BUILD_STOCK_WOOD && !stocktry)
    {
        HumanBuild(AIGame.human[1].SN, BUILDING_STOCK, BuildingStockX, BuildingStockY);
        StockFrame = AIGame.GameFrame + 5;
        stocktry=true;
    }
    // 如果5帧以后，人还没有进入到工作状态
    if(StockFrame == AIGame.GameFrame && (AIGame.human[1].NowState == HUMAN_STATE_IDLE || AIGame.human[1].NowState == HUMAN_STATE_STOP))
    {
        BuildingStockX ++;
        BuildingStockY ++;

        // 在坐标不超过地图的情况下，继续尝试在其他位置建造
        if(BuildingStockX >= 72 || BuildingStockY >= 72){
            StockFrame = 0;
            stocktry=false;
        }
        else {
            emit AIDebugText("AI获取到建造失败，尝试在(" + QString::number(BuildingStockX) + "," + QString::number(BuildingStockY) + ")处再次建立仓库");
            StockFrame = AIGame.GameFrame + 5;
            HumanBuild(AIGame.human[1].SN, BUILDING_STOCK, BuildingStockX, BuildingStockY);
            stocktry=true;
        }
    }

    //找建房子的空地
    static bool flag1=true;
    static int positionl[6]={0};
    static int positionu[6]={0};
    static int haction=0;
    static int bl=AIGame.human[0].BlockL;
    static int bu=AIGame.human[0].BlockU;
    if(AIGame.GameFrame%100==0&&haction<6){
        bl=AIGame.human[0].BlockL;
        bu=AIGame.human[0].BlockU;
        for(int i=-1;i<3;i++){
            for(int j=-1;j<3;j++){
                if(Gamemap[bl+i][bu+j]!=0){
                    flag1=false;
                }
            }
        }
        if(flag1==true){
            positionl[haction]=AIGame.human[0].BlockL;
            positionu[haction]=AIGame.human[0].BlockU;
            haction++;
            qDebug()<<positionl[haction-1]<<"find it"<<positionu[haction-1];
        }
        if(flag1==false){
            flag1=true;
        }
    }
    //建完仓库建谷仓

if(StockJudge==true&&AIGame.GameFrame>=200){
    if(AIGame.Wood>=120&&AIGame.human[2].NowState==HUMAN_STATE_IDLE&&GranaryJudge==false){
        int t=HumanBuild(AIGame.human[2].SN,BUILDING_GRANARY,positionl[0],positionu[0]);
        if(t!=0){
            HumanBuild(AIGame.human[2].SN,BUILDING_GRANARY,positionl[5],positionu[5]);
        }
        qDebug()<<"谷仓位置"<<AIGame.building[2].BlockL<<" "<<AIGame.building[2].BlockU;
    }
}
    // 第2个村民建完建筑后，让他去砍最近的树
    static int Human2Action = 0;
    if(AIGame.building[1].Percent == 100 && Human2Action == 0) {
        qDebug()<<AIGame.Wood;
        // 计算出距离第2个村民最近的树的全局编号SN
        dis = 1e6;
        targetResSN = 0;
        for(int i = 0; i < AIGame.resource_n; i++) {
            if(AIGame.resource[i].Type == RESOURCE_TREE) {
                double tempDistance = calDistance(AIGame.human[1].L, AIGame.human[1].U, AIGame.resource[i].L, AIGame.resource[i].U);
                if(dis > tempDistance) {
                    dis = tempDistance;
                    targetResSN = AIGame.resource[i].SN;
                }
            }
        }
        if(targetResSN != 0) {
            int t = HumanAction(AIGame.human[1].SN, targetResSN);
            qDebug()<<"2正在砍树中"<<AIGame.human[1].Resource<<AIGame.Wood;
            if(t==0)
            {
                targetResSN=0;
                AIGame.Wood+=75;
                qDebug()<<"2新增一棵树"<<AIGame.Wood;
                        Human2Action = 1;
            }
        }

    }
    if(Human2Action == 1)
    {
        for(int i=0;i<AIGame.resource_n;i++)
        {

            if(AIGame.resource[i].Type==RESOURCE_TREE)
            {
                int t =HumanAction(AIGame.human[1].SN,AIGame.resource[i].SN);
                qDebug()<<"第2正在砍树中";
                if(t==0 &&AIGame.resource[i].SN!=0) //执行成功而且该资源还存在
                {
                    AIGame.resource[i].SN=0;    //置空
                    AIGame.resource[i].Cnt-=75;
                    AIGame.human[1].Resource+=75;
                    AIGame.Wood+=75;
                    qDebug()<<"新增一棵树"<<AIGame.Wood;
                }
            }
        }
        Human2Action=2;
        //qDebug()<<AIGame.human[1].NowState<<"状态1";
        //AIGame.human[1].NowState=0;

    }

    static bool flagt=true;
    static int q=1;
    if(StockJudge==true&&GranaryJudge==true&&Human2Action==2&&AIGame.human[1].NowState==HUMAN_STATE_IDLE)
    {
        if(AIGame.Wood>=30&&haction>0){
            qDebug()<<"准备建第一个";
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    if(Gamemap[positionl[q]+i][positionu[q]+j]!=0 ){
                        flagt=false;
                    }
                }
            }
            if(flagt==true){
                int t=HumanBuild(AIGame.human[1].SN,BUILDING_HOME,positionl[q],positionu[q]);
                if(t==0){
                    qDebug()<<"建完第一个房子"<<positionl[q]<<positionu[q];
                    q++;
                    Human2Action=3;
                }

            }
            if(flagt==false){
                q++;
                flagt=true;
            }
        }
    }
    if(AIGame.building[3].Percent==100&&Human2Action==3){
        if(AIGame.Wood>=30){
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    if(Gamemap[positionl[q]+i][positionu[q]+j]!=0 ){
                        flagt=false;
                    }
                }
            }
            if(flagt==true){
                int t=HumanBuild(AIGame.human[1].SN,BUILDING_HOME,positionl[q],positionu[q]);
                if(t==0){
                    qDebug()<<"建完第二个房子"<<positionl[q]<<positionu[q];
                    q++;
                    Human2Action=4;
                }

            }
            if(flagt==false){
                q++;
                flagt=true;
            }
        }
    }
    if(AIGame.building[4].Percent==100&&Human2Action==4){
        if(AIGame.Wood>=30){
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    if(Gamemap[positionl[q]+i][positionu[q]+j]!=0 ){
                        flagt=false;
                    }
                }
            }
            if(flagt==true){
                int t=HumanBuild(AIGame.human[1].SN,BUILDING_HOME,positionl[q],positionu[q]);
                if(t==0){
                    qDebug()<<"建完第三个房子"<<positionl[q]<<positionu[q];
                    q++;
                    Human2Action=5;
                }

            }
            if(flagt==false){
                q++;
                flagt=true;
            }
        }
    }
    if(AIGame.building[5].Percent==100&&Human2Action==5){
        if(AIGame.Wood>=30&&q<16){
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    if(Gamemap[positionl[q]+i][positionu[q]+j]!=0 ){
                        flagt=false;
                    }
                }
            }
            if(flagt==true){
                int t=HumanBuild(AIGame.human[1].SN,BUILDING_HOME,positionl[q],positionu[q]);
                if(t==0){
                    qDebug()<<"建完第4个房子"<<positionl[q]<<positionu[q];
                    q++;
                    Human2Action=6;
                }

            }
            if(flagt==false){
                q++;
                flagt=true;
            }
        }
    }
    //第2个人去打铃羊,在工具时代
    if(AIGame.building[6].Percent==100&&Human2Action==6&&AIGame.human[1].NowState==HUMAN_STATE_IDLE&&AIGame.civilizationStage==CIVILIZATION_STONEAGE){
        dis = 1e6;
        targetResSN = 0;
        for(int i = 0; i < AIGame.resource_n; i++) {
            if(AIGame.resource[i].Type == RESOURCE_GAZELLE) {
                double tempDistance = calDistance(AIGame.human[1].L, AIGame.human[1].U, AIGame.resource[i].L, AIGame.resource[i].U);
                if(dis > tempDistance) {
                    dis = tempDistance;
                    targetResSN = AIGame.resource[i].SN;
                }
            }
        }
        if(targetResSN != 0) {
            HumanAction(AIGame.human[1].SN, targetResSN);
            qDebug()<<"第2个村民打羚羊";
        }
    }
    //如果第一个人被卡住
    if(AIGame.human[1].NowState==HUMAN_STATE_STOP&&Human2Action==2){
        if(AIGame.Wood>=30){
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    if(Gamemap[positionl[q]+i][positionu[q]+j]!=0 ){
                        flagt=false;
                    }
                }
            }
            if(flagt==true){
                HumanBuild(AIGame.human[3].SN,BUILDING_HOME,positionl[q],positionu[q]);
                qDebug()<<"建完第1个房子"<<positionl[q]<<positionu[q];
                q++;
                Human2Action=7;
            }
            if(flagt==false){
                q++;
                flagt=true;
            }
        }
    }
    if(AIGame.human[1].NowState==HUMAN_STATE_STOP&&Human2Action==7){
        if(AIGame.Wood>=30){
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    if(Gamemap[positionl[q]+i][positionu[q]+j]!=0 ){
                        flagt=false;
                    }
                }
            }
            if(flagt==true){
                HumanBuild(AIGame.human[3].SN,BUILDING_HOME,positionl[q],positionu[q]);
                qDebug()<<"建完第2个房子"<<positionl[q]<<positionu[q];
                q++;
                Human2Action=8;
            }
            if(flagt==false){
                q++;
                flagt=true;
            }
        }
    }





    // 第3个村民建完建筑后，让他去采集最近的浆果丛
    static int Human3Action = 0;
    if(AIGame.building[2].Percent == 100 && Human3Action == 0&&AIGame.human[2].NowState==HUMAN_STATE_IDLE) {
        // 计算出距离第3个村民最近的树的全局编号SN
        dis = 1e6;
        targetResSN = 0;
        for(int i = 0; i < AIGame.resource_n; i++) {
            if(AIGame.resource[i].Type == RESOURCE_BUSH) {
                double tempDistance = calDistance(AIGame.human[2].L, AIGame.human[2].U, AIGame.resource[i].L, AIGame.resource[i].U);
                if(dis > tempDistance) {
                    dis = tempDistance;
                    targetResSN = AIGame.resource[i].SN;
                }
            }
        }
        if(targetResSN != 0) {
            HumanAction(AIGame.human[2].SN, targetResSN);
        }
    }


    //第四个村民
    // 第4个让他去砍最近的树
    static int Human4Action = 0;
    //100帧后村民产生才可以去建造
    if(Human4Action==0&&AIGame.GameFrame>200&&AIGame.human[3].NowState==HUMAN_STATE_IDLE&&AIGame.human[3].SN!=0) {
        // 计算出距离第4个村民最近的浆果丛的全局编号SN
            dis = 1e6;
            targetResSN = 0;
            for(int i = 0; i < AIGame.resource_n; i++) {
                if(AIGame.resource[i].Type == RESOURCE_TREE) {
                    double tempDistance = calDistance(AIGame.human[3].L, AIGame.human[3].U, AIGame.resource[i].L, AIGame.resource[i].U);
                    if(dis > tempDistance) {
                        dis = tempDistance;
                        targetResSN = AIGame.resource[i].SN;
                    }
                }
            }
            if(targetResSN != 0) {
                int t=HumanAction(AIGame.human[3].SN, targetResSN);
                if(t==0){
                    qDebug()<<"第4个村民砍树";
                }
            }
        }

    if(AIGame.human_n >= 5) {
        // 让第5个村民去采集离他最近的浆果丛
        if(AIGame.human[4].NowState==HUMAN_STATE_IDLE&&AIGame.human[4].SN!=0) {
            // 计算出距离第5个村民最近的浆果丛的全局编号SN
            dis = 1e6;
            targetResSN = 0;
            for(int i = 0; i < AIGame.resource_n; i++) {
                if(AIGame.resource[i].Type == RESOURCE_BUSH) {
                    double tempDistance = calDistance(AIGame.human[4].L, AIGame.human[4].U, AIGame.resource[i].L, AIGame.resource[i].U);
                    if(dis > tempDistance) {
                        dis = tempDistance;
                        targetResSN = AIGame.resource[i].SN;
                    }
                }
            }
            if(targetResSN != 0) {
                HumanAction(AIGame.human[4].SN, targetResSN);
                qDebug()<<"第5个村民采浆果";
            }
        }
    }

        //第六个人
    if(AIGame.human_n >= 6) {
        static int Human6Action = 0;
        if(Human6Action == 0&&AIGame.human[5].NowState==HUMAN_STATE_IDLE&&AIGame.human[5].SN!=0) {
            dis = 1e6;
            targetResSN = 0;
            for(int i = 0; i < AIGame.resource_n; i++) {
                if(AIGame.resource[i].Type == RESOURCE_TREE) {
                    double tempDistance = calDistance(AIGame.human[5].L, AIGame.human[5].U, AIGame.resource[i].L, AIGame.resource[i].U);
                    if(dis > tempDistance) {
                        dis = tempDistance;
                        targetResSN = AIGame.resource[i].SN;
                    }
                }
            }
            if(targetResSN != 0) {
                HumanAction(AIGame.human[5].SN, targetResSN);
                qDebug()<<"第6个村民砍树";
            }
        }
    }
    if(AIGame.human_n>=7){
        //第七个人
        if(AIGame.human[6].NowState==HUMAN_STATE_IDLE&&AIGame.human[6].SN!=0) {

            // 计算出距离第7个村民最近的浆果丛的全局编号SN
            dis = 1e6;
            targetResSN = 0;
            for(int i = 0; i < AIGame.resource_n; i++) {
                if(AIGame.resource[i].Type == RESOURCE_BUSH) {
                    double tempDistance = calDistance(AIGame.human[6].L, AIGame.human[6].U, AIGame.resource[i].L, AIGame.resource[i].U);
                    if(dis > tempDistance) {
                        dis = tempDistance;
                        targetResSN = AIGame.resource[i].SN;
                    }
                }
            }
            if(targetResSN != 0) {
                HumanAction(AIGame.human[6].SN, targetResSN);
                qDebug()<<"第7个村民采浆果";
            }

        }
    }
        //第八个人
    if(AIGame.human_n>=8&&AIGame.human[7].SN!=0){
        static int rl8=38;
        static int ru8=38;
        static int temp=0;
        if(temp==0){
            HumanMove(AIGame.human[7].SN,rl8*BLOCKSIDELENGTH,ru8*BLOCKSIDELENGTH);
            qDebug()<<"移动到这里";
            temp=1;
        }
        double fl8=fabs(AIGame.human[7].L-rl8*BLOCKSIDELENGTH);
        double fu8=fabs(AIGame.human[7].U-ru8*BLOCKSIDELENGTH);
        if(temp==1&&AIGame.human[7].Blood>0){
            if((fl8<1&&fu8<1)||AIGame.human[7].NowState==HUMAN_STATE_STOP){
                rl8=rand()%72;
                ru8=rand()%72;
                qDebug()<<rl8<<"人物8随机数"<<ru8;
                HumanMove(AIGame.human[7].SN,rl8*BLOCKSIDELENGTH,ru8*BLOCKSIDELENGTH);
            }
        }
}

    //第二个房子建好后产生4个村民
    if(AIGame.human_n>=9&&AIGame.human[8].SN!=0){
        //第九个人探路
        static int rl9=38;
        static int ru9=26;
        static int temp9=0;
        if(temp9==0){
            HumanMove(AIGame.human[8].SN,rl9*BLOCKSIDELENGTH,ru9*BLOCKSIDELENGTH);
            qDebug()<<"移动到这里9";
            temp9=1;
        }
        double fl9=fabs(AIGame.human[8].L-rl9*BLOCKSIDELENGTH);
        double fu9=fabs(AIGame.human[8].U-ru9*BLOCKSIDELENGTH);
        if(temp9==1&&AIGame.human[8].Blood>0){
            if((fl9<1&&fu9<1)||AIGame.human[8].NowState==HUMAN_STATE_STOP){
                rl9=rand()%72;
                ru9=rand()%72;
                qDebug()<<rl9<<"人物9随机数"<<ru9;
                HumanMove(AIGame.human[8].SN,rl9*BLOCKSIDELENGTH,ru9*BLOCKSIDELENGTH);
            }
        }
    }
    static int beingattack1=0;
    if(AIGame.human[8].NowState==HUMAN_STATE_GOTO_RESOURCE){
        beingattack1=1;
        qDebug()<<"第9个人受到攻击";
    }
    if(AIGame.human[8].NowState==HUMAN_STATE_IDLE&&beingattack1==1){
        dis = 1e6;
        targetResSN = 0;
        for(int i = 0; i < AIGame.resource_n; i++) {
            if(AIGame.resource[i].Type == RESOURCE_TREE) {
                double tempDistance = calDistance(AIGame.human[8].L, AIGame.human[8].U, AIGame.resource[i].L, AIGame.resource[i].U);
                if(dis > tempDistance) {
                    dis = tempDistance;
                    targetResSN = AIGame.resource[i].SN;
                }
            }
        }
        if(targetResSN != 0) {
            HumanAction(AIGame.human[8].SN, targetResSN);
            qDebug()<<"第9个村民砍树";
        }
    }
        //第10个人探路
     if(AIGame.human_n>=10){
        static int rl10=57;
        static int ru10=16;
        static int temp10=0;
        if(temp10==0){
            HumanMove(AIGame.human[9].SN,rl10*BLOCKSIDELENGTH,ru10*BLOCKSIDELENGTH);
            qDebug()<<"移动到这里10";
            temp10=1;
        }
        double fl10=fabs(AIGame.human[9].L-rl10*BLOCKSIDELENGTH);
        double fu10=fabs(AIGame.human[9].U-ru10*BLOCKSIDELENGTH);
        if(temp10==1&&AIGame.human[9].Blood>0){
            if((fl10<1&&fu10<1)||AIGame.human[9].NowState==HUMAN_STATE_STOP){
                rl10=rand()%72;
                ru10=rand()%72;
                qDebug()<<rl10<<"人物10随机数"<<ru10;
                HumanMove(AIGame.human[9].SN,rl10*BLOCKSIDELENGTH,ru10*BLOCKSIDELENGTH);
            }
        }
        //找市场,农田的位置
        static bool flagt=true;

        static int ht=0;
        static int blt=AIGame.human[9].BlockL;
        static int but=AIGame.human[9].BlockU;
        if(AIGame.GameFrame%100==0&&ht<15){
            blt=AIGame.human[9].BlockL;
            but=AIGame.human[9].BlockU;
            for(int i=0;i<5;i++){
                for(int j=0;j<5;j++){
                    if(Gamemap[blt+i][but+j]!=0){
                        flagt=false;
                    }
                }
            }
            if(flagt==true){
                pl[ht]=AIGame.human[9].BlockL;
                pu[ht]=AIGame.human[9].BlockU;
                qDebug()<<pl[ht]<<"find it市场"<<pu[ht];
                for(int i=0;i<5;i++){
                    for(int j=0;j<5;j++){
                        Gamemap[pl[ht]+i][pu[ht]+j]=5;//表明该建筑附近已经被占用
                    }
                }
                ht++;
                qDebug()<<ht<<"ht大小";
            }
            if(flagt==false){
                flagt=true;
            }
        }

    }
    //第三个房子建好
    if(AIGame.human_n>=11){
        //第11个人砍树
        if(AIGame.human[10].NowState==HUMAN_STATE_IDLE) {
            dis = 1e6;
            targetResSN = 0;
            for(int i = 0; i < AIGame.resource_n; i++) {
                if(AIGame.resource[i].Type == RESOURCE_GAZELLE) {
                    double tempDistance = calDistance(AIGame.human[10].L, AIGame.human[10].U, AIGame.resource[i].L, AIGame.resource[i].U);
                    if(dis > tempDistance) {
                        dis = tempDistance;
                        targetResSN = AIGame.resource[i].SN;
                    }
                }
            }
            if(targetResSN != 0) {
                HumanAction(AIGame.human[10].SN, targetResSN);
                qDebug()<<"第11个村民砍树";
            }
        }
    }
        //第12个人采石头
    if(AIGame.human_n>=12){
        if(AIGame.human[11].NowState==HUMAN_STATE_IDLE) {
            dis = 1e6;
            targetResSN = 0;
            for(int i = 0; i < AIGame.resource_n; i++) {
                if(AIGame.resource[i].Type == RESOURCE_STONE) {
                    double tempDistance = calDistance(AIGame.human[11].L, AIGame.human[11].U, AIGame.resource[i].L, AIGame.resource[i].U);
                    if(dis > tempDistance) {
                        dis = tempDistance;
                        targetResSN = AIGame.resource[i].SN;
                    }
                }
            }
            if(targetResSN != 0) {
                HumanAction(AIGame.human[11].SN, targetResSN);
                qDebug()<<"第12个村民采石头";
            }
        }
    }
        //13,14,15打狮子
    //第13个人
    if(AIGame.human_n>=13){
        if(AIGame.human[12].NowState==HUMAN_STATE_IDLE) {
            dis = 1e6;
            targetResSN = 0;
            for(int i = 0; i < AIGame.resource_n; i++) {
                if(AIGame.resource[i].Type == RESOURCE_LION) {
                    double tempDistance = calDistance(AIGame.human[12].L, AIGame.human[12].U, AIGame.resource[i].L, AIGame.resource[i].U);
                    if(dis > tempDistance) {
                        dis = tempDistance;
                        targetResSN = AIGame.resource[i].SN;
                    }
                }
            }
            if(targetResSN != 0) {
                HumanAction(AIGame.human[12].SN, targetResSN);
                qDebug()<<"第13个村民打狮子";
            }
        }
    }
    //第14个人
    if(AIGame.human_n>=14){
        if(AIGame.human[13].NowState==HUMAN_STATE_IDLE){
            dis = 1e6;
            targetResSN = 0;
            for(int i = 0; i < AIGame.resource_n; i++) {
                if(AIGame.resource[i].Type == RESOURCE_GAZELLE) {
                    double tempDistance = calDistance(AIGame.human[13].L, AIGame.human[13].U, AIGame.resource[i].L, AIGame.resource[i].U);
                    if(dis > tempDistance) {
                        dis = tempDistance;
                        targetResSN = AIGame.resource[i].SN;
                    }
                }
            }
            if(targetResSN != 0) {
                HumanAction(AIGame.human[13].SN, targetResSN);
                qDebug()<<"第14个村民打羚羊";
            }
        }
    }
    //第15个人
    if(AIGame.human_n>=15){
        if(AIGame.human[14].NowState==HUMAN_STATE_IDLE){
            dis = 1e6;
            targetResSN = 0;
            for(int i = 0; i < AIGame.resource_n; i++) {
                if(AIGame.resource[i].Type == RESOURCE_GAZELLE) {
                    double tempDistance = calDistance(AIGame.human[14].L, AIGame.human[14].U, AIGame.resource[i].L, AIGame.resource[i].U);
                    if(dis > tempDistance) {
                        dis = tempDistance;
                        targetResSN = AIGame.resource[i].SN;
                    }
                }
            }
            if(targetResSN != 0) {
                HumanAction(AIGame.human[14].SN, targetResSN);
                qDebug()<<"第15个村民打羚羊";
            }
        }
    }
        //第16个人采浆果
    if(AIGame.human_n>=16){
        if(AIGame.human[15].NowState==HUMAN_STATE_IDLE){
            dis = 1e6;
            targetResSN = 0;
            for(int i = 0; i < AIGame.resource_n; i++) {
                if(AIGame.resource[i].Type == RESOURCE_BUSH) {
                    double tempDistance = calDistance(AIGame.human[15].L, AIGame.human[15].U, AIGame.resource[i].L, AIGame.resource[i].U);
                    if(dis > tempDistance) {
                        dis = tempDistance;
                        targetResSN = AIGame.resource[i].SN;
                    }
                }
            }
            if(targetResSN != 0) {
                HumanAction(AIGame.human[15].SN, targetResSN);
                qDebug()<<"第16个村民采浆果";
            }
        }

    }

}



    //进入工具时代
    static int action1=1;
    static int action2=1;
    static int action3=1;
    static int action4=1;

    static int action6=1;
    if(AIGame.civilizationStage==CIVILIZATION_TOOLAGE&&AIGame.human_n>10){

        if(AIGame.building[0].Project == BUILDING_FREE && AIGame.Meat >= BUILDING_CENTER_CREATEFARMER_FOOD && AIGame.human_n <20)
                    BuildingAction(AIGame.building[0].SN, BUILDING_CENTER_CREATEFARMER);



        if(action6==1&&AIGame.Wood>=150&&AIGame.building[7].Percent!=100&&AIGame.human[16].NowState==HUMAN_STATE_IDLE){
            qDebug()<<"进入";
            int t=HumanBuild(AIGame.human[16].SN,BUILDING_MARKET,pl[0],pu[0]);
            qDebug()<<pl[0]<<" 市场"<<pu[0];
            if(t!=0){
                int t2=HumanBuild(AIGame.human[16].SN,BUILDING_MARKET,pl[1],pu[1]);
                if(t2!=0){
                    HumanBuild(AIGame.human[16].SN,BUILDING_MARKET,pl[10],pu[10]);
                    action6=2;
                }
                else{
                    action6=2;
                    qDebug()<<pl[1]<<" 市场"<<pu[1];
                }

            }
            else{
                qDebug()<<"建好市场";
                action6=2;
            }

        }
        static int action7=1;
        static int action8=1;
        if(AIGame.building[7].Percent==100&&AIGame.building[7].Project==BUILDING_FREE&&action7==1&&AIGame.Wood>BUILDING_MARKET_FARM_UPGRADE_WOOD&&AIGame.Meat>BUILDING_MARKET_FARM_UPGRADE_FOOD){
              int t=BuildingAction(AIGame.building[2].SN,BUILDING_MARKET_FARM_UPGRADE);
              if(t==0){
                  qDebug()<<"农场升级完毕";
                  action7=2;
                  action8=2;
              }
          }
          if(action7==2&&AIGame.building[7].Project==BUILDING_FREE){
              int t=BuildingAction(AIGame.building[2].SN,BUILDING_MARKET_STONE_UPGRADE);
              if(t==0){
                  qDebug()<<"采石量升级完毕";
                  action7=3;
              }
          }
          if(action7==3&&AIGame.building[7].Project==BUILDING_FREE){
              int t=BuildingAction(AIGame.building[2].SN,BUILDING_MARKET_WOOD_UPGRADE);
              if(t==0){
                  qDebug()<<"伐木量升级完毕";
                  action7=4;
              }
          }


//          static int farmnum=0;
//          static int
//          for(int i=0;i<AIGame.building_n;i++){
//              if(AIGame.building[i].Type==BUILDING_FARM){
//                  farmnum++;
//              }
//          }
        if(AIGame.building[7].Percent==100&&AIGame.human[16].NowState==HUMAN_STATE_IDLE&&AIGame.Wood>=75&&action1==1&&action8==2){
            int t=HumanBuild(AIGame.human[16].SN,BUILDING_FARM,pl[2],pu[2]);
            if(t==0){
                qDebug()<<"建好第1个农田";
                action1=2;
            }
            else{
                pl[2]++;
            }
        }
        if(AIGame.building[7].Percent==100&&AIGame.human[17].NowState==HUMAN_STATE_IDLE&&AIGame.Wood>=75&&action2==1&&action8==2){
            int t=HumanBuild(AIGame.human[17].SN,BUILDING_FARM,pl[3],pu[3]);
            if(pl[3]>70){
                action2=2;
            }
            if(t==0){
                qDebug()<<"建好第2个农田";
                action2=2;
            }
            else{
                pl[3]++;
            }
        }
        if(AIGame.building[7].Percent==100&&AIGame.human[18].NowState==HUMAN_STATE_IDLE&&AIGame.Wood>=75&&action3==1&&action8==2){
            int t=HumanBuild(AIGame.human[18].SN,BUILDING_FARM,pl[4],pu[4]);
            if(pl[4]>70){
                action3=2;
            }
            if(t==0){
                qDebug()<<"建好第3个农田";
                action3=2;
            }
            else{
                pl[4]++;
            }
        }
        if(AIGame.building[7].Percent==100&&AIGame.human[19].NowState==HUMAN_STATE_IDLE&&AIGame.Wood>=75&&action4==1&&action8==2){
            int t=HumanBuild(AIGame.human[19].SN,BUILDING_FARM,pl[5],pu[5]);
            if(pl[5]>70){
                action4=2;
            }
            if(t==0){
                qDebug()<<"建好第4个农田";
                action4=2;
            }
            else{
                pl[5]++;
            }
        }
        //新增人种田
        static int a9=1;
        if(AIGame.building[7].Percent==100&&AIGame.human[10].NowState==HUMAN_STATE_IDLE&&AIGame.Wood>=75&&a9==1&&action8==2){
            int t=HumanBuild(AIGame.human[10].SN,BUILDING_FARM,pl[6],pu[6]);
            if(pl[6]>70){
                a9=2;
            }
            if(t==0){
                qDebug()<<"建好第5个农田";
                a9=2;
            }
            else{
                pl[6]++;
            }
        }
        static int action11=1;
        if(AIGame.building[7].Percent==100&&AIGame.human[11].NowState==HUMAN_STATE_IDLE&&AIGame.Wood>=75&&action11==1&&action8==2){
            int t=HumanBuild(AIGame.human[11].SN,BUILDING_FARM,pl[7],pu[7]);
            if(pu[7]>70){
                action11=2;
            }
            if(t==0){
                qDebug()<<"建好第6个农田";
                action11=2;
            }
            else{
                pu[7]++;
            }
        }

        //升级谷仓

          //进不来
          static int action5=1;
          if(action5==1&&AIGame.building[2].Project==BUILDING_FREE){
              int t=1;
              t=BuildingAction(AIGame.building[2].SN,BUILDING_GRANARY_ARROWTOWER);
              if(t==0){
                  qDebug()<<action5<<"谷仓升级完毕";
                  action5=2;
              }
          }
        //箭塔
          static int towernum=0;
          static int tn=0;
          for(int i=tn;i<AIGame.building_n;i++){
              if(AIGame.building[i].Type==BUILDING_ARROWTOWER){
                  towernum++;
                  tn=i+1;
                  break;
              }
          }
        static int tx[3]={0};
        static int ty[3]={0};
        static int ti=0;
        if(ti<=2){
            for(int i=0;i<72;i++){
                for(int j=0;j<72;j++){
                    if(Gamemap[i][j]==3){
                        tx[ti]=i;
                        ty[ti]=j;
                        Gamemap[i][j]=6;//箭塔
                        ti++;
                    }
                }
            }
        }
        if(action5==2&&AIGame.human[13].NowState==HUMAN_STATE_IDLE&&AIGame.Stone>=BUILD_ARROWTOWER_STONE&&ti>=0){
            int t=1;
            t=HumanBuild(AIGame.human[13].SN,BUILDING_ARROWTOWER,tx[0],ty[0]);
            if(t==0){
                qDebug()<<"建好一个箭塔";
                action5=3;
            }
        }
        if(towernum==1&&action5==3&&AIGame.human[13].NowState==HUMAN_STATE_IDLE&&AIGame.Stone>=BUILD_ARROWTOWER_STONE&&ti>=1){
            int t=HumanBuild(AIGame.human[13].SN,BUILDING_ARROWTOWER,tx[1],ty[1]);
            if(t==0){
                qDebug()<<"建好两个箭塔";
                action5=4;
            }

        }
        if(towernum==2&&action5==4&&AIGame.human[13].NowState==HUMAN_STATE_IDLE&&AIGame.Stone>=BUILD_ARROWTOWER_STONE&&ti>=2){
            int t=HumanBuild(AIGame.human[13].SN,BUILDING_ARROWTOWER,tx[2],ty[2]);
            if(t==0){
                qDebug()<<"建好三个箭塔";
                action5=5;
            }
        }
        if(towernum==3&&action5==5&&AIGame.human[13].NowState==HUMAN_STATE_IDLE&&AIGame.Wood<1000){
            //砍树
            dis = 1e6;
            targetResSN = 0;
            for(int i = 0; i < AIGame.resource_n; i++) {
                if(AIGame.resource[i].Type == RESOURCE_TREE) {
                    double tempDistance = calDistance(AIGame.human[13].L, AIGame.human[13].U, AIGame.resource[i].L, AIGame.resource[i].U);
                    if(dis > tempDistance) {
                        dis = tempDistance;
                        targetResSN = AIGame.resource[i].SN;
                    }
                }
            }
            if(targetResSN != 0) {
                HumanAction(AIGame.human[13].SN, targetResSN);
                qDebug()<<"第14个村民砍树1";
            }
        }

        //处置闲置农民

        //重新安排
        //第3个人
       if(AIGame.human[2].SN!=0){
        static int rx1=60;
        static int ry1=26;
        static int tp1=0;
        if(tp1==0){
            HumanMove(AIGame.human[2].SN,rx1*BLOCKSIDELENGTH,ry1*BLOCKSIDELENGTH);
            qDebug()<<"移动到这里1";
            tp1=1;
        }
        double fx1=fabs(AIGame.human[2].L-rx1*BLOCKSIDELENGTH);
        double fy1=fabs(AIGame.human[2].U-ry1*BLOCKSIDELENGTH);
        if(tp1==1&&AIGame.human[2].Blood>0){
            if((fx1<1&&fy1<1)||AIGame.human[2].NowState==HUMAN_STATE_STOP){
                rx1=rand()%72;
                ry1=rand()%72;
                qDebug()<<rx1<<"人物1随机数"<<ry1;
                HumanMove(AIGame.human[2].SN,rx1*BLOCKSIDELENGTH,ry1*BLOCKSIDELENGTH);
            }
        }
       }
       static int beingattack2=0;
       if(AIGame.human[2].NowState==HUMAN_STATE_GOTO_RESOURCE){
           beingattack2=1;
           //qDebug()<<"第3个人受到攻击";
       }
       if(AIGame.human[2].NowState==HUMAN_STATE_IDLE&&beingattack2==1){
           dis = 1e6;
           targetResSN = 0;
           for(int i = 0; i < AIGame.resource_n; i++) {
               if(AIGame.resource[i].Type == RESOURCE_STONE) {
                   double tempDistance = calDistance(AIGame.human[2].L, AIGame.human[2].U, AIGame.resource[i].L, AIGame.resource[i].U);
                   if(dis > tempDistance) {
                       dis = tempDistance;
                       targetResSN = AIGame.resource[i].SN;
                   }
               }
           }
           if(targetResSN != 0) {
               HumanAction(AIGame.human[2].SN, targetResSN);
               qDebug()<<"第3个村民打羚羊";
           }
       }
       //第二个人
       if(AIGame.human[1].SN!=0){
        static int rx2=10;
        static int ry2=21;
        static int tp2=0;
        if(tp2==0){
            HumanMove(AIGame.human[1].SN,rx2*BLOCKSIDELENGTH,ry2*BLOCKSIDELENGTH);
            qDebug()<<"移动到这里2";
            tp2=1;
        }
        double fx2=fabs(AIGame.human[1].L-rx2*BLOCKSIDELENGTH);
        double fy2=fabs(AIGame.human[1].U-ry2*BLOCKSIDELENGTH);
        if(tp2==1&&AIGame.human[1].Blood>0){
            if((fx2<1&&fy2<1)||AIGame.human[1].NowState==HUMAN_STATE_STOP){
                rx2=rand()%72;
                ry2=rand()%72;
                qDebug()<<rx2<<"人物2随机数"<<ry2;
                HumanMove(AIGame.human[1].SN,rx2*BLOCKSIDELENGTH,ry2*BLOCKSIDELENGTH);
            }
        }
       }
       //第三个人
       if(AIGame.human[0].NowState==HUMAN_STATE_IDLE){
           dis = 1e6;
           targetResSN = 0;
           for(int i = 0; i < AIGame.resource_n; i++) {
               if(AIGame.resource[i].Type == RESOURCE_TREE) {
                   double tempDistance = calDistance(AIGame.human[0].L, AIGame.human[0].U, AIGame.resource[i].L, AIGame.resource[i].U);
                   if(dis > tempDistance) {
                       dis = tempDistance;
                       targetResSN = AIGame.resource[i].SN;
                   }
               }
           }
           if(targetResSN != 0) {
               HumanAction(AIGame.human[0].SN, targetResSN);
               qDebug()<<"第3个村民砍树1";
           }
       }
       //第四个人
       if(AIGame.human[3].NowState==HUMAN_STATE_IDLE){
           dis = 1e6;
           targetResSN = 0;
           for(int i = 0; i < AIGame.resource_n; i++) {
               if(AIGame.resource[i].Type == RESOURCE_TREE) {
                   double tempDistance = calDistance(AIGame.human[3].L, AIGame.human[3].U, AIGame.resource[i].L, AIGame.resource[i].U);
                   if(dis > tempDistance) {
                       dis = tempDistance;
                       targetResSN = AIGame.resource[i].SN;
                   }
               }
           }
           if(targetResSN != 0) {
               HumanAction(AIGame.human[3].SN, targetResSN);
               qDebug()<<"第4个村民砍树1";
           }
       }
       //第五个人
       if(AIGame.human[4].NowState==HUMAN_STATE_IDLE){
           dis = 1e6;
           targetResSN = 0;
           for(int i = 0; i < AIGame.resource_n; i++) {
               if(AIGame.resource[i].Type == RESOURCE_TREE) {
                   double tempDistance = calDistance(AIGame.human[4].L, AIGame.human[4].U, AIGame.resource[i].L, AIGame.resource[i].U);
                   if(dis > tempDistance) {
                       dis = tempDistance;
                       targetResSN = AIGame.resource[i].SN;
                   }
               }
           }
           if(targetResSN != 0) {
               HumanAction(AIGame.human[4].SN, targetResSN);
               qDebug()<<"第5个村民砍树1";
           }
       }
       //第六个人
       if(AIGame.human[5].NowState==HUMAN_STATE_IDLE){
           dis = 1e6;
           targetResSN = 0;
           for(int i = 0; i < AIGame.resource_n; i++) {
               if(AIGame.resource[i].Type == RESOURCE_STONE) {
                   double tempDistance = calDistance(AIGame.human[5].L, AIGame.human[5].U, AIGame.resource[i].L, AIGame.resource[i].U);
                   if(dis > tempDistance) {
                       dis = tempDistance;
                       targetResSN = AIGame.resource[i].SN;
                   }
               }
           }
           if(targetResSN != 0) {
               HumanAction(AIGame.human[5].SN, targetResSN);
               qDebug()<<"第6个村民采石头1";
           }
       }
       //第七个人
       if(AIGame.human[6].NowState==HUMAN_STATE_IDLE){
           dis = 1e6;
           targetResSN = 0;
           for(int i = 0; i < AIGame.resource_n; i++) {
               if(AIGame.resource[i].Type == RESOURCE_STONE) {
                   double tempDistance = calDistance(AIGame.human[6].L, AIGame.human[6].U, AIGame.resource[i].L, AIGame.resource[i].U);
                   if(dis > tempDistance) {
                       dis = tempDistance;
                       targetResSN = AIGame.resource[i].SN;
                   }
               }
           }
           if(targetResSN != 0) {
               HumanAction(AIGame.human[6].SN, targetResSN);
               qDebug()<<"第7个村民采石头1";
           }
       }
       //第八个人
       if(AIGame.human[7].NowState==HUMAN_STATE_IDLE){
           dis = 1e6;
           targetResSN = 0;
           for(int i = 0; i < AIGame.resource_n; i++) {
               if(AIGame.resource[i].Type == RESOURCE_STONE) {
                   double tempDistance = calDistance(AIGame.human[7].L, AIGame.human[7].U, AIGame.resource[i].L, AIGame.resource[i].U);
                   if(dis > tempDistance) {
                       dis = tempDistance;
                       targetResSN = AIGame.resource[i].SN;
                   }
               }
           }
           if(targetResSN != 0) {
               HumanAction(AIGame.human[7].SN, targetResSN);
               qDebug()<<"第8个村民采石头1";
           }
       }
       //第九个人
       if(AIGame.human[8].NowState==HUMAN_STATE_IDLE&&gazellejudge==1){
           dis = 1e6;
           targetResSN = 0;
           for(int i = 0; i < AIGame.resource_n; i++) {
               if(AIGame.resource[i].Type == RESOURCE_GAZELLE) {
                   double tempDistance = calDistance(AIGame.human[8].L, AIGame.human[8].U, AIGame.resource[i].L, AIGame.resource[i].U);
                   if(dis > tempDistance) {
                       dis = tempDistance;
                       targetResSN = AIGame.resource[i].SN;
                   }
               }
           }
           if(targetResSN != 0) {
               HumanAction(AIGame.human[8].SN, targetResSN);
               qDebug()<<"第9个村民打羚羊1";
           }
       }
       if(AIGame.human[8].NowState==HUMAN_STATE_IDLE&&gazellejudge==0){
           dis = 1e6;
           targetResSN = 0;
           for(int i = 0; i < AIGame.resource_n; i++) {
               if(AIGame.resource[i].Type == RESOURCE_STONE) {
                   double tempDistance = calDistance(AIGame.human[8].L, AIGame.human[8].U, AIGame.resource[i].L, AIGame.resource[i].U);
                   if(dis > tempDistance) {
                       dis = tempDistance;
                       targetResSN = AIGame.resource[i].SN;
                   }
               }
           }
           if(targetResSN != 0) {
               HumanAction(AIGame.human[8].SN, targetResSN);
               qDebug()<<"第9个村民采石头1";
           }
       }
       //第10个人
       if(AIGame.human[9].NowState==HUMAN_STATE_IDLE){
           dis = 1e6;
           targetResSN = 0;
           for(int i = 0; i < AIGame.resource_n; i++) {
               if(AIGame.resource[i].Type == RESOURCE_GAZELLE) {
                   double tempDistance = calDistance(AIGame.human[9].L, AIGame.human[9].U, AIGame.resource[i].L, AIGame.resource[i].U);
                   if(dis > tempDistance) {
                       dis = tempDistance;
                       targetResSN = AIGame.resource[i].SN;
                   }
               }
           }
           if(targetResSN != 0) {
               HumanAction(AIGame.human[9].SN, targetResSN);
               qDebug()<<"第10个村民打羚羊1";
           }
       }
       //第13个人
       if(AIGame.human[12].NowState==HUMAN_STATE_IDLE&&bushjudge==1){
           dis = 1e6;
           targetResSN = 0;
           for(int i = 0; i < AIGame.resource_n; i++) {
               if(AIGame.resource[i].Type == RESOURCE_BUSH) {
                   double tempDistance = calDistance(AIGame.human[12].L, AIGame.human[12].U, AIGame.resource[i].L, AIGame.resource[i].U);
                   if(dis > tempDistance) {
                       dis = tempDistance;
                       targetResSN = AIGame.resource[i].SN;
                   }
               }
           }
           if(targetResSN != 0) {
               HumanAction(AIGame.human[12].SN, targetResSN);
               qDebug()<<"第13个村民采浆果1";
           }
       }
       if(AIGame.human[12].NowState==HUMAN_STATE_IDLE&&bushjudge==0){
           dis = 1e6;
           targetResSN = 0;
           for(int i = 0; i < AIGame.resource_n; i++) {
               if(AIGame.resource[i].Type == RESOURCE_GAZELLE) {
                   double tempDistance = calDistance(AIGame.human[12].L, AIGame.human[12].U, AIGame.resource[i].L, AIGame.resource[i].U);
                   if(dis > tempDistance) {
                       dis = tempDistance;
                       targetResSN = AIGame.resource[i].SN;
                   }
               }
           }
           if(targetResSN != 0) {
               HumanAction(AIGame.human[12].SN, targetResSN);
               qDebug()<<"第13个村民打羚羊1";
           }
       }
       //第15个人
       if(AIGame.human[14].NowState==HUMAN_STATE_IDLE&&AIGame.Wood<1000){
           dis = 1e6;
           targetResSN = 0;
           for(int i = 0; i < AIGame.resource_n; i++) {
               if(AIGame.resource[i].Type == RESOURCE_TREE) {
                   double tempDistance = calDistance(AIGame.human[14].L, AIGame.human[14].U, AIGame.resource[i].L, AIGame.resource[i].U);
                   if(dis > tempDistance) {
                       dis = tempDistance;
                       targetResSN = AIGame.resource[i].SN;
                   }
               }
           }
           if(targetResSN != 0) {
               HumanAction(AIGame.human[14].SN, targetResSN);
               qDebug()<<"第15个村民砍树1";
           }
       }
       if(AIGame.human[14].NowState==HUMAN_STATE_IDLE&&AIGame.Wood>1000){
           dis = 1e6;
           targetResSN = 0;
           for(int i = 0; i < AIGame.resource_n; i++) {
               if(AIGame.resource[i].Type == RESOURCE_STONE) {
                   double tempDistance = calDistance(AIGame.human[14].L, AIGame.human[14].U, AIGame.resource[i].L, AIGame.resource[i].U);
                   if(dis > tempDistance) {
                       dis = tempDistance;
                       targetResSN = AIGame.resource[i].SN;
                   }
               }
           }
           if(targetResSN != 0) {
               HumanAction(AIGame.human[14].SN, targetResSN);
               qDebug()<<"第15个村民采石头1";
           }
       }
       if(AIGame.human[15].NowState==HUMAN_STATE_IDLE&&AIGame.Stone<1000){
           dis = 1e6;
           targetResSN = 0;
           for(int i = 0; i < AIGame.resource_n; i++) {
               if(AIGame.resource[i].Type == RESOURCE_STONE) {
                   double tempDistance = calDistance(AIGame.human[15].L, AIGame.human[15].U, AIGame.resource[i].L, AIGame.resource[i].U);
                   if(dis > tempDistance) {
                       dis = tempDistance;
                       targetResSN = AIGame.resource[i].SN;
                   }
               }
           }
           if(targetResSN != 0) {
               HumanAction(AIGame.human[15].SN, targetResSN);
               qDebug()<<"第16个村民采石头1";
           }
       }

    }

//    if(!tryBuild) tryBuild = true; // 只尝试建造一次


    // 与主线程通讯

    ProcessDataWork = 0;
}
