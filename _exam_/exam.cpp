#include"exam.h"

/*����ϵͳ*/

struct tt {
	int hrs, min, sec;
};

int choice = 0;

int tleft = EXAMTIME;

void starttimer();
void fsleep(double s);
int inline getpri(char c);

void fsleep(double s) { //fine-sleep
	static clock_t pclock = clock();
	pclock += s * CLK_TCK;
	if (clock() > pclock) {
		pclock = clock();
	}
	else {
		while (clock() < pclock) {
			Sleep(1);
		}
	}
}

tt timeconversion(int timestamp) { //converting ticks into timestamp
	tt newt;
	newt.hrs = timestamp / 3600; timestamp -= newt.hrs * 3600;
	newt.min = timestamp / 60; timestamp -= newt.min * 60;
	newt.sec = timestamp;
	return newt;
}

void starttimer() { //timer thread initializer
	int i = 0;
	while (1) {
		i++;
		fsleep(1.0);
		char time[MAXSTRSIZE];
		tt t = timeconversion(EXAMTIME - i);
		tleft = EXAMTIME - i;
		sprintf(time, "title ʣ��%dʱ%d��%d��", t.hrs, t.min, t.sec);
		tleft >= 0 ? system(time) : system("title ����ʱ�����");
	}
	return;
}

void Exam::examMain() { //main
	loadQBank();

	loadUsers();
	loginsys();
	userPanel();

	if (eflag) {
		thread timer(starttimer);
		if (randomPick) {
			makePaper();
		}
		else {
			makePaper(this->qlist);
		}
		system("title ѧ������ģʽ");
		while (1) {
			showQ(PAPER);
			cout << "a/d�л���Ŀ\n�ո������ǰ��Ŀ" << endl;
			cout << "c������ת���׵�δ������Ŀ\ns�����ύ�Ծ�" << endl;
			if (tleft <= 0) {
				timer.detach();
				system("cls");
				cout << "����ʱ������������Զ�����..." << endl;
				system("pause");
				gradePaper();
			}
			char ch = _getch();
			switch (ch) {
			case 'a':
				toPrior(PAPER);
				break;
			case 'd':
				toNext(PAPER);
				break;
			case ' ':
				system("cls");
				showQ(PAPER);
				answerQ();
				break;
			case 's':
				submitPaper();
				timer.detach();
				break;
			case 'c':
				jumptoblank();
				break;
			default:
				break;
			}
			system("cls");
		}
	}
	else {
		return;
	}
}

void Exam::answerQ() {
	int type = paper[cur]->getqtype();
	if (type == selection) {
		sq* nsq = dynamic_cast<sq*>(paper[cur]);
		nsq->answersq();
	}
	else if (type == judge) {
		jq* njq = dynamic_cast<jq*>(paper[cur]);
		njq->answerjq();
	}
	else if (type == completion) {
		cq* ncq = dynamic_cast<cq*>(paper[cur]);
		ncq->answercq();
	}
}

int answerWheel(int cnt) { //cycling through all answers
	int result = ((++choice)) % cnt;
	return result ? result : cnt;
}


void sq::answersq() {
	while (1) {
		cout << "q��ѡ��� | c��ʹ�ü�����\nt��ʹ����ֵ�� | �ո񱣴��" << endl;
		char ch = _getch();
		switch (ch) {
		case 'q':
			setstuans(answerWheel(optcnt));
			qstate = answered;
			system("cls");
			showsq();
			break;
		case ' ':
			cout << "���ѱ���" << endl;
			choice = 0;
			system("pause");
			return;
		case 'c':
			system("cls");
			showsq();
			secretExprCalc();
			break;
		case 't':
			system("cls");
			showsq();
			secretTruthTable();
			break;
		default:
			system("cls");
			showsq();
			break;
		}
	}
}

void jq::answerjq() {
	while (1) {
		cout << "q��ѡ��� | c��ʹ�ü�����\nt��ʹ����ֵ�� | �ո񱣴��" << endl;
		char ch = _getch();
		switch (ch) {
		case 'q':
			setstuans(answerWheel(2));
			qstate = answered;
			system("cls");
			showjq();
			cout << endl;
			break;
		case 'c':
			system("cls");
			showjq();
			secretExprCalc();
			break;
		case 't':
			system("cls");
			showjq();
			secretTruthTable();
			break;
		case ' ':
			cout << "���ѱ���" << endl;
			choice = 0;
			system("pause");
			return;
		default:
			system("cls");
			showjq();
			break;
		}
	}
}

void cq::answercq() {
	if (cin.peek() == ' ' || cin.peek() == '\n') {
		cin.get();
	}
	cout << "�������(��$����) �ո񱣴��" << endl;
	char ans[MAXSTRSIZE]; string cans;
	cout << "��������";
	cin.getline(ans, MAXSTRSIZE - 1, '$');
	cin.clear();
	cans = ans;
	setstuans(cans);
	cout << "���ѱ���" << endl;
	system("pause");
	qstate = answered;
	system("cls");
	showcq();
	return;
}

