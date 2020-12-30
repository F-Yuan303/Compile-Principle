#include<iostream>
#include<fstream>
#include<vector>
#include<set>
using namespace std;

typedef struct P{
    string key;           // 产生式左部
    vector<string> value;  // 产生式右部
    int doc;          // 点的位置
}P;
typedef struct G{
    vector<string> vn ;     // 非终结符集合
    vector<string> vt ;     // 终结符集合
    P   produce[100];      // 产生式
    string start;           // 开始符号
    int pcount ;
}G;
typedef struct Project{
    vector<P> produce;
}Project;
G Grammer;                           //文法
vector<Project> project_set;            //项目集


void read_file(){                                  //读入数据，生成非终结符
    Grammer.pcount = 1;
    string str, str1, temp_key;
    char c;
    fstream bnf;
    bool flag;
    bnf.open("BNF_source.txt");
    while (getline(bnf, str)){
        //Grammer.produce[Grammer.pcount].sentence = str; 
        //Grammer.produce[Grammer.pcount].count = 1;
        flag = 1;
        for (int i=0; i<=str.size(); i++){
            if (str[i] != ' ' && str[i] != '\0')
                str1 += str[i];
            else {
                if (str1 != "" && str1 == "->")
                    flag = 0;
                else if (str1 != "" && str1 == "|"){  //处理多条规则
                    str1 = "";
                    temp_key = Grammer.produce[Grammer.pcount].key;
                    Grammer.pcount ++;
                    Grammer.produce[Grammer.pcount].key = temp_key;
                    continue;
                }
                else if (str1 != "" && flag == 1){
                    Grammer.produce[Grammer.pcount].key = str1;
                    Grammer.vn.push_back(str1);
                }
                else if (str1 != "" && flag == 0)
                    Grammer.produce[Grammer.pcount].value.push_back(str1);
                str1 = "";
            }
        }
        Grammer.pcount ++;
    }
    bnf.close();
    Grammer.start = Grammer.produce[1].key;
    Grammer.produce[0].key = "start";
    Grammer.produce[0].value.push_back(Grammer.produce[1].key);
}

void make_vt(){                                       //生成终结符
    set<string> vt;
    int flag;
    for (int i=0; i<Grammer.pcount; i++){
        for (int j=0; j<Grammer.produce[i].value.size(); j++){
                flag = 1;
                for (int k=0; k<Grammer.vn.size(); k++){
                    if (Grammer.produce[i].value[j] == Grammer.vn[k])
                        flag = 0;
                }
                if (flag == 1)
                    vt.insert(Grammer.produce[i].value[j]);
        }
    }
    vt.erase("|");
    set<string>::iterator it;
    for (it = vt.begin(); it!=vt.end(); it++)
        Grammer.vt.push_back(*it);
}


/*bool if_produce_exit(){                  //判断产生式是否存在


}
*/

bool if_projectset_exit(struct Project layer){               //判断项目集是否存在
    int flag = 0;
    P tmp;
    for (int i=0; i<project_set.size(); i++){
        flag = 0;
        if (project_set[i].produce.size() != layer.produce.size())
            continue;
        else{
            for (int j=0; j<project_set[i].produce.size(); j++){
                tmp = project_set[i].produce[j];
                for (int k=0; k<layer.produce.size(); k++){
                    if (tmp.key == layer.produce[k].key && tmp.value == layer.produce[k].value)
                        flag++;
                }

            }
            if (flag == project_set[i].produce.size())
            return true;
        }
    }
    return false;
}




