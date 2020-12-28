#include<iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;

string pretreat()
{
	fstream file;
	file.open("source.txt", ios::in);
	if (!file)
	{
		cout << "source.txt文件打开失败！\n";
		exit(0);
	}

	string str;
	string pre_str;
	bool judge_comment = 0;
	while (getline(file, str))
	{
		for (unsigned i = 0; i < str.size(); i++) {
			//去除"/**/"注释
			if (judge_comment) {
				if (str[i] == '*' && str[i + 1] == '/') {
					judge_comment = 0;
					i++;
				}
				continue;
			}
			if (str[i] == '/' && str[i + 1] == '*') {
				judge_comment = 1;
				continue;
			}

			//去除"//"注释
			if (str[i] == '/' && str[i + 1] == '/') {
				break;
			}

			//将双运算符两边添加空格
			char op[2];
			op[0] = str[i];
			op[1] = str[i + 1];
			if (op == "==" || op == "++" || op == "--" || op == "!=" || op == "&&" || op == "||") {
				if (pre_str[pre_str.size() - 1] != ' ')
					pre_str.push_back(' ');
				pre_str.push_back(str[i]);
				i++;
				pre_str.push_back(str[i]);
				pre_str.push_back(' ');
				continue;
			}

			//将单个运算符和界符两边都添加空格
			if (str[i] == ',' || str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '(' || str[i] == ')'
				|| str[i] == '<' || str[i] == '>' || str[i] == '=' || str[i] == ';' || str[i] == '&' ||
				str[i] == '%' || str[i] == '#' || str[i] == '!' || str[i] == '{' || str[i] == '}' || str[i] == '[' || str[i] == ']') {
				if (pre_str[pre_str.size() - 1] != ' ')
					pre_str.push_back(' ');
				pre_str.push_back(str[i]);
				pre_str.push_back(' ');
				continue;
			}

			//多个空格和换行合并成一个空格
			if (str[i] != ' ' || (str[i] == ' ' && pre_str[pre_str.size() - 1] != ' ')) {
				pre_str.push_back(str[i]);
			}
		}
		if (pre_str.size() > 0 && pre_str[pre_str.size() - 1] != ' ')
			pre_str.push_back(' ');
	}
	//cout << pre_str;
	file.close();

	return pre_str;
}

struct Category {
	int num;
	string name;
};

void add_cate(vector<Category> &cate, int num, string name)
{
	Category cate_temp;
	cate_temp.name = name;
	cate_temp.num = num;
	cate.push_back(cate_temp);
}

void category(vector<Category>& cate)
{
	//关键字
	add_cate(cate, 1, "main");
	add_cate(cate, 2, "int");
	add_cate(cate, 3, "char");
	add_cate(cate, 4, "string");
	add_cate(cate, 5, "long");
	add_cate(cate, 6, "short");
	add_cate(cate, 7, "void");
	add_cate(cate, 8, "if");
	add_cate(cate, 9, "else");
	add_cate(cate, 10, "return");
	add_cate(cate, 11, "while");
	add_cate(cate, 12, "for");
	add_cate(cate, 13, "do");

	//运算符
	add_cate(cate, 100, "+");
	add_cate(cate, 101, "-");
	add_cate(cate, 102, "*");
	add_cate(cate, 103, "/");
	add_cate(cate, 104, "=");
	add_cate(cate, 105, "==");
	add_cate(cate, 106, "++");
	add_cate(cate, 107, "--");
	add_cate(cate, 108, "!=");
	add_cate(cate, 109, "%");
	add_cate(cate, 110, "!");
	add_cate(cate, 111, "&");
	add_cate(cate, 112, "|");
	add_cate(cate, 113, ">");
	add_cate(cate, 114, "<");
	add_cate(cate, 115, ">=");
	add_cate(cate, 116, "<=");

	//标识符
	add_cate(cate, 150, "标识符");

	//常数变量
	add_cate(cate, 200, "int");
	add_cate(cate, 201, "char");
	add_cate(cate, 202, "long");
	add_cate(cate, 203, "string");
	add_cate(cate, 204, "short");
	add_cate(cate, 205, "bool");
	add_cate(cate, 206, "float");
	add_cate(cate, 207, "double");

	//界符
	add_cate(cate, 300, "(");
	add_cate(cate, 301, ")");
	add_cate(cate, 302, "[");
	add_cate(cate, 303, "]");
	add_cate(cate, 304, "{");
	add_cate(cate, 305, "}");
	add_cate(cate, 306, ";");
	add_cate(cate, 307, ",");
	add_cate(cate, 308, "#");
	add_cate(cate, 300, "'");
	/*add_cate(cate, 300, """);*/
}
int judge_id(vector<Category> cate, string str)
{
	int flag = 1;
	for (unsigned i = 0; i < cate.size(); i++) {
		if (str == cate[i].name) {
			flag = 0;
		}
	}
	return flag;
}


