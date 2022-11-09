#pragma once

#ifndef EXAM_SYS_H
#define EXAM_SYS_H

#include<iostream>
#include<string>
#include<iomanip>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<vector>
#include<regex>
#include<list>
#include<fstream>
#include<conio.h>
#include<cstdlib>
#include<thread>
#include<Windows.h>
#include<cwchar>
#include<sstream>
#include<stack>

using namespace std;

#define MAXQBANKSIZE 128
#define MAXSTRSIZE 256

#define EXAMTIME 5400 //seconds

//int-char conversions
constexpr char i2ch(int x) { return '@' + x; }
constexpr char i2tf(int x) { return x == 1 ? 'T' : 'F'; }

enum EXIT_INFO {
	CANNOT_FIND_USER = -4, UNKNOWN_IDENTITY, FILES_DO_NOT_EXIST,
	FORCEFULLY_EXIT, EXAM_FINISHED, EXIT_WITH_STU,
	EXIT_WITH_NON_STU,
};

class Exam;

typedef struct Option { //basic struct for options
	string description;
	int no;
} opt;

class Question { //base class for questions
protected:

	enum QTYPE {
		undefined = 0, selection, judge, completion
	};
	enum QSTATE {
		blank = 0, answered, correct, wrong
	};

	int qno;
	int qtype;
	string description;
	double qscore;
	int qstate;

public:

	Question(int no, int type, string& desc, double score) {
		qno = no; qtype = type; description = desc;
		qscore = score; qstate = blank;
	}

	int getqno() { return qno; }
	void setqno(int no) { qno = no; return; }

	int getqtype() { return qtype; }
	void setqtype(int type) { qtype = type; return; }

	string& getdesc() { return description; }
	void setdesc(string& desc) { description = desc; return; }

	double getqscore() { return qscore; }
	void setqscore(double score) { qscore = score; return; }

	int getqstate() { return qstate; }
	void setqstate(int state) { qstate = state; return; }

	virtual ~Question() = default;

};

typedef Question q;

class selectionQuestion : virtual public q { //selection question
protected:

	int optcnt;
	vector<opt> options;

	int correctAns;
	int stuAns;

public:

	selectionQuestion(int no, int cnt, double score, int cans, int type, string& desc) :
		Question(no, type, desc, score) {
		optcnt = cnt; correctAns = cans; stuAns = 0;
	}

	void setoptcnt(int cnt) { optcnt = cnt; return; }
	int getoptcnt() { return optcnt; }
	void getopt(ifstream& src);
	void getopt(ofstream& dst);
	void getopt();
	void editopt();

	void setcorrectans(int ans) { correctAns = ans; return; }
	bool iscorrect() { return correctAns == stuAns ? true : false; }
	int getcorrectans() { return correctAns; }

	void answersq();
	void setstuans(int ans) { stuAns = ans; return; }

	void showsq(int withans = 0);
	void showsq(ofstream& log);

	friend int answerWheel(int);

};  typedef selectionQuestion sq;

class judgeQuestion : virtual public q { //judge question
protected:

	int correctAns;
	int stuAns;

public:

	judgeQuestion(int no, double score, int cans, int type, string& desc) :
		Question(no, type, desc, score) {
		correctAns = cans; stuAns = 0;
	}

	void setcorrectans(int ans) { correctAns = ans; return; }
	bool iscorrect() { return correctAns == stuAns ? true : false; }
	int getcorrectans() { return correctAns; }

	void answerjq();
	void setstuans(int ans) { stuAns = ans; return; }

	void showjq(int withans = 0);
	void showjq(ofstream& log);

	friend int answerWheel(int);

}; typedef judgeQuestion jq;

class completionQuestion : virtual public q { //completion question
protected:

	string correctAns;
	string stuAns;

public:

	completionQuestion(int no, double score, string& cans, int type, string& desc) :
	Question(no, type, desc, score) {
		correctAns = cans;
	}

	void setcorrectans(string& ans) { correctAns = ans; return; }
	bool iscorrect() { return (!correctAns.compare(stuAns)) ? true : false; }
	string& getcorrectans() { return correctAns; }

	void answercq();
	void setstuans(string& ans) { stuAns = ans; return; }

	void showcq(int withans = 0);
	void showcq(ofstream& log);

}; typedef completionQuestion cq;

class User { //basic class for users
protected:
	enum UTYPE {
		ADMINISTRATOR = 0, STUDENT, TEACHER
	};

	string uno;
	string uname;
	string upw;

	int utype;

public:
	User(string& no, string& name, string& pw) {
		uno = no; uname = name; upw = pw;
	}

