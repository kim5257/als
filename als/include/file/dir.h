/*
 * @file dir.h
 * @date 2015. 9. 7.
 *      Author: asran
 */
#ifndef FILE_DIR_H_
#define FILE_DIR_H_

#include <string>
#include <file/dir_entry.h>

namespace file
{

class Dir
{
private:
	DirEntry* mRoot;
private:
	Dir (std::string rootPath);
	virtual
	~Dir (void);
private:
	void
	init (std::string path);
	void
	deinit (void);
	void
	setRootPath (std::string path);
	void
	addRecursivePath (DirEntry* baseWatch);
public:
	static Dir*
	create (std::string rootPath);
	static void
	destroy (Dir* fileNotify);
};

}

#endif /* FILE_DIR_H_ */
