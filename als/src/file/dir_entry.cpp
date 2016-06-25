/*
 * @file dir_entry.cpp
 * @date 2015. 9. 7.
 *      Author: asran
 */

#include <stdio.h>
#include "error/err_msg.h"
#include "file/dir_entry.h"

namespace file
{

DirEntry::DirEntry (std::string fileName,
					DirEntry* parent)
		: mDirType (0),
			mParent (NULL)
{

}

DirEntry::~DirEntry (void)
{

}

void
init (void)
{

}

void
DirEntry::deinit (void)
{
	for(dir_entry_list_itor itor = mChilds.begin ();itor != mChilds.end ();
			++itor)
	{
		delete *itor;
	}
}

void
DirEntry::addChild (DirEntry* child)
{
	if ( this != child->getParent () )
	{
		throw error::ErrMsg::createErrMsg ("추가하려는 자식 폴더의 부모가 아닙니다.");
	}
	mChilds.push_back (child);
}

DirEntry::dir_entry_list_itor
DirEntry::getChild (std::string name)
{
	dir_entry_list_itor itor = mChilds.begin ();

	for(;itor != mChilds.end ();++itor)
	{
		if ( (*itor)->getFileName () == name )
		{
			break;
		}
	}

	return itor;
}

std::string
DirEntry::getPath (void) const
{
	std::string path = mFileName;
	DirEntry* parent = getParent ();

	while( parent != NULL )
	{
		path = parent->getFileName () + '/' + path;
		parent = parent->getParent ();
	}

	return path;
}

DirEntry*
DirEntry::create (	std::string fileName,
					DirEntry* parent)
{
	DirEntry* dirEntry = NULL;

	dirEntry = new DirEntry (fileName, parent);

	return dirEntry;
}

void
DirEntry::destroy (DirEntry* dirEntry)
{
	delete dirEntry;
}

}
