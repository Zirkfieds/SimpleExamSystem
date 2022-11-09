#include"exam.h"

/*�û�ϵͳ*/

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
	cout << "ѧ��" << uname << " ѧ��" << uno
		<< "\n�Ƿ��Ѳμӿ��ԣ�";
	if (participated) {
		cout << "��" << " �÷֣�" << sscore << endl;
	}
	else {
		cout << "��" << endl;
	}
	return;
}

void teach::showteach() {
	cout << "��ʦ" << uname << " ����" << uno << endl;
	return;
}

void admin::showadmin() {
	cout << "����Ա" << uname << " ����" << uno << endl;
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
	system("title ��¼ϵͳ-ѧ������ҳ��");
	system("cls");
	if (participated) {
		cout << "���Ѿ��μӹ��˿���\n�ɼ���" << sscore << endl;
		system("pause");
		exit(EXIT_WITH_STU);
	}
	else {
		cout << "�㻹û�вμӹ�����\n�Ƿ����ڿ�ʼ����? (y/n)" << endl;
		char ch; cin >> ch;
		if (ch == 'y') {
			E.seteflag(1);
			cout << "���Լ�����ʼ��" << endl;
			system("pause");
			system("cls");
			return;
		}
		else {
			cout << "���Գ��򼴽��˳���" << endl;
			system("pause");
			exit(EXIT_WITH_STU);
		}
	}
}

void teach::showMenu(Exam& E) {
	while (1) {
		system("cls");
		system("title ��¼ϵͳ-��ʦ����ҳ��");
		cout << "q: ����ѧ���ɼ� | c: ָ����Ŀ���/�������ģʽ\nx: �˳�ϵͳ" << endl;
		char ch = _getch();
		switch (ch) {
		case 'q':
			querystu(E);
			break;
		case 'c':
			E.customizePaper();
			break;
		case 'x':
			cout << "ϵͳ���˳���" << endl;
			return;
		default:
			cout << "��Ч������" << endl;
			system("pause");
			break;
		}
	}
}

void teach::querystu(Exam& E) {
	system("title ѧ���ɼ�����(��ʦ��)");
	system("cls");
	while (1) {
		cout << "q: ��ѯѧ���ɼ� | l: ��ʾȫ��ѧ���ɼ�\nr: ��������ѧ���ɼ� | x: �˳�ѧ���ɼ�����" << endl;
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
			cout << "�����˳�ѧ���ɼ�������棡" << endl;
			system("pause");
			return;
		default:
			cout << "��Ч������" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
}

void Exam::getsscore() {
	system("title ѧ���ɼ�����-��ѯѧ���ɼ�");
	system("cls");
	cout << "���������ѯѧ���û�����" << endl;
	string namebuf; cin >> namebuf;
	bool fflag = false;
	for (auto& s : stus) {
		if ((s->getuname()).compare(namebuf) == 0) {
			fflag = true;
			cout << endl;  drawlines();
			cout << "ѧ��" << s->getuname() << " ѧ�ţ�" << s->getuno();
			if (s->getsscore() == -1) {
				cout << " ��δ�μӹ�����" << endl;
			}
			else {
				cout << " �ɼ���" << s->getsscore() << endl;
			}
			drawlines(); cout << endl;
			break;
		}
	}
	if (!fflag) {
		cout << "δ�ҵ�ָ��ѧ����" << endl;
	}
	return;
}

void Exam::getallscore() {
	system("title ѧ���ɼ�����-��ʾѧ���ɼ�");
	system("cls");
	cout << endl;
	drawlines();
	for (auto& s : stus) {
		cout << "ѧ��" << s->getuname() << " ѧ�ţ�" << s->getuno();
		if (s->getsscore() == -1) {
			cout << " ��δ�μӹ�����" << endl;
		}
		else {
			cout << " �ɼ���" << s->getsscore() << endl;
		}
	}
	drawlines();
	cout << endl;
	return;
}

void Exam::rankscore() {
	system("title ѧ���ɼ�����-����ѧ���ɼ�");
	system("cls");

	vector<stu*> rstu;
	for (auto& s : stus) {
		rstu.push_back(s);
	}
	sort(rstu.begin(), rstu.end(), scmpdown);

	cout << endl;
	drawlines();
	for (auto& s : rstu) {
		cout << "ѧ��" << s->getuname() << " ѧ�ţ�" << s->getuno();
		if (s->getsscore() == -1) {
			cout << " ��δ�μӹ�����" << endl;
		}
		else {
			cout << " �ɼ���" << s->getsscore() << endl;
		}
	}
	drawlines();
	cout << endl;
	return;
}

void admin::querystu(Exam& E) {
	system("title ѧ���ɼ�����(����Ա��)");
	system("cls");
	while (1) {
		cout << "q: ��ѯѧ���ɼ� | l: ��ʾȫ��ѧ���ɼ�\nr: ��������ѧ���ɼ� | d:�޸�ѧ��������Ϣ\nx: �˳�ѧ���ɼ�����" << endl;
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
			cout << "�����˳�ѧ���ɼ�������棡" << endl;
			system("pause");
			return;
		default:
			cout << "��Ч������" << endl;
			system("pause");
			system("cls");
			break;
		}
	}
}

void admin::showMenu(Exam& E) {
	while (1) {
		system("cls");
		system("title ��¼ϵͳ-����Ա����ҳ��");
		cout << "q: ����ѧ���ɼ� | c: ָ����Ŀ���/�������ģʽ\ne: �༭��� | x: �˳�ϵͳ" << endl;
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
			cout << "ϵͳ���˳���" << endl;
			return;
		default:
			cout << "��Ч������" << endl;
			system("pause");
			break;
		}
	}
}

