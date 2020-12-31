#include<iostream>
#include<map>
#include<utility>
#include<vector>
#include<string>
using namespace std;


typedef struct P {
    string key;           // 产生式左部
    vector<vector<string> > value;  // 产生式右部
    int count;          // 几组规则
}P;
typedef struct G {
    vector<string> vn;     // 非终结符集合
    vector<string> vt;     // 终结符集合
    P   produce[100];      // 产生式
    string start;     // 开始符号
    int pcount;
}G;
G Grammer;
vector<vector<string> > first_set(5);
vector<vector<string> > follow_set(5);

map<pair<string, string>, vector<string> > forecast_table;      //预测分析表



void create_forecast_table(){        //构建预测分析表
    //pair<string, string> tmp_index;
    int line;     //行
    int row;      //列
    int tmp_vn;
    int vt_or_vn;                  //0是vt，1是vn
    vector<string> e;
    e.push_back("empty");
    for (int i=0; i<Grammer.pcount; i++){                   //对每一组产生式进行扫描
        
        for (int k=0; k<Grammer.vn.size(); k++){
            if (Grammer.produce[i].key == Grammer.vn[k]){        
                    line = k;                               //获取行
                    //cout << Grammer.produce[i].key;
                }
        }
        //cout << Grammer.produce[i].value.size() << endl;
        for (int k=0; k<Grammer.produce[i].value.size(); k++){

            string first = Grammer.produce[i].value[k][0];   //获取右部的第一个字符串
            //cout << first << endl;
            for (int j=0; j<Grammer.vt.size(); j++){
                if (first == Grammer.vt[j]){                  //若第一个字符串是终结符
                    row = j;                                  //获取列
                    vt_or_vn = 0;
                }
            }
            for (int j=0; j<Grammer.vn.size(); j++){
                if (first == Grammer.vn[j]){                 //若第一个字符串是终结符
                    tmp_vn = j;                               //获取非终结符的位置
                    vt_or_vn = 1;
                }
            }


            if (vt_or_vn == 0){                               //若第一个字符串是终结符(包含空)
                if (Grammer.vt[row] == "empty"){              //若是空
                    for (int m=0; m<follow_set[line].size(); m++){
                        for (int n=0; n<Grammer.vt.size(); n++){
                            if (Grammer.vt[n] == follow_set[line][m]){
                                row = n;
                                pair<string, string> tmp_index = make_pair(Grammer.vn[line], Grammer.vt[row]);
                                forecast_table.insert(make_pair(tmp_index, e));
                            }
                        }
                    }
                }
                else{
                    pair<string, string> tmp_index = make_pair(Grammer.vn[line], Grammer.vt[row]);
                    forecast_table.insert(make_pair(tmp_index, Grammer.produce[i].value[k]));
                }
            }
            else{                                             //若第一个字符串是非终结符
                int if_exit_empty = 0;
                for (int m=0; m<first_set[tmp_vn].size(); m++){    //判断first集合中是否存在空
                    //cout << first_set[tmp_vn][m] << "   ";
                    if (first_set[tmp_vn][m] == "empty"){
                        //cout << "yes" << endl;
                        if_exit_empty = 1;
                    }
                }
                //cout << endl;
                if (if_exit_empty == 1){                                    //如果first集存在空
                    for (int m=0; m<first_set[tmp_vn].size(); m++){
                        for (int n=0; n<Grammer.vt.size(); n++){
                            if ((Grammer.vt[n] == first_set[tmp_vn][m]) && (first_set[tmp_vn][m] != "empty")){
                                row = n;
                                pair<string, string> tmp_index = make_pair(Grammer.vn[line], Grammer.vt[row]);
                                forecast_table.insert(make_pair(tmp_index, Grammer.produce[i].value[k]));
                            }
                        }
                    }

                    for (int m=0; m<follow_set[line].size(); m++){
                        for (int n=0; n<Grammer.vt.size(); n++){
                            if (Grammer.vt[n] == follow_set[line][m]){
                                row = n;
                                pair<string, string> tmp_index = make_pair(Grammer.vn[line], Grammer.vt[row]);
                                forecast_table.insert(make_pair(tmp_index, e));
                            }
                        }
                    }
                }
                else if (if_exit_empty == 0){                                         //如果first集不存在空
                    for (int m=0; m<first_set[tmp_vn].size(); m++){
                        for (int n=0; n<Grammer.vt.size(); n++){
                            if (Grammer.vt[n] == first_set[tmp_vn][m]){
                                row = n;
                                pair<string, string> tmp_index = make_pair(Grammer.vn[line], Grammer.vt[row]);
                                forecast_table.insert(make_pair(tmp_index, Grammer.produce[i].value[k]));
                            }
                        }
                    }
                }
            }
        }
    }
}




