#include"exam.h"

/*用户系统*/

#define U_SRC "./files/users.u"

bool scmpdown(stu* s1, stu* s2) {
	return s1->getsscore() > s2->getsscore();
}

bool scmprise(stu* s1, stu* s2) {
	return s1->getsscore() < s2->getsscore();
}

void drawlines() {
	for (int i = 0; i < 32; i++) {
		cout << '-';
	}
	cout << endl;
}

void drawlines(int length) {
	for (int i = 0; i < length; i++) {
		cout << '-';
	}
	cout << endl;
}

void Exam::loadUsers() {
	ifstream src;
	src.open(U_SRC);

	int i = 0;

	while (1) {
		if (src.peek() == EOF) {
			ucnt = i;
			break;
		}
		int qt;
		src >> qt;
		src.get();

		if (qt == STUDENT) {
			char snamebuf[MAXSTRSIZE], snobuf[MAXSTRSIZE], spwbuf[MAXSTRSIZE];
			string sname, sno, spw;
			int pstate, stuscore;

			src.getline(snobuf, MAXSTRSIZE - 1, '$');
			src.getline(snamebuf, MAXSTRSIZE - 1, '$');
			src.getline(spwbuf, MAXSTRSIZE - 1, '$');
			sname = snamebuf; sno = snobuf; spw = spwbuf;

			src >> pstate >> stuscore;

			stu* nstu = new stu(sname, sno, spw, pstate, stuscore);
			stus.push_back(nstu);

		}
		else if (qt == TEACHER) {
			char tnamebuf[MAXSTRSIZE], tnobuf[MAXSTRSIZE], tpwbuf[MAXSTRSIZE];
			string tname, tno, tpw;

			src.getline(tnobuf, MAXSTRSIZE - 1, '$');
			src.getline(tnamebuf, MAXSTRSIZE - 1, '$');
			src.getline(tpwbuf, MAXSTRSIZE - 1, '$');
			tname = tnamebuf; tno = tnobuf; tpw = tpwbuf;

			teach* nteach = new teach(tname, tno, tpw);
			users.push_back(nteach);
			i++;
		}
		else if (qt == ADMINISTRATOR) {
			char anamebuf[MAXSTRSIZE], anobuf[MAXSTRSIZE], apwbuf[MAXSTRSIZE];
			string aname, ano, apw;

			src.getline(anobuf, MAXSTRSIZE - 1, '$');
			src.getline(anamebuf, MAXSTRSIZE - 1, '$');
			src.getline(apwbuf, MAXSTRSIZE - 1, '$');
			aname = anamebuf; ano = anobuf; apw = apwbuf;

			admin* nadmin = new admin(aname, ano, apw);
			users.push_back(nadmin);
			i++;
		}
	}
	src.close();
}

void Exam::showUsers() {
	for (auto& sv : stus) {
		sv->showstu();
	}
	for (int i = 0; i < users.size(); i++) {
		int utype = users[i]->getutype();
		if (utype == TEACHER) {
			teach* nteach = dynamic_cast<teach*>(users[i]);
			nteach->showteach();
		}
		else if (utype == ADMINISTRATOR) {
			admin* nadmin = dynamic_cast<admin*>(users[i]);
			nadmin->showteach();
		}
	}
}

void stu::showstu() {
	cout << "学生" << uname << " 学号" << uno
		<< "\n是否已参加考试：";
	if (participated) {
		cout << "是" << " 得分：" << sscore << endl;
	}
	else {
		cout << "否" << endl;
	}
	return;
}

void teach::showteach() {
	cout << "教师" << uname << " 工号" << uno << endl;
	return;
}

void admin::showadmin() {
	cout << "管理员" << uname << " 工号" << uno << endl;
	return;
}

