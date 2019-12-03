#include <sys/stat.h>
#include <dirent.h>

namespace ml {

namespace util
{
	//
	// These hash functions are taken from http://www.burtleburtle.net/bob/hash/doobs.html
	//

	inline void hashMix(UINT &a, UINT &b, UINT &c)
	{
		a -= b; a -= c; a ^= (c>>13);
		b -= c; b -= a; b ^= (a<<8);
		c -= a; c -= b; c ^= (b>>13);
		a -= b; a -= c; a ^= (c>>12);
		b -= c; b -= a; b ^= (a<<16);
		c -= a; c -= b; c ^= (b>>5);
		a -= b; a -= c; a ^= (c>>3);
		b -= c; b -= a; b ^= (a<<10);
		c -= a; c -= b; c ^= (b>>15);
	}

	UINT hash32(const BYTE *k, UINT length)
	{
		UINT a, b, c, LocalLength;

		/* Set up the internal state */
		LocalLength = length;
		a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
		c = 0x9b9773e9;

		/*---------------------------------------- handle most of the key */
		while (LocalLength >= 12)
		{
			a += (k[0] + ((UINT)k[1]<<8) + ((UINT)k[2]<<16) + ((UINT)k[3]<<24));
			b += (k[4] + ((UINT)k[5]<<8) + ((UINT)k[6]<<16) + ((UINT)k[7]<<24));
			c += (k[8] + ((UINT)k[9]<<8) + ((UINT)k[10]<<16)+ ((UINT)k[11]<<24));
			hashMix(a, b, c);
			k += 12;
			LocalLength -= 12;
		}

		/*------------------------------------- handle the last 11 bytes */
		c += length;
		switch(LocalLength)              /* all the case statements fall through */
		{
		case 11: c += ((UINT)k[10]<<24);
		case 10: c += ((UINT)k[9]<<16);
		case 9 : c += ((UINT)k[8]<<8);
			/* the first byte of c is reserved for the length */
		case 8 : b += ((UINT)k[7]<<24);
		case 7 : b += ((UINT)k[6]<<16);
		case 6 : b += ((UINT)k[5]<<8);
		case 5 : b += k[4];
		case 4 : a += ((UINT)k[3]<<24);
		case 3 : a += ((UINT)k[2]<<16);
		case 2 : a += ((UINT)k[1]<<8);
		case 1 : a += k[0];
			/* case 0: nothing left to add */
		}
		hashMix(a, b, c);
		/*-------------------------------------------- report the result */
		return c;
	}

	UINT64 hash64(const BYTE *k, UINT length)
	{
		UINT a, b, c, LocalLength;

		/* Set up the internal state */
		LocalLength = length;
		a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
		c = 0x9b9773e9;

		/*---------------------------------------- handle most of the key */
		while (LocalLength >= 12)
		{
			a += (k[0] + ((UINT)k[1]<<8) + ((UINT)k[2]<<16) + ((UINT)k[3]<<24));
			b += (k[4] + ((UINT)k[5]<<8) + ((UINT)k[6]<<16) + ((UINT)k[7]<<24));
			c += (k[8] + ((UINT)k[9]<<8) + ((UINT)k[10]<<16)+ ((UINT)k[11]<<24));
			hashMix(a, b, c);
			k += 12;
			LocalLength -= 12;
		}

		/*------------------------------------- handle the last 11 bytes */
		c += length;
		switch(LocalLength)              /* all the case statements fall through */
		{
		case 11: c += ((UINT)k[10]<<24);
		case 10: c += ((UINT)k[9]<<16);
		case 9 : c += ((UINT)k[8]<<8);
			/* the first byte of c is reserved for the length */
		case 8 : b += ((UINT)k[7]<<24);
		case 7 : b += ((UINT)k[6]<<16);
		case 6 : b += ((UINT)k[5]<<8);
		case 5 : b += k[4];
		case 4 : a += ((UINT)k[3]<<24);
		case 3 : a += ((UINT)k[2]<<16);
		case 2 : a += ((UINT)k[1]<<8);
		case 1 : a += k[0];
			/* case 0: nothing left to add */
		}
		hashMix(a, b, c);
		/*-------------------------------------------- report the result */
		return UINT64(c) + UINT64(UINT64(a) << 32);
	}

	std::istream& safeGetline(std::istream& is, std::string& t)
	{
		t.clear();

		// The characters in the stream are read one-by-one using a std::streambuf.
		// That is faster than reading them one-by-one using the std::istream.
		// Code that uses streambuf this way must be guarded by a sentry object.
		// The sentry object performs various tasks,
		// such as thread synchronization and updating the stream state.

		std::istream::sentry se(is, true);
		std::streambuf* sb = is.rdbuf();

		for (;;) {
			int c = sb->sbumpc();
			switch (c) {
			case '\n':
				return is;
			case '\r':
				if (sb->sgetc() == '\n')
					sb->sbumpc();
				return is;
			case EOF:
				// Also handle the case when the last line has no line ending
				if (t.empty())
					is.setstate(std::ios::eofbit);
				return is;
			default:
				t += (char)c;
			}
		}
	}

