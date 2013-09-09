
#include "BreadSerializeBase.h"
#include "BreadUtils.h"
#include "BreadStringHelper.h"
#include "BreadFileHelper.h"
#include "BreadFileSystem.h"

namespace Bread
{
	/*
	 *	
	 */
	cSerializeBase::cSerializeBase() : m_file(NULL)
	{
	}
	/*
	 *	
	 */
	cSerializeBase::~cSerializeBase()
	{
		safeDelete(m_file);
	}
	/*
	 *	
	 */
	bool cSerializeBase::open(std_string const& filename)
	{
		m_file = _getFileSystem()->open(filename);

		if (!m_file)
			return false;

		return true;
	}
	/*
	 *	
	 */
	void cSerializeBase::close()
	{
		m_file->close();
	}
	/*
	*/
	bool cSerializeBase::eof()
	{
		return m_file->eof();
	}
	/*
	 *	
	 */
	void cSerializeBase::getLine(char* buffer, size_t bufferSize)
	{
		m_file->getLine(buffer, bufferSize);
	}
	/*
	 *	
	 */
	void cSerializeBase::readName(TCHAR* name)
	{
		char _name[NAME_LEN];
		m_file->read(_name, sizeof (_name));
		_getStringHelper()->m2w(_name, name, NAME_LEN);
	}
	/*
	 *	
	 */
	void cSerializeBase::readNameW(TCHAR* name)
	{
		m_file->read((char*)name, NAME_LEN * sizeof (TCHAR));
	}
	/*
	 *	
	 */
	void cSerializeBase::readBUID(BUID& value)
	{
		TCHAR name[NAME_LEN];
		readName(name);
		value = name;
	}
	/*
	 *	
	 */
	void cSerializeBase::readBUIDW(BUID& value)
	{
		TCHAR name[NAME_LEN];
		readNameW(name);
		value = name;
	}
	/*
	 *	
	 */
	void cSerializeBase::readBuffer(char* buffer, size_t bufferSize)
	{
		m_file->read(buffer, bufferSize);
	}
	/*
	*/
	string_id cSerializeBase::getFullName() const
	{
		return m_file->getFullName();
	}
	/*
	*/
	string_id cSerializeBase::getFileName() const
	{
		return m_file->getFileName();
	}
	/*
	*/
	string_id cSerializeBase::getDir() const
	{
		return m_file->getDir();
	}
}