void Exam::updateUsers() {
	ofstream dst; 
	dst.open(U_SRC);
	for (auto& s : stus) {
		dst << STUDENT << ' ';
		dst << s->getuname() << '$' << s->getuno() << '$' << s->getupw() << '$';
		dst << (s->participatedExam(1) ? '1' : '0') << ' ' << s->getsscore() << '\n';
	}
	for (auto& u : users) {
		int utype = u->getutype();
		if (utype == TEACHER) {
			teach* nteach = dynamic_cast<teach*>(u);
			dst << TEACHER << ' ';
			dst << nteach->getuname() << '$' << nteach->getuno() << '$' << nteach->getupw() << '$';
		}
		else if (utype == ADMINISTRATOR) {
			admin* nadmin = dynamic_cast<admin*>(u);
			dst << ADMINISTRATOR << ' ';
			dst << nadmin->getuname() << '$' << nadmin->getuno() << '$' << nadmin->getupw() << '$';
		}
		if (u != *(users.end() - 1)) {
			dst << '\n';
		}
	}
	dst.close();
	return;
}

void stu::showMenu(Exam& E) {
	system("title 登录系统-学生操作页面");
	system("cls");
	if (participated) {
		cout << "你已经参加过了考试\n成绩：" << sscore << endl;
		system("pause");
		exit(EXIT_WITH_STU);
	}
	else {
		cout << "你还没有参加过考试\n是否现在开始考试? (y/n)" << endl;
		char ch; cin >> ch;
		if (ch == 'y') {
			E.seteflag(1);
			cout << "考试即将开始！" << endl;
			system("pause");
			system("cls");
			return;
		}
		else {
			cout << "考试程序即将退出！" << endl;
			system("pause");
			exit(EXIT_WITH_STU);
		}
	}
}

void teach::showMenu(Exam& E) {
	while (1) {
		system("cls");
		system("title 登录系统-教师操作页面");
		cout << "q: 管理学生成绩 | c: 指定题目组卷/调整组卷模式\nx: 退出系统" << endl;
		char ch = _getch();
		switch (ch) {
		case 'q':
			querystu(E);
			break;
		case 'c':
			E.customizePaper();
			break;
		case 'x':
			cout << "系统已退出！" << endl;
			return;
		default:
			cout << "无效操作！" << endl;
			system("pause");
			break;
		}
	}
}

