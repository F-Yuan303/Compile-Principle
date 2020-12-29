#include<iostream>
#include<fstream>
#include<vector>
#include<set>
using namespace std;

typedef struct P{
    string key;           // 产生式左部
    vector<vector<string> > value;  // 产生式右部
    //string sentence;     //产生式子
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

void read_file(){        //读入数据，生成非终结符
    Grammer.pcount = 0;
    string str, str1, temp_key;
    char c;
    fstream bnf;
    bool flag;
    vector<string> tmp_str;
    int index_now = 0;
    bnf.open("BNF.txt");
    while (getline(bnf, str)){
        //Grammer.produce[Grammer.pcount].sentence = str; 
        Grammer.produce[Grammer.pcount].count = 1;
        flag = 1;
        for (int i=0; i<=str.size(); i++){
            if (str[i] != ' ' && str[i] != '\0')
                str1 += str[i];
            else {
                if (str1 != "" && str1 == "->")
                    flag = 0;
                else if (str1 != "" && str1 == "|"){  //处理多条规则
                    Grammer.produce[Grammer.pcount].count++;
                    Grammer.produce[Grammer.pcount].value.push_back(tmp_str);
                    tmp_str.clear();
                    str1 = "";
                    continue;
                    /*
                    str1 = "";
                    temp_key = Grammer.produce[Grammer.pcount].key;
                    Grammer.pcount ++;
                    Grammer.produce[Grammer.pcount].key = temp_key;
                    continue;
                    */
                }
                else if (str1 != "" && flag == 1){
                    Grammer.produce[Grammer.pcount].key = str1;
                    Grammer.vn.push_back(str1);
                }
                else if (str1 != "" && flag == 0)
                    tmp_str.push_back(str1);
                    //Grammer.produce[Grammer.pcount].value.push_back(str1);
                str1 = "";
            }
        }
        Grammer.produce[Grammer.pcount].value.push_back(tmp_str);
        tmp_str.clear();
        Grammer.pcount ++;
    }
    bnf.close();
    Grammer.start = Grammer.produce[0].key;
}

void make_vt(){                     //生成终结符
    set<string> vt;
    int flag;
    for (int i=0; i<Grammer.pcount; i++){
        for (int j=0; j<Grammer.produce[i].value.size(); j++){
            for (int l=0; l<Grammer.produce[i].value[j].size(); l++){
                flag = 1;
                for (int k=0; k<Grammer.vn.size(); k++){
                    if (Grammer.produce[i].value[j][l] == Grammer.vn[k])
                        flag = 0;
                }
                if (flag == 1)
                    vt.insert(Grammer.produce[i].value[j][l]);
            }
        }
    }
    vt.erase("|");
    set<string>::iterator it;
    for (it = vt.begin(); it!=vt.end(); it++)
        Grammer.vt.push_back(*it);
} 
/*
void eliminate(){
    for(int i=0; i<Grammer.pcount; i++){
        int j = 0;
        for(; j<i; j++){
            // 将间接左递归转换成直接左递归
            // 扫描Ai的开始符号，一定要是非终结符
            int k;
            for(k=0; k<Grammer.produce[i].count; k++){
                string i_start = Grammer.produce[i].value[k];
                //printf("%c\n",start);
                if(i_start==g.p[j].key){
                    // 满足 Ai->Ajr
                    char tmp[255];
                    char fiel[255];
                    strcpy(fiel,&g.p[i].value[k][1]);
  
                    strcpy(tmp,g.p[j].value[0]);
                    strcpy(g.p[i].value[k],strcat(tmp,fiel));
                    printf("%d %s\n",k,g.p[i].value[k]);
                    int m;
                    for(m=1;m<g.p[j].count;m++){
                        strcpy(tmp,g.p[j].value[m]);
                        g.p[i].value[g.p[i].count] = (char *) malloc(sizeof(char)*255);
                        strcpy(g.p[i].value[g.p[i].count],strcat(tmp,fiel));
                        printf("%d %s\n",g.p[i].count,g.p[i].value[g.p[i].count]);
                        g.p[i].count++;
                    }
                }
            }
        }
    }
}
*/





int main(){
    read_file();
    make_vt();
    
    for (int i=0; i< Grammer.pcount; i++){
        cout << Grammer.produce[i].key << ":  (" << Grammer.produce[i].count << ")  ";
        for (int j=0; j<Grammer.produce[i].value.size(); j++){
            for (int k=0; k<Grammer.produce[i].value[j].size(); k++){
                cout << Grammer.produce[i].value[j][k] << "  ";
            }
            cout << " | ";
        }
        cout << endl;
    }
    /*
    for (int i=0; i<Grammer.vn.size(); i++)
        cout << Grammer.vn[i] << "  ";
    cout << endl << endl;
    */
    for (int i=0; i<Grammer.vt.size(); i++)
        cout << Grammer.vt[i] << "  ";
    cout << endl;
    return 0;
}

/*
for(i=0;i<n;i++){
          int j = 0;
          for(;j<i;j++){
              // 将间接左递归转换成直接左递归
              // 扫描Ai的开始符号，一定要是非终结符
              int k;
              for(k=0;k<g.p[i].count;k++){
                  char i_start = g.p[i].value[k][0];
                 //printf("%c\n",start);
                  if(i_start==g.p[j].key){
                      // 满足 Ai->Ajr
                      char tmp[255];
                      char fiel[255];
                      strcpy(fiel,&g.p[i].value[k][1]);
  
                      strcpy(tmp,g.p[j].value[0]);
                      strcpy(g.p[i].value[k],strcat(tmp,fiel));
                      printf("%d %s\n",k,g.p[i].value[k]);
                      int m;
                      for(m=1;m<g.p[j].count;m++){
                          strcpy(tmp,g.p[j].value[m]);
                          g.p[i].value[g.p[i].count] = (char *) malloc(sizeof(char)*255);
                          strcpy(g.p[i].value[g.p[i].count],strcat(tmp,fiel));
                          printf("%d %s\n",g.p[i].count,g.p[i].value[g.p[i].count]);
                          g.p[i].count++;
  
                      }
                  }
  
              }
          }
          // 消除直接左递归
          // 扫描Pi.key 为产生式右部的所有产生式
          for(j=0;j<g.p[i].count;j++){
              char * pivj = g.p[i].value[j];
              if(g.p[i].key == pivj[0]){
                 // 存在直接左递归
                 int m;
                  for(m=0;m<g.p[i].count;m++){
                      if(m!=j){
                          // A->ρ1A'|ρ2A'|ρ3A'    ρρσσαα
                          char aci[2] = {g.p[i].key-32,'\0'};
                          strcat(g.p[i].value[m],aci);         // 这里A'直接已A的ASCII码值减32表示
                      }else{
                          // A'->α1A'|α2A'|....|ε
                          g.p[g.pcount].key = g.p[i].key-32;
                         g.p[g.pcount].value[0] = (char *) malloc(sizeof(char)*255);
                         strcpy(g.p[g.pcount].value[0],&pivj[1]);
                         char aci[2] = {g.p[g.pcount].key,'\0'};
                         strcat(g.p[g.pcount].value[0],aci);
                         g.p[g.pcount].value[1] = (char *) malloc(sizeof(char)*255);
                         strcpy(g.p[g.pcount].value[1],"kong");
                         g.p[g.pcount].count = 2;
                         g.p[i].value[j] = NULL;
                         g.pcount++;
                     }
                 }
                 break;
             }
         }
 
     }*/