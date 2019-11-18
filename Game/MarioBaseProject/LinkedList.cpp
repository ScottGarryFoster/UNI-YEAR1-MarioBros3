#include "LinkedList.h"



LinkedList::LinkedList()
{

}

LinkedList::LinkedList(string f_sData)
{
	set_data(f_sData);
}

LinkedList::LinkedList(int f_iData)
{
	set_data(f_iData);
}

LinkedList::LinkedList(char f_cData)
{
	set_data(f_cData);
}

LinkedList::LinkedList(bool f_bData)
{
	set_data(f_bData);
}
LinkedList::LinkedList(float f_bData)
{
	set_data(f_bData);
}


LinkedList::~LinkedList()
{
	if (m_linkedList != nullptr)
		delete m_linkedList;
	if (m_ahead != nullptr)
		delete m_ahead;
	m_ahead = nullptr;
}

LinkedList* LinkedList::AddNewToEnd(string f_sData)
{
	LinkedList* l_llReturn = AddNewToEnd();
	l_llReturn->set_data(f_sData);
	return l_llReturn;
}
LinkedList* LinkedList::AddNewToEnd(int f_iData)
{
	LinkedList* l_llReturn = AddNewToEnd();
	l_llReturn->set_data(f_iData);
	return l_llReturn;
}
LinkedList* LinkedList::AddNewToEnd(char f_cData)
{
	LinkedList* l_llReturn = AddNewToEnd();
	l_llReturn->set_data(f_cData);
	return l_llReturn;
}
LinkedList* LinkedList::AddNewToEnd(bool f_bData)
{
	LinkedList* l_llReturn = AddNewToEnd();
	l_llReturn->set_data(f_bData);
	return l_llReturn;
}
LinkedList* LinkedList::AddNewToEnd(float f_fData)
{
	LinkedList* l_llReturn = AddNewToEnd();
	l_llReturn->set_data(f_fData);
	return l_llReturn;
}

LinkedList* LinkedList::AddNewToEnd(LinkedList* f_llData)
{
	LinkedList* l_llReturn = AddNewToEnd();
	l_llReturn->set_data(f_llData);
	return l_llReturn;
}

LinkedList* LinkedList::AddNewToEnd()
{
	if (m_ahead != nullptr)
		return m_ahead->AddNewToEnd();

	m_ahead = new LinkedList();
	m_ahead->set_behind(this);
	m_ahead->set_identifier(m_iIdentifier + 1);
	return m_ahead;
}

LinkedList* LinkedList::insert_new(int f_iIden)
{
	return insert_newAfter(f_iIden);
}

LinkedList* LinkedList::insert_new(int f_iIden, string f_sData)
{
	LinkedList *l_llReturn = insert_newAfter(f_iIden);
	l_llReturn->set_data(f_sData);
	return l_llReturn;
}
LinkedList* LinkedList::insert_new(int f_iIden, int f_iData)
{
	LinkedList *l_llReturn = insert_newAfter(f_iIden);
	l_llReturn->set_data(f_iData);
	return l_llReturn;
}
LinkedList* LinkedList::insert_new(int f_iIden, char f_cData)
{
	LinkedList *l_llReturn = insert_newAfter(f_iIden);
	l_llReturn->set_data(f_cData);
	return l_llReturn;
}
LinkedList* LinkedList::insert_new(int f_iIden, bool f_bData)
{
	LinkedList *l_llReturn = insert_newAfter(f_iIden);
	l_llReturn->set_data(f_bData);
	return l_llReturn;
}

LinkedList* LinkedList::insert_new(int f_iIden, float f_fData)
{
	LinkedList *l_llReturn = insert_newAfter(f_iIden);
	l_llReturn->set_data(f_fData);
	return l_llReturn;
}

LinkedList* LinkedList::insert_newAfter(int f_iIden)
{
	if (f_iIden != m_iIdentifier)//-1 is insert before
	{
		if (m_ahead == nullptr)
			return nullptr;
		else
			return m_ahead->insert_newAfter(f_iIden);
	}

	LinkedList *l_insertedAhead = m_ahead;//The head of the new insert
	LinkedList *l_insertedTail = this;//The behind or tail of the new insert

	m_ahead = new LinkedList();//Make the new insert
	if(l_insertedAhead != nullptr)
		m_ahead->set_ahead(l_insertedAhead);//Set the ahead to the previous ahead
	m_ahead->set_behind(l_insertedTail);//Behind is now this

	if(l_insertedAhead != nullptr)
		l_insertedAhead->set_behind(m_ahead);//Set the behind of the one infront of the new insert

	m_ahead->update_identifier(f_iIden + 1);//Updates the numbers
	return m_ahead;
}