struct table {
	string identity;    //定义名称
	string type;        //类型-const等
	int val;            //值
	//int addr;         //偏移量
};

struct tablelink {
	vector<table> level_table;
	//int level;
}Tablelink[200];        //符号表

int find(string name) {//查找符号
	for (int i = 0; i < Tablelink[0].level_table.size(); i++)
	{
		if (Tablelink[0].level_table[i].identity == name)
		{
			return i;
		}
	}
	return -1;
}

void add(string name, string type, int val) {//添加符号
	table new_symbol;
	new_symbol.identity = name;
	new_symbol.type = type;
	new_symbol.val = val;
	Tablelink[0].level_table.push_back(new_symbol);
}

void del(string name) {//删除符号
	for (int i = 0; i < Tablelink[0].level_table.size(); i++)
	{
		if (Tablelink[0].level_table[i].identity == name)
		{
			Tablelink[0].level_table.erase(Tablelink[0].level_table.begin() + i);
		}
	}
}

void display(){
    cout << "                  **符号表**                  " << endl;
    cout << "---------------------------------------------" << endl;
    cout << "     符号名     ｜     类型     ｜      值      " << endl;
    cout << "---------------------------------------------" << endl;
    for (int i=0; i< Tablelink[0].level_table.size(); i++){
        cout << "       " << Tablelink[0].level_table[i].identity << "            " << Tablelink[0].level_table[i].type << "             " << Tablelink[0].level_table[i].val;
        cout << endl;
    }
}

void create_table(vector<Category> cate, string source_str, vector<Category> &source_cate)
{
	int flag;
	int judge;
	string str;
	Category temp;
	//string last_str;
	for (unsigned i = 0; i < source_str.size(); i++) {
		if (source_str[i] != ' ') {
			str.push_back(source_str[i]);
		}
		else {
			judge = 0;
			for (unsigned i = 0; i < cate.size(); i++) {
				if (str == cate[i].name) {
					temp.name = str;
					temp.num = cate[i].num;
					source_cate.push_back(temp);
					judge = 1;
					break;
				}
			}
			if (judge == 0) {
				temp.name = str;
				temp.num = 150;
				source_cate.push_back(temp);
				if (judge_id(cate, str)) {
					flag = find(str);
					if (flag == -1) {
						add(str, "NULL", 0);
					}
				}
			}
			str.clear();
		}
		//last_str = str;
	}
}

vector<Category> lex()
{
	string source_str;
	source_str = pretreat();
	//cout << source_str;

	vector<Category> cate;
	vector<Category> source_cate;

	category(cate);
	create_table(cate, source_str, source_cate);

	return source_cate;
}



int main()
{
	vector<Category> source_cate;
	source_cate = lex();
	for (unsigned i = 0; i < source_cate.size(); i++) {
		cout << "<" << source_cate[i].num << "," << source_cate[i].name << ">" << endl;
	}
	display();
	return 0;
}