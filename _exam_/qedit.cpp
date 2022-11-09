#include"exam.h"

/*������ϵͳ*/

void drawlines();

void Exam::customizePaper() {
	system("title ѧ���ɼ�����-���ϵͳ");
	system("cls");
	cout << "��ǰ���ģʽ��" << (randomPick ? "���" : "��ʦָ��") << endl;
	cout << "�Ƿ�Ҫ�����������? (y/n)" << endl;
	char ch; cin >> ch;
	if (ch == 'y') {
		cout << "�������µ����ģʽ: (1: ��� 0: ָ��)" << endl;
		int op; cin >> op;
		if (op) {
			randomPick = 1;
			cout << "�����������Ŀ����Q\n����Q���� 0��Q��" << maxqcnt << endl;
			int cnt; cin >> cnt; paperqcnt = cnt;
			updateQList();
			updateQBank();
			cout << "������ø�����ϣ�" << endl;
			system("pause");
		}
		else {
			randomPick = 0;
			cout << "�����뿼����Ŀ��Q\n����Q���� 0��Q��" << maxqcnt << endl;
			cin.get();
			int cnt; cin >> cnt; paperqcnt = cnt;
			cout << "������������� (�Կո�ָ����)\n���Q���� 0��Q��" << maxqcnt << endl;
			int cur;
			qlist = new int[cnt];
			for (int i = 0; i < cnt; i++) {
				int ibuf; cin >> ibuf;
				if (ibuf <= 0 || ibuf >= maxqcnt + 1) {
					cout << "��Ч���룡" << endl;
				}
				else {
					qlist[i] = ibuf;
				}
			}
			updateQList();
			updateQBank();
			cout << "������ø�����ϣ�" << endl;
			system("pause");
		}
	}
	else {
		cout << "���������ĸ��ģ�" << endl;
		system("pause");
		return;
	}
}

void Exam::editQBank() {
	cur = bot;
	while (1) {
		system("title ���༭ϵͳ");
		system("cls");
		showQ(BANK);
		drawlines();
		cout << "���༭ϵͳ" << endl;
		cout << "n�������ĩβ�������Ŀ\ne�����ĵ�ǰ��Ŀ\na/d���л���Ŀ\nq��ɾ����Ŀ\nx:�˳�ϵͳ" << endl;
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
			cout << "�����˳����༭ϵͳ��" << endl;
			system("pause");
			updateQBank();
			return;
		default:
			cout << "��Ч����" << endl;
			break;
		}
	}
}

void Exam::addQ() {
	system("cls");
	system("title ���༭ϵͳ-�����Ŀ");
	cout << "����������Ŀ���� (1��ѡ�� 2���ж� 3�����)" << endl;
	cin.clear();
	int qt = _getch() - '0';
	if (qt == selection) {
		cout << "ѡ���⣺" << endl;
		int no, cnt, cans; double score; char dbuf[MAXSTRSIZE]; string desc;
		no = maxqcnt + 1;
		cout << "���������룺ѡ���� ��ֵ ��ȷ��" << endl;
		cin >> cnt >> score >> cans;
		cin.get();
		cout << "���������(��$����)" << endl;
		cin.getline(dbuf, MAXSTRSIZE - 1, '$');
		desc = dbuf;
		cin.get();
		sq* n = new sq(no, cnt, score, cans, selection, desc);

		n->getopt();

		qbank.push_back(n);
	}
	else if (qt == judge) {
		cout << "�ж��⣺" << endl;
		int no, cnt, cans; double score; char dbuf[MAXSTRSIZE]; string desc;
		no = maxqcnt + 1;
		cout << "���������룺��ֵ ��ȷ��" << endl;
		cin >> score >> cans;
		cin.get();
		cout << "���������(��$����)" << endl;
		cin.getline(dbuf, MAXSTRSIZE - 1, '$');
		desc = dbuf;
		cin.get();
		jq* j = new jq(no, score, cans, judge, desc);
		qbank.push_back(j);
	}
	else if (qt == completion) {
		cout << "����⣺" << endl;
		int no; double score; 
		char dbuf[MAXSTRSIZE]; string desc;
		char abuf[MAXSTRSIZE]; string ans;
		no = maxqcnt + 1;
		cout << "�������ֵ��" << endl;
		cin >> score;
		cin.get();
		cout << "���������(��$����)" << endl;
		cin.getline(dbuf, MAXSTRSIZE - 1, '$');
		desc = dbuf;
		cin.get();
		cout << "�������(��$����)" << endl;
		cin.getline(abuf, MAXSTRSIZE - 1, '$');
		ans = abuf;
		cin.get();
		cq* c = new cq(no, score, ans, completion, desc);
		qbank.push_back(c);
	}
	else {
		cout << "δ֪��Ŀ����\n���������Ŀ" << endl;
		system("pause");
		system("cls");
		return;
	}
	cout << "��Ŀ�����" << endl;
	system("pause");
	btop++;
	top++; cur = top; maxqcnt++;
	return;
}

