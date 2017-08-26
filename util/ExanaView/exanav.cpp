///////////////////////////////////////////////////////////////////////////////
// Name:        exanav.cpp
// Purpose:     Exana result viewer
// Author:      T.Y
// Created:     2015
// Copyright:   (c) 2015 T.Y.
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

//wx routine start
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef  WX_PRECOMP
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#   include "wx/wx.h"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
//wx routine end

//my include
#include "exanavadd.h"
#include "wx/config.h"
#include "rapidjson/document.h"
#include <fstream>
#include <iostream>
#include <string>
#include <wx/aui/aui.h>
#include <wx/dataview.h>
#include <wx/variant.h>
#include <wx/vector.h>
#include <wx/stc/stc.h>
#include <sstream>
#include <wx/dir.h>
#include <wx/file.h>

//just avoid "warning: unused parameter"
#define AVOID_UNUSED_WARNING(a) (a=a)
#define AVOID_UNUSED_WARNING_CONST(a) (a==a)
#define AVOID_UNUSED_WARNING_VOID(a) ((void)a)

//Specifications of json files. JD: Json Data
#define JD_COLNUM 15
#define JD_INSTADDR 0
#define JD_EST_LATENCY 1
#define JD_AVG_LATENCY 2
#define JD_L1_HIT 3
#define JD_L1_MISS 4
#define JD_L1_CONFL 5
#define JD_L2_HIT 6
#define JD_L2_MISS 7
#define JD_L2_CONFL 8
#define JD_L3_HIT 9
#define JD_L3_MISS 10
#define JD_L3_CONFL 11
#define JD_TOTALACCESS 12
#define JD_MEMOBJ 13
#define JD_SRC 14

#define JD_TYPE_STRING 0
#define JD_TYPE_DECIMAL 1
#define JD_TYPE_HEXADECIMAL 2
#define JD_TYPE_DOUBLE 3
#define JD_TYPE_SRC 4

//my debug code memo
//wxMessageBox(wxString::Format("Debug Data: %s", "test"), "debug", wxOK | wxICON_INFORMATION);

//-----------------------------------------------
// My Utility
//-----------------------------------------------

std::vector<std::string> split(const std::string &str, char sep){
    std::vector<std::string> v;
    std::stringstream ss(str);
    std::string buffer;
    while( std::getline(ss, buffer, sep) ) {
        v.push_back(buffer);
    }
    return v;
}

int getflnum(std::string str){
	int count = 0;
	std::string line_str;
	std::ifstream ifs(str.c_str());

	while(std::getline(ifs, line_str)){
		count++;
	}

	return count;
}

//-----------------------------------------------
// My Classes (for Utility)
//-----------------------------------------------
class AnnotInfo {
public:
	int rownum;
	int annotnum;
	int str_rownum;
};

bool operator<(const AnnotInfo& left, const AnnotInfo& right){
	return left.rownum < right.rownum;
}

class OneFileInfo {
public:
	std::string filename;
	std::string filename_only;
	std::vector<AnnotInfo> annot_info;
	int filelinecount;
	bool fileexist;
	std::vector<int> listrownum;
	unsigned int max_numofchar[JD_COLNUM-2];
	unsigned int max_max_numofchar;
	bool nofilenameflag;
};

//-----------------------------------------------
// My Classes (for wx)
//-----------------------------------------------

class ExanavListModel: public wxDataViewModel {
public:
	ExanavListModel(){
		//define datatype 
		for(int i=0; i<JD_COLNUM; i++){
			switch(i){
			case JD_INSTADDR:
				datatype[i] = JD_TYPE_HEXADECIMAL;
				break;
			case JD_EST_LATENCY:
			case JD_L1_HIT:
			case JD_L1_MISS:
			case JD_L1_CONFL:
			case JD_L2_HIT:
			case JD_L2_MISS:
			case JD_L2_CONFL:
			case JD_L3_HIT:
			case JD_L3_MISS:
			case JD_L3_CONFL:
			case JD_TOTALACCESS:
				datatype[i] = JD_TYPE_DECIMAL;
				break;
			case JD_SRC:
				datatype[i] = JD_TYPE_SRC;
				break;
			case JD_MEMOBJ:
				datatype[i] = JD_TYPE_STRING;
				break;
			case JD_AVG_LATENCY:
				datatype[i] = JD_TYPE_DOUBLE;
				break;
			default:
				break;
			}
		}
	}
	~ExanavListModel(){}