LinkedList* LinkedList::insert_newAfter(int f_iIden, string f_sData)
{
	LinkedList *l_llReturn = insert_newAfter(f_iIden);
	l_llReturn->set_data(f_sData);
	return l_llReturn;
}
LinkedList* LinkedList::insert_newAfter(int f_iIden, int f_iData)
{
	LinkedList *l_llReturn = insert_newAfter(f_iIden);
	l_llReturn->set_data(f_iData);
	return l_llReturn;
}
LinkedList* LinkedList::insert_newAfter(int f_iIden, char f_cData)
{
	LinkedList *l_llReturn = insert_newAfter(f_iIden);
	l_llReturn->set_data(f_cData);
	return l_llReturn;
}
LinkedList* LinkedList::insert_newAfter(int f_iIden, bool f_bData)
{
	LinkedList *l_llReturn = insert_newAfter(f_iIden);
	l_llReturn->set_data(f_bData);
	return l_llReturn;
}

LinkedList* LinkedList::insert_newAfter(int f_iIden, float f_fData)
{
	LinkedList *l_llReturn = insert_newAfter(f_iIden);
	l_llReturn->set_data(f_fData);
	return l_llReturn;
}

LinkedList* LinkedList::insert_newBefore(int f_iIden)
{
	if (f_iIden != m_iIdentifier)//-1 is insert before
	{
		if (m_ahead == nullptr)
			return nullptr;
		else
			return m_ahead->insert_newBefore(f_iIden);
	}

	LinkedList *l_insertedAhead = this;//The head of the new insert
	LinkedList *l_insertedTail = m_behind;//The behind or tail of the new insert

	m_behind = new LinkedList();//Make the new insert
	if (l_insertedTail != nullptr)
		m_behind->set_behind(l_insertedTail);//Set the ahead to the previous ahead
	m_behind->set_ahead(l_insertedAhead);//Behind is now this

	if (l_insertedTail != nullptr)
		l_insertedTail->set_ahead(m_behind);//Set the behind of the one infront of the new insert

	l_insertedTail->update_identifier(f_iIden - 1);//Updates the numbers
	return m_behind;
}

LinkedList* LinkedList::insert_newBefore(int f_iIden, string f_sData)
{
	LinkedList *l_llReturn = insert_newBefore(f_iIden);
	l_llReturn->set_data(f_sData);
	return l_llReturn;
}
LinkedList* LinkedList::insert_newBefore(int f_iIden, int f_iData)
{
	LinkedList *l_llReturn = insert_newBefore(f_iIden);
	l_llReturn->set_data(f_iData);
	return l_llReturn;
}
LinkedList* LinkedList::insert_newBefore(int f_iIden, char f_cData)
{
	LinkedList *l_llReturn = insert_newBefore(f_iIden);
	l_llReturn->set_data(f_cData);
	return l_llReturn;
}
LinkedList* LinkedList::insert_newBefore(int f_iIden, bool f_bData)
{
	LinkedList *l_llReturn = insert_newBefore(f_iIden);
	l_llReturn->set_data(f_bData);
	return l_llReturn;
}
LinkedList* LinkedList::insert_newBefore(int f_iIden, float f_fData)
{
	LinkedList *l_llReturn = insert_newBefore(f_iIden);
	l_llReturn->set_data(f_fData);
	return l_llReturn;
}

bool LinkedList::delete_item(int f_iIden)
{
	if (f_iIden != m_iIdentifier)
	{
		if (m_ahead == nullptr)
			return false;
		else
			return m_ahead->delete_item(f_iIden);
	}
	//So this one is the one to delete
	if (m_ahead != nullptr)
	{
		if (m_behind != nullptr)
		{
			m_ahead->set_behind(m_behind);
			m_ahead->update_identifier(f_iIden);//Updates the numbers
		}
		else
			m_ahead->set_behind(nullptr);//I'm the first element
	}
	if (m_behind != nullptr)
	{
		if (m_ahead != nullptr)
			m_behind->set_ahead(m_ahead);
		else
			m_behind->set_ahead(nullptr);//I'm the last element
	}
	m_ahead = nullptr;
	m_behind = nullptr;
	//Now we've taken ourselves from the list time to delete:
	delete this;
	return true;
}

