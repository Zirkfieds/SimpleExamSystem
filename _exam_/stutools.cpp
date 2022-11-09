#include"exam.h"

/*����*/

////int/bool-char conversions
constexpr char i2bin(int i) { return i ? '1' : '0'; }
constexpr char b2char(bool i) { return i ? 'T' : 'F'; }

//truth table presets
int const varset1[2][1] = { //2items
	{0}, {1} };
int const varset2[4][2] = { //8items
	{0,0}, {0,1}, {1,0}, {1,1} };
int const varset3[8][3] = { //24items
	{0,0,0}, {0,0,1}, {0,1,0}, {0,1,1},
	{1,0,0}, {1,0,1}, {1,1,0}, {1,1,1} };
int const varset4[16][4] = { //64items
	{0,0,0,0}, {0,0,0,1}, {0,0,1,0}, {0,0,1,1},
	{0,1,0,0}, {0,1,0,1}, {0,1,1,0}, {0,1,1,1},
	{1,0,0,0}, {1,0,0,1}, {1,0,1,0}, {1,0,1,1},
	{1,1,0,0}, {1,1,0,1}, {1,1,1,0}, {1,1,1,1} };

int const *permutationmap[4] = { varset1[0], varset2[0], varset3[0], varset4[0] };

void secretExprCalc() { //expression calculator

	string inexpr;

	cout << "������Ҫ����ı��ʽ������ʹ��+-*/^()��x~1Ϊ-x����" << endl;
	cin >> inexpr;

	//checking in-expression
	if (!exprLegal(inexpr)) {
		cout << "���ʽ�Ƿ����޷����㣡" << endl;
		return;
	}
	if (!checkBrackets(inexpr)) {
		cout << "���Ų�ƥ�䣬�޷����㣡" << endl;
		return;
	}

	string postexpr = exprconversion(inexpr); //in-post conversion

	cout << "���ʽ��ֵ��" << calcexpr(postexpr) << endl;

}

template<typename T> //template of calc core
T ccalc(T num1, T num2, char opr) {
	switch (opr) {
	case '+':
		return num1 + num2;
	case '-':
		return num1 - num2;
	case '*':
		return num1 * num2;
	case '/':
		return num1 / num2;
	case '~':
		return -num1;
	case '^':
		return pow(num1, num2);
	}
}

bool exprLegal(string& expr) {
	regex legal("^[+*-/^~.0123456789()]*$");
	return regex_match(expr, legal);
}

int inline getcpri(char c) { //get the priority of oprators
	switch (c) {
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	case '^':
		return 3;
	case '~':
		return 4;
	default:
		return 0;
	}
}

string exprconversion(string& inexpr) { //in-post conversion

	char cpostexpr[512]; //dst

	stack<char> cs;
	int pos = 0;
	int numflag = false;
	for (auto c : inexpr) {
		if ((c >= '0' && c <= '9') || c == '.') {
			numflag = true;
			cpostexpr[pos++] = c;
		}
		else {
			if (numflag) {
				cpostexpr[pos++] = ' ';
			}
			if (cs.empty()) {
				cs.push(c);
			}
			else if (c == '(') {
				cs.push(c);
			}
			else if (c == ')') {
				while (cs.top() != '(') {
					cpostexpr[pos++] = cs.top();
					cpostexpr[pos++] = ' ';
					cs.pop();
				}
				cs.pop();
			}
			else {
				while (getcpri(c) <= getcpri(cs.top())) {
					cpostexpr[pos++] = cs.top();
					cpostexpr[pos++] = ' ';
					cs.pop();
					if (cs.empty()) {
						break;
					}
				}
				cs.push(c);
			}
		}
	}
	cpostexpr[pos++] = ' ';
	while (!cs.empty()) {
		char c = cs.top();
		cpostexpr[pos++] = c;
		cpostexpr[pos++] = ' ';
		cs.pop();
	}
	cpostexpr[pos++] = ' ';
	cpostexpr[pos++] = '`';
	cpostexpr[pos] = '\0';
	string postexpr = cpostexpr;
	return postexpr;
}

double calcexpr(string& expr) {
	stack <double> is;
	istringstream exp(expr);

	while (exp.peek() != '`') {
		if (isdigit(exp.peek())) {
			double num; exp >> num;
			is.push(num);
		}
		else if (isspace(exp.peek())) {
			exp.get();
		}
		else {
			if (exp.peek() == '+' || exp.peek() == '-' ||
				exp.peek() == '*' || exp.peek() == '/' ||
				exp.peek() == '^' || exp.peek() == '~') {
				char op = exp.get();
				double elem1 = is.top();
				is.pop();
				double elem2 = is.top();
				is.pop();
				double elem = ccalc(elem2, elem1, op);
				is.push(elem);
			}
		}
	}
	return is.top();
}

bool checkBrackets(string& expr) {
	stack<char> brackets;
	for (auto c : expr) {
		switch (c) {
		case '(':
			brackets.push(c);
			break;
		case ')':
			if (brackets.empty()) {
				return false;
			}
			if (brackets.top() == '(') {
				brackets.pop();
			}
			else {
				return false;
			}
			break;
		}
	}
	if (brackets.empty()) {
		return true;
	}
	return false;
}

bool varlegal(char vars[]) { //check the viability of args given by user
	if (strlen(vars) > 4) {
		return false;
	}
	regex r("^[A-Za-z0-9]*$");
	return regex_match(vars, r);
}