	//my methods
	bool initDoc(std::string str){
		char* alljc;
		rapidjson::Document doc;

		//string to char
		alljc = new char[strlen(str.c_str())+1];
   		std::sprintf(alljc, "%s", str.c_str());

		//parse json
    	if(doc.ParseInsitu(alljc).HasParseError()) {delete[] alljc; return false;}

		//Don't uncomment below code!! If you do, you cannot read document
		//delete[] alljc;

		//error check
		if(
			(!doc.HasMember("header")) || 
			(!doc["header"].IsArray()) || 
			(doc["header"].Size() != JD_COLNUM) || 
			(!doc.HasMember("data")) || 
			(!doc["data"].IsArray())
		)
		{delete[] alljc; return false;}

		//copy header data
		for(rapidjson::SizeType i=0; i<JD_COLNUM; i++){
			headers[i] = doc["header"][i].GetString();
		}

		//prepare
		for(int i=0; i<JD_COLNUM; i++){
			switch(datatype[i]){
			case JD_TYPE_DECIMAL:
				jdatapp[i] = new wxULongLong[(int)doc["data"].Size()];
				break;
			case JD_TYPE_HEXADECIMAL:
			case JD_TYPE_STRING:
			case JD_TYPE_SRC:
				jdatapp[i] = new std::string[(int)doc["data"].Size()];
				break;
			case JD_TYPE_DOUBLE:
				jdatapp[i] = new double[(int)doc["data"].Size()];
				break;
			default:
				break;
			}
		}
		allrownum = (int)doc["data"].Size();
		fileid = new int[allrownum];
		filelinenum = new int[allrownum];
		instaddrnum = new unsigned long long[allrownum];

		//all row scan
		fileinfov.clear(); //just in case
		const rapidjson::Value& jd = doc["data"]; //cache
		rapidjson::SizeType datarow_size = jd.Size(); //cache
		for(rapidjson::SizeType i=0; i<datarow_size; i++){
			const rapidjson::Value& jdi = jd[i]; //cache
			//error check
			if(
				(!jdi.IsArray()) || 
				(jdi.Size() != JD_COLNUM)
			)
			{delete[] alljc; return false;}
			//I don't delete jdatapp's mem, because of simple code.
			//The more times file reading fail occur, the more dead mem.

			//copy all data
			for(rapidjson::SizeType j=0; j<JD_COLNUM; j++){
				switch(datatype[j]){
				case JD_TYPE_DECIMAL:
					//error check
					if(!jdi[j].IsUint64()) {delete[] alljc; return false;}
					((wxULongLong*)jdatapp[j])[i] = jdi[j].GetUint64();
					break;
				case JD_TYPE_HEXADECIMAL:
					if(!jdi[j].IsString()) {delete[] alljc; return false;}
					instaddrnum[i] = strtoull((char*)jdi[j].GetString(), NULL, 16);
					((std::string*)jdatapp[j])[i] = jdi[j].GetString();
					break;
				case JD_TYPE_STRING:
				case JD_TYPE_SRC:
					//error check
					if(!jdi[j].IsString()) {delete[] alljc; return false;}
					((std::string*)jdatapp[j])[i] = jdi[j].GetString();
					break;
				case JD_TYPE_DOUBLE:
					//error check
					if(!jdi[j].IsDouble()) {delete[] alljc; return false;}
					((double*)jdatapp[j])[i] = jdi[j].GetDouble();
					break;
				default:
					break;
				}
			}

			//create exanav original data start
			std::string fname, linenum_str;
			int linenum;
			std::vector<std::string> strvec;

			strvec = split(((std::string*)jdatapp[JD_SRC])[i], ':');

			//save filelinenum
			linenum_str = strvec.at(1);
			std::stringstream ss(linenum_str);
			ss >> linenum;
			filelinenum[i] = linenum;

			//save fileinfo
			fname = strvec.at(0);
			int fid = -1;
			int count = 0;
			for(std::vector<OneFileInfo>::iterator it = fileinfov.begin(); it != fileinfov.end(); ++it){
				if(it->filename == fname) {fid = count;}
				count++;
			}
			if(fid > -1){
				//fileid already exists
				if(fileinfov.at(fid).fileexist){
					//adjust annot_info
					fileinfov.at(fid).listrownum.push_back(i);
					int annotid = -1;
					int count2 = 0;
					for(std::vector<AnnotInfo>::iterator it2 = fileinfov.at(fid).annot_info.begin(); it2 != fileinfov.at(fid).annot_info.end(); ++it2) {
						if(it2->rownum == linenum) {annotid = count2;}
						count2++;
					}
					if(annotid > -1) {
						//annotid already exists
						fileinfov.at(fid).annot_info.at(annotid).annotnum++;
					} else {
						//new annotid
						annotid = fileinfov.at(fid).annot_info.size();
						fileinfov.at(fid).annot_info.push_back(AnnotInfo());
						fileinfov.at(fid).annot_info.at(annotid).rownum = linenum;
						fileinfov.at(fid).annot_info.at(annotid).annotnum = 1;
					}
				}
			} else {
				//new fileid
				fid = fileinfov.size();
				fileinfov.push_back(OneFileInfo());
				fileinfov.at(fid).filename = fname;

				std::vector<std::string> fnv;
				fnv = split(fname, '/');
				fileinfov.at(fid).filename_only = fnv.back();

				if(fname == "??"){
					fileinfov.at(fid).nofilenameflag = true;
				} else {
					fileinfov.at(fid).nofilenameflag = false;
				}
				fileinfov.at(fid).listrownum.push_back(i);
				fileinfov.at(fid).annot_info.push_back(AnnotInfo());
				fileinfov.at(fid).annot_info.at(0).rownum = linenum;
				fileinfov.at(fid).annot_info.at(0).annotnum = 1;

				if(wxFile::Exists(fname)){
					//source code file exist
					int linecount;
					linecount = getflnum(fname);
					fileinfov.at(fid).filelinecount = linecount;
					fileinfov.at(fid).fileexist = true;
				} else {
					//source code file not exist
					fileinfov.at(fid).fileexist = false;
				}
			}

			fileid[i] = fid;
			//create exanav original data end
		}

		//final adjust. sort, str_rownum, max_numofchar, max_max_numofchar
		for(std::vector<OneFileInfo>::iterator it = fileinfov.begin(); it != fileinfov.end(); ++it){
			std::sort(it->annot_info.begin(), it->annot_info.end());

			int gap_count = 0;
			for(std::vector<AnnotInfo>::iterator it2 = it->annot_info.begin(); it2 != it->annot_info.end(); ++it2){
				it2->str_rownum = it2->rownum + gap_count;
				gap_count += it2->annotnum - 1;

			}

			std::stringstream ss_oneline("");
			for(int i=0; i<JD_COLNUM-2; i++){
				it->max_numofchar[i] = 0;
				for(std::vector<int>::iterator it2 = it->listrownum.begin(); it2 != it->listrownum.end(); ++it2){
					switch(datatype[i]){
					case JD_TYPE_DECIMAL:
						ss_oneline << ((wxULongLong*)jdatapp[i])[(*it2)];
						break;
					case JD_TYPE_HEXADECIMAL:
					case JD_TYPE_STRING:
						ss_oneline << ((std::string*)jdatapp[i])[(*it2)];
						break;
					case JD_TYPE_SRC:
						ss_oneline << fileinfov.at(fileid[(*it2)]).filename_only;
						break;
					case JD_TYPE_DOUBLE:
						ss_oneline << ((double*)jdatapp[i])[(*it2)];
						break;
					default:
						break;
					}

					if(it->max_numofchar[i] < ss_oneline.str().length()){
						it->max_numofchar[i] = ss_oneline.str().length();
					}
					ss_oneline.str("");
				}
			}
			it->max_max_numofchar = 0;
			for(int i=0; i<JD_COLNUM-2; i++){
				if(it->max_max_numofchar < it->max_numofchar[i]){
					it->max_max_numofchar = it->max_numofchar[i];
				}
			}
		}

		rowidpp = new int*[(int)doc["data"].Size()];
		//mem clean
		delete[] alljc;

		return true;
	}

	wxString GetHeaderStr(int col) {
		return headers[col];
	}

	//override below methods
	unsigned int GetColumnCount() const {
		return JD_COLNUM;
	}

	wxString GetColumnType(unsigned int column) const {
		switch(datatype[column]){
		case JD_TYPE_DECIMAL:
			return "ulonglong";
		case JD_TYPE_HEXADECIMAL:
		case JD_TYPE_STRING:
		case JD_TYPE_SRC:
			return "string";
		case JD_TYPE_DOUBLE:
			return "double";
		default:
			return "string";
		}
	}

	bool IsContainer(const wxDataViewItem& item) const {
		if (!item.IsOk()) {
			//invisible root of root
			return true;
		}
		return false;
	}

    wxDataViewItem GetParent(const wxDataViewItem& item) const {
		AVOID_UNUSED_WARNING_CONST(item);
        return wxDataViewItem(NULL);
    }

