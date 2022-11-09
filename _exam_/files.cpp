#include"exam.h"

/*文件读写系统*/

#define Q_SRC "./files/qbank.q"
#define QLIST_SRC "./files/qlist.q"
#define QRLIST_SRC "./files/qrlist.q"

void Exam::loadQBank() {
	ifstream src;
	src.open(Q_SRC);

	src >> maxqcnt;
	src >> randomPick;

	int qno = 0;

	while (1) {
		if (src.peek() == EOF) {
			maxqcnt = qno;
			break;
		}
		int qt;
		src >> qt;

		if (qt == selection) {

			double qscore;
			char desc[MAXSTRSIZE]; string descstr;
			int optcnt, cAns;

			src >> optcnt >> qscore >> cAns;
			//totalscore += qscore;

			src.get();
			src.getline(desc, MAXSTRSIZE - 1, '$');
			descstr = desc;
			sq* newsq = new sq(++qno, optcnt, qscore, cAns, qt, descstr);
			newsq->getopt(src);
			qbank.push_back(newsq);
		}
		else if (qt == judge) {

			double qscore;
			char desc[MAXSTRSIZE]; string descstr;
			int optcnt, cAns;

			src >> optcnt >> qscore >> cAns;
			//totalscore += qscore;

			src.get();
			src.getline(desc, MAXSTRSIZE - 1, '$');
			descstr = desc;
			jq* newjq = new jq(++qno, qscore, cAns, qt, descstr);
			qbank.push_back(newjq);

		}
		else if (qt == completion) {
			double qscore;
			char desc[MAXSTRSIZE]; string descstr;
			char cAns[MAXSTRSIZE]; string ccAns;

			src >> qscore;

			src.get();
			src.getline(desc, MAXSTRSIZE - 1, '$');
			descstr = desc;
			src.getline(cAns, MAXSTRSIZE - 1, '$');
			ccAns = cAns;
			cq* newcq = new cq(++qno, qscore, ccAns, qt, descstr);
			qbank.push_back(newcq);
		}
	}
	src.close();
	cur = 0, bot = 0; //setting indexes
	btop = maxqcnt - 1;
	top = maxqcnt - 1;
}

void sq::getopt(ifstream& src) {
	for (int i = 1; i < optcnt + 1; i++) {
		opt nopt;
		if (src.peek() == EOF) {
			return;
		}
		char desc[MAXSTRSIZE]; string descstr;
		src.getline(desc, MAXSTRSIZE - 1, '$');
		nopt.description = desc;
		nopt.no = i;
		options.push_back(nopt);
	}
	src.get();
}

void sq::getopt(ofstream& dst) {
	for (int i = 0; i < optcnt; i++) {
		dst << options[i].description << '$';
	}
}

void sq::getopt() {
	for (int i = 0; i < optcnt; i++) {
		cout << "请输入" << (char)('A' + i) << "选项(以$结束)" << endl;
		char abuf[MAXSTRSIZE]; string ans;
		opt nopt;
		cin.getline(abuf, MAXSTRSIZE - 1, '$');
		ans = abuf;
		cin.get();
		nopt.description = ans;
		nopt.no = i + 1;
		options.push_back(nopt);
	}
	return;
}

void sq::editopt() {
	options.clear();
	for (int i = 0; i < optcnt; i++) {
		cout << "请输入" << (char)('A' + i) << "选项(以$结束)" << endl;
		char abuf[MAXSTRSIZE]; string ans;
		opt nopt;
		cin.getline(abuf, MAXSTRSIZE - 1, '$');
		ans = abuf;
		cin.get();
		nopt.description = ans;
		nopt.no = i + 1;
		options.push_back(nopt);
	}
	return;
}

