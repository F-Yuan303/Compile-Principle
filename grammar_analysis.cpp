#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

typedef struct P{
    string key;           // 产生式左部
    vector<string> value;  // 产生式右部
    string sentence;//产生式子
    int count;          // 几组规则
}P;
 typedef struct G{
    vector<string> vn ;     // 非终结符集合
    vector<string> vt ;     // 终结符集合
    P   produce[100];      // 产生式
    string start;     // 开始符号
    int pcount ;
 }G;
G Grammer;

void read_file(){
    Grammer.pcount = 0;
    string str, str1;
    char c;
    fstream bnf;
    bool flag;
    int index_now = 0;
    bnf.open("BNF.txt");
    while (getline(bnf, str)){
        Grammer.produce[Grammer.pcount].sentence = str;
        
        flag = 1;
        for (int i=0; i<=str.size(); i++){
            if (str[i] != ' ' && str[i] != '\0')
                str1 += str[i];
            else {
                //cout << str1 << "         ";
                if (str1 != "" && str1 == "->")
                    flag = 0;
                else if (str1 != "" && flag == 1)
                    Grammer.produce[Grammer.pcount].key = str1;
                else if (str1 != "" && flag == 0)
                    Grammer.produce[Grammer.pcount].value.push_back(str1);
                str1 = "";
            }
        }
        //cout << Grammer.produce[Grammer.pcount].key << endl;
        Grammer.pcount ++;
    }
    bnf.close();
    for (int i=0; i< Grammer.pcount; i++){
        cout << Grammer.produce[i].key << ":   ";
        for (int j=0; j<Grammer.produce[i].value.size(); j++)
            cout << Grammer.produce[i].value[j] << "  *  ";
        cout << endl;
    }
}




void first_list(){

}

void follow_list(){

}








int main(){
    read_file();
    
    return 0;
}