int main(){
    
    Grammer.start = "E";
    
    Grammer.vn.push_back("E");
    Grammer.vn.push_back("E'");
    Grammer.vn.push_back("T");
    Grammer.vn.push_back("T'");
    Grammer.vn.push_back("F");
    Grammer.vt.push_back("i");
    Grammer.vt.push_back("+");
    Grammer.vt.push_back("*");
    Grammer.vt.push_back("(");
    Grammer.vt.push_back(")");
    Grammer.vt.push_back("#");
    Grammer.vt.push_back("empty");
    
    Grammer.pcount = 5;
    
    Grammer.produce[0].key = "E";
    Grammer.produce[1].key = "E'";
    Grammer.produce[2].key = "T";
    Grammer.produce[3].key = "T'";
    Grammer.produce[4].key = "F";

    vector<string>  tmp;
    tmp.push_back("T");
    tmp.push_back("E'");
    
    Grammer.produce[0].value.push_back(tmp);
    tmp.clear();
    tmp.push_back("+");
    tmp.push_back("T");
    tmp.push_back("E'");
    
    Grammer.produce[1].value.push_back(tmp);
    tmp.clear();
    tmp.push_back("empty");
    Grammer.produce[1].value.push_back(tmp);
    tmp.clear();
    tmp.push_back("F");
    tmp.push_back("T'");
    Grammer.produce[2].value.push_back(tmp);
    tmp.clear();
    tmp.push_back("*");
    tmp.push_back("F");
    tmp.push_back("T'");
    Grammer.produce[3].value.push_back(tmp);
    tmp.clear();
    tmp.push_back("empty");
    Grammer.produce[3].value.push_back(tmp);
    tmp.clear();
    tmp.push_back("(");
    tmp.push_back("E");
    tmp.push_back(")");
    Grammer.produce[4].value.push_back(tmp);
    tmp.clear();
    tmp.push_back("i");
    Grammer.produce[4].value.push_back(tmp);
    tmp.clear();

    first_set[0].push_back("(");
    first_set[0].push_back("i");
    first_set[1].push_back("+");
    first_set[1].push_back("empty");
    first_set[2].push_back("(");
    first_set[2].push_back("i");
    first_set[3].push_back("*");
    first_set[3].push_back("empty");
    first_set[4].push_back("(");
    first_set[4].push_back("i");

    follow_set[0].push_back(")");
    follow_set[0].push_back("#");
    follow_set[1].push_back(")");
    follow_set[1].push_back("#");
    follow_set[2].push_back("+");
    follow_set[2].push_back(")");
    follow_set[2].push_back("#");
    follow_set[3].push_back("+");
    follow_set[3].push_back(")");
    follow_set[3].push_back("#");
    follow_set[4].push_back("*");
    follow_set[4].push_back("+");
    follow_set[4].push_back(")");
    follow_set[4].push_back("#");


    create_forecast_table();

    cout << " 行     列     产生式" << endl;
    map<pair<string, string>, vector<string> >::iterator it;
    
    for (it = forecast_table.begin(); it!=forecast_table.end(); it++){
        pair<string, string> p = it->first;
        cout << p.first << "      " << p.second << "     ";
        vector<string> v = it->second;
        cout << p.first << "->";
        for (int i=0; i<v.size(); i++)
            cout << v[i] << " ";
        cout << endl;
    }
    
    return 0;
}