bool exprlegal(string& expr, char vars[]) { //check the viability of the in-expression
	if (strlen(vars) == 1) {
		regex r("^[+>*=~]*$");
		return (!regex_match(vars, r));
	}
	for (auto c : expr) {
		if (isalpha(c)) {
			int vc = strlen(vars);
			bool invars = false;
			for (int i = 0; i < vc; i++) {
				if (c == vars[i]) {
					invars = true;
					break;
				}
			}
			if (!invars) {
				return false;
			}
		}
	}
	return true;
}

int inline gettpri(char ch) { //get priority of the operators
	switch (ch) {
	case '=':
		return 1;
	case '>':
		return 2;
	case '+':
		return 3;
	case '*':
		return 4;
	case '!':
		return 5;
	case '~':
		return 6;
	default:
		return -1;
	}
}

bool inline isopr(char ch) {
	switch (ch) {
	case '!':
	case '+':
	case '*':
	case '>':
	case '=':
	case '~':
		return true;
	default:
		return false;
	}
}

string getpostexpr(string& inexpr) { //in-post conversion

	stack<char>cs;
	string postexpr;
	enum FSTATE {
		NONE = 0, VAR, OPR, ERR
	};
	int state = NONE;

	for (auto c : inexpr) {
		if (isopr(c)) {
			if (state == OPR && c != '!') {
				cout << "���������ʽ���޷����㣡" << endl;
				return inexpr;
				break;
			}
			state = OPR;
			while (!cs.empty() && isopr(cs.top()) && gettpri(cs.top()) >= gettpri(c)) {
				postexpr.push_back(cs.top());
				cs.pop();
			}
			cs.push(c);
		}
		else if (c == '(') {
			cs.push(c);
		}
		else if (c == ')') {
			while (cs.top() != '(') {
				postexpr.push_back(cs.top());
				cs.pop();
			}
			cs.pop();
		}
		else if (isalnum(c)) {
			state = VAR;
			postexpr.push_back(c);
		}
		else {
			cout << "���ʽ���Ϸ����޷����㣡" << endl;
			return inexpr;
			break;
		}
	}
	while (!cs.empty()) {
		postexpr.push_back(cs.top());
		cs.pop();
	}
	if (state == OPR) {
		cout << "���������ʽ���޷����㣡" << endl;
		return inexpr;
	}
	return postexpr;
}

bool gettval(string& postexpr) { //handling supported bool operators +*=>!
	stack<int> rs;
	int lelem, relem, rsegment;

	for (auto c : postexpr) {
		switch (c) {
		case '+':
			relem = rs.top(); rs.pop(); lelem = rs.top(); rs.pop();
			rsegment = (!relem && !lelem) ? false : true;
			rs.push(rsegment);
			break;
		case '*':
			relem = rs.top(); rs.pop(); lelem = rs.top(); rs.pop();
			rsegment = (lelem && relem) ? true : false;
			rs.push(rsegment);
			break;
		case '>':
			relem = rs.top(); rs.pop(); lelem = rs.top(); rs.pop();
			rsegment = lelem ? (relem ? true : false) : true;
			rs.push(rsegment);
			break;
		case '=':
			relem = rs.top(); rs.pop(); lelem = rs.top(); rs.pop();
			rsegment = (lelem == relem) ? true : false;
			rs.push(rsegment);
			break;
		case '!':
			rsegment = rs.top(); rs.pop();
			rsegment = rsegment ? false : true;
			rs.push(rsegment);
			break;
		default:
			rs.push(c - '0');
			break;
		}
	}
	return rs.top();
}

void dispt(string& expr, char vars[]) { //generating truth table

	int vc = strlen(vars);
	string postexpr;
	postexpr = getpostexpr(expr);

	if (expr.compare(postexpr) == 0) {
		cout << "��ֵ���޷����ɣ�������ʽ��" << endl;
		system("pause");
		return;
	}

	int iter = int(pow(2, vc));

	for (int i = 0; i < vc; i++) {
		cout << "\t" << vars[i];
	}
	cout << '\t' << expr << endl;

	for (int j = 0; j < iter; j++) {
		string tpost = postexpr;
		for (int k = 0; k < vc; k++) {
			cout << '\t' << *(permutationmap[vc - 1] + (j * vc) + k);
		}
		int i = 0;
		while (i < postexpr.size()) {
			for (int m = 0; m < vc; m++) {
				if (tpost[i] == vars[m]) {
					tpost[i] = i2bin(*(permutationmap[vc - 1] + (j * vc) + m));
					break;
				}
			}
			i++;
		}
		cout << '\t' << b2char(gettval(tpost)) << endl;
	}
	return;
}

//void karnaugh(int vars[], int ttable[]) {
//}
//
//void minimizekmap(vector<string>& result, int vars[], int ttable[]) {
//}

void secretTruthTable() { //truth table generator
	char vars[5];
	char tres[32];

	cout << "������1-4����ĸ��Ϊ��Ԫ��������@@�˳���" << endl;
	cin >> vars;

	if (!strcmp(vars, "@@")) {
		cout << "�����˳���ֵ���������" << endl;
		return;
	}
	//checking args
	if (!varlegal(vars)) {
		printf("��Ԫ���Ϸ���\n");
		system("pause");
		return;
	}

	string texpr;
	cout << "�����뺬��ָ����Ԫ���߼����ʽ��\n*��ȡ +��ȡ !�� >������ =˫����)" << endl;
	cin >> texpr;
	//checking in-expression
	if (!exprlegal(texpr, vars)) {
		cout << "��Ԫ���Ʋ�ƥ�䣬�޷����㣡" << endl;
		system("pause");
		return;
	}
	dispt(texpr, vars);
	return;
}