	bool fileExists(const std::string &filename)
	{
		std::ifstream file(filename);
		return (!file.fail());
	}

	std::string getNextLine(std::ifstream &file)
	{
		std::string line;
		getline(file, line);
		return std::string(line.c_str());
	}

	std::vector<std::string> getFileLines(std::ifstream &file, UINT minLineLength)
	{
		std::vector<std::string> result;
		std::string line;
		while(!file.fail())
		{
			getline(file, line);
			if(!file.fail() && line.length() >= minLineLength) result.push_back(line.c_str());
		}
		return result;
	}

	void writeToFile(const std::string &line, const std::string& filename) {
		std::ofstream file;
		file.open(filename, std::ios::out);
		file << line << std::endl;
		file.close();
	}

	// TODO: rename this writeToFile.
	void saveLinesToFile(const std::vector<std::string>& lines, const std::string& filename) {
		std::ofstream file;
		file.open(filename, std::ios::out);
		for (const std::string line : lines) { file << line << std::endl; }
		file.close();
	}

	std::vector<std::string> splitPath(const std::string& path)
	{
		if (path.size() == 0) return std::vector<std::string>();
		const std::string dir = replace(path, '\\', '/');
		std::vector<std::string> dirParts = split(dir, '/');
		return dirParts;
	}

    std::string directoryFromPath(const std::string& path)
    {
		if (path.size() == 0) return path;
		if (path.back() == '\\' || path.back() == '/') return path;		
		size_t c = path.size();
		while (c) {
			c--;
			if (path[c] == '\\' || path[c] == '/') {
				if (c == 0) return "";
				else return path.substr(0, c) + '/';
			}
		}
		return "";
    }

    std::string fileNameFromPath(const std::string& path)
    {
		//TODO maybe use: PathRemoveFilespec ?
        return ml::util::split(ml::util::replace(path, '\\', '/'), '/').back();
    }

    std::string removeExtensions(const std::string& path)
    {
		std::string filename = util::fileNameFromPath(path);
		std::string directory = util::directoryFromPath(path);
		std::size_t found = filename.find_last_of('.');
		if (found != std::string::npos) {
			if (!directory.empty())	return directory + filename.substr(0, found);
			return filename.substr(0, found);
		}
		return path;
    }

	std::vector<std::string> getFileLines(const std::string &filename, UINT minLineLength)
	{
		std::ifstream file(filename);
		MLIB_ASSERT_STR(!file.fail(), std::string("Failed to open ") + filename);
		return getFileLines(file, minLineLength);
	}

	std::vector<BYTE> getFileData(const std::string &filename)
	{
		FILE *inputFile = util::checkedFOpen(filename.c_str(), "rb");
		UINT64 fileSize = util::getFileSize(filename);
		std::vector<BYTE> result(fileSize);
		util::checkedFRead(&result[0], sizeof(BYTE), fileSize, inputFile);
		fclose(inputFile);
		return result;
	}

	void copyFile(const std::string &sourceFile, const std::string &destFile)
	{
		std::vector<BYTE> data = getFileData(sourceFile);
		FILE *file = util::checkedFOpen(destFile.c_str(), "wb");
		util::checkedFWrite(&data[0], sizeof(BYTE), data.size(), file);
		fclose(file);
	}

	void renameFile(const std::string& oldFilename, const std::string& newFilename) 
	{
		rename(oldFilename.c_str(), newFilename.c_str());
	}

	void copyStringToClipboard(const std::string &S)
	{

	}

	std::string loadStringFromClipboard()
	{
		return "";
	}

	size_t getFileSize(const std::string &filename)
	{
		struct stat statbuf;
		int success = stat(filename.c_str(), &statbuf);
		MLIB_ASSERT_STR(success == 0, std::string("stat failed on ") + filename);
		return statbuf.st_size;
	}

	// Create a process with the given command line, and wait until it returns
	int runCommand(const std::string &executablePath, const std::string &commandLine, bool block)
	{
		return 0;
	}

	void makeDirectory(const std::string &directory)
	{
		mkdir(directory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
	
	bool directoryExists(const std::string& directory) {
		const char *pzPath = directory.c_str();
		DIR *pDir;
		bool bExists = false;
		pDir = opendir(pzPath);
		if (pDir != NULL)
		{
			bExists = true;
			(void)closedir(pDir);
		}

		return bExists;
		// int state = access(directory.c_str(), F_OK | R_OK | W_OK);
		// if (state == 0) return true;  // this is not a directory!
		// else return false;    // this is not a directory!
	}
	void deleteFile(const std::string& file) {
        std::remove(file.c_str());
    }
	
	
	
}  // namespace util

}  // namespace ml
