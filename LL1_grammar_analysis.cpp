#include<iostream>
#include<fstream>
#include<vector>
#include<set>
#include<map>
#include <string>
#include"LL1.h"

using namespace std;

typedef struct P {
    string key;           // 产生式左部
    vector<vector<string> > value;  // 产生式右部
    //string sentence;     //产生式子
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
vector<vector<string> > first_set;


/**********************读文件_start*************************/
void read_file() {        //读入数据，生成非终结符
    Grammer.pcount = 0;
    string str, str1, temp_key;
    char c;
    fstream bnf;
    bool flag;
    vector<string> tmp_str;
    int index_now = 0;
    bnf.open("BNF.txt");
    while (getline(bnf, str)) {
        //Grammer.produce[Grammer.pcount].sentence = str; 
        Grammer.produce[Grammer.pcount].count = 1;
        flag = 1;
        for (int i = 0; i <= str.size(); i++) {
            if (str[i] != ' ' && str[i] != '\0')
                str1 += str[i];
            else {
                if (str1 != "" && str1 == "->")
                    flag = 0;
                else if (str1 != "" && str1 == "|") {  //处理多条规则
                    Grammer.produce[Grammer.pcount].count++;
                    Grammer.produce[Grammer.pcount].value.push_back(tmp_str);
                    tmp_str.clear();
                    str1 = "";
                    continue;
                }
                else if (str1 != "" && flag == 1) {
                    Grammer.produce[Grammer.pcount].key = str1;
                    Grammer.vn.push_back(str1);
                }
                else if (str1 != "" && flag == 0)
                    tmp_str.push_back(str1);
                str1 = "";
            }
        }
        Grammer.produce[Grammer.pcount].value.push_back(tmp_str);
        tmp_str.clear();
        Grammer.pcount++;
    }
    bnf.close();
    Grammer.start = Grammer.produce[0].key;
}

void make_vt() {                                       //生成终结符
    set<string> vt;
    int flag;
    for (int i = 0; i < Grammer.pcount; i++) {
        for (int j = 0; j < Grammer.produce[i].value.size(); j++) {
            for (int m = 0; m < Grammer.produce[i].value[j].size(); m++) {
                flag = 1;
                for (int k = 0; k < Grammer.vn.size(); k++) {
                    if (Grammer.produce[i].value[j][m] == Grammer.vn[k])
                        flag = 0;
                }
                if (flag == 1)
                    vt.insert(Grammer.produce[i].value[j][m]);
            }
        }
    }
    vt.erase("|");
    set<string>::iterator it;
    for (it = vt.begin(); it != vt.end(); it++)
        Grammer.vt.push_back(*it);
}

void print_grammer()
{
    for (int i = 0; i < Grammer.pcount; i++) {
        cout << Grammer.produce[i].key << ": ";
        for (int j = 0; j < Grammer.produce[i].value.size(); j++) {
            for (int k = 0; k < Grammer.produce[i].value[j].size(); k++) {
                cout << Grammer.produce[i].value[j][k] << " ";
            }
            if (j < Grammer.produce[i].value.size() - 1)
                cout << "| ";
        }
        cout << endl;
    }
}
/*********************读文件_end******************************/


/********************消除左递归_start**********************/
int search_vn(string str)
{
    for (unsigned i = 0; i < Grammer.vn.size(); i++) {
        if (str == Grammer.vn[i])
            return i;
    }
    return -1;
}

void merge(vector<int> &direct)
{
    int num;
    vector<vector<string> > temp2;
    vector<string> temp1;
    bool judge;
    for (unsigned i = 0; i < Grammer.vn.size(); i++) {
        while (1) {
            judge = true;
            for (unsigned j = 0; j < Grammer.produce[i].value.size(); j++) {
                num = search_vn(Grammer.produce[i].value[j][0]);
                //如果是非终结符
                if (num != -1 && direct[num] != 1 && Grammer.produce[i].key != Grammer.produce[i].value[j][0]) {
                    judge = false;
                    vector<string>::iterator h = Grammer.produce[i].value[j].begin() + 1;
                    //Grammer.produce[i].value[j].erase(h);       //删除第一个单词
                    for (unsigned k = 0; k < Grammer.produce[num].value.size(); k++) {          //合并产生式
                        temp1.insert(temp1.end(), Grammer.produce[num].value[k].begin(), Grammer.produce[num].value[k].end());
                        temp1.insert(temp1.end(), h, Grammer.produce[i].value[j].end());
                        temp2.push_back(temp1);
                        temp1.clear();
                    }
                }
                else {
                    temp2.push_back(Grammer.produce[i].value[j]);
                }
                if (Grammer.produce[i].key == Grammer.produce[i].value[j][0]) {
                    direct[i] = 1;
                }
            }
            Grammer.produce[i].value = temp2;
            temp2.clear();
            if (judge == true)
                break;
        }
    }
}

void directly(vector<int> &direct)
{
    for (unsigned i = 0; i < Grammer.pcount; i++) {
        for (unsigned j = 0; j < Grammer.produce[i].value.size(); j++) {
            if (Grammer.produce[i].value[j][0] == Grammer.produce[i].key) {
                direct[i] = 1;
                break;
            }
        }
    }
}

void direct_left_recur(vector<int> direct)
{
    vector<vector<string> > temp1;      //P'的产生式右部
    vector<vector<string> > temp2;      //P的产生式右部
    vector<string> mer;                 //合并的临时变量
    string str;
    vector<string> empty;
    empty.push_back("empty");
    unsigned n = Grammer.vn.size();
    for (unsigned i = 0; i < n; i++) {
        if (direct[i] == 1) {
            for (unsigned j = 0; j < Grammer.produce[i].value.size(); j++) {
                str = Grammer.produce[i].key + "'";
                if (Grammer.produce[i].value[j][0] == Grammer.produce[i].key) {
                    vector<string>::iterator h = Grammer.produce[i].value[j].begin() + 1;
                    mer.insert(mer.end(), h, Grammer.produce[i].value[j].end());
                    mer.push_back(str);
                    temp1.push_back(mer);
                }
                else if (Grammer.produce[i].value[j].size() > 1 || (Grammer.produce[i].value[j].size() == 1 && Grammer.produce[i].value[j][0] != "empty")) {
                    mer = Grammer.produce[i].value[j];
                    mer.push_back(str);
                    temp2.push_back(mer);
                }
                else if (Grammer.produce[i].value[j].size() == 1 && Grammer.produce[i].value[j][0] == "empty") {
                    mer.push_back(str);
                    temp2.push_back(mer);
                }
                mer.clear();
            }
            Grammer.produce[i].value = temp2;
            Grammer.produce[Grammer.pcount].key = str;
            temp1.push_back(empty);
            Grammer.produce[Grammer.pcount].value = temp1;
            Grammer.vn.push_back(str);
            Grammer.pcount++;
            temp1.clear();
            temp2.clear();
        }
    }
}


void left_recur()
{
    vector<int> direct(Grammer.vn.size());  //直接左递归的非终结符
    directly(direct);
    
    //合并产生式
    merge(direct);

    for (unsigned i = 0; i < Grammer.pcount; i++) {
        if (direct[i] == 1)
            cout << Grammer.produce[i].key << ":" << direct[i] << endl;
    }

    cout << endl << "/********************************消除间接左递归后：***********************************/" << endl;
    print_grammer();
    direct_left_recur(direct);
    cout << endl << "/********************************消除直接左递归后：***********************************/" << endl;
    print_grammer();

}

/********************消除左递归_end**********************/

/********************提左因子_start**********************/

bool extract_left_factor()
{
    bool judge_over = true;
    vector<string> common_factor;
    int time;
    vector<vector<string> > new_val;               //存新的产生式
    vector<unsigned> del;
    vector<vector<string> > temp;
    unsigned m;
    int n = Grammer.vn.size();
    for (unsigned i = 0; i < n; i++) {
        for (unsigned j = 0; j < Grammer.produce[i].value.size(); j++) {
            time = 1;
            for (unsigned k = j + 1; k < Grammer.produce[i].value.size(); k++) {
                if (Grammer.produce[i].value[j][0] == Grammer.produce[i].value[k][0]) {
                    if (time == 1) {
                        judge_over = false;
                        time = 0;
                        common_factor.push_back(Grammer.produce[i].value[j][0]);
                        del.push_back(j);            //记得要清空
                        new_val.push_back(Grammer.produce[i].value[j]);
                    }
                    del.push_back(k);
                    new_val.push_back(Grammer.produce[i].value[k]);
                }
            }
            if (time == 0) {        //有公共左因子
                for (unsigned p = 0; p < new_val.size(); p++) {         //删除第一个单词
                    new_val[p].erase(new_val[p].begin());
                    if (new_val[p].size() == 0)
                        new_val[p].push_back("empty");
                }
                //添加新产生式
                Grammer.produce[Grammer.pcount].key = Grammer.produce[i].key + "_t";
                Grammer.produce[Grammer.pcount].value = new_val;
                Grammer.vn.push_back(Grammer.produce[Grammer.pcount].key);
                Grammer.pcount++;
                //删除旧产生式的有公共左因子的单词串

                m = 0;
                for (unsigned p = 0; p < Grammer.produce[i].value.size(); p++) {
                    if (m < del.size() && p == del[m]) {
                        m++;
                        continue;
                    }
                    else {
                        temp.push_back(Grammer.produce[i].value[p]);
                    }
                }
                Grammer.produce[i].value = temp;
                temp.clear();


                //再添加新的一个单词串到旧的产生式中
                common_factor.push_back(Grammer.produce[i].key + "_t");
                Grammer.produce[i].value.push_back(common_factor);
                common_factor.clear();
                //cout << endl;
                //cout << Grammer.produce[i].key << ":  ";
                //for (unsigned p1 = 0; p1 < Grammer.produce[i].value.size(); p1++) {
                //    for (unsigned p2 = 0; p2 < Grammer.produce[i].value[p1].size(); p2++) {
                //        cout << Grammer.produce[i].value[p1][p2] << " ";
                //    }
                //    cout << "|";
                //}
                //cout << endl;
                //cout << endl;
                //cout << Grammer.produce[i].key + "_t" << " :   ";
                //for (unsigned p1 = 0; p1 < new_val.size(); p1++) {
                //    for (unsigned p2 = 0; p2 < new_val[p1].size(); p2++) {
                //        cout << new_val[p1][p2] << " ";
                //    }
                //    cout << "|";
                //}


                del.clear();
                new_val.clear();
                break;
            }
        }
    }
    return judge_over;
}

void extract_left_factor_main()
{
    while (1) {
        if (extract_left_factor())
            break;
    }

    cout << endl << "/********************************提取左公共因子后：***********************************/" << endl;
    print_grammer();

}

/********************提左因子_end**********************/

/***********************first集_start********************/

bool judge_rm_empty(vector<string>& str)
{
    vector<string> temp;
    bool judge = false;
    for (unsigned i = 0; i < str.size(); i++) {
        if (str[i] == "empty") {
            judge = true;
            continue;
        }
        temp.push_back(str[i]);
    }
    str = temp;
    return judge;
}

vector<string> first(int i)
{
    int num;
    vector<string> str;
    vector<string> temp;
    for (unsigned j = 0; j < Grammer.produce[i].value.size(); j++) {
        num = search_vn(Grammer.produce[i].value[j][0]);
        if (num != -1) {
            for (unsigned k = 0; k < Grammer.produce[i].value[j].size(); k++) {
                temp = first(num);
                if (judge_rm_empty(temp)) {
                    str.insert(str.end(), temp.begin(), temp.end());
                    if (k == Grammer.produce[i].value[j].size() - 1) {
                        str.push_back("empty");
                        break;
                    }
                    num = search_vn(Grammer.produce[i].value[j][k + 1]);
                    if (num == -1)
                        break;
                }
                else {
                    str.insert(str.end(), temp.begin(), temp.end());
                    break;
                }
            }
        }
        else {
            str.push_back(Grammer.produce[i].value[j][0]);
        }
    }
    return str;
}

bool judge_exit(string str, vector<string> result)
{
    for (unsigned i = 0; i < result.size(); i++) {
        if (result[i] == str)
            return true;
    }
    return false;
}

void rm_rep()
{
    vector<string> result;
    for (unsigned i = 0; i < first_set.size(); i++) {
        for (unsigned j = 0; j < first_set[i].size(); j++) {
            if (judge_exit(first_set[i][j], result))
                continue;
            result.push_back(first_set[i][j]);
        }
        first_set[i] = result;
        result.clear();
    }
}

void first_main()
{
    
    for (unsigned i = 0; i < Grammer.vn.size(); i++) {
        first_set.push_back(first(i));
    }
    rm_rep();
    cout << endl << "/********************************first集：***********************************/" << endl;
    //cout << "first集如下：" << endl;
    for (unsigned i = 0; i < first_set.size(); i++) {
        cout << Grammer.vn[i] << ":  ";
        for (unsigned j = 0; j < first_set[i].size(); j++) {
            cout << first_set[i][j] << " ";
        }
        cout << endl;
    }
}

/***********************first集_end********************/

/***********************follow集_start********************/

vector<string>  first_part(vector<string> val)
{
    vector<string> first;
    int num;
    num = search_vn(val[0]);
    if (num == -1) { //非终结符
        first.push_back(val[0]);
        return first;
    }
    else {
        for (unsigned i = 0; i < val.size(); i++) {
            first.insert(first.end(), first_set[num].begin(), first_set[num].end());
            if (judge_rm_empty(first)) {
                if (i < val.size() - 1) {
                    num = search_vn(val[i + 1]);
                    if (num == -1) {
                        first.push_back(val[i + 1]);
                        return first;
                    }
                }
                else {
                    first.push_back("empty");
                }
            }
            else
                break;
        }
    }
    return first;
}


void rm_rep_follow(vector<vector<string> > &follow_set)
{
    vector<string> result;
    for (unsigned i = 0; i < follow_set.size(); i++) {
        for (unsigned j = 0; j < follow_set[i].size(); j++) {
            if (judge_exit(follow_set[i][j], result))
                continue;
            result.push_back(follow_set[i][j]);
        }
        follow_set[i] = result;
        result.clear();
    }
}

bool update_num_follow(vector<vector<string> > &follow_set, vector<int> &num_follow)
{
    bool judge = true;
    for (unsigned i = 0; i < follow_set.size(); i++) {
        if (follow_set[i].size() > num_follow[i]) {
            num_follow[i] = follow_set[i].size();
            judge = false;
        }
    }
    return judge;
}

bool follow(vector<vector<string> > &follow_set, vector<int> &num_follow)
{
    vector<string> temp_follow;
    vector<string> temp1;
    vector<string> temp2;
    int num;
    int num_key;
    bool judge_over = true;
    for (unsigned i = 0; i < Grammer.pcount; i++) {
        for (unsigned j = 0; j < Grammer.produce[i].value.size(); j++) {
            for (unsigned k = 0; k < Grammer.produce[i].value[j].size(); k++) {
                num = search_vn(Grammer.produce[i].value[j][k]);
                if ( num != -1) {  //找到一个非终结符
                    if (k < Grammer.produce[i].value[j].size() - 1) {      //非最后
                        temp1.insert(temp1.end(), Grammer.produce[i].value[j].begin() + k + 1, Grammer.produce[i].value[j].end());  //提取非终结符后的单词串
                        temp2 = first_part(temp1);              //单词串的first集
                        temp1.clear();

                        bool t;
                        t = judge_rm_empty(temp2);
                        if (t) {
                            num_key = search_vn(Grammer.produce[i].key);
                            follow_set[num].insert(follow_set[num].end(), follow_set[num_key].begin(), follow_set[num_key].end());
                        }
                        else {
                            follow_set[num].insert(follow_set[num].end(), temp2.begin(), temp2.end());
                        }
                        temp2.clear();
                    }
                    else {      //最后
                        num_key = search_vn(Grammer.produce[i].key);
                        follow_set[num].insert(follow_set[num].end(), follow_set[num_key].begin(), follow_set[num_key].end());
                    }
                }
            }
        }
    }
    rm_rep_follow(follow_set);
    
    return update_num_follow(follow_set, num_follow);
}


vector<vector<string> > follow_main()
{
    vector<vector<string> > follow_set(Grammer.pcount);
    vector<int> num_follow(Grammer.pcount);
    num_follow[0] = 1;
    follow_set[0].push_back("#");       //开始符号添加 # 号
    while(1) {
        
        if (follow(follow_set, num_follow)) {
            cout << endl << "/********************************follow集：***********************************/" << endl;
            //cout << endl << "follow集如下" << endl;
            for (unsigned i = 0; i < follow_set.size(); i++) {
                cout << Grammer.produce[i].key << ":     ";
                for (unsigned j = 0; j < follow_set[i].size(); j++) {
                    cout << follow_set[i][j] << " ";
                }
                cout << endl;
            }
            break;
        }
    }
    return follow_set;
}
/***********************follow集_end********************/

/***********************判断是否为LL1文法_start********************/

bool judge_LL1(vector<vector<string> > follow_set)
{
    bool judge = true;
    vector<string> temp1;// 记得清空
    vector<string> temp2;// 记得清空

    //条件二
    for (unsigned i = 0; i < Grammer.pcount; i++) {
        for (unsigned j = 0; j < Grammer.produce[i].value.size(); j++) {
            temp1 = first_part(Grammer.produce[i].value[j]);
            temp2.insert(temp2.end(),temp1.begin(),temp1.end());
        }

        temp1.clear();
        for (unsigned j = 0; j < temp2.size(); j++) {
            temp1.push_back(temp2[j]);
            if (judge_exit(temp2[j], temp1)) {
                //judge = false;
                //break;
                return false;
            }
        }
        temp1.clear();
        temp2.clear();
    }
    //return judge;

    //条件三
    //vector<string> empty;
    //empty.push_back("empry");
    for (unsigned i = 0; i < first_set.size(); i++) {
        for (unsigned j = 0; j < first_set[i].size(); j++) {
            if (first_set[i][j] == "empty") {
                temp1 = first_set[i];
                temp1.insert(temp1.end(), follow_set[i].begin(), follow_set[i].end());
                for (unsigned k = 0; k < temp1.size(); k++) {
                    temp2.push_back(temp1[k]);
                    if (judge_exit(temp1[k], temp2))
                        return false;
                }
            }
            temp1.clear();
            temp2.clear();
        }
    }
    return true;
}

/***********************判断是否为LL1文法_end********************/


/***********************构建预测分析表_start********************/


map<pair<string, string>, vector<string> > forecast_table;      //预测分析表



void create_forecast_table(vector<vector<string> > follow_set) {        //构建预测分析表
    //pair<string, string> tmp_index;
    int line;     //行
    int row;      //列
    int tmp_vn;
    int vt_or_vn;                  //0是vt，1是vn
    vector<string> e;
    vector<string> s;
    e.push_back("empty");
    s.push_back("synch");
    for (int i = 0; i < Grammer.pcount; i++) {                   //对每一组产生式进行扫描

        for (int k = 0; k < Grammer.vn.size(); k++) {
            if (Grammer.produce[i].key == Grammer.vn[k]) {
                line = k;                               //获取行
                //cout << Grammer.produce[i].key;
            }
        }
        //cout << Grammer.produce[i].value.size() << endl;
        for (int k = 0; k < Grammer.produce[i].value.size(); k++) {

            string first = Grammer.produce[i].value[k][0];   //获取右部的第一个字符串
            cout << first << endl;
            for (int j = 0; j < Grammer.vt.size(); j++) {
                if (first == Grammer.vt[j]) {                  //若第一个字符串是终结符
                    row = j;                                  //获取列
                    vt_or_vn = 0;
                }
            }
            for (int j = 0; j < Grammer.vn.size(); j++) {
                if (first == Grammer.vn[j]) {                 //若第一个字符串是非终结符
                    tmp_vn = j;                               //获取非终结符的位置
                    vt_or_vn = 1;
                }
            }
            cout << vt_or_vn << endl;

            if (vt_or_vn == 0) {                               //若第一个字符串是终结符(包含空)
                if (Grammer.vt[row] == "empty") {              //若是空
                    for (int m = 0; m < follow_set[line].size(); m++) {
                        for (int n = 0; n < Grammer.vt.size(); n++) {
                            if (Grammer.vt[n] == follow_set[line][m]) {
                                row = n;
                                pair<string, string> tmp_index = make_pair(Grammer.vn[line], Grammer.vt[row]);
                                cout << Grammer.vt[n] << endl;
                                forecast_table.insert(make_pair(tmp_index, e));
                            }
                        }
                    }
                }
                else {
                    pair<string, string> tmp_index = make_pair(Grammer.vn[line], Grammer.vt[row]);
                    forecast_table.insert(make_pair(tmp_index, Grammer.produce[i].value[k]));
                }
            }
            else {                                             //若第一个字符串是非终结符
                int if_exit_empty = 0;
                for (int m = 0; m < first_set[tmp_vn].size(); m++) { //判断first集合中是否存在空
                    //cout << first_set[tmp_vn][m] << "   ";
                    if (first_set[tmp_vn][m] == "empty") {
                        //cout << "yes" << endl;
                        if_exit_empty = 1;
                    }
                }
                //cout << endl;
                if (if_exit_empty == 1) {                                    //如果first集存在空
                    for (int m = 0; m < first_set[tmp_vn].size(); m++) {
                        for (int n = 0; n < Grammer.vt.size(); n++) {
                            if ((Grammer.vt[n] == first_set[tmp_vn][m]) && (first_set[tmp_vn][m] != "empty")) {
                                row = n;
                                pair<string, string> tmp_index = make_pair(Grammer.vn[line], Grammer.vt[row]);
                                forecast_table.insert(make_pair(tmp_index, Grammer.produce[i].value[k]));
                            }
                        }
                    }

                    for (int m = 0; m < follow_set[line].size(); m++) {
                        for (int n = 0; n < Grammer.vt.size(); n++) {
                            if (Grammer.vt[n] == follow_set[line][m]) {
                                row = n;
                                pair<string, string> tmp_index = make_pair(Grammer.vn[line], Grammer.vt[row]);
                                forecast_table.insert(make_pair(tmp_index, e));
                            }
                        }
                    }
                }
                else if (if_exit_empty == 0) {                                         //如果first集不存在空
                    for (int m = 0; m < first_set[tmp_vn].size(); m++) {
                        for (int n = 0; n < Grammer.vt.size(); n++) {
                            if (Grammer.vt[n] == first_set[tmp_vn][m]) {
                                row = n;
                                pair<string, string> tmp_index = make_pair(Grammer.vn[line], Grammer.vt[row]);
                                forecast_table.insert(make_pair(tmp_index, Grammer.produce[i].value[k]));
                            }
                        }
                    }
                    for (int m = 0; m < follow_set[tmp_vn].size(); m++){             //错误处理
                        for (int n = 0; n < Grammer.vt.size(); n++) {
                            if (Grammer.vt[n] == first_set[tmp_vn][m]) {
                                row = n;
                                pair<string, string> tmp_index = make_pair(Grammer.vn[line], Grammer.vt[row]);
                                forecast_table.insert(make_pair(tmp_index, s));
                            }
                        }
                    }
                }
            }
        }
    }
}

/***********************构建预测分析表_end********************/


void LL1() {
    read_file();        //读文件
    make_vt();
    print_grammer();       //输出文法
    left_recur();           //消除左递归
    extract_left_factor_main(); //提左因子
    first_main();           //first集

    vector<vector<string> > follow_set(Grammer.pcount);

    follow_set = follow_main();     //follow集

    //cout << follow_set.size();
    if (judge_LL1(follow_set))
        cout << "不是LL1文法" << endl;
    else
        cout << "是LL1文法" << endl;



    create_forecast_table(follow_set);

    for (unsigned i = 0; i < Grammer.vn.size(); i++) {
        cout << Grammer.vn[i] << " ";
    }
    cout << endl;
    cout << endl;
    cout << endl;
    for (unsigned i = 0; i < Grammer.vt.size(); i++)
        cout << Grammer.vt[i] << " ";
    cout << endl;
    cout << endl;
    cout << "--------------------" << endl;
    cout << "|     预测分析表      |" << endl; 
    cout << "--------------------" << endl;
    cout << "|行     列     产生式 |" << endl;
    cout << "--------------------" << endl;
    map<pair<string, string>, vector<string> >::iterator it;
    cout << forecast_table.size() << endl;
    for (it = forecast_table.begin(); it != forecast_table.end(); it++) {
        pair<string, string> p = it->first;
        cout << p.first << "      " << p.second << " :    ";
        vector<string> v = it->second;
        cout << p.first << "->";
        for (int i = 0; i < v.size(); i++)
            cout << v[i] << " ";
        cout << endl;
    }
}