void Exam::delQ() {
	system("cls");
	system("title ���༭ϵͳ-ɾ����Ŀ");
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
	cout << "��Ŀ��ɾ��" << endl;
	system("pause");
	top--; btop--; maxqcnt--;
	cur == bot ? cur = bot : cur--;
	return;
}

void Exam::editQ() {
	system("cls");
	system("title ���༭ϵͳ-�༭��Ŀ");
	int qt = qbank[cur]->getqtype();
	if (qt == selection) {
		cout << "�༭ѡ���⣺" << endl;
		int no, cnt, cans; double score; char dbuf[MAXSTRSIZE]; string desc;
		no = maxqcnt + 1;
		cout << "������������ĺ�ģ�ѡ���� ��ȷ�� ��ֵ" << endl;
		cin >> cnt >> cans >> score;
		cout << "��������ĺ�����(��$����)" << endl;
		cin.get();
		cin.getline(dbuf, MAXSTRSIZE - 1, '$');
		desc = dbuf;
		cin.get();
		sq* q = dynamic_cast<sq*>(qbank[cur]);
		q->setoptcnt(cnt); q->setcorrectans(cans); q->setqscore(score);
		q->setdesc(desc);
		cout << "�����������µ�ѡ�" << endl;
		q->editopt();
		cout << "�ѱ�����ģ�" << endl;
	}
	else if (qt == judge) {
		cout << "�༭�ж��⣺" << endl;
		int no, cnt, cans; double score; char dbuf[MAXSTRSIZE]; string desc;
		no = maxqcnt + 1;
		cout << "������������ĺ�ģ���ȷ�� ��ֵ" << endl;
		cin >> cans >> score;
		cin.get();
		cout << "��������ĺ�����(��$����)" << endl;
		cin.getline(dbuf, MAXSTRSIZE - 1, '$');
		desc = dbuf;
		cin.get();
		jq* q = dynamic_cast<jq*>(qbank[cur]);
		q->setcorrectans(cans); q->setqscore(score);
		q->setdesc(desc);
		cout << "�ѱ�����ģ�" << endl;
	}
	else if (qt == completion) {
		cout << "����⣺" << endl;
		int no; double score;
		char dbuf[MAXSTRSIZE]; string desc;
		char abuf[MAXSTRSIZE]; string ans;
		no = maxqcnt + 1;
		cout << "�������ֵ��" << endl;
		cin >> score;
		cin.get();
		cout << "���������(��$����)" << endl;
		cin.getline(dbuf, MAXSTRSIZE - 1, '$');
		desc = dbuf;
		cin.get();
		cout << "�������(��$����)" << endl;
		cin.getline(abuf, MAXSTRSIZE - 1, '$');
		ans = abuf;
		cin.get();
		cq* q = dynamic_cast<cq*>(qbank[cur]);
		q->setcorrectans(ans); q->setqscore(score);
		q->setdesc(desc);
		cout << "�ѱ�����ģ�" << endl;
	}
	return;
}