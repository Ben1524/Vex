#include <iostream>
#include<string.h>
#define MAX_PATH 32768
using namespace std;
typedef struct{
    char name[100];
    char info[10000];
}VertexType; //顶点结构
typedef struct{
    VertexType vexs[10];
    int arcs[100][100];//邻接矩阵
    int vexnum,arcnum;//顶点个数，边的个数
}MGraph; //图结构
void CreateVertex(MGraph &G)
{
    // 初始化邻接矩阵
    for(int i=0;i<100;i++)
        for(int j=0;j<100;j++)
            G.arcs[i][j]=MAX_PATH;
    FILE *fp=fopen("D:\\Users\\a1767\\CLionProjects\\Vex\\graph.txt","r");
    fscanf(fp,"%d %d",&G.vexnum,&G.arcnum);
    char ch1[100],ch2[100];
    int i=0,m,j,weight;
    while(i<8)
    {
        fscanf(fp, "%s %s", G.vexs[i].name, G.vexs[i].info);
        i++;
    }
    while(!feof(fp))
    {
        fscanf(fp,"%s %s %d",ch1,ch2,&weight);
        for (int k = 0; k < G.vexnum; k++)
        {
            if(strcmp(ch1,G.vexs[k].name)==0)
                m=k;
            if(strcmp(ch2,G.vexs[k].name)==0)
                j=k;
        }
        G.arcs[m][j]=G.arcs[j][m]=weight;
    }
    fclose(fp);
}
void Dijkstra2(MGraph G,int v0,int v,int *P,int *D)
{
    int final[100];
    int i,j,k,min;
    for(i=0;i<G.vexnum;i++)
    {
        final[i]=0;
        D[i]=G.arcs[v0][i];
        for(j=0;j<G.vexnum;j++)
            P[i]=0;
        if(D[i]<MAX_PATH)
            P[i]=v0;
    }
    D[v0]=0;
    final[v0]=1;
    for(i=1;i<G.vexnum;i++)
    {
        min=MAX_PATH;
        for(j=0;j<G.vexnum;j++)
            if(!final[j]&&D[j]<min)
            {
                k=j;
                min=D[j];
            }
        final[k]=1;
        for(j=0;j<G.vexnum;j++)
            if(!final[j]&&(min+G.arcs[k][j]<D[j]))
            {
                D[j]=min+G.arcs[k][j];
                P[j]=k;
            }
    }
    cout<<"从"<<G.vexs[v0].name<<"到"<<G.vexs[v].name<<"的最短路径为："<<endl;
    cout<<D[v]<<endl;
    cout<<"路径为："<<endl;
    int path[100];
    int n=0;
    while(v!=v0)
    {
        path[n++]=v;
        v=P[v];
    }
    path[n++]=v0;
    for(i=n-1;i>=0;i--)
        cout<<G.vexs[path[i]].name<<" ";
    cout<<endl;
}
void AddPlace(MGraph &G)
{
    // 添加景点
    char ch1[100],ch2[100];
    char weight[5];
    char infomain[100][100];
    cout<<"请输入景点名称和信息："<<endl;
    cin>>ch1>>ch2;
    FILE *fp=fopen("D:\\Users\\a1767\\CLionProjects\\Vex\\graph.txt","r");
    // 将文件信息写入infomain
    int i=0;
    while(!feof(fp))
    {
        fscanf(fp, "\n%[^\n]", infomain[i]);
        i++;
    }
    fclose(fp);
    strcat(ch1,"\t");
    strcat(ch1,ch2);
    // 将新景点信息插入infomain的某一行
    for(int j=i;j>G.vexnum;j--)
        strcpy(infomain[j],infomain[j-1]);
    strcpy(infomain[G.vexnum+1],ch1);//写入G.vexnum+1行，因为第一行是顶点数和边数
    G.vexnum++;
    cout<<"添加成功！"<<endl;
    cout << "输入与该景点相连的景点名称和距离(输入0退出)：" << endl;
    for(;;)
    {
        cin >> ch1 >> ch2 >> weight;
        if(strcmp(ch1,"0") == 0||strcmp(ch2,"0") == 0||strcmp(weight,"0") == 0)
            break;
        strcat(ch1, "\t");
        strcat(ch1, ch2);;
        strcat(ch1, "\t");
        strcat(ch1, weight);
        // 将新景点信息插入infomain的最后一行
        strcpy(infomain[i], ch1);
        i++;
        G.arcnum++;
    }
    // 将infomain写入文件
    fp=fopen("D:\\Users\\a1767\\CLionProjects\\Vex\\graph.txt","w");
    fprintf(fp,"%d %d\n",G.vexnum,G.arcnum);// 写入顶点数和边数
    for(int j=1;j<i;j++)// 从第二行开始写入
        fprintf(fp,"%s\n",infomain[j]);
    fclose(fp);
    CreateVertex(G);
}
void DePlace(MGraph &G,char ch3[])
{
    // 删除景点
    char infomain[100][100];
    FILE *fp=fopen("D:\\Users\\a1767\\CLionProjects\\Vex\\graph.txt","r");
    // 将文件信息写入infomain
    int i=0;
    while(!feof(fp))
    {
        fscanf(fp, "\n%[^\n]", infomain[i]);
        i++;
    }
    // i为文件行数
    fclose(fp);
    // 将infomain中的景点信息删除
    for(int j=0;j<i;j++)
    {
        if(strstr(infomain[j],ch3)!=NULL)// strstr函数在infomain[j]中查找ch3，若找到则返回ch3在infomain[j]中的首地址，否则返回NULL
        {
            for(int k=j;k<i-1;k++)
                strcpy(infomain[k],infomain[k+1]);
            i--;
            j--;// 因为删除了一行，所以j要减一，否则会跳过一行
        }
    }
    G.vexnum--;
    G.arcnum=i-1-G.vexnum;// i-1为去掉顶点数的行，G.vexnum为顶点数
    // 将infomain写入文件
    fp=fopen("D:\\Users\\a1767\\CLionProjects\\Vex\\graph.txt","w");
    fprintf(fp,"%d %d\n",G.vexnum,G.arcnum);// 写入顶点数和边数
    for(int j=1;j<i;j++)// 从第二行开始写入
        fprintf(fp,"%s\n",infomain[j]);
    fclose(fp);
    cout<<"删除成功！"<<endl;
    CreateVertex(G);
}
void AddPath(MGraph &G)
{
    // 添加路径
    char ch1[100],ch2[100];
    char weight[5];
    char infomain[100][100];
    cout<<"请输入两个景点名称和距离："<<endl;
    cin>>ch1>>ch2>>weight;
    FILE *fp=fopen("D:\\Users\\a1767\\CLionProjects\\Vex\\graph.txt","r");
    // 将文件信息写入infomain
    int i=0;
    while(!feof(fp))
    {
        fscanf(fp, "\n%[^\n]", infomain[i]);
        i++;
    }
    fclose(fp);
    strcat(ch1,"\t");
    strcat(ch1,ch2);
    strcat(ch1,"\t");
    strcat(ch1,weight);
    // 将新景点信息插入infomain的最后一行
    strcpy(infomain[i],ch1);
    i++;
    G.arcnum++;
    // 将infomain写入文件
    fp=fopen("D:\\Users\\a1767\\CLionProjects\\Vex\\graph.txt","w");
    fprintf(fp,"%d %d\n",G.vexnum,G.arcnum);// 写入顶点数和边数
    for(int j=1;j<i;j++)// 从第二行开始写入
        fprintf(fp,"%s\n",infomain[j]);
    fclose(fp);
    cout<<"添加成功！"<<endl;
    CreateVertex(G);
}
int Judge(int n,MGraph G)
{
    if(n>=G.vexnum||n<0)
    {
        cout<<"输入错误，请重新输入"<<endl;
        return 0;
    }
    else
        return 1;
}
void DePath(MGraph &G)
{
    // 删除路径
    char ch1[100],ch2[100];
    char infomain[100][100];
    cout<<"请输入两个景点名称："<<endl;
    cin>>ch1>>ch2;
    FILE *fp=fopen("D:\\Users\\a1767\\CLionProjects\\Vex\\graph.txt","r");
    // 将文件信息写入infomain
    int i=0;
    while(!feof(fp))
    {
        fscanf(fp, "\n%[^\n]", infomain[i]);
        i++;
    }
    fclose(fp);
    // 将infomain中的景点信息删除
    for(int j=0;j<i;j++)
    {
        if(strstr(infomain[j],ch1)!=NULL&&strstr(infomain[j],ch2)!=NULL)// strstr函数在infomain[j]中查找ch3，若找到则返回ch3在infomain[j]中的首地址，否则返回NULL
        {
            for(int k=j;k<i-1;k++)
                strcpy(infomain[k],infomain[k+1]);
            i--;
            j--;// 因为删除了一行，所以j要减一，否则会跳过一行
        }
    }
    G.arcnum--;
    // 将infomain写入文件
    fp=fopen("D:\\Users\\a1767\\CLionProjects\\Vex\\graph.txt","w");
    fprintf(fp,"%d %d\n",G.vexnum,G.arcnum);// 写入顶点数和边数
    for(int j=1;j<i;j++)// 从第二行开始写入
        fprintf(fp,"%s\n",infomain[j]);
    fclose(fp);
    cout<<"删除成功！"<<endl;
    CreateVertex(G);
}
void menu(MGraph &G)
{
    int command;
    cout<<"------------欢迎来到北京林业大学"<<endl;
    for (int i = 0; i < G.vexnum;i++)
    {
        cout<<i+1<<"."<<G.vexs[i].name << endl;
    }
    cout<<"输入要执行的操作"<<endl;
    cout<<"-----------------------------"<<endl;
    cout<<"1 查看具体景点的信息"<<endl;
    cout<<"2 问路查询"<<endl;
    cout<<"3 增加一个景点"<<endl;
    cout<<"4 删除一个景点"<<endl;
    cout<<"5 查看所有景点"<<endl;
    cout<<"6 输出各景点之间的距离"<<endl;
    cout<<"7 修改一个已有景点的相关信息"<<endl;
    cout<<"8 增加一条新的路径"<<endl;
    cout<<"9 删除一条路径"<<endl;
    cout<<"-----------------------------"<<endl;
    do
    {
        cout << "输入要执行的操作：";
        cin >> command;
        switch (command)
        {
            case 1:
                int i;
                cout << "输入要查看哪个景点 ：";
                while(1)
                {
                    cin >> i;
                    if(Judge(i-1,G))
                        break;
                }
                i--;
                cout << G.vexs[i].name << " " << G.vexs[i].info << endl;
                break;
            case 2:
                cout << "输入你的位置和想去的地点的代号："<<endl;
                for (int i = 0; i < G.vexnum; i++)
                {
                    cout << i + 1 << "." << G.vexs[i].name << endl;
                }
                int loca, loca2;
                while(1)
                {
                    cin >> loca >> loca2;
                    if (Judge(loca, G) && Judge(loca2, G))
                        break;
                }
                loca--;
                loca2--;
                // 最短路径
                int P[100], D[100];
                Dijkstra2(G, loca, loca2, P, D);
                break;
            case 3:
                cout << "输入增加景点的信息" << endl;
                AddPlace(G);
                break;
            case 4 :
                cout << "输入删除景点的信息" << endl;
                char ch3[100];
                cin >> ch3;
                DePlace(G, ch3);
                break;
            case 5:
                for (int i = 0; i < G.vexnum; i++)
                {
                    cout << i + 1 << "." << G.vexs[i].name << endl;
                }
                break;
            case 6 :
                // 输出各景点之间的距离
                for (int i = 0; i < G.vexnum; i++)
                {
                    for (int j = 0; j < G.vexnum; j++)
                    {
                        if (G.arcs[i][j] != MAX_PATH)
                            cout << G.vexs[i].name << " " << G.vexs[j].name << " " << G.arcs[i][j] << endl;
                    }
                }
                break;
            case 7:
                cout<<"想要修改景点的代号："<<endl;
                for (int i = 0; i < G.vexnum; i++)
                {
                    cout << i + 1 << "." << G.vexs[i].name << endl;
                }
                int loca3;
                while(1)
                {
                    cin >> loca3;
                    if(Judge(loca3-1,G))
                        break;
                }
                loca3--;
                cout<<"输入修改后的景点介绍："<<endl;
                char ch2[100];
                cin >> ch2; CreateVertex(G);
                strcpy(G.vexs[loca3].info, ch2);
                cout<<"修改成功！"<<endl;
                break;
            case 8:
                cout<<"输入增加路径的信息(景点名称和路径长度)"<<endl;
                AddPath(G);
                break;
            case 9:
                cout<<"输入删除路径的信息"<<endl;
                DePath(G);
                break;
            default:
                cout << "输入错误" << endl;
                break;
        }
    } while (command != 0);
}