void teach::querystu(Exam& E) {
	system("title 学生成绩管理(教师版)");
	system("cls");
	while (1) {
		cout << "q: 查询学生成绩 | l: 显示全部学生成绩\nr: 降序排列学生成绩 | x: 退出学生成绩管理" << endl;
		char ch = _getch();
		switch (ch) {
		case 'q':
			E.getsscore();
			system("pause");
			system("cls");
			break;
		case 'l':
			E.getallscore();
			system("pause");
			system("cls");
			break;
		case 'r':
			E.rankscore();
			system("pause");
			system("cls");
			break;
		case 'x':
			cout << "即将退出学生成绩管理界面！" << endl;
			system("pause");
			return;
		default:
			cout << "无效操作！" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
}

void Exam::getsscore() {
	system("title 学生成绩管理-查询学生成绩");
	system("cls");
	cout << "请输入待查询学生用户名：" << endl;
	string namebuf; cin >> namebuf;
	bool fflag = false;
	for (auto& s : stus) {
		if ((s->getuname()).compare(namebuf) == 0) {
			fflag = true;
			cout << endl;  drawlines();
			cout << "学生" << s->getuname() << " 学号：" << s->getuno();
			if (s->getsscore() == -1) {
				cout << " 还未参加过考试" << endl;
			}
			else {
				cout << " 成绩：" << s->getsscore() << endl;
			}
			drawlines(); cout << endl;
			break;
		}
	}
	if (!fflag) {
		cout << "未找到指定学生！" << endl;
	}
	return;
}

void Exam::getallscore() {
	system("title 学生成绩管理-显示学生成绩");
	system("cls");
	cout << endl;
	drawlines();
	for (auto& s : stus) {
		cout << "学生" << s->getuname() << " 学号：" << s->getuno();
		if (s->getsscore() == -1) {
			cout << " 还未参加过考试" << endl;
		}
		else {
			cout << " 成绩：" << s->getsscore() << endl;
		}
	}
	drawlines();
	cout << endl;
	return;
}

void Exam::rankscore() {
	system("title 学生成绩管理-排序学生成绩");
	system("cls");

	vector<stu*> rstu;
	for (auto& s : stus) {
		rstu.push_back(s);
	}
	sort(rstu.begin(), rstu.end(), scmpdown);

	cout << endl;
	drawlines();
	for (auto& s : rstu) {
		cout << "学生" << s->getuname() << " 学号：" << s->getuno();
		if (s->getsscore() == -1) {
			cout << " 还未参加过考试" << endl;
		}
		else {
			cout << " 成绩：" << s->getsscore() << endl;
		}
	}
	drawlines();
	cout << endl;
	return;
}

void admin::querystu(Exam& E) {
	system("title 学生成绩管理(管理员版)");
	system("cls");
	while (1) {
		cout << "q: 查询学生成绩 | l: 显示全部学生成绩\nr: 降序排列学生成绩 | d:修改学生考试信息\nx: 退出学生成绩管理" << endl;
		char ch = _getch();
		switch (ch) {
		case 'q':
			E.getsscore();
			system("pause");
			system("cls");
			break;
		case 'l':
			E.getallscore();
			system("pause");
			system("cls");
			break;
		case 'r':
			E.rankscore();
			system("pause");
			system("cls");
			break;
		case 'd':
			E.editstustate();
			system("pause");
			system("cls");
			break;
		case 'x':
			cout << "即将退出学生成绩管理界面！" << endl;
			system("pause");
			return;
		default:
			cout << "无效操作！" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
}

void admin::showMenu(Exam& E) {
	while (1) {
		system("cls");
		system("title 登录系统-管理员操作页面");
		cout << "q: 管理学生成绩 | c: 指定题目组卷/调整组卷模式\ne: 编辑题库 | x: 退出系统" << endl;
		char ch = _getch();
		switch (ch) {
		case 'q':
			querystu(E);
			break;
		case 'c':
			E.customizePaper();
			break;
		case 'e':
			E.editQBank();
			break;
		case 'x':
			cout << "系统已退出！" << endl;
			return;
		default:
			cout << "无效操作！" << endl;
			system("pause");
			break;
		}
	}
}

void Exam::editstustate() {
	system("title 学生成绩管理-考试状态");
	system("cls");
	cout << "请输入待操作学生用户名：" << endl;
	string namebuf; cin >> namebuf;
	bool fflag = false;
	stu* tgt;
	for (auto& s : stus) {
		if ((s->getuname()).compare(namebuf) == 0) {
			fflag = true;
			cout << endl;  drawlines();
			cout << "学生" << s->getuname() << " 学号：" << s->getuno();
			if (s->getsscore() == -1) {
				cout << " 还未参加过考试" << endl;
			}
			else {
				cout << " 成绩：" << s->getsscore() << endl;
			}
			drawlines(); cout << endl;
			tgt = s;
			break;
		}
	}
	if (!fflag) {
		cout << "未找到指定学生！" << endl;
	}
	cout << "是否要更改学生考试状态?\ny：设置为已考试 | n：设置为未考试\nx: 放弃更改" << endl;
	char ch = _getch();
	if (ch == 'y') {
		cout << "请手动输入学生分数：";
		double sbuf; cin >> sbuf;
		tgt->setsscore(sbuf);
		tgt->setparticipated(true);
		cout << "已添加学生考试记录" << endl;
		return;
	}
	else if (ch == 'n') {
		tgt->setsscore(-1);
		tgt->setparticipated(false);
		cout << "已移除学生考试记录" << endl;
		return;
	}
	else {
		cout << "放弃更改学生考试状态" << endl;
	}
	return;
}

void Exam::loginsys() {
	system("title 登录系统");
	const string exitStr = "@@";
	cout << "请输入用户身份: (0: 管理员 1: 学生 2 老师)" << endl;
	char identityc; cin >> identityc;
	int identity = identityc - '0';
	if (identity == STUDENT) {
		utp = STUDENT;
		cout << "请输入用户名" << endl;
		string ubuf; cin >> ubuf;
		for (auto& sv : stus) {
			if (ubuf.compare(sv->getuname()) == 0) {

				while (1) {
					system("cls");
					cout << "请输入学生" << sv->getuname() << "的密码：" << endl;
					cout << "输入@@以退出考试系统" << endl;
					string pbuf; cin >> pbuf;
					if (pbuf.compare(exitStr) == 0) {
						cout << "程序即将退出" << endl;
						system("pause");
						exit(FORCEFULLY_EXIT);
					}
					if (sv->pwCorrect(pbuf)) {
						cout << "已登录为学生" << sv->getuname() << endl;
						break;
					}
					else {
						cout << "密码错误，请重试！" << endl;
						system("pause");
					}
				}
				ustu = sv;
				system("pause");
				return;
			}
		}
		cout << "未找到指定用户！程序即将退出";
		exit(CANNOT_FIND_USER);
	}
	else if (identity == TEACHER || identity == ADMINISTRATOR) {
		utp = identity;
		cout << "请输入用户名" << endl;
		string ubuf; cin >> ubuf;
		for (int i = 0; i < users.size(); i++) {
			int utype = users[i]->getutype();
			if (utype == TEACHER) {
				teach* nteach = dynamic_cast<teach*>(users[i]);
				if (ubuf.compare(nteach->getuname()) == 0) {

					while (1) {
						system("cls");
						cout << "请输入教师" << nteach->getuname() << "的密码：" << endl;
						cout << "输入@@以退出考试系统" << endl;
						string pbuf; cin >> pbuf;
						if (pbuf.compare(exitStr) == 0) {
							cout << "程序即将退出" << endl;
							system("pause");
							exit(FORCEFULLY_EXIT);
						}
						if (nteach->pwCorrect(pbuf)) {
							cout << "已登录为教师" << nteach->getuname() << endl;
							break;
						}
						else {
							cout << "密码错误，请重试！" << endl;
							system("pause");
						}
					}

					uteach = nteach;
					system("pause");
					return;
				}
			}
			else if (utype == ADMINISTRATOR) {
				admin* nadmin = dynamic_cast<admin*>(users[i]);
				if (ubuf.compare(nadmin->getuname()) == 0) {

					while (1) {
						system("cls");
						cout << "请输入教师" << nadmin->getuname() << "的密码：" << endl;
						cout << "输入@@以退出考试系统" << endl;
						string pbuf; cin >> pbuf;
						if (pbuf.compare(exitStr) == 0) {
							cout << "程序即将退出" << endl;
							system("pause");
							exit(FORCEFULLY_EXIT);
						}
						if (nadmin->pwCorrect(pbuf)) {
							cout << "已登录为管理员" << nadmin->getuname() << endl;

							break;
						}
						else {
							cout << "密码错误，请重试！" << endl;
							system("pause");
						}
					}

					uadmin = nadmin;
					system("pause");
					return;
				}
			}
		}
		cout << "未找到指定用户！程序即将退出";
		exit(CANNOT_FIND_USER);
	}
	else {
		cout << "未知身份！程序即将退出" << endl;
		exit(UNKNOWN_IDENTITY);
	}
	return;
}

void Exam::userPanel() {
	switch (utp) {
	case STUDENT:
		cout << "欢迎学生" << ustu->getuname() << "！" << endl;
		ustu->showMenu(*this);
		break;
	case TEACHER:
		cout << "欢迎教师" << uteach->getuname() << "！" << endl;
		uteach->showMenu(*this);
		break;
	case ADMINISTRATOR:
		cout << "欢迎管理员" << uadmin->getuname() << "！" << endl;
		uadmin->showMenu(*this);
		break;
	}
	updateUsers();
}