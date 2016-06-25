/*
 * @file dir.cpp
 * @date 2015. 9. 7.
 *      Author: asran
 */

#include "file/dir.h"
#include <stdio.h>

namespace file
{

Dir::Dir (std::string rootPath)
:mRoot(NULL)
{

}

Dir::~Dir (void)
{

}

void
Dir::init (std::string path)
{

}

void
Dir::deinit (void)
{

}

void
Dir::setRootPath (std::string path)
{

}

void
Dir::addRecursivePath (DirEntry* baseWatch)
{
	//mRoot = addPath (NULL, path);
	addRecursivePath (mRoot);
}

Dir*
Dir::create (std::string rootPath)
{
	return NULL;
}

void
Dir::destroy (Dir* fileNotify)
{

}

}