void Exam::showQBank() {
	for (int i = 0; i < qbank.size(); i++) {
		int type = qbank[i]->getqtype();
		if (type == selection) {
			sq* nsq = dynamic_cast<sq*>(qbank[i]);
			nsq->showsq();
		}
		else if (type == judge) {
			jq* njq = dynamic_cast<jq*>(qbank[i]);
			njq->showjq();
		}
		else if (type == completion) {
			cq* ncq = dynamic_cast<cq*>(qbank[i]);
			ncq->showcq();
		}
	}
}

void Exam::showPaper() {
	for (int i = 0; i < maxqcnt; i++) {
		int type = paper[i]->getqtype();
		if (type == selection) {
			sq* nsq = dynamic_cast<sq*>(paper[i]);
			nsq->showsq();
		}
		else if (type == judge) {
			jq* njq = dynamic_cast<jq*>(paper[i]);
			njq->showjq();
		}
		else if (type == completion) {
			cq* ncq = dynamic_cast<cq*>(paper[i]);
			ncq->showcq();
		}
	}
}

void sq::showsq(int withans) {
	cout << qno << '.' << "(ѡ����) ��ֵ:" << qscore << endl;
	if (withans) {
		cout << "��ȷ�� " << i2ch(correctAns) << endl;
	}
	cout << description << endl;
	for (int i = 0; i < optcnt; i++) {
		cout << i2ch(options[i].no) << ". " << options[i].description << endl;
	}
	cout << endl;
	if (qstate) {
		cout << "��Ĵ���" << i2ch(stuAns) << endl;
		cout << endl;
	}
	else {
		cout << "�㻹δ������" << endl;
		cout << endl;
	}
}

void sq::showsq(ofstream& log) {
	log << qno << '.' << "(ѡ����) ��ֵ:" << qscore << '\n';
	log << "��ȷ�� " << i2ch(correctAns) << '\n';
	log << description << '\n';
	for (int i = 0; i < optcnt; i++) {
		log << i2ch(options[i].no) << ". " << options[i].description << '\n';
	}
	log << '\n';
	if (qstate) {
		log << "��Ĵ���" << i2ch(stuAns) << '\n';
		log << '\n';
	}
	else {
		log << "��δ������" << '\n';
		log << '\n';
	}
	//log << "����÷�:" << ((qstate == correct) ? getqscore() : '0') << "\n\n";
	//log << "�������: ";
	//switch (qstate) {
	//case correct:
	//	log << "��ȷ\n";
	//	break;
	//case wrong:
	//	log << "����\n";
	//	break;
	//default:
	//	log << "δ����\n";
	//}
}

void jq::showjq(int withans) {
	cout << qno << '.' << "(�ж���) ��ֵ:" << qscore << endl;
	if (withans) {
		cout << "��ȷ�� " << i2tf(correctAns) << endl;
	}
	cout << description << endl;
	cout << i2tf(1) << " / TRUE" << endl;
	cout << i2tf(2) << " / FALSE" << endl;
	cout << endl;
	if (qstate) {
		cout << "��Ĵ��� " << i2tf(stuAns) << endl;
		cout << endl;
	}
	else {
		cout << "�㻹δ������" << endl;
		cout << endl;
	}
}

void jq::showjq(ofstream& log) {
	log << qno << '.' << "(�ж���) ��ֵ:" << qscore << '\n';
	log << "��ȷ�� " << i2tf(correctAns) << '\n';
	log << description << '\n';
	log << i2tf(1) << " / TRUE" << '\n';
	log << i2tf(2) << " / FALSE" << '\n';
	log << '\n';
	if (qstate) {
		log << "��Ĵ��� " << i2tf(stuAns) << '\n';
		log << '\n';
	}
	else {
		log << "��δ������" << '\n';
		log << '\n';
	}
}

void cq::showcq(int withans) {
	cout << qno << '.' << "(�����) ��ֵ:" << qscore << endl;
	if (withans) { 
		cout << "��ȷ�� " << correctAns << endl; 
	}
	cout << description << endl;
	cout << endl;
	if (qstate) {
		cout << "��Ĵ��� " << stuAns << "($)" << endl;
	}
	else {
		cout << "�㻹δ������" << endl;
		cout << endl;
	}
}

void cq::showcq(ofstream& log) {
	log << qno << '.' << "(�����) ��ֵ:" << qscore << '\n';
	log << "��ȷ�� " << correctAns << '\n';
	log << description << '\n';
	log << '\n';
	if (qstate) {
		log << "��Ĵ��� " << stuAns << '\n';
	}
	else {
		log << "��δ������" << '\n';
		log << '\n';
	}
}