void Exam::editstustate() {
	system("title ѧ���ɼ�����-����״̬");
	system("cls");
	cout << "�����������ѧ���û�����" << endl;
	string namebuf; cin >> namebuf;
	bool fflag = false;
	stu* tgt;
	for (auto& s : stus) {
		if ((s->getuname()).compare(namebuf) == 0) {
			fflag = true;
			cout << endl;  drawlines();
			cout << "ѧ��" << s->getuname() << " ѧ�ţ�" << s->getuno();
			if (s->getsscore() == -1) {
				cout << " ��δ�μӹ�����" << endl;
			}
			else {
				cout << " �ɼ���" << s->getsscore() << endl;
			}
			drawlines(); cout << endl;
			tgt = s;
			break;
		}
	}
	if (!fflag) {
		cout << "δ�ҵ�ָ��ѧ����" << endl;
	}
	cout << "�Ƿ�Ҫ����ѧ������״̬?\ny������Ϊ�ѿ��� | n������Ϊδ����\nx: ��������" << endl;
	char ch = _getch();
	if (ch == 'y') {
		cout << "���ֶ�����ѧ��������";
		double sbuf; cin >> sbuf;
		tgt->setsscore(sbuf);
		tgt->setparticipated(true);
		cout << "�����ѧ�����Լ�¼" << endl;
		return;
	}
	else if (ch == 'n') {
		tgt->setsscore(-1);
		tgt->setparticipated(false);
		cout << "���Ƴ�ѧ�����Լ�¼" << endl;
		return;
	}
	else {
		cout << "��������ѧ������״̬" << endl;
	}
	return;
}

void Exam::loginsys() {
	system("title ��¼ϵͳ");
	const string exitStr = "@@";
	cout << "�������û����: (0: ����Ա 1: ѧ�� 2 ��ʦ)" << endl;
	char identityc; cin >> identityc;
	int identity = identityc - '0';
	if (identity == STUDENT) {
		utp = STUDENT;
		cout << "�������û���" << endl;
		string ubuf; cin >> ubuf;
		for (auto& sv : stus) {
			if (ubuf.compare(sv->getuname()) == 0) {

				while (1) {
					system("cls");
					cout << "������ѧ��" << sv->getuname() << "�����룺" << endl;
					cout << "����@@���˳�����ϵͳ" << endl;
					string pbuf; cin >> pbuf;
					if (pbuf.compare(exitStr) == 0) {
						cout << "���򼴽��˳�" << endl;
						system("pause");
						exit(FORCEFULLY_EXIT);
					}
					if (sv->pwCorrect(pbuf)) {
						cout << "�ѵ�¼Ϊѧ��" << sv->getuname() << endl;
						break;
					}
					else {
						cout << "������������ԣ�" << endl;
						system("pause");
					}
				}
				ustu = sv;
				system("pause");
				return;
			}
		}
		cout << "δ�ҵ�ָ���û������򼴽��˳�";
		exit(CANNOT_FIND_USER);
	}
	else if (identity == TEACHER || identity == ADMINISTRATOR) {
		utp = identity;
		cout << "�������û���" << endl;
		string ubuf; cin >> ubuf;
		for (int i = 0; i < users.size(); i++) {
			int utype = users[i]->getutype();
			if (utype == TEACHER) {
				teach* nteach = dynamic_cast<teach*>(users[i]);
				if (ubuf.compare(nteach->getuname()) == 0) {

					while (1) {
						system("cls");
						cout << "�������ʦ" << nteach->getuname() << "�����룺" << endl;
						cout << "����@@���˳�����ϵͳ" << endl;
						string pbuf; cin >> pbuf;
						if (pbuf.compare(exitStr) == 0) {
							cout << "���򼴽��˳�" << endl;
							system("pause");
							exit(FORCEFULLY_EXIT);
						}
						if (nteach->pwCorrect(pbuf)) {
							cout << "�ѵ�¼Ϊ��ʦ" << nteach->getuname() << endl;
							break;
						}
						else {
							cout << "������������ԣ�" << endl;
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
						cout << "�������ʦ" << nadmin->getuname() << "�����룺" << endl;
						cout << "����@@���˳�����ϵͳ" << endl;
						string pbuf; cin >> pbuf;
						if (pbuf.compare(exitStr) == 0) {
							cout << "���򼴽��˳�" << endl;
							system("pause");
							exit(FORCEFULLY_EXIT);
						}
						if (nadmin->pwCorrect(pbuf)) {
							cout << "�ѵ�¼Ϊ����Ա" << nadmin->getuname() << endl;

							break;
						}
						else {
							cout << "������������ԣ�" << endl;
							system("pause");
						}
					}

					uadmin = nadmin;
					system("pause");
					return;
				}
			}
		}
		cout << "δ�ҵ�ָ���û������򼴽��˳�";
		exit(CANNOT_FIND_USER);
	}
	else {
		cout << "δ֪��ݣ����򼴽��˳�" << endl;
		exit(UNKNOWN_IDENTITY);
	}
	return;
}

void Exam::userPanel() {
	switch (utp) {
	case STUDENT:
		cout << "��ӭѧ��" << ustu->getuname() << "��" << endl;
		ustu->showMenu(*this);
		break;
	case TEACHER:
		cout << "��ӭ��ʦ" << uteach->getuname() << "��" << endl;
		uteach->showMenu(*this);
		break;
	case ADMINISTRATOR:
		cout << "��ӭ����Ա" << uadmin->getuname() << "��" << endl;
		uadmin->showMenu(*this);
		break;
	}
	updateUsers();
}