	string& getuno() { return uno; }
	void setuno(string& no) { uno = no; return; }

	string& getuname() { return uname; }
	void setuname(string& name) { uname = name; return; }

	string& getupw() { return upw; }
	void setupw(string& pw) { upw = pw; return; }
	bool pwCorrect(string& pwbuf) { return pwbuf.compare(upw) ? false : true; }

	int getutype() { return utype; }
	void setutype(int type) { utype = type; return; }

	friend inline void drawlines();
	friend inline void drawlines(int);

	virtual void showMenu(Exam& E) = 0;

}; typedef User u;

class Student : virtual public u { //permission group: students
protected:

	bool participated;
	double sscore;

public:

	Student(string& no, string& name, string& pw, int pstate, double score) :
	User(no, name, pw) {
		participated = pstate;
		sscore = score;
		utype = STUDENT;
	}

	void showMenu(Exam& E);
	void showstu();

	bool participatedExam(bool pflag) { 
		if (pflag) { return (participated) ? true : false; }
		else { return (sscore == -1) ? false : true; }
	}
	void setparticipated(bool flag) { participated = flag; return; }
	

	double getsscore() { return sscore; }
	void setsscore(double score) { sscore = score; return; }

}; typedef Student stu;

class Teacher : virtual public u { //permission group: teachers
protected:
public:

	Teacher(string& no, string& name, string& pw) :
		User(no, name, pw) {
		utype = TEACHER;
	}

	void showMenu(Exam& E);
	void showteach();

	void querystu(Exam& E);


}; typedef Teacher teach;


class Administrator : virtual public teach { //permission group: admins
protected:
public:

	Administrator(string& no, string& name, string& pw) :
		teach(no, name, pw), User(no, name, pw) {
		utype = ADMINISTRATOR;
	}

	void showMenu(Exam& E);
	void showadmin();

	void querystu(Exam& E);

}; typedef Administrator admin;


class Exam { //examsys main
protected:
	enum UTYPE {
		ADMINISTRATOR = 0, STUDENT, TEACHER
	};
	enum QTYPE {
		undefined = 0, selection, judge, completion
	};
	enum QSTATE {
		blank = 0, answered, correct, wrong
	};
	enum QSRC {
		BANK = 0, PAPER
	};

	int maxqcnt;
	int paperqcnt;

	vector<u*> users;
	vector<stu*> stus;
	int utp;
	stu* ustu; teach* uteach; admin* uadmin;
	int ucnt;

	vector<q*> qbank;
	int cur, bot, top, btop; //index

	bool randomPick = false;
	vector<q*> paper;
	int* qlist;

	double sscore = 0;
	double totalscore = 0;

	bool eflag = false;

public:
	Exam(int maxq, int paperq) {
		maxqcnt = maxq; paperqcnt = paperq;
		totalscore = 0;
	}
	Exam() {
		totalscore = 0;
	}
	//USERS
	void loadUsers();
	void showUsers();
	void updateUsers();
	void loginsys();
	void userPanel();
	void customizePaper();

	//FILES
	void loadQBank();
	void showQBank();
	void editQBank();
	void updateQBank();
	void updateQList();

	//EXAM
	void examMain();
	void makePaper();
	void makePaper(int* qlist);
	void showPaper(); 
	bool geteflag() { return eflag; }
	void seteflag(int flag) { eflag = flag; return; }

	//QUESTIONS
	void showQ(int src);
	void answerQ();
	void toPrior(int src);
	void toNext(int src);
	void delQ();
	void addQ();
	void editQ();

	//SUBMIT
	void jumptoblank();
	bool checkblank();
	void submitPaper();
	void gradePaper();
	void genLog();

	//STUDENTS
	void getsscore();
	void getallscore();
	void rankscore();
	void editstustate();


	friend inline void drawlines();
	friend inline void drawlines(int);
	friend void starttimer();

	~Exam();
};

//MISC
bool scmpdown(stu* s1, stu* s2);
bool scmprise(stu* s1, stu* s2);

//SECRET CALCULATOR
void secretExprCalc();
bool exprLegal(string& expr);
int inline getcpri(char c);
bool checkBrackets(string& expr);
string exprconversion(string& inexpr);
double calcexpr(string& expr);

//SECRET TRUTH TABLE
void secretTruthTable();
void dispt(string& expr, char vars[]);
bool varlegal(char vars[]);
bool exprlegal(string& expr, char vars[]);
int inline gettpri(char ch);
bool inline isopr(char ch);
bool gettval(string& postexpr);
void karnaugh(int vars[], int ttable[]);
void minimizekmap(vector<string>& result, int vars[], int ttable[]);

#endif;