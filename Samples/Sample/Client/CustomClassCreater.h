
#ifndef _CustomClassCreater_h_
#define _CustomClassCreater_h_


class cCustomClassCreater : public cClassCreater
{
public:
	cCustomClassCreater() {}
	virtual ~cCustomClassCreater() {}
	virtual cGui* newGui();
};










#endif