    unsigned int GetChildren(const wxDataViewItem& item, wxDataViewItemArray& children) const {
		if(!item.IsOk()) {
			//root layer
			rapidjson::SizeType rs = (rapidjson::SizeType)allrownum; //cache
			for(rapidjson::SizeType i=0; i<rs; i++){
				rowidpp[i] = new int(i);
				children.Add(wxDataViewItem((void*)rowidpp[i]));
			}

			return (unsigned int)allrownum;
		} else {
			return 0;
		}
    }

    void GetValue(wxVariant& val, const wxDataViewItem& item, unsigned int column) const {
		int *rowidp;
		rowidp = (int*)item.GetID();

		std::stringstream srcss("");

		switch(datatype[column]){
		case JD_TYPE_DECIMAL:
			//val = wxVariant(((unsigned long long*)jdatapp[column])[*rowidp]);
			val = wxVariant(((wxULongLong*)jdatapp[column])[*rowidp]);
			return;
		case JD_TYPE_HEXADECIMAL:
		case JD_TYPE_STRING:
			val = wxVariant(((std::string*)jdatapp[column])[*rowidp]);
			return;
		case JD_TYPE_SRC:
			srcss << fileinfov.at(fileid[*rowidp]).filename_only << ":" << filelinenum[*rowidp];
			val = wxVariant(srcss.str());
			return;
		case JD_TYPE_DOUBLE:
			val = wxVariant(((double*)jdatapp[column])[*rowidp]);
			return;
		default:
			val = wxVariant(((std::string*)jdatapp[column])[*rowidp]);
			return;
		}
    }

    bool SetValue(const wxVariant& val, const wxDataViewItem& item, unsigned int column) {
		AVOID_UNUSED_WARNING_CONST(val);
		AVOID_UNUSED_WARNING_CONST(item);
		AVOID_UNUSED_WARNING(column);
		return true;
    }

	//additional override
	int Compare(const wxDataViewItem &item1, const wxDataViewItem &item2, unsigned int column, bool ascending ) const {
		int rownum1, rownum2, rnum;
		rownum1 = *((int*)item1.GetID());
		rownum2 = *((int*)item2.GetID());

		switch(datatype[column]){
		case JD_TYPE_DECIMAL:
			unsigned long long i1, i2;
			i1 = ((unsigned long long*)jdatapp[column])[rownum1];
			i2 = ((unsigned long long*)jdatapp[column])[rownum2];
			if((i1>i2) != (ascending)){
				rnum = 1;
			} else {
				rnum = -1;
			}
			return rnum;
		case JD_TYPE_DOUBLE:
			double d1, d2;
			d1 = ((double*)jdatapp[column])[rownum1];
			d2 = ((double*)jdatapp[column])[rownum2];
			if((d1>d2) != (ascending)){
				rnum = 1;
			} else {
				rnum = -1;
			}
			return rnum;
		case JD_TYPE_HEXADECIMAL:
			unsigned long long ull1, ull2;
			ull1 = instaddrnum[rownum1];
			ull2 = instaddrnum[rownum2];
			if((ull1>ull2) != (ascending)){
				rnum = 1;
			} else {
				rnum = -1;
			}
			return rnum;
		case JD_TYPE_SRC:
			int fid1, fid2;
			fid1 = fileid[rownum1];
			fid2 = fileid[rownum2];
			if(fid1==fid2){
				//same file
				if((filelinenum[rownum1] > filelinenum[rownum2]) != (ascending)){
					rnum = 1;
				} else {
					rnum = -1;
				}
			//not same file
			} else if((fid1>fid2) != (ascending)){
				rnum = 1;
			} else {
				rnum = -1;
			}

			//In the case of ??filename
			if(fileinfov.at(fid1).nofilenameflag) {
				if(ascending){
					rnum = -1;
				} else {
					rnum = 1;
				}
			} else if (fileinfov.at(fid2).nofilenameflag) {
				if(ascending){
					rnum = 1;
				} else {
					rnum = -1;
				}
			}

			return rnum;
		case JD_TYPE_STRING:
    		return wxDataViewModel::Compare(item1, item2, column, ascending);
		default:
    		return wxDataViewModel::Compare(item1, item2, column, ascending);
		}
	}

	bool GetAttr(const wxDataViewItem &item, unsigned int col, wxDataViewItemAttr &attr) const {
		int *rowidp;
		rowidp = (int*)item.GetID();
		if((col == JD_SRC) && (!fileinfov.at(fileid[*rowidp]).fileexist)){
			attr.SetColour(wxColour(150, 150, 150));
			return true;
		} else {
			return false;
		}
	}

//member
	int** rowidpp;
	void* jdatapp[JD_COLNUM];
	int* fileid;
	int* filelinenum;
	unsigned long long* instaddrnum;
	std::string headers[JD_COLNUM];
	std::vector<OneFileInfo> fileinfov;
	int datatype[JD_COLNUM];
	int allrownum;
private:
//nothing
};

class STCScroll : public wxStyledTextCtrl {
public:
	STCScroll(wxWindow *parent, wxWindowID id) : wxStyledTextCtrl(parent, id){
		initThis();
	}
	STCScroll(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size) : wxStyledTextCtrl(parent, id, pos, size){
		initThis();
	}

	void initThis(){
		myscrollend = false;

		//font
		//8 is font size. If you set nothing, font size becomes huge in Linux gtk env.
		wxFont myfont(wxFontInfo(9).Family(wxFONTFAMILY_MODERN));
		StyleSetFont(wxSTC_STYLE_DEFAULT, myfont);
	}

	void AddSyncWin(STCScroll *win){
		syncstc.push_back(win);
	}

	//override
	void ScrollWindow(int dx, int dy, const wxRect *rect = NULL) {
		if(!myscrollend){
			wxStyledTextCtrl::ScrollWindow(dx, dy, rect);
			myscrollend = true; 
			for(std::vector<STCScroll*>::iterator it = syncstc.begin(); it != syncstc.end(); ++it){
				(*it)->ScrollToLine(this->GetFirstVisibleLine());
			}
			myscrollend = false; 
		}
	}
private:
	std::vector<STCScroll*> syncstc;
	bool myscrollend;
};

class IBoxPanel : public wxPanel {
public:
	IBoxPanel(wxWindow *parent) : wxPanel(parent){
	}
	IBoxPanel(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size) : wxPanel(parent, id, pos, size){
	}

	void RegisterIBSTC(STCScroll *ibstc){
		ib_stc = ibstc;
	}
	void RegisterExanavModel(ExanavListModel *em){
		e_model = em;
	}
	void RegisterFileID(int id){
		e_fid = id;
	}
	void RegisterTBoxSizer(wxBoxSizer *tb){
		tbox = tb;
	}
	void RegisterRownumber(int rn){
		rownumber = rn;
	}
	void RegisterStctrl(STCScroll *st){
		stctrl = st;
	}
	void RegisterChoiceCTRL(wxChoice *cctrl){
		choicectrl = cctrl;
	}