void project_set_create(){                      //构建项目集
    Project tmp_p;
    bool flag = true;                  //用于判断项目集是否生成完成
    //初始化项目集P1
    for (int i=0; i<Grammer.pcount; i++){              
        tmp_p.produce.push_back(Grammer.produce[i]);
    }
    for (int i=0; i<tmp_p.produce.size(); i++){
        tmp_p.produce[i].value.insert(tmp_p.produce[i].value.begin(), ".");
        tmp_p.produce[i].doc = 0;
    }
    project_set.push_back(tmp_p);
    /*
    for (int i=0; i<project_set.size(); i++)
        for (int j=0; j<project_set[i].produce.size(); j++){
            cout << project_set[i].produce[j].key << ":   ";
            for (int k=0; k<project_set[i].produce[j].value.size(); k++){
            cout << project_set[i].produce[j].value[k] << "  ";
        }
        cout << endl; 
    }
    */

    //构建剩余项目集
    int tmp_layer = 0;          //表示第几个项目集
    int doc_locate;
    int produce_locate = 0;     //在当前项目集已扫描到的位置
    P tmp_produce;
    tmp_p.produce.clear();
    while (flag){               //当可以产生新项目集时，循环继续
        if (produce_locate == project_set[tmp_layer].produce.size()){  //但扫描完当前项目集时，进行下一个项目集的扫描
            tmp_p.produce.clear();  //存放当前新产生的项目集
            tmp_layer++;
            produce_locate = 0;
        }
        for (int i=produce_locate; i<project_set[tmp_layer].produce.size(); i++){  //分析某一个项目集中的每一句产生式子
            /*
            for (int j=0; j<project_set[tmp_layer].produce[i].value.size(); j++){  //寻找当前产生式子点的位置
                if (project_set[tmp_layer].produce[i].value[j] == "."){
                        doc_locate = j;
                }
            }
            */
            doc_locate = project_set[tmp_layer].produce[i].doc;
            //进行分析
            for (int j=0; j< Grammer.vn.size(); j++)
                if (project_set[tmp_layer].produce[i].value[doc_locate + 1] == Grammer.vn[j]){ //当点的下一个为非终结符
                    tmp_produce.key = project_set[tmp_layer].produce[i].key;
                    for (int k=0; k<doc_locate; k++)
                        tmp_produce.value.push_back(project_set[tmp_layer].produce[i].value[k]);
                    tmp_produce.value.push_back(project_set[tmp_layer].produce[i].value[doc_locate+1]);
                    tmp_produce.value.push_back(".");
                    tmp_produce.doc = doc_locate + 1;
                    for (int k=doc_locate+2; k<project_set[tmp_layer].produce[i].value.size(); k++)
                        tmp_produce.value.push_back(project_set[tmp_layer].produce[i].value[k]);
                    tmp_p.produce.push_back(tmp_produce);   //添加当前产生式
                    //从第一个项目集中寻找并添加这个非终结符生成新的产生式子
                    int line = 0;
                    int total = tmp_p.produce.size();
                    while (line <= total){   //遍历这个新项目集
                        for (int k=0; k< Grammer.vn.size(); k++){
                            if (tmp_p.produce[line].value[tmp_p.produce[line].doc+1] == Grammer.vn[k]){
                                for (int m=0; m<project_set[0].produce.size(); m++){
                                    /*
                                    if (project_set[0].produce[m].key == Grammer.vn[k]){       //找到后加入到项目集中
                                        tmp_p.produce.push_back(project_set[0].produce[m]);
                                    }
                                    */
                                }
                            }
                        }
                        total = tmp_p.produce.size();
                        line++;
                    }
                    
                    tmp_produce.value.clear();                   //清空暂时产生式

                    //在当前项目集中往下遍历看是否还有需要添加的
                    for (int k=i+1; k<project_set[tmp_layer].produce.size(); k++){
                        doc_locate = project_set[tmp_layer].produce[k].doc;
                        if (project_set[tmp_layer].produce[k].value[doc_locate+1] == Grammer.vn[j]){
                            tmp_produce.key = project_set[tmp_layer].produce[k].key;
                            for (int n=0; n<doc_locate; n++)
                                tmp_produce.value.push_back(project_set[tmp_layer].produce[k].value[n]);
                            tmp_produce.value.push_back(project_set[tmp_layer].produce[k].value[doc_locate+1]);
                            tmp_produce.value.push_back(".");
                            tmp_produce.doc = doc_locate + 1;
                            for (int n=doc_locate+2; n<project_set[tmp_layer].produce[k].value.size(); n++)
                                tmp_produce.value.push_back(project_set[tmp_layer].produce[k].value[n]);
                            tmp_p.produce.push_back(tmp_produce);   //添加当前产生式子
                            //从第一个项目集中寻找并添加这个非终结符生成新的产生式子（需要判断是否存在重复）
                            string tmp_key = tmp_produce.key;
                            for (int n=0; n<project_set[0].produce.size(); n++){


                                        
                            }
                        tmp_produce.value.clear();                   //清空暂时产生式
                        }
                    }
                    //判断这个新生成的项目集是否已存在，若不存在，加入到项目集集合中
        if (!if_projectset_exit(tmp_p))
            project_set.push_back(tmp_p);
        tmp_p.produce.clear();
                }
        }
        //tmp_layer++;
        //if (tmp_layer > project_set.size()){
            flag = false;
        //}

    }
}



int main(){
    read_file();
    make_vt();
    project_set_create();
    /*
    for (int i=0; i< Grammer.pcount; i++){
        cout << Grammer.produce[i].key << ":  ";
        for (int j=0; j<Grammer.produce[i].value.size(); j++){
            cout << Grammer.produce[i].value[j] << "  ";
        }
        cout << endl;
    }*/
    
    for (int i=0; i<Grammer.vn.size(); i++)
        cout << Grammer.vn[i] << "  ";
    cout << endl << endl;
    
    for (int i=0; i<Grammer.vt.size(); i++)
        cout << Grammer.vt[i] << "  ";
    cout << endl;
    

   for (int i=0; i<project_set.size(); i++){
        cout << "----------------------" << endl;
        cout << "       项目集 " << i+1 << endl;
        cout << "----------------------" << endl;
        for (int j=0; j<project_set[i].produce.size(); j++){
            cout << project_set[i].produce[j].key << ":  ";
            for (int k=0; k<project_set[i].produce[j].value.size(); k++)
                cout << project_set[i].produce[j].value[k] << "  ";
            cout << endl;
        } 

        cout << endl;
   }

   cout << "done!";
    return 0;
}