#ifndef __BASELIB_INI_FILE__H_
#define __BASELIB_INI_FILE__H_
/*==============================================================
 * FileName:      IniFile.h
 * Version:        1.0
 * Created by:    liuxj
 * Copyright (c) 2011 qvod Corporation.  All Rights Reserved. 
 *--------------------------------------------------------------
 * Description:   
 *      ʵ�ֶ�Ini�ļ��Ķ���д��
 *=============================================================*/

// C++ Includes
#include <string>
#include <vector>
using namespace std;

// C Includes
#include <stdlib.h>

//��������󳤶ȡ�
#define MAX_SEC_NAME   128

#define MAX_KEY  128
#define MAX_VALUE 2048

class CIniFile  
{
public:
	enum errors{ noID = -1};

	/*--------------------------------------------------------------
	 * ���ܣ�		���캯����
	 * ������		iniPath ini�����ļ�����
	 * ����ֵ��	�ޡ�
	 *-------------------------------------------------------------*/
	CIniFile(string const iniPath = "");
	virtual ~CIniFile(){}

	/*--------------------------------------------------------------
	 * ���ܣ�		���óɴ�Сд���е�[ָsecionName��key], Ĭ��Ϊ�����еġ�
	 * ������		�ޡ�
	 * ����ֵ��	�ޡ�
	 *-------------------------------------------------------------*/
	void CaseSensitive(){
		caseInsensitive = false;
	}
	
	/*--------------------------------------------------------------
	 * ���ܣ�		���óɴ�Сд�����еġ� Ĭ��Ϊ�����еġ�
	 * ������		�ޡ�
	 * ����ֵ��	�ޡ�
	 *-------------------------------------------------------------*/
	void CaseInsensitive(){
		caseInsensitive = true;
	}

	string GetPath() const{
		return path;
	}
	/*--------------------------------------------------------------
	 * ���ܣ�		�����µ������ļ�·����
	 * ������		newPath, �µ��ļ���·����
	 * ����ֵ��	�ޡ�
	 *-------------------------------------------------------------*/
	void SetPath(string const newPath){
		path = newPath;
	}

	/*--------------------------------------------------------------
	 * ���ܣ�		��ȡ�����������ļ���
	 * ������		�ޡ�
	 * ����ֵ��	��ȡ״̬��true��ʾ�ɹ���false��ʾʧ�ܡ�
	 *-------------------------------------------------------------*/
	bool ReadFile();

	/*--------------------------------------------------------------
	 * ���ܣ�		����ָ���Ķ�[section]
	 * ������		secname, Ҫ�ҵ��Ķε����ơ�
	 * ����ֵ��	���ظöε�ID, ���δ�ҵ�����noID��
	 *-------------------------------------------------------------*/
	long FindSection( string const secname) const;

	/*--------------------------------------------------------------
	 * ���ܣ�		����ָ���ļ�[Key]
	 * ������		sectionID, Ҫ�ҵ��Ķε�ID��
	 *			key, Ҫ�ҵ���Key��
	 * ����ֵ��	���ظü���ID, ���δ�ҵ�����noID��
	 *-------------------------------------------------------------*/
	long FindKey( unsigned const sectionID, string const key) const;

	/*--------------------------------------------------------------
	 * ���ܣ�		��ȡ��ǰ�ļ��Ķε�������
	 * ������		�ޡ�
	 * ����ֵ��	���ضε�������
	 *-------------------------------------------------------------*/
	unsigned GetNumSections() const{return m_secnames.size();}

	/*--------------------------------------------------------------
	 * ���ܣ�		���һ����[���ڴ�]
	 * ������		secname, ��ӵĶε����ơ�
	 * ����ֵ��	����ӵĶε�ID��
	 *-------------------------------------------------------------*/
	unsigned AddSectionName( string const secname);

	/*--------------------------------------------------------------
	 * ���ܣ�		��ȡָ��ID��Ӧ�Ķ����ơ�
	 * ������		sectinID �ε�ID��
	 * ����ֵ��	���ض����ơ�
	 *-------------------------------------------------------------*/
	string GetSectionName( unsigned const sectionID) const;

	unsigned GetNumKeys( unsigned const sectionID);
	/*--------------------------------------------------------------
	 * ���ܣ�		��ȡָ���Ķ������ж���Key.
	 * ������		secname �����ơ�
	 * ����ֵ��	����secname�¶ε�������
	 *-------------------------------------------------------------*/
	unsigned GetNumKeys( string const secname);

	/*--------------------------------------------------------------
	 * ���ܣ�		��ȡָ���Ķ��������е�Key.
	 * ������		secname �����ơ�
	 * ����ֵ��	����secname�����е�key
	 *-------------------------------------------------------------*/
	vector<string>* GetKeys(string const secname);
	