	//event
	void OnIBoxChoice(wxCommandEvent &event){
		int cnum = (int)event.GetInt();
		int flc = e_model->fileinfov.at(e_fid).filelinecount;
		std::vector<std::string> linestrv(flc);
		std::stringstream ss("");

		for(int i=0; i<flc; i++){
			linestrv.at(i) = "";
		}

		std::string spacestr;
		unsigned int maxlen = e_model->fileinfov.at(e_fid).max_max_numofchar;
		for(std::vector<int>::iterator it = e_model->fileinfov.at(e_fid).listrownum.begin(); it != e_model->fileinfov.at(e_fid).listrownum.end(); ++it){
			spacestr = " ";
			switch(e_model->datatype[cnum]){
			case JD_TYPE_DECIMAL:
				ss << ((wxULongLong*)e_model->jdatapp[cnum])[(*it)];
				break;
			case JD_TYPE_HEXADECIMAL:
			case JD_TYPE_STRING:
				ss << ((std::string*)e_model->jdatapp[cnum])[(*it)];
				break;
			case JD_TYPE_SRC:
				ss << e_model->fileinfov.at(e_fid).filename_only;
				break;
			case JD_TYPE_DOUBLE:
				ss << ((double*)e_model->jdatapp[cnum])[(*it)];
				break;
			default:
				break;
			}

			for(unsigned int i=0; i< maxlen - ss.str().length(); i++){
				spacestr += " ";
			}
			if(linestrv.at(e_model->filelinenum[(*it)]-1) == ""){
				linestrv.at(e_model->filelinenum[(*it)]-1) += (spacestr + ss.str());
			} else {
				linestrv.at(e_model->filelinenum[(*it)]-1) += ("\n" + spacestr + ss.str());
			}
			ss.str("");
		}

		std::string allstr = "";
		for(int i=0; i<flc; i++){
			allstr += (linestrv.at(i) + "\n");
		}

		ib_stc->SetReadOnly(false);
		ib_stc->SetText(allstr);
		ib_stc->SetReadOnly(true);

		enum {
			LINE_STYLE,
			T_LINE_STYLE,
			UNDERLINE_STYLE
		};

		//maker setting
		ib_stc->StyleClearAll();

		ib_stc->MarkerDefine(LINE_STYLE, wxSTC_MARK_BACKGROUND, wxColour(0, 0, 0), wxColour(240, 240, 255));
		ib_stc->MarkerDefine(T_LINE_STYLE, wxSTC_MARK_BACKGROUND, wxColour(0, 0, 0), wxColour(255, 240, 240));
		ib_stc->StyleSetUnderline(UNDERLINE_STYLE, true);
		ib_stc->MarkerDefine(UNDERLINE_STYLE, wxSTC_MARK_UNDERLINE, wxColour(0, 0, 0), wxColour(255, 255, 255));

		//add marker
		for(std::vector<AnnotInfo>::iterator it = e_model->fileinfov.at(e_fid).annot_info.begin(); it != e_model->fileinfov.at(e_fid).annot_info.end(); ++it){
			ib_stc->MarkerAdd(it->str_rownum-1-1, UNDERLINE_STYLE);
			for(int j=0; j < it->annotnum; j++){
				if(it->rownum == e_model->filelinenum[rownumber]){
					ib_stc->MarkerAdd(it->str_rownum-1+j, T_LINE_STYLE);
				} else {
					ib_stc->MarkerAdd(it->str_rownum-1+j, LINE_STYLE);
				}
			}
		}

		ib_stc->ScrollToLine(stctrl->GetFirstVisibleLine());

		//this->SetSize(wxSize(20, -1));
		//tbox->Layout();
	}

//public member
	STCScroll *ib_stc;
	STCScroll *stctrl;
	wxChoice *choicectrl;
	int e_fid;
private:
	wxDECLARE_EVENT_TABLE();
	ExanavListModel *e_model;
	wxBoxSizer *tbox;
	int rownumber;
};

class TabPanel : public wxPanel {
public:
	TabPanel(wxWindow *parent) : wxPanel(parent){
		ib_num = 0;
	}

	void RegisterIBoxPanel(int num, IBoxPanel *ibp){
		ib_panel[num] = ibp;
	}

	void RegisterTBoxSizer(wxBoxSizer *tb){
		tbox = tb;
	}

	void RegisterPlusButton(wxButton *pb){
		plus_button = pb;
	}
	void RegisterMinusButton(wxButton *mb){
		minus_button = mb;
	}

	//event
	void OnIBoxPlusButton(wxCommandEvent &event){
		AVOID_UNUSED_WARNING_VOID(event);
		ib_num++;
		tbox->Show(ib_panel[ib_num-1]);
		if(ib_num == JD_COLNUM-2){
			plus_button->Enable(false);
		}
		if(ib_num == 1){
			minus_button->Enable(true);
		}
		tbox->Layout();

		minus_button->SetLabel("-"); //for display bug

		//below code does not work
		//ib_panel[ib_num-1]->ib_stc->ScrollToLine(ib_panel[ib_num-1]->stctrl->GetFirstVisibleLine());

		wxCommandEvent start_event;
		start_event.SetInt(ib_num-1);
		ib_panel[ib_num-1]->OnIBoxChoice(start_event);
		ib_panel[ib_num-1]->choicectrl->SetSelection(ib_num-1);
	}

	void OnIBoxMinusButton(wxCommandEvent &event){
		AVOID_UNUSED_WARNING_VOID(event);
		ib_num--;
		tbox->Hide(ib_panel[ib_num]);
		if(ib_num == JD_COLNUM-3){
			plus_button->Enable(true);
		}
		if(ib_num == 0){
			minus_button->Enable(false);
		}
		tbox->Layout();
	}

//public member
	IBoxPanel *ib_panel[JD_COLNUM-2];
	int ib_num;
private:
	wxDECLARE_EVENT_TABLE();
	wxButton *plus_button, *minus_button;
	wxBoxSizer *tbox;
};

//-----------------------------------------------
// wx Classes
//-----------------------------------------------

class MyApp : public wxApp {
public:
	virtual bool OnInit() wxOVERRIDE;
	virtual int OnExit() wxOVERRIDE;
};

class MyFrame : public wxFrame {
public:
	MyFrame(const wxString& title);
	virtual ~MyFrame();