void Exam::showQ(int src) {
	if (src) {
		int type = paper[cur]->getqtype();
		if (type == selection) {
			sq* nsq = dynamic_cast<sq*>(paper[cur]);
			nsq->showsq();
		}
		else if (type == judge) {
			jq* njq = dynamic_cast<jq*>(paper[cur]);
			njq->showjq();
		}
		else if (type == completion) {
			cq* ncq = dynamic_cast<cq*>(paper[cur]);
			ncq->showcq();
		}
	}
	else {
		int type = qbank[cur]->getqtype();
		if (type == selection) {
			sq* nsq = dynamic_cast<sq*>(qbank[cur]);
			nsq->showsq(1);
		}
		else if (type == judge) {
			jq* njq = dynamic_cast<jq*>(qbank[cur]);
			njq->showjq(1);
		}
		else if (type == completion) {
			cq* ncq = dynamic_cast<cq*>(qbank[cur]);
			ncq->showcq(1);
		}
	}
}

void Exam::toPrior(int src) { //jump to the prior question
	if (cur == bot) {
		cout << "�Ѿ��ǵ�һ��!" << endl;
		system("pause");
	}
	else {
		cur--;
	}
}

void Exam::toNext(int src) { //jump to the next question
	if (src) {
		if (cur == top) {
			cout << "�Ѿ������һ��!" << endl;
			system("pause");
		}
		else {
			cur++;
		}
	}
	else {
		if (cur == btop) {
			cout << "�Ѿ������һ��!" << endl;
			system("pause");
		}
		else {
			cur++;
		}
	}
}

void Exam::jumptoblank() { //skip to the first unanswered question
	int unanswered = 0;
	for (auto& qv : paper) {
		int type = qv->getqtype();
		if (type == selection) {
			sq* nsq = dynamic_cast<sq*>(qv);
			if (nsq->getqstate() == blank) {
				cur = unanswered;
				return;
			}
		}
		else if (type == judge) {
			jq* njq = dynamic_cast<jq*>(qv);
			if (njq->getqstate() == blank) {
				cur = unanswered;
				return;
			}
		}
		else if (type == completion) {
			cq* ncq = dynamic_cast<cq*>(qv);
			if (ncq->getqstate() == blank) {
				cur = unanswered;
				return;
			}
		}
		unanswered++; //index
	}
}

bool Exam::checkblank() {
	for (auto& qv : paper) {
		int type = qv->getqtype();
		if (type == selection) {
			sq* nsq = dynamic_cast<sq*>(qv);
			if (nsq->getqstate() == blank) {
				return 1;
			}
		}
		else if (type == judge) {
			jq* njq = dynamic_cast<jq*>(qv);
			if (njq->getqstate() == blank) {
				return 1;
			}
		}
		else if (type == completion) {
			cq* ncq = dynamic_cast<cq*>(qv);
			if (ncq->getqstate() == blank) {
				return 1;
			}
		}
	}
	return 0;
}

void Exam::submitPaper() {
	if (checkblank()) {	
		cout << "����Ŀ��δ�����Ƿ���Ҫ����? (y/n)" << endl;
		char choice;
		cin >> choice;
		if (choice == 'y') {
			system("cls");
			cout << "�Ѿ�����!" << endl;
			gradePaper();
			system("pause");
		}
		else {
			if (tleft <= 0) {
				cout << "����ʱ������������Զ�����..." << endl;
				gradePaper();
			}
			return;
		}
	}
	else {
		cout << "�Ƿ�Ҫ����? (y/n)" << endl;
		char choice;
		cin >> choice;
		if (choice == 'y') {
			system("cls");
			cout << "�Ѿ�����!" << endl;
			gradePaper();
			system("pause");
		}
		else {
			if (tleft <= 0) {
				cout << "����ʱ������������Զ�����..." << endl;
				gradePaper();
			}
			return;
		}
	}
}

void Exam::gradePaper() {
	for (auto& qv : paper) {
		int type = qv->getqtype();
		if (type == selection) {
			sq* nsq = dynamic_cast<sq*>(qv);
			if (nsq->iscorrect()) {
				sscore += nsq->getqscore();
			}
		}
		else if (type == judge) {
			jq* njq = dynamic_cast<jq*>(qv);
			if (njq->iscorrect()) {
				sscore += njq->getqscore();
			}
		}
		else if (type == completion) {
			cq* ncq = dynamic_cast<cq*>(qv);
			if (ncq->iscorrect()) {
				sscore += ncq->getqscore();
			}
		}
	}
	system("cls");
	cout << "���յ÷֣�" << sscore << " / " << totalscore << endl;
	cout << "�÷��ʣ�" << (sscore * 100.0) / totalscore << '%' << endl;
	genLog();

	exit(EXAM_FINISHED);
}