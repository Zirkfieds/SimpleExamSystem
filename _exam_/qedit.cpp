#include"exam.h"

/*题库管理系统*/

void drawlines();

void Exam::customizePaper() {
	system("title 学生成绩管理-组卷系统");
	system("cls");
	cout << "当前组卷模式：" << (randomPick ? "随机" : "教师指定") << endl;
	cout << "是否要更改组卷设置? (y/n)" << endl;
	char ch; cin >> ch;
	if (ch == 'y') {
		cout << "请输入新的组卷模式: (1: 随机 0: 指定)" << endl;
		int op; cin >> op;
		if (op) {
			randomPick = 1;
			cout << "请输入随机题目总数Q\n总数Q满足 0≤Q≤" << maxqcnt << endl;
			int cnt; cin >> cnt; paperqcnt = cnt;
			updateQList();
			updateQBank();
			cout << "组卷设置更改完毕！" << endl;
			system("pause");
		}
		else {
			randomPick = 0;
			cout << "请输入考卷题目数Q\n总数Q满足 0≤Q≤" << maxqcnt << endl;
			cin.get();
			int cnt; cin >> cnt; paperqcnt = cnt;
			cout << "请依次输入题号 (以空格分隔题号)\n题号Q满足 0≤Q≤" << maxqcnt << endl;
			int cur;
			qlist = new int[cnt];
			for (int i = 0; i < cnt; i++) {
				int ibuf; cin >> ibuf;
				if (ibuf <= 0 || ibuf >= maxqcnt + 1) {
					cout << "无效输入！" << endl;
				}
				else {
					qlist[i] = ibuf;
				}
			}
			updateQList();
			updateQBank();
			cout << "组卷设置更改完毕！" << endl;
			system("pause");
		}
	}
	else {
		cout << "放弃对题库的更改！" << endl;
		system("pause");
		return;
	}
}

void Exam::editQBank() {
	cur = bot;
	while (1) {
		system("title 题库编辑系统");
		system("cls");
		showQ(BANK);
		drawlines();
		cout << "题库编辑系统" << endl;
		cout << "n：在题库末尾添加新题目\ne：更改当前题目\na/d：切换题目\nq：删除题目\nx:退出系统" << endl;
		drawlines();
		char ch = _getch();
		switch (ch) {
		case 'n':
			addQ();
			break;
		case 'e':
			editQ();
			break;
		case 'a':
			toPrior(BANK);
			break;
		case 'd':
			toNext(BANK);
			break;
		case 'q':
			delQ();
			break;
		case 'x':
			cout << "即将退出题库编辑系统！" << endl;
			system("pause");
			updateQBank();
			return;
		default:
			cout << "无效操作" << endl;
			break;
		}
	}
}

void Exam::addQ() {
	system("cls");
	system("title 题库编辑系统-添加题目");
	cout << "请输入新题目类型 (1：选择 2：判断 3：填空)" << endl;
	cin.clear();
	int qt = _getch() - '0';
	if (qt == selection) {
		cout << "选择题：" << endl;
		int no, cnt, cans; double score; char dbuf[MAXSTRSIZE]; string desc;
		no = maxqcnt + 1;
		cout << "请依次输入：选项数 分值 正确答案" << endl;
		cin >> cnt >> score >> cans;
		cin.get();
		cout << "请输入题干(以$结束)" << endl;
		cin.getline(dbuf, MAXSTRSIZE - 1, '$');
		desc = dbuf;
		cin.get();
		sq* n = new sq(no, cnt, score, cans, selection, desc);

		n->getopt();

		qbank.push_back(n);
	}
	else if (qt == judge) {
		cout << "判断题：" << endl;
		int no, cnt, cans; double score; char dbuf[MAXSTRSIZE]; string desc;
		no = maxqcnt + 1;
		cout << "请依次输入：分值 正确答案" << endl;
		cin >> score >> cans;
		cin.get();
		cout << "请输入题干(以$结束)" << endl;
		cin.getline(dbuf, MAXSTRSIZE - 1, '$');
		desc = dbuf;
		cin.get();
		jq* j = new jq(no, score, cans, judge, desc);
		qbank.push_back(j);
	}
	else if (qt == completion) {
		cout << "填空题：" << endl;
		int no; double score; 
		char dbuf[MAXSTRSIZE]; string desc;
		char abuf[MAXSTRSIZE]; string ans;
		no = maxqcnt + 1;
		cout << "请输入分值：" << endl;
		cin >> score;
		cin.get();
		cout << "请输入题干(以$结束)" << endl;
		cin.getline(dbuf, MAXSTRSIZE - 1, '$');
		desc = dbuf;
		cin.get();
		cout << "请输入答案(以$结束)" << endl;
		cin.getline(abuf, MAXSTRSIZE - 1, '$');
		ans = abuf;
		cin.get();
		cq* c = new cq(no, score, ans, completion, desc);
		qbank.push_back(c);
	}
	else {
		cout << "未知题目类型\n放弃添加题目" << endl;
		system("pause");
		system("cls");
		return;
	}
	cout << "题目已添加" << endl;
	system("pause");
	btop++;
	top++; cur = top; maxqcnt++;
	return;
}