	/*--------------------------------------------------------------
	 * ���ܣ�		��ȡsecname��key��Ӧ��ֵ�����صĺ����ֱ���Զ�ȡInt�ͣ�Bool�ͣ������͵ȡ�
	 * ������		secname �����ơ�
	 *			key Ϊ�����ơ�
	 *			defValueΪĬ��ֵ��
	 * ����ֵ��	���ض�ȡ����ֵ��
	 *-------------------------------------------------------------*/
	string GetValue(string const secname, string const key, string const defValue = "") const; 
	int    GetValueI(string const secname, string const key, int const defValue = 0) const;
	bool   GetValueB(string const secname, string const key, bool const defValue = false) const;
	double   GetValueF(string const secname, string const key, double const defValue = 0.0) const;
	long long GetValueLL(string const secname, string const key, long long const defValue=0) const;
	 
	/*--------------------------------------------------------------
	 * ���ܣ�		��formatָ���ĸ�ʽ��ȡһ�����߶��ֵ��format�еĸ�ʽ��scanf,sscanf����ͬ��
	 * ������		secname, Ҫ��ȡ�Ķ����ơ�
	 *			key, Ҫ��ȡ��Key��
	 *			format, ��ʽ������scanf��ʽ����ͬ��
	 *			v1..v16 Ϊָ�롣
	 * ����ֵ��	���ض�ȡ��ֵ��������
	 *-------------------------------------------------------------*/
	unsigned GetValueV( string const secname, string const key, const char *format,
	  void *v1 = 0, void *v2 = 0, void *v3 = 0, void *v4 = 0,
	      void *v5 = 0, void *v6 = 0, void *v7 = 0, void *v8 = 0,
	      void *v9 = 0, void *v10 = 0, void *v11 = 0, void *v12 = 0,
	      void *v13 = 0, void *v14 = 0, void *v15 = 0, void *v16 = 0);
#if 0
	// Writes data stored in class to ini file.
	/*--------------------------------------------------------------
	 * ���ܣ�		���������ļ�, �����õ��޸�ֻ��WriteFile�����Ч��
	 * ������		�ޡ�
	 * ����ֵ��	����״̬��true��ʾ�ɹ���false��ʾʧ�ܡ�
	 *-------------------------------------------------------------*/
	bool WriteFile(); 

	/*--------------------------------------------------------------
	 * ���ܣ�		ɾ������(�ڴ��е�)���á�
	 * ������		�ޡ�
	 * ����ֵ��	�ޡ�
	 *-------------------------------------------------------------*/
	void Clear();

	/*--------------------------------------------------------------
	 * ���ܣ�		����һ��ֵ�����صļ��������ֱ��������Int,Bool,Float�����͵�ֵ��
	 * ������		secname, �����ơ�
	 *			key, ������
	 *			value, ֵ��
	 *			create, ��secname������ʱ�����createΪtrue��ᴴ����section,�����ʧ�ܡ�
	 * ����ֵ��	����״̬��
	 *-------------------------------------------------------------*/
	bool SetValue( string const secname, string const key, string const value, bool const create = true);
	bool SetValueI( string const secname, string const key, int const value, bool const create = true);
	bool SetValueB( string const secname, string const key, bool const value, bool const create = true) {
	return SetValue( secname, key, value?"true":"false", create);
	}
	bool SetValueF( string const secname, string const key, double const value, bool const create = true);

	/*--------------------------------------------------------------
	 * ���ܣ�		��ָ���ĸ�ʽ����һ�����߶��ֵ��
	 * ������		�ޡ�
	 * ����ֵ��	����״̬��
	 *-------------------------------------------------------------*/
	bool SetValueV( string const secname, string const key, const char *format, ...);

	/*--------------------------------------------------------------
	 * ���ܣ�		ɾ��һ����ֵ��
	 * ������		secname���ε����ơ� 
	 *			key��Ҫɾ���ļ���
	 * ����ֵ��	ɾ��״̬�� true��ʾɾ���ɹ���false��ʾɾ��ʧ�ܡ�
	 *-------------------------------------------------------------*/
	bool DeleteKey( string const secname, string const key);

	// Deletes specified Section and all values contained within.
	// Returns true if Section existed and deleted, false otherwise.
	/*--------------------------------------------------------------
	 * ���ܣ�		ɾ��һ���Ρ�
	 * ������		secname Ҫɾ���Ķε����ơ�
	 * ����ֵ��	ɾ��״̬�� true��ʾɾ���ɹ���false��ʾɾ��ʧ�ܡ�
	 *-------------------------------------------------------------*/
	bool DeleteSection(string secname);

	
#endif
	bool SetValue( string const secname, string const key, string const value, bool const create = true);

	void     AddHeaderComment( string const comment);
	bool     AddSectionComment( unsigned const sectionID, string const comment);
	bool     AddSectionComment( string const secname, string const comment);

protected:
	string GetValue( unsigned const sectionID, unsigned const keyId, string const defValue = "") const;
	bool SetValue( unsigned const sectionID, unsigned const keyId, string const value);

private:
	bool   caseInsensitive;
	string path;
	struct Section {
		vector<string> keys;
		vector<string> values; 
		vector<string> comments; //section comments.
	};
	vector<Section> m_sections; 
	vector<string> m_secnames; 
	vector<string> m_hdrcomments;
	string CheckCase( string s) const;
	
};

#endif
