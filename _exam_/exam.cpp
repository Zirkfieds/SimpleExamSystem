#include"exam.h"

/*考试系统*/

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
		sprintf(time, "title 剩余%d时%d分%d秒", t.hrs, t.min, t.sec);
		tleft >= 0 ? system(time) : system("title 考试时间结束");
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
		system("title 学生考试模式");
		while (1) {
			showQ(PAPER);
			cout << "a/d切换题目\n空格键作答当前题目" << endl;
			cout << "c可以跳转至首道未作答题目\ns可以提交试卷" << endl;
			if (tleft <= 0) {
				timer.detach();
				system("cls");
				cout << "考试时间结束！即将自动交卷..." << endl;
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
		cout << "q键选择答案 | c键使用计算器\nt键使用真值表 | 空格保存答案" << endl;
		char ch = _getch();
		switch (ch) {
		case 'q':
			setstuans(answerWheel(optcnt));
			qstate = answered;
			system("cls");
			showsq();
			break;
		case ' ':
			cout << "答案已保存" << endl;
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
		cout << "q键选择答案 | c键使用计算器\nt键使用真值表 | 空格保存答案" << endl;
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
			cout << "答案已保存" << endl;
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
	cout << "请输入答案(以$结束) 空格保存答案" << endl;
	char ans[MAXSTRSIZE]; string cans;
	cout << "作答区域：";
	cin.getline(ans, MAXSTRSIZE - 1, '$');
	cin.clear();
	cans = ans;
	setstuans(cans);
	cout << "答案已保存" << endl;
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
	cout << qno << '.' << "(选择题) 分值:" << qscore << endl;
	if (withans) {
		cout << "正确答案 " << i2ch(correctAns) << endl;
	}
	cout << description << endl;
	for (int i = 0; i < optcnt; i++) {
		cout << i2ch(options[i].no) << ". " << options[i].description << endl;
	}
	cout << endl;
	if (qstate) {
		cout << "你的答案是" << i2ch(stuAns) << endl;
		cout << endl;
	}
	else {
		cout << "你还未作答本题" << endl;
		cout << endl;
	}
}

void sq::showsq(ofstream& log) {
	log << qno << '.' << "(选择题) 分值:" << qscore << '\n';
	log << "正确答案 " << i2ch(correctAns) << '\n';
	log << description << '\n';
	for (int i = 0; i < optcnt; i++) {
		log << i2ch(options[i].no) << ". " << options[i].description << '\n';
	}
	log << '\n';
	if (qstate) {
		log << "你的答案是" << i2ch(stuAns) << '\n';
		log << '\n';
	}
	else {
		log << "你未作答本题" << '\n';
		log << '\n';
	}
	//log << "本题得分:" << ((qstate == correct) ? getqscore() : '0') << "\n\n";
	//log << "作答情况: ";
	//switch (qstate) {
	//case correct:
	//	log << "正确\n";
	//	break;
	//case wrong:
	//	log << "错误\n";
	//	break;
	//default:
	//	log << "未作答\n";
	//}
}

void jq::showjq(int withans) {
	cout << qno << '.' << "(判断题) 分值:" << qscore << endl;
	if (withans) {
		cout << "正确答案 " << i2tf(correctAns) << endl;
	}
	cout << description << endl;
	cout << i2tf(1) << " / TRUE" << endl;
	cout << i2tf(2) << " / FALSE" << endl;
	cout << endl;
	if (qstate) {
		cout << "你的答案是 " << i2tf(stuAns) << endl;
		cout << endl;
	}
	else {
		cout << "你还未作答本题" << endl;
		cout << endl;
	}
}

void jq::showjq(ofstream& log) {
	log << qno << '.' << "(判断题) 分值:" << qscore << '\n';
	log << "正确答案 " << i2tf(correctAns) << '\n';
	log << description << '\n';
	log << i2tf(1) << " / TRUE" << '\n';
	log << i2tf(2) << " / FALSE" << '\n';
	log << '\n';
	if (qstate) {
		log << "你的答案是 " << i2tf(stuAns) << '\n';
		log << '\n';
	}
	else {
		log << "你未作答本题" << '\n';
		log << '\n';
	}
}

void cq::showcq(int withans) {
	cout << qno << '.' << "(填空题) 分值:" << qscore << endl;
	if (withans) { 
		cout << "正确答案 " << correctAns << endl; 
	}
	cout << description << endl;
	cout << endl;
	if (qstate) {
		cout << "你的答案是 " << stuAns << "($)" << endl;
	}
	else {
		cout << "你还未作答本题" << endl;
		cout << endl;
	}
}

void cq::showcq(ofstream& log) {
	log << qno << '.' << "(填空题) 分值:" << qscore << '\n';
	log << "正确答案 " << correctAns << '\n';
	log << description << '\n';
	log << '\n';
	if (qstate) {
		log << "你的答案是 " << stuAns << '\n';
	}
	else {
		log << "你未作答本题" << '\n';
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
		cout << "已经是第一题!" << endl;
		system("pause");
	}
	else {
		cur--;
	}
}

void Exam::toNext(int src) { //jump to the next question
	if (src) {
		if (cur == top) {
			cout << "已经是最后一题!" << endl;
			system("pause");
		}
		else {
			cur++;
		}
	}
	else {
		if (cur == btop) {
			cout << "已经是最后一题!" << endl;
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
		cout << "有题目仍未作答，是否仍要交卷? (y/n)" << endl;
		char choice;
		cin >> choice;
		if (choice == 'y') {
			system("cls");
			cout << "已经交卷!" << endl;
			gradePaper();
			system("pause");
		}
		else {
			if (tleft <= 0) {
				cout << "考试时间结束！即将自动交卷..." << endl;
				gradePaper();
			}
			return;
		}
	}
	else {
		cout << "是否要交卷? (y/n)" << endl;
		char choice;
		cin >> choice;
		if (choice == 'y') {
			system("cls");
			cout << "已经交卷!" << endl;
			gradePaper();
			system("pause");
		}
		else {
			if (tleft <= 0) {
				cout << "考试时间结束！即将自动交卷..." << endl;
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
	cout << "最终得分：" << sscore << " / " << totalscore << endl;
	cout << "得分率：" << (sscore * 100.0) / totalscore << '%' << endl;
	genLog();

	exit(EXAM_FINISHED);
}