void Exam::makePaper() { //making paper from randomly picked questions
	ifstream src;
	src.open(QRLIST_SRC);
	src >> paperqcnt;
	src.close();
	//paperqcnt = maxqcnt;

	qlist = new int[paperqcnt];
	int arrsize = 0;
	int i = 0;

	while (true) {  //generating qno array w/o repetition

		int tmp = (rand() % maxqcnt) + 1;
		if (arrsize == 0) { 
			qlist[i++] = tmp;
			arrsize++;
		}
		else {
			bool dflag = false;
			for (int j = 0; j < arrsize; j++) {
				if (qlist[j] == tmp) {
					dflag = true;
					break;
				}
			}
			if (!dflag) {
				qlist[i++] = tmp;
				arrsize++;
			}
		}
		if (arrsize == paperqcnt) {
			break;
		}

	}
	for (int i = 0; i < paperqcnt; i++) { //pushing q into vector

		int no = qlist[i] - 1;

		int type = qbank[no]->getqtype();
		if (type == selection) {
			sq* nsq = dynamic_cast<sq*>(qbank[no]);
			nsq->setqno(i + 1);
			paper.push_back(nsq);
			totalscore += nsq->getqscore();
		}
		else if (type == judge) {
			jq* njq = dynamic_cast<jq*>(qbank[no]);
			njq->setqno(i + 1);
			paper.push_back(njq);
			totalscore += njq->getqscore();
		}
		else if (type == completion) {
			cq* ncq = dynamic_cast<cq*>(qbank[no]);
			ncq->setqno(i + 1);
			paper.push_back(ncq);
			totalscore += ncq->getqscore();
		}
	}
	bot = 0; //resetting indexes
	cur = 0;
	top = paperqcnt - 1; btop = maxqcnt - 1;
	return;

}

void Exam::makePaper(int* qlist) { //making paper with given qno array
	ifstream src;
	src.open(QLIST_SRC);
	src >> paperqcnt;
	qlist = new int[paperqcnt];
	for (int i = 0; i < paperqcnt; i++) { //load qno array
		src >> qlist[i];
	}
	for (int i = 0; i < paperqcnt; i++) {

		int no = qlist[i] - 1;

		int type = qbank[no]->getqtype();
		if (type == selection) {
			sq* nsq = dynamic_cast<sq*>(qbank[no]);
			nsq->setqno(i + 1);
			paper.push_back(nsq);
			totalscore += nsq->getqscore();
		}
		else if (type == judge) {
			jq* njq = dynamic_cast<jq*>(qbank[no]);
			njq->setqno(i + 1);
			paper.push_back(njq);
			totalscore += njq->getqscore();
		}
		else if (type == completion) {
			cq* ncq = dynamic_cast<cq*>(qbank[no]);
			ncq->setqno(i + 1);
			paper.push_back(ncq);
			totalscore += ncq->getqscore();
		}
	}
	bot = 0; //resetting indexes
	cur = 0;
	top = paperqcnt - 1;
	src.close();
}

void Exam::updateQBank() {
	ofstream dst;
	dst.open(Q_SRC);
	dst << maxqcnt << ' ';
	dst << randomPick ? '1' : '0';
	dst << '\n';
	int curqes = 0;
	for (auto& qv : qbank) {
		int type = qv->getqtype();
		if (type == selection) {
			sq* nsq = dynamic_cast<sq*>(qv);
			dst << selection << ' ' << nsq->getoptcnt() << ' '
				<< (nsq->getqscore()) << ' ' << nsq->getcorrectans() << ' '
				<< nsq->getdesc() << '$';
			nsq->getopt(dst);
		}
		else if (type == judge) {
			jq* njq = dynamic_cast<jq*>(qv);
			dst << judge << ' ' << '2' << ' '
				<< (njq->getqscore()) << ' ' << njq->getcorrectans() << ' '
				<< njq->getdesc() << '$';
		}
		else if (type == completion) {
			cq* ncq = dynamic_cast<cq*>(qv);
			dst << completion << ' ' << (ncq->getqscore()) << ' '
				<< ncq->getdesc() << '$' << ncq->getcorrectans() << '$';
		}
		if (++curqes != maxqcnt) {
			dst << '\n';
		}
	}
	dst.close();
}

void Exam::updateQList() {
	if (randomPick) {
		ofstream dst;
		dst.open(QRLIST_SRC);
		dst << paperqcnt <<'\n';
		dst.close();
	}
	else {
		ofstream dst;
		dst.open(QLIST_SRC);
		dst << paperqcnt << '\n';
		for (int i = 0; i < paperqcnt; i++) {
			dst << qlist[i];
			if (i == paperqcnt - 1) {
				dst << '\n';
			}
			else {
				dst << ' ';
			}
		}
		dst.close();
	}
	return;
}

Exam::~Exam() {
	for (auto& qv : qbank) {
		delete qv;
	}
	for (auto& sv : stus) {
		delete sv;
	}
	for (auto& uv : users) {
		int ut = uv->getutype();
		if (ut == ADMINISTRATOR) {
			admin* n = dynamic_cast<admin*>(uv);
			delete n;
		}		
		if (ut == TEACHER) {
			teach* n = dynamic_cast<teach*>(uv);
			delete n;
		}
	}
}