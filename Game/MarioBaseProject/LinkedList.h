#pragma once
#include <string>

using namespace::std;
class LinkedList
{
protected:
	LinkedList *m_ahead = nullptr;//One ahead
	LinkedList *m_behind = nullptr;//One behind

	int m_iIdentifier = 0;

	bool m_bSet = false;//If true something was set

	string m_sData = "Undefined";
	int m_iData = -1;
	char m_cData = '`';
	bool m_bData = false;
	float m_fData = -1;
	LinkedList *m_linkedList = nullptr;
	void *m_void = nullptr;
public:
	LinkedList();
	LinkedList(string f_sData);
	LinkedList(int f_iData);
	LinkedList(char f_cData);
	LinkedList(bool f_bData);
	LinkedList(float f_bData);

	~LinkedList();

	void set_data(string f_s){m_sData = f_s; m_bSet = true;}
	void set_data(int f_i) { m_iData = f_i;  m_bSet = true;}
	void set_data(char f_c) { m_cData = f_c;  m_bSet = true;}
	void set_data(bool f_b) { m_bData = f_b;  m_bSet = true;}
	void set_data(float f_f) { m_fData = f_f;  m_bSet = true;}
	void set_data(LinkedList *f_ll) { m_linkedList = f_ll;  m_bSet = true;}
	void set_data(void* f_void, bool f_b) { m_void = f_void;  m_bSet = true;}

	void set_ahead(LinkedList* f_ll) { m_ahead = f_ll; }
	void set_behind(LinkedList* f_ll) { m_behind = f_ll; }
	void set_identifier(int f_iIden) { m_iIdentifier = f_iIden; }

	string get_data(string f_s) { return m_sData; }
	int get_data(int f_s) { return m_iData; }
	char get_data(char f_s) { return m_cData; }
	bool get_data(bool f_s) { return m_bData; }
	float get_data(float f_fData) { return m_fData; }
	LinkedList* get_data(LinkedList *f_ll) { return m_linkedList; }
	void* get_data(void* f_void, bool f_b) { return m_void; }

	bool get_dataSet() { return m_bSet; }

	LinkedList *get_ahead() { return m_ahead; }
	LinkedList *get_behind() { return m_behind; }
	int get_identifier() { return m_iIdentifier; }

	string read_data(int f_iIden, string f_s, bool *f_bSuccess = nullptr);
	int read_data(int f_iIden, int f_i, bool *f_bSuccess = nullptr);
	char read_data(int f_iIden, char f_c, bool *f_bSuccess = nullptr);
	bool read_data(int f_iIden, bool f_b, bool *f_bSuccess = nullptr);
	float read_data(int f_iIden, float f_f, bool *f_bSuccess = nullptr);
	LinkedList* read_data(int f_iIden, LinkedList* f_ll, bool *f_bSuccess = nullptr);
	void* read_data(int f_iIden, void *f_void, bool f_b, bool *f_bSuccess = nullptr);

	int length(int f_iLength = 0);
	int update_identifier(int f_iCurrentIden = 0);

	LinkedList* insert_new(int f_iIden);
	LinkedList* insert_new(int f_iIden, string f_sData);
	LinkedList* insert_new(int f_iIden, int f_iData);
	LinkedList* insert_new(int f_iIden, char f_cData);
	LinkedList* insert_new(int f_iIden, bool f_bData);
	LinkedList* insert_new(int f_iIden, float f_fData);
	LinkedList* insert_newAfter(int f_iIden);
	LinkedList* insert_newAfter(int f_iIden, string f_sData);
	LinkedList* insert_newAfter(int f_iIden, int f_iData);
	LinkedList* insert_newAfter(int f_iIden, char f_cData);
	LinkedList* insert_newAfter(int f_iIden, bool f_bData);
	LinkedList* insert_newAfter(int f_iIden, float f_fData);
	LinkedList* insert_newBefore(int f_iIden);
	LinkedList* insert_newBefore(int f_iIden, string f_sData);
	LinkedList* insert_newBefore(int f_iIden, int f_iData);
	LinkedList* insert_newBefore(int f_iIden, char f_cData);
	LinkedList* insert_newBefore(int f_iIden, bool f_bData);
	LinkedList* insert_newBefore(int f_iIden, float f_fData);
	bool delete_item(int f_iIden);

	LinkedList* AddNewToEnd();
	LinkedList* AddNewToEnd(string f_sData);
	LinkedList* AddNewToEnd(int f_iData);
	LinkedList* AddNewToEnd(char f_cData);
	LinkedList* AddNewToEnd(bool f_bData);
	LinkedList* AddNewToEnd(float f_fData);
	LinkedList* AddNewToEnd(LinkedList* f_llData);

	LinkedList* ReadFromList(int f_iIdentifier);

	LinkedList* search_list(string f_sData);
};

