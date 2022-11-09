#include"exam.h"

/*考试记录*/

void Exam::genLog() {

	ustu->setparticipated(true);
	ustu->setsscore(sscore);

	enum TIMETYPE {
		y = 0, m, d, h, min, s
	};

	time_t tt;
	time(&tt);
	auto t = localtime(&tt);

	string tstring[6];
	tstring[y] = to_string(t->tm_year + 1900);
	tstring[m] = (t->tm_mon + 1 > 9) ? to_string(t->tm_mon + 1) : '0' + to_string(t->tm_mon + 1);
	tstring[d] = (t->tm_mday > 9) ? to_string(t->tm_mday) : '0' + to_string(t->tm_mday);
	tstring[h] = (t->tm_hour > 9) ? to_string(t->tm_hour) : '0' + to_string(t->tm_hour);
	tstring[min] = (t->tm_min > 9) ? to_string(t->tm_min) : '0' + to_string(t->tm_min);
	tstring[s] = (t->tm_sec > 9) ? to_string(t->tm_sec) : '0' + to_string(t->tm_sec);

	string filename = "./log/";
	int cap = 6;
	for (int i = 0; i < cap; i++) {
		if (i == cap - 1) {
			tstring[i] += "_log.txt";
		}
		else {
			tstring[i] += '_';
		}
		filename.append(tstring[i]);
	}
	ofstream log;
	log.open(filename);

	if (ustu) {
		log << "学生" << ustu->getuname() << " 学号" << ustu->getuno() << "完成了考试" << '\n';
		log << "最终得分：" << sscore << " / " << totalscore << '\n';
		log << "得分率：" << (sscore * 100.0) / totalscore << '%' << "\n\n";
	}

	for (auto& q : paper) {
		int type = q->getqtype();
		if (type == selection) {
			sq* nsq = dynamic_cast<sq*>(q);
			nsq->showsq(log);
		}
		else if (type == judge) {
			jq* njq = dynamic_cast<jq*>(q);
			njq->showjq(log);
		}
		else if (type == completion) {
			cq* ncq = dynamic_cast<cq*>(q);
			ncq->showcq(log);
		}
	}

	log.close();

	updateUsers();
}