#include<iostream>
#include<fstream>
#include<vector>
#include<set>
using namespace std;

typedef struct P{
    string key;           // 产生式左部
    vector<string> value;  // 产生式右部
    //string sentence;     //产生式子
    //int count;          // 几组规则
}P;
 typedef struct G{
    vector<string> vn ;     // 非终结符集合
    vector<string> vt ;     // 终结符集合
    P   produce[100];      // 产生式
    string start;     // 开始符号
    int pcount ;
 }G;
G Grammer;

void read_file(){        //读入数据，生成非终结符
    Grammer.pcount = 0;
    string str, str1, temp_key;
    char c;
    fstream bnf;
    bool flag;
    int index_now = 0;
    bnf.open("BNF.txt");
    while (getline(bnf, str)){
        //Grammer.produce[Grammer.pcount].sentence = str; 
        flag = 1;
        for (int i=0; i<=str.size(); i++){
            if (str[i] != ' ' && str[i] != '\0')
                str1 += str[i];
            else {
                if (str1 != "" && str1 == "->")
                    flag = 0;
                /*
                else if (str1 != "" && str1 == "|"){  //处理多条规则
                    str1 = "";
                    temp_key = Grammer.produce[Grammer.pcount].key;
                    Grammer.pcount ++;
                    Grammer.produce[Grammer.pcount].key = temp_key;
                    continue;
                }
                */
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
    Grammer.start = Grammer.produce[0].key;
}

void make_vt(){         //生成终结符
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
    set<string>::iterator it;
    for (it = vt.begin(); it!=vt.end(); it++)
        Grammer.vt.push_back(*it);
} 






void first_list(){

}

void follow_list(){

}








int main(){
    read_file();
    make_vt();
    
    for (int i=0; i< Grammer.pcount; i++){
        cout << Grammer.produce[i].key << ":   ";
        for (int j=0; j<Grammer.produce[i].value.size(); j++)
            cout << Grammer.produce[i].value[j] << "  ";
        cout << endl;
    }
    /*
    for (int i=0; i<Grammer.vn.size(); i++)
        cout << Grammer.vn[i] << "  ";
    cout << endl << endl;
    
    for (int i=0; i<Grammer.vt.size(); i++)
        cout << Grammer.vt[i] << "  ";
    */
    return 0;
}