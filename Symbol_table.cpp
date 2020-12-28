#include<iostream>
#include<vector>
using namespace std;

struct table {
	string identity;    //定义名称
	string type;        //类型-const等
	int val;            //值
	//int addr;         //偏移量
    //123
};

struct tablelink {
	vector<table> level_table;
	//int level;
}Tablelink[200];        //符号表

int find(string name){//查找符号
    for (int i = 0; i < Tablelink[0].level_table.size(); i++)
		{
			if (Tablelink[0].level_table[i].identity == name)
			{
				return i;
			}
		}
	return -1;
}

void add(string name, string type, int val){//添加符号
    table new_symbol;
    new_symbol.identity = name;
    new_symbol.type = type;
    new_symbol.val = val;
    Tablelink[0].level_table.push_back(new_symbol);
}

void del(string name){//删除符号
    for (int i = 0; i < Tablelink[0].level_table.size(); i++)
		{
			if (Tablelink[0].level_table[i].identity == name)
			{
                Tablelink[0].level_table.erase(Tablelink[0].level_table.begin()+i);
			}
		}
}

/*int main(){
    string a = "a";
    string int_d = "int_d";
    string b = "b";
    
    string c = "c";
    
    string d = "d";
    
    add(a, int_d , 1);
    add(b, int_d , 2);
    add(c, int_d , 3);
    add(d, int_d , 4);
    

    cout << Tablelink[0].level_table.size() << endl;
    cout << Tablelink[0].level_table[0].identity << endl;
    del(a);
    cout << Tablelink[0].level_table.size() << endl;
    cout << Tablelink[0].level_table[0].identity << endl;
    return 0;
}
*/