	void OnOpen(wxCommandEvent& event);
	void OnSaveTabFile(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnSelectionChanged(wxDataViewEvent &event);

//public member
private:
	wxAuiManager auimgr;
	wxPanel *startuppanel;
	wxDECLARE_EVENT_TABLE();
	wxAuiNotebook *nb;
	ExanavListModel *exanav_model;
	bool guiend;
};

enum {
	ID_EXANAV_OPEN,
	ID_SAVE_TABFILE,
	ID_LIST_CTRL,
	ID_IBOX_CHOICE,
	ID_IBOXPLUS_BUTTON,
	ID_IBOXMINUS_BUTTON
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(ID_EXANAV_OPEN,  MyFrame::OnOpen)
	EVT_MENU(ID_SAVE_TABFILE,  MyFrame::OnSaveTabFile)
	EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    EVT_DATAVIEW_SELECTION_CHANGED(ID_LIST_CTRL, MyFrame::OnSelectionChanged)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(TabPanel, wxPanel)
	EVT_BUTTON(ID_IBOXPLUS_BUTTON, TabPanel::OnIBoxPlusButton)
	EVT_BUTTON(ID_IBOXMINUS_BUTTON, TabPanel::OnIBoxMinusButton)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(IBoxPanel, wxPanel)
	EVT_CHOICE(ID_IBOX_CHOICE, IBoxPanel::OnIBoxChoice)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
	if ( !wxApp::OnInit() )
		return false;

	//config
	SetVendorName(wxT("ExanaVTY"));
	SetAppName(wxT("exanav"));
	wxConfigBase *pConfig = wxConfigBase::Get();
	pConfig->SetRecordDefaults();

	//frame
	MyFrame *frame = new MyFrame("ExanaV");
	frame->Show(true);

	return true;
}

int MyApp::OnExit() {
	delete wxConfigBase::Set((wxConfigBase *) NULL);
	return 0;
}

MyFrame::MyFrame(const wxString& title)
		: wxFrame(NULL, wxID_ANY, title) {
	guiend = false;
	//notify wxAUI which frame to use
	auimgr.SetManagedWindow(this);

	//icon
	//SetIcon(wxICON(exanav));

	//menu
	wxMenu *fileMenu = new wxMenu;
	fileMenu->Append(ID_EXANAV_OPEN, "&Open JSON file\tCtrl-O", "Open .json file and set the directory include source code files");
	fileMenu->AppendSeparator();
	fileMenu->Append(ID_SAVE_TABFILE, "&Save source code file\tCtrl-S", "Save current tab's source code file");
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, "&Exit\tAlt-X", "Exit this program");

	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(wxID_ABOUT, "&About\tF1", "Show the information of this application");

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, "&File");
	menuBar->Append(helpMenu, "&Help");

	SetMenuBar(menuBar);

	//status bar
#if wxUSE_STATUSBAR
	CreateStatusBar();
	SetStatusText("Please open file");
#endif

	//restore frame position, size
    wxConfigBase *pConfig = wxConfigBase::Get();

    int x = pConfig->Read(wxT("/MainFrame/x"), 50),
        y = pConfig->Read(wxT("/MainFrame/y"), 50),
        w = pConfig->Read(wxT("/MainFrame/w"), 350),
        h = pConfig->Read(wxT("/MainFrame/h"), 200);
    Move(x, y);
    SetClientSize(w, h);

	//startup display
	startuppanel = new wxPanel(this);
	wxBoxSizer *startupbox = new wxBoxSizer(wxVERTICAL);
	startupbox->AddStretchSpacer(1);
	wxStaticText *st1 = new wxStaticText(startuppanel, wxID_ANY, "Exana Viewer");
	wxStaticText *st2 = new wxStaticText(startuppanel, wxID_ANY, "Alpha version");
	//st1->SetFont(wxFont(20, wxNORMAL, wxNORMAL, wxNORMAL));
	startupbox->Add(st1, 0,  wxALIGN_CENTER_HORIZONTAL);
	startupbox->Add(st2, 0,  wxALIGN_CENTER_HORIZONTAL);
	startupbox->AddStretchSpacer(1);
	startuppanel->SetSizer(startupbox);
}

MyFrame::~MyFrame() {
	wxConfigBase *pConfig = wxConfigBase::Get();
	if ( pConfig == NULL )
		return;
	
	// save the frame position
	int x, y, w, h;
	GetClientSize(&w, &h);
	GetPosition(&x, &y);
	pConfig->Write(wxT("/MainFrame/x"), (long) x);
	pConfig->Write(wxT("/MainFrame/y"), (long) y);
	pConfig->Write(wxT("/MainFrame/w"), (long) w);
	pConfig->Write(wxT("/MainFrame/h"), (long) h);

	auimgr.UnInit();
}

void MyFrame::OnOpen(wxCommandEvent& WXUNUSED(event)) {
	//get file path
	wxString fpath;
    fpath = wxFileSelector(
		wxT("Select .json file"),
		wxEmptyString, ".json",
		"json",
		wxString::Format
		(
		    wxT("JSON (*.json)|*.json"),
		    wxFileSelectorDefaultWildcardStr,
		    wxFileSelectorDefaultWildcardStr
		),
		wxFD_OPEN|wxFD_CHANGE_DIR|wxFD_PREVIEW|wxFD_NO_FOLLOW,
		this
	);

    if(!fpath) {
		//wxMessageBox("Invalid path", "Failed", wxOK | wxICON_INFORMATION, this);
        return;
	}

	//file open and read
	std::ifstream ifs(fpath);
	if (ifs.fail()) {
		wxMessageBox("Failed in opening file", "Failed", wxOK | wxICON_INFORMATION, this);
		return;
	}
    std::string str((std::istreambuf_iterator<char>(ifs)),
		std::istreambuf_iterator<char>());

	//create gui start
	exanav_model = new ExanavListModel();
	if(!exanav_model->initDoc(str)){
		wxMessageBox("Failed in parsing file", "Failed", wxOK | wxICON_INFORMATION, this);
		delete exanav_model;
		return;
	}

	delete(startuppanel);

#if defined __WXMSW__
    wxDataViewCtrl* listctrl = new wxDataViewCtrl(this, ID_LIST_CTRL, wxDefaultPosition, wxDefaultSize, wxDV_HORIZ_RULES | wxDV_VERT_RULES);
#elif defined __WXGTK__
    wxDataViewCtrl* listctrl = new wxDataViewCtrl(this, ID_LIST_CTRL);
#else
    wxDataViewCtrl* listctrl = new wxDataViewCtrl(this, ID_LIST_CTRL);
#endif
	listctrl->AssociateModel(exanav_model);
	exanav_model->DecRef();  // avoid memory leak

	//create column
	wxDataViewTextRenderer *tr;
	wxAlignment align;
    wxDataViewColumn *dvcol;

	for(int i=0; i<(int)(exanav_model->GetColumnCount()); i++) {
		switch(exanav_model->datatype[i]){
		case JD_TYPE_DECIMAL:
			tr = new wxDataViewTextRenderer("ulonglong", wxDATAVIEW_CELL_INERT);
			align = wxALIGN_RIGHT;
			break;
		case JD_TYPE_DOUBLE:
			tr = new wxDataViewTextRenderer("double", wxDATAVIEW_CELL_INERT);
			align = wxALIGN_RIGHT;
			break;
		case JD_TYPE_HEXADECIMAL:
			tr = new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_INERT);
			align = wxALIGN_LEFT;
			break;
		case JD_TYPE_STRING:
		case JD_TYPE_SRC:
			tr = new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_INERT);
			align = wxALIGN_LEFT;
			break;
		default:
			tr = new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_INERT);
			align = wxALIGN_LEFT;
			break;
		}

    	dvcol = new wxDataViewColumn(
			exanav_model->GetHeaderStr(i), tr, i, wxCOL_WIDTH_AUTOSIZE, align, 
			wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE | wxDATAVIEW_COL_REORDERABLE
		);
		//if you uncomment below, "gtk-critical warning" occurs
		//dvcol->SetMinWidth(30);
		listctrl->AppendColumn(dvcol);
	}

	auimgr.AddPane(listctrl, wxCENTER, wxT("Exana result"));

	//create aui notebook
	nb = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(800, 600), 
		wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER
		);

	wxAuiPaneInfo auipi;
	auipi.CloseButton(false);
	auipi.Top();
	auipi.Caption(wxT("Drag this bar separates window"));

    auimgr.AddPane(nb, auipi);

	auimgr.SetDockSizeConstraint(0.3, 0.7);

	guiend = true;
	auimgr.Update();
	//create gui end

	SetStatusText("Opening file was successful");
}