void Exam::delQ() {
	system("cls");
	system("title 题库编辑系统-删除题目");
	int qt = qbank[cur]->getqtype();
	int pos = 0;
	for (auto qit = qbank.begin(); qit != qbank.end(); qit++, pos++) {
		if (pos == cur) {
			if (qit != qbank.begin()) { //deleting 2nd - end
				delete *qit;
				qit = qbank.erase(qit) - 1;
				break;
			}
			else { //deleting 1st
				delete *qit;
				qit = qbank.erase(qit);
				break;
			}
		}
	}
	cout << "题目已删除" << endl;
	system("pause");
	top--; btop--; maxqcnt--;
	cur == bot ? cur = bot : cur--;
	return;
}

void Exam::editQ() {
	system("cls");
	system("title 题库编辑系统-编辑题目");
	int qt = qbank[cur]->getqtype();
	if (qt == selection) {
		cout << "编辑选择题：" << endl;
		int no, cnt, cans; double score; char dbuf[MAXSTRSIZE]; string desc;
		no = maxqcnt + 1;
		cout << "请依次输入更改后的：选项数 正确答案 分值" << endl;
		cin >> cnt >> cans >> score;
		cout << "请输入更改后的题干(以$结束)" << endl;
		cin.get();
		cin.getline(dbuf, MAXSTRSIZE - 1, '$');
		desc = dbuf;
		cin.get();
		sq* q = dynamic_cast<sq*>(qbank[cur]);
		q->setoptcnt(cnt); q->setcorrectans(cans); q->setqscore(score);
		q->setdesc(desc);
		cout << "请依次输入新的选项：" << endl;
		q->editopt();
		cout << "已保存更改！" << endl;
	}
	else if (qt == judge) {
		cout << "编辑判断题：" << endl;
		int no, cnt, cans; double score; char dbuf[MAXSTRSIZE]; string desc;
		no = maxqcnt + 1;
		cout << "请依次输入更改后的：正确答案 分值" << endl;
		cin >> cans >> score;
		cin.get();
		cout << "请输入更改后的题干(以$结束)" << endl;
		cin.getline(dbuf, MAXSTRSIZE - 1, '$');
		desc = dbuf;
		cin.get();
		jq* q = dynamic_cast<jq*>(qbank[cur]);
		q->setcorrectans(cans); q->setqscore(score);
		q->setdesc(desc);
		cout << "已保存更改！" << endl;
	}
	else if (qt == completion) {
		cout << "填空题：" << endl;
		int no; double score;
		char dbuf[MAXSTRSIZE]; string desc;
		char abuf[MAXSTRSIZE]; string ans;
		no = maxqcnt + 1;
		cout << "请输入分值：" << endl;
		cin >> score;
		cin.get();
		cout << "请输入题干(以$结束)" << endl;
		cin.getline(dbuf, MAXSTRSIZE - 1, '$');
		desc = dbuf;
		cin.get();
		cout << "请输入答案(以$结束)" << endl;
		cin.getline(abuf, MAXSTRSIZE - 1, '$');
		ans = abuf;
		cin.get();
		cq* q = dynamic_cast<cq*>(qbank[cur]);
		q->setcorrectans(ans); q->setqscore(score);
		q->setdesc(desc);
		cout << "已保存更改！" << endl;
	}
	return;
}