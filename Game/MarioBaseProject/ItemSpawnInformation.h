#pragma once
/*
	THIS IS DESIGNED TO SEND INFORMATION OF A NEW ITEM
	WITHOUT CIRCULAR REFERENCES.
*/
#include <string>

using namespace std;

class ItemSpawnInformation
{
public:
	enum ItemSpawnEvent : int { iseAsIs = 0, iseFromBlock };
private:
	bool m_bIsUsed = false;

	string m_sItemName = "";
	int m_iLocationX = -1;
	int m_iLocationY = -1;
	ItemSpawnEvent m_ise = iseAsIs;
public:
	ItemSpawnInformation();
	virtual ~ItemSpawnInformation();

	bool get_isUsed() { return m_bIsUsed; }
	string get_itemName() { return m_sItemName; }
	int get_locationX() { return m_iLocationX; }
	int get_locationY() { return m_iLocationY; }
	ItemSpawnEvent get_ise() { return m_ise; }

	bool setup_all(string f_sItemName, int f_iX, int f_iY, ItemSpawnEvent f_ise);

	bool reset_all();
};

//Used if you want to store a item spawn for later:
struct ItemSpawnInformationPacket
{
	string p_sItemName = "";
	int p_iLocationX = -1;
	int p_iLocationY = -1;
	ItemSpawnInformation::ItemSpawnEvent p_ise = ItemSpawnInformation::ItemSpawnEvent::iseAsIs;
};