void MyFrame::OnSaveTabFile(wxCommandEvent& WXUNUSED(event)) {
	if(nb->GetPageCount() == 0) {
		wxMessageBox(wxString::Format("There are no opened file."), "error", wxOK | wxICON_INFORMATION);
	} else {
		IBoxPanel *ibp = ((TabPanel *)nb->GetCurrentPage())->ib_panel[0];
		ibp->stctrl->SaveFile(exanav_model->fileinfov.at(ibp->e_fid).filename);
		wxMessageBox(wxString::Format("Saving file was successful"), "message", wxOK | wxICON_INFORMATION);
	}
}

void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event)) {
	Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
	wxMessageBox(wxString::Format
		(
			"ExanaV is an Exana Relust Viewer Application.\n"
			"ExanaV Copyright (c) 2015 T.Y.\n"
			"\n"
			"--------------Using Library Licence--------------\n"
			"This application is made using RapidJSON.\n"
			"RapidJSON Copyright (c) 1998-2005 Julian Smart, Robert Roebling et al\n"
			"Released under the MIT license\n"
			"http://opensource.org/licenses/mit-license.php\n"
		),
		"About ExanaV",
		wxOK | wxICON_INFORMATION,
		this);
}

void MyFrame::OnSelectionChanged(wxDataViewEvent &event) {
	if(!guiend) {return;} //this code is for Linux gtk version
	if(!event.GetItem()){
		//click blank space
		return;
	}

	int rownumber = *(int*)(event.GetItem().GetID());

	//if source code file not exist
	if(!(exanav_model->fileinfov.at(exanav_model->fileid[rownumber]).fileexist)){
		return;
	}

	//in the case that source code file have alreadly opened.
	int newtabidx = -1;
	for(size_t i=0; i<nb->GetPageCount(); i++){
		if(((TabPanel *)nb->GetPage(i))->ib_panel[0]->e_fid == exanav_model->fileid[rownumber]){
			newtabidx = i;
		}
	}

	nb->Freeze();

	TabPanel *tpanel = new TabPanel(nb);
	wxBoxSizer *tbox = new wxBoxSizer(wxHORIZONTAL);
	tpanel->RegisterTBoxSizer(tbox);

	//styles
	enum {
	    MARGIN_LINE_NUMBERS, //for line number space setting
		//ANNOT_STYLE,
		LINE_STYLE,
		//T_ANNOT_STYLE,
		T_LINE_STYLE,
		UNDERLINE_STYLE
	};
	//avoid STYLE number confliction
	const int ANNOT_STYLE = wxSTC_STYLE_LASTPREDEFINED + 1;
	const int T_ANNOT_STYLE = wxSTC_STYLE_LASTPREDEFINED + 2;


	//make infoboxes
	IBoxPanel *ib_panel[JD_COLNUM-2];
	wxBoxSizer *ib_box[JD_COLNUM-2];
	STCScroll *ib_stc[JD_COLNUM-2];
	wxString choices[JD_COLNUM-2];
	wxChoice *choicectrl[JD_COLNUM-2];
	for(int i=0; i<JD_COLNUM-2; i++){
		choices[i] = exanav_model->headers[i];
	}
	int t_fileid = exanav_model->fileid[rownumber];
	STCScroll forTextWidth(tpanel, wxID_ANY); //just for cal text width
	int maxwidth = forTextWidth.TextWidth(0, " ") * (exanav_model->fileinfov.at(t_fileid).max_max_numofchar + 1) * 1.2;
	for(int i=0; i<JD_COLNUM-2; i++){
		ib_panel[i] = new IBoxPanel(tpanel, wxID_ANY, wxDefaultPosition, wxSize(maxwidth, -1));
		ib_box[i] = new wxBoxSizer(wxVERTICAL);
		choicectrl[i] = new wxChoice(ib_panel[i], ID_IBOX_CHOICE, wxDefaultPosition, wxSize(-1, 25), JD_COLNUM-2, choices);
		ib_box[i]->Add(choicectrl[i], 0, wxEXPAND | wxALL, 0);
		ib_stc[i] = new STCScroll(ib_panel[i], wxID_ANY, wxDefaultPosition, wxSize(-1, -1));

		ib_stc[i]->SetUseVerticalScrollBar(false);
		//if you use wxSTC_WRAP_NONE, horizontal scroll bar appears
		//if you use wxSTC_WRAP_WORD, horizontal scroll bar disappears
		ib_stc[i]->SetWrapMode(wxSTC_WRAP_NONE);
		ib_stc[i]->SetScrollWidth(1);

		//margin number 1 default is wasted space
		ib_stc[i]->SetMarginWidth(1, 0);

		ib_panel[i]->RegisterIBSTC(ib_stc[i]);
		ib_panel[i]->RegisterExanavModel(exanav_model);
		ib_panel[i]->RegisterFileID(exanav_model->fileid[rownumber]);
		ib_panel[i]->RegisterRownumber(rownumber);
		ib_panel[i]->RegisterTBoxSizer(tbox);
		ib_panel[i]->RegisterChoiceCTRL(choicectrl[i]);
		tpanel->RegisterIBoxPanel(i, ib_panel[i]);

		ib_box[i]->Add(ib_stc[i], 1, wxEXPAND | wxALL, 0);
		ib_panel[i]->SetSizer(ib_box[i]);
		tbox->Add(ib_panel[i], 0, wxEXPAND | wxALL, 0);
		tbox->Hide(ib_panel[i]);

		ib_stc[i]->SetReadOnly(true);
	}


	//make source code box
	wxPanel *s_panel = new wxPanel(tpanel);
	wxBoxSizer *s_box = new wxBoxSizer(wxVERTICAL);
	//s_box->Add(new wxTextCtrl(s_panel, wxID_ANY, wxT("SOURCE CODE")), 0, wxEXPAND | wxALL, 0);
	//s_box->Add(new wxButton(s_panel, wxID_ANY, wxT("SOURCE CODE")), 0, wxEXPAND | wxALL, 0);
	//s_box->Add(new wxTextCtrl(s_panel, wxID_ANY, wxT("SOURCE CODE"), wxDefaultPosition, wxSize(-1, -1), wxALIGN_CENTRE_HORIZONTAL), 0, wxEXPAND | wxALL, 0);

	wxPanel *topbar_panel = new wxPanel(s_panel);
	wxBoxSizer *topbar_box = new wxBoxSizer(wxHORIZONTAL);
	wxButton *minus_button = new wxButton(topbar_panel, ID_IBOXMINUS_BUTTON, wxT("-"), wxDefaultPosition, wxSize(25, 25));
	wxButton *plus_button = new wxButton(topbar_panel, ID_IBOXPLUS_BUTTON, wxT("+"), wxDefaultPosition, wxSize(25, 25));
	topbar_box->Add(minus_button, wxALIGN_LEFT, 0, wxEXPAND | wxALL, 0);
	topbar_box->Add(plus_button, wxALIGN_LEFT, 0, wxEXPAND | wxALL, 0);
	tpanel->RegisterMinusButton(minus_button);
	tpanel->RegisterPlusButton(plus_button);

	topbar_box->Add(new wxStaticText(topbar_panel, wxID_ANY, wxT("SOURCE CODE"), wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTRE_HORIZONTAL), wxEXPAND | wxALIGN_CENTRE | wxALL);

	topbar_panel->SetSizer(topbar_box);
	s_box->Add(topbar_panel, 0, wxEXPAND | wxALL, 0);

	STCScroll* stctrl = new STCScroll(s_panel, wxID_ANY);

	//stctrl style setting start
	stctrl->StyleClearAll();

	//line number space setting
	stctrl->SetMarginWidth(MARGIN_LINE_NUMBERS, 40); //width
	//stctrl->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(75, 75, 75)); //darkgrey
	//stctrl->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(220, 220, 220)); //lightgrey
	stctrl->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(75, 75, 75));
	stctrl->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(220, 220, 220));
	stctrl->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);

	stctrl->SetWrapMode(wxSTC_WRAP_NONE);

	//syntax highlighting
	std::vector<std::string> sv;
	sv = split(exanav_model->fileinfov.at(exanav_model->fileid[rownumber]).filename, '.');
	if(sv.back() == "f"){
		stctrl->SetLexer(wxSTC_LEX_FORTRAN);

		//stctrl->StyleSetForeground(wxSTC_F_DEFAULT, wxColour());
		stctrl->StyleSetForeground(wxSTC_F_COMMENT, wxColour(150,150,150));
		stctrl->StyleSetForeground(wxSTC_F_NUMBER, wxColour(0,150,0));
		stctrl->StyleSetForeground(wxSTC_F_STRING1, wxColour(150,0,0));
		//stctrl->StyleSetForeground(wxSTC_F_STRING2, wxColour());
		//stctrl->StyleSetForeground(wxSTC_F_STRINGEOL, wxColour());
		//stctrl->StyleSetForeground(wxSTC_F_OPERATOR, wxColour());
		stctrl->StyleSetForeground(wxSTC_F_IDENTIFIER, wxColour(40,0,60));
		stctrl->StyleSetForeground(wxSTC_F_WORD, wxColour(0,0,150));
		stctrl->StyleSetForeground(wxSTC_F_WORD2, wxColour(0,150,0));
		//stctrl->StyleSetForeground(wxSTC_F_WORD3, wxColour());
		stctrl->StyleSetForeground(wxSTC_F_PREPROCESSOR, wxColour(165,105,0));
		//stctrl->StyleSetForeground(wxSTC_F_OPERATOR2, wxColour());
		//stctrl->StyleSetForeground(wxSTC_F_LABEL, wxColour());
		//stctrl->StyleSetForeground(wxSTC_F_CONTINUATION, wxColour());
	} else if((sv.back() == "c") || (sv.back() == "cpp") || (sv.back() == "h")){
		stctrl->SetLexer(wxSTC_LEX_CPP);

		//stctrl->StyleSetForeground(wxSTC_C_DEFAULT, wxColour());
		stctrl->StyleSetForeground(wxSTC_C_COMMENT, wxColour(150,150,150));
		stctrl->StyleSetForeground(wxSTC_C_COMMENTLINE, wxColour(150,150,150));
		stctrl->StyleSetForeground(wxSTC_C_COMMENTDOC, wxColour(150,150,150));
		stctrl->StyleSetForeground(wxSTC_C_NUMBER, wxColour(0,150,0));
		stctrl->StyleSetForeground(wxSTC_C_WORD, wxColour(0,0,150));
		stctrl->StyleSetForeground(wxSTC_C_STRING, wxColour(150,0,0));
		stctrl->StyleSetForeground(wxSTC_C_CHARACTER, wxColour(150,0,0));
		//stctrl->StyleSetForeground(wxSTC_C_UUID, wxColour());
		stctrl->StyleSetForeground(wxSTC_C_PREPROCESSOR, wxColour(165,105,0));
		//stctrl->StyleSetForeground(wxSTC_C_OPERATOR, wxColour());
		stctrl->StyleSetForeground(wxSTC_C_IDENTIFIER, wxColour(40,0,60));
		//stctrl->StyleSetForeground(wxSTC_C_STRINGEOL, wxColour());
		//stctrl->StyleSetForeground(wxSTC_C_VERBATIM, wxColour());
		//stctrl->StyleSetForeground(wxSTC_C_REGEX, wxColour());
		//stctrl->StyleSetForeground(wxSTC_C_COMMENTLINEDOC, wxColour());
		stctrl->StyleSetForeground(wxSTC_C_WORD2, wxColour(0,150,0));
		stctrl->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, wxColour(0,0,200));
		stctrl->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour(0,0,200));
		//stctrl->StyleSetForeground(wxSTC_C_GLOBALCLASS, wxColour());
		//stctrl->StyleSetForeground(wxSTC_C_STRINGRAW, wxColour());
		//stctrl->StyleSetForeground(wxSTC_C_TRIPLEVERBATIM, wxColour());
		//stctrl->StyleSetForeground(wxSTC_C_HASHQUOTEDSTRING, wxColour());
		//stctrl->StyleSetForeground(wxSTC_C_PREPROCESSORCOMMENT, wxColour());
		//stctrl->StyleSetForeground(wxSTC_C_PREPROCESSORCOMMENTDOC, wxColour());
		//stctrl->StyleSetForeground(wxSTC_C_USERLITERAL, wxColour());
		//stctrl->StyleSetForeground(wxSTC_C_TASKMARKER, wxColour());
		//stctrl->StyleSetForeground(wxSTC_C_ESCAPESEQUENCE, wxColour());

		//stctrl->StyleSetBold(wxSTC_C_WORD, true);
		//stctrl->StyleSetBold(wxSTC_C_WORD2, true);
		//stctrl->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, true);

		// a sample list of keywords, I haven't included them all to keep it short...
		stctrl->SetKeyWords(0, wxT("return for while break continue"));
		stctrl->SetKeyWords(1, wxT("const int float void char double"));
	} else {
		//no syntax highlighting
	}

	//default width is very wide
	stctrl->SetScrollWidth(1);
	//stctrl style setting end

	//stctrl load file
	wxString targetpath;
	targetpath = exanav_model->fileinfov.at(exanav_model->fileid[rownumber]).filename;
	stctrl->LoadFile(targetpath);

	//annotation and marker style
	stctrl->StyleSetBackground(ANNOT_STYLE, wxColour(240, 240, 255));
	//stctrl->StyleSetForeground(ANNOT_STYLE, *wxBLACK);
	//stctrl->StyleSetSizeFractional(ANNOT_STYLE, (StyleGetSizeFractional(wxSTC_STYLE_DEFAULT)*4)/5);
	stctrl->StyleSetBackground(T_ANNOT_STYLE, wxColour(255, 240, 240));
	stctrl->MarkerDefine(LINE_STYLE, wxSTC_MARK_BACKGROUND, wxColour(0, 0, 0), wxColour(217, 217, 255));
	stctrl->MarkerDefine(T_LINE_STYLE, wxSTC_MARK_BACKGROUND, wxColour(0, 0, 0), wxColour(255, 217, 217));

	//stctrl add annotations and marker
	for(std::vector<AnnotInfo>::iterator it = exanav_model->fileinfov.at(t_fileid).annot_info.begin(); it != exanav_model->fileinfov.at(t_fileid).annot_info.end(); ++it){
		std::string nstr = "";
		for(int i=0; i<(it->annotnum-2); i++){
			nstr += "\n";
		}
		if(it->rownum == exanav_model->filelinenum[rownumber]){
			if(it->annotnum > 1) {
    			stctrl->AnnotationSetText(it->rownum-1, nstr);
    			stctrl->AnnotationSetStyle(it->rownum-1, T_ANNOT_STYLE);
			}
			stctrl->MarkerAdd(it->rownum-1, T_LINE_STYLE);
		} else {
			if(it->annotnum > 1) {
    			stctrl->AnnotationSetText(it->rownum-1, nstr);
    			stctrl->AnnotationSetStyle(it->rownum-1, ANNOT_STYLE);
			}
			stctrl->MarkerAdd(it->rownum-1, LINE_STYLE);
		}
	}
    stctrl->AnnotationSetVisible(wxSTC_ANNOTATION_STANDARD);

	stctrl->SetUseVerticalScrollBar(true);

	s_box->Add(stctrl, 1, wxEXPAND | wxALL, 0);
	s_panel->SetSizer(s_box);
	tbox->Add(s_panel, 1, wxEXPAND | wxALL, 0);


	stctrl->SetReadOnly(false);

	//sync scroll
	for(int i=0; i<JD_COLNUM-2; i++){
		stctrl->AddSyncWin(ib_stc[i]);

		ib_stc[i]->AddSyncWin(stctrl);
		for(int j=0; j<JD_COLNUM-2; j++){
			if(i != j){
				ib_stc[i]->AddSyncWin(ib_stc[j]);
			}
		}
	}

	for(int i=0; i<JD_COLNUM-2; i++){
		ib_panel[i]->RegisterStctrl(stctrl);
	}

	tpanel->SetSizer(tbox);


	int dtabIBOXnum = 0;
	int dtabIBOXchoice[JD_COLNUM-2];
	TabPanel *dtp;
	if(newtabidx < 0) {
   		nb->AddPage(tpanel, exanav_model->fileinfov.at(exanav_model->fileid[rownumber]).filename_only, true);
	} else {
		dtp = (TabPanel *)nb->GetPage(newtabidx);
		dtabIBOXnum = dtp->ib_num;
		for(int i=0; i<dtabIBOXnum; i++){
			dtabIBOXchoice[i] = dtp->ib_panel[i]->choicectrl->GetSelection();
		}

		nb->DeletePage(newtabidx);
   		nb->InsertPage(newtabidx, tpanel, exanav_model->fileinfov.at(exanav_model->fileid[rownumber]).filename_only, true);

		dtp = (TabPanel *)nb->GetPage(newtabidx);
		for(int i=0; i<dtabIBOXnum; i++){
			dtp->ib_panel[i]->choicectrl->SetSelection(dtabIBOXchoice[i]);
		}
	}

	//goto line
	stctrl->GotoLine(exanav_model->filelinenum[rownumber]-1);
	for(int i=0; i<JD_COLNUM-2; i++){
		ib_stc[i]->ScrollToLine(stctrl->GetFirstVisibleLine());
	}
	//default start up UI
	wxCommandEvent start_event;
	for(int i=0; i<JD_COLNUM-2; i++){
		tpanel->OnIBoxPlusButton(start_event);
		if(i < dtabIBOXnum){
			choicectrl[i]->SetSelection(dtabIBOXchoice[i]);
			start_event.SetInt(dtabIBOXchoice[i]);
		} else {
			choicectrl[i]->SetSelection(i);
			start_event.SetInt(i);
		}
		ib_panel[i]->OnIBoxChoice(start_event);
	}
	if(newtabidx<0) { dtabIBOXnum = 2; }
	for(int i=0; i<JD_COLNUM-2-dtabIBOXnum; i++){
		tpanel->OnIBoxMinusButton(start_event);
	}


	nb->Thaw();
}

