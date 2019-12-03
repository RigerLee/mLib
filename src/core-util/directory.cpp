
namespace ml {

Directory::Directory(const std::string& path)
{
    init(path);
}

std::vector<std::string> Directory::getFilesWithSuffix(const std::string& suffix) const
{
    std::vector<std::string> result;
    for (size_t fileIndex = 0; fileIndex < m_files.size(); fileIndex++)
    {
        const std::string& filename = m_files[fileIndex];
        if(util::endsWith(filename, suffix))
        {
            result.push_back(filename);
        }
    }
    return result;
}



std::vector<std::string> Directory::getFilesWithPrefix(const std::string& prefix) const
{
	std::vector<std::string> result;
	for (size_t fileIndex = 0; fileIndex < m_files.size(); fileIndex++)
	{
		const std::string& filename = m_files[fileIndex];
		if (util::startsWith(filename, prefix))
		{
			result.push_back(filename);
		}
	}
	return result;
}

std::vector<std::string> Directory::getFilesContaining(const std::string &str) const
{
	std::vector<std::string> result;
	for (size_t fileIndex = 0; fileIndex < m_files.size(); fileIndex++)
	{
		const std::string& filename = m_files[fileIndex];
		if (util::contains(filename, str))
		{
			result.push_back(filename);
		}
	}
	return result;
}




std::vector<std::string> Directory::getDirectoriesWithSuffix(const std::string& suffix) const
{
	std::vector<std::string> result;
	for (size_t fileIndex = 0; fileIndex < m_directories.size(); fileIndex++)
	{
		const std::string& filename = m_directories[fileIndex];
		if (util::endsWith(filename, suffix))
		{
			result.push_back(filename);
		}
	}
	return result;
}



std::vector<std::string> Directory::getDirectoriesWithPrefix(const std::string& prefix) const
{
	std::vector<std::string> result;
	for (size_t fileIndex = 0; fileIndex < m_directories.size(); fileIndex++)
	{
		const std::string& filename = m_directories[fileIndex];
		if (util::startsWith(filename, prefix))
		{
			result.push_back(filename);
		}
	}
	return result;
}

std::vector<std::string> Directory::getDirectoriesContaining(const std::string &str) const
{
	std::vector<std::string> result;
	for (size_t fileIndex = 0; fileIndex < m_directories.size(); fileIndex++)
	{
		const std::string& filename = m_directories[fileIndex];
		if (util::contains(filename, str))
		{
			result.push_back(filename);
		}
	}
	return result;
}


void Directory::init(const std::string &path)
{
    //std::cout << "Loading all files in " << path << std::endl;
	m_path = path + "\\";
	m_files.clear();
	m_directories.clear();

	auto dir = opendir(path.c_str());
    if (dir == nullptr)
    {
        std::cout << "Could not open " << path << std::endl;
        return; // could not open directory
    }

	auto entity = readdir(dir);

	while (entity != nullptr) {
		auto entity = readdir(dir);
        //std::cout << "Reading entity " << entity << std::endl;
        if (entity == nullptr) break;
		if (entity->d_type == DT_DIR) {
			// don't process  '..' & '.' directories
			if(entity->d_name[0] != '.')
				m_directories.push_back(std::string(entity->d_name));
		}
		else if (entity->d_type == DT_REG) {
            //std::cout << "Reading file " << entity->d_name << std::endl;
			m_files.push_back(std::string(entity->d_name));
		}
	}
	
	closedir(dir);
}

}  // namespace ml
