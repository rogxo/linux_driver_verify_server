#pragma once
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>


class c_KeyHandler {

private:
	std::map<std::string, uint64_t> m_KeyData;
	std::string m_DataFilePath;

	bool ReadMapFile(std::string file_path) {
		
		std::ifstream infile(file_path);
		if (infile.is_open())
		{
			std::string line, key;
			uint64_t val = 0;
			while (std::getline(infile, line))
			{
				key = line.substr(0, line.find('\t'));
				if (key == "")	continue;
				std::stringstream(line.substr(line.find('\t') + 1)) >> std::dec >> val;
				m_KeyData.insert({ key ,val });
			}
			infile.close();
			return true;
		}
		return false;
	}

	bool WriteMapFile(std::string file_path) {
		
		std::ofstream outfile(file_path);
		if (outfile.is_open())
		{
			for (const auto& object : this->m_KeyData) {
				outfile << object.first << "\t" << object.second << std::endl;
			}
			outfile.close();
			return true;
		}
		return false;
	}


public:
	c_KeyHandler(std::string DataPath) {
		this->m_DataFilePath = DataPath;
		ReadMapFile(DataPath);
	}

	uint64_t QueryKey(std::string key) {
		if (m_KeyData.find(key) == m_KeyData.end())
			return 0;
		return m_KeyData[key];
	}

	bool InsertKey(std::pair< std::string, uint64_t > key_val) {
		bool ret = m_KeyData.insert(key_val).second;
		ret &= WriteMapFile(m_DataFilePath);
		return ret;
	}
	
	bool ModifyKey(std::pair< std::string, uint64_t > key_val) {
		m_KeyData[key_val.first] = key_val.second;
		return WriteMapFile(m_DataFilePath);
	}

	void DeleteKey(std::string key) {
		m_KeyData.erase(key);
	}
};

static c_KeyHandler* KeyHandler = new c_KeyHandler(std::string("data.dat"));