int LinkedList::update_identifier(int f_iCurrentIden)
{
	if (f_iCurrentIden < 0) f_iCurrentIden = 0;
	m_iIdentifier = f_iCurrentIden;
	if (m_ahead == nullptr)
		return f_iCurrentIden;
	else
		return m_ahead->update_identifier(f_iCurrentIden + 1);
}

LinkedList* LinkedList::ReadFromList(int f_iIdentifier)
{
	if (f_iIdentifier != m_iIdentifier)
	{
		if (m_ahead == nullptr)
			return nullptr;
		else
			return m_ahead->ReadFromList(f_iIdentifier);
	}
	return this;
}

LinkedList* LinkedList::search_list(string f_sData)
{
	if (m_sData != f_sData)
	{
		if (m_ahead == nullptr)
			return nullptr;
		else
			return m_ahead->search_list(f_sData);
	}
	return this;
}

string LinkedList::read_data(int f_iIden, string f_s, bool *f_bSuccess)
{
	if (f_bSuccess != nullptr)
		*f_bSuccess = false;
	LinkedList* f_llData = ReadFromList(f_iIden);
	if (f_llData == nullptr)
		return "Unfound";
	
	if (f_bSuccess != nullptr)
		*f_bSuccess = true;
	return f_llData->get_data(f_s);
}
int LinkedList::read_data(int f_iIden, int f_i, bool *f_bSuccess)
{
	if (f_bSuccess != nullptr)
		*f_bSuccess = false;
	LinkedList* f_llData = ReadFromList(f_iIden);
	if (f_llData == nullptr)
		return -1;

	if (f_bSuccess != nullptr)
		*f_bSuccess = true;
	return f_llData->get_data(f_i);
}
char LinkedList::read_data(int f_iIden, char f_c, bool *f_bSuccess)
{
	if (f_bSuccess != nullptr)
		*f_bSuccess = false;
	LinkedList* f_llData = ReadFromList(f_iIden);
	if (f_llData == nullptr)
		return '`';

	if (f_bSuccess != nullptr)
		*f_bSuccess = true;
	return f_llData->get_data(f_c);
}
bool LinkedList::read_data(int f_iIden, bool f_b, bool *f_bSuccess)
{
	if (f_bSuccess != nullptr)
		*f_bSuccess = false;
	LinkedList* f_llData = ReadFromList(f_iIden);
	if (f_llData == nullptr)
		return false;

	if (f_bSuccess != nullptr)
		*f_bSuccess = true;
	return f_llData->get_data(f_b);
}
LinkedList* LinkedList::read_data(int f_iIden, LinkedList* f_ll, bool *f_bSuccess)
{
	if (f_bSuccess != nullptr)
		*f_bSuccess = false;
	LinkedList* f_llData = ReadFromList(f_iIden);
	if (f_llData == nullptr)
		return false;

	if (f_bSuccess != nullptr)
		*f_bSuccess = true;
	return f_llData->get_data(f_ll);
}
float LinkedList::read_data(int f_iIden, float f_f, bool *f_bSuccess)
{
	if (f_bSuccess != nullptr)
		*f_bSuccess = false;
	LinkedList* f_llData = ReadFromList(f_iIden);
	if (f_llData == nullptr)
		return false;

	if (f_bSuccess != nullptr)
		*f_bSuccess = true;
	return f_llData->get_data(f_f);
}

void* LinkedList::read_data(int f_iIden, void *f_void, bool f_b, bool *f_bSuccess)
{
	if (f_bSuccess != nullptr)
		*f_bSuccess = false;
	LinkedList* f_llData = ReadFromList(f_iIden);
	if (f_llData == nullptr)
		return false;

	if (f_bSuccess != nullptr)
		*f_bSuccess = true;
	return f_llData->get_data(f_void, f_b);
}

int LinkedList::length(int f_iLength)
{
	f_iLength++;
	if (m_ahead == nullptr)
		return f_iLength;
	else
		return m_ahead->length(f_iLength);
}