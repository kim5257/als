/*
 * @file dir_entry.h
 * @date 2015. 9. 7.
 *      Author: asran
 */

#include <list>
#include <string>

namespace file
{

class DirEntry
{
public:
	typedef std::list<DirEntry*> dir_entry_list;
	typedef dir_entry_list::iterator dir_entry_list_itor;
private:
	int mDirType;
	std::string mFileName;
	DirEntry* mParent;
	dir_entry_list mChilds;
private:
	DirEntry (	std::string fileName,
				DirEntry* parent);
	virtual
	~DirEntry (void);
private:
	void
	init (void);
	void
	deinit (void);
public:
	void
	addChild (DirEntry* child);
	dir_entry_list_itor
	getChild (std::string name);
	std::string
	getPath (void) const;
public:
	inline int
	getDirType (void) const;
	inline std::string
	getFileName (void) const;
	inline DirEntry*
	getParent (void) const;
	inline dir_entry_list&
	getChilds (void);
	inline bool
	isFound (const dir_entry_list_itor& itor) const;
public:
	static DirEntry*
	create (std::string fileName,
			DirEntry* parent);
	static void
	destroy (DirEntry* dirEntry);
};

int
DirEntry::getDirType (void) const
{
	return mDirType;
}

std::string
DirEntry::getFileName (void) const
{
	return mFileName;
}

DirEntry*
DirEntry::getParent (void) const
{
	return mParent;
}

DirEntry::dir_entry_list&
DirEntry::getChilds (void)
{
	return mChilds;
}

bool
DirEntry::isFound (const DirEntry::dir_entry_list_itor& itor) const
{
	return (mChilds.end () != itor)? (true):(false);
}

}

