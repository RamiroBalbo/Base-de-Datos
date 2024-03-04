#ifndef RWSTRING_HPP_INCLUDED
#define RWSTRING_HPP_INCLUDED

// Funciones auxiliares para serializar struct que contengan strings

void writestring(std::fstream &fs, std::string str, int largo)
{
	str.resize(largo);
	fs.write(str.data(), largo);
}

std::string readstring(std::fstream &fs, int largo)
{
	char *buffer = new char[largo + 1];
	buffer[largo] = '\0';
	fs.read(buffer, largo);
	std::string str(buffer);
	delete[] buffer;
	return str;
}

#endif // RWSTRING_HPP_INCLUDED
