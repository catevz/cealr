//
// Created by Olaf Zumpe on 6/6/18.
//

#include <cursesf.h>
#include "Properties.h"

FileException::FileException(const string &file) {
    _file = (string *) &file;
}

const char *FileException::what() {
    return ("Error on opening file \"" + (*_file) + "\"").c_str();
}

//class Properties: public map<string,string> {
Properties::Properties(const string &fileName) : sFile(fileName) {
    readFromFile();
}

Properties::Properties() : Properties(DEFAULT_PROPERTIES){};

void Properties::readFromFile() {
    ifstream ifs(sFile.c_str());
    if (ifs.is_open()) {
        saved = true;
        while (ifs/*.good()*/) {
            ifs >> ws;
            string line;
            string key, value;
            getline(ifs, key, '=');
            getline(ifs, value);
            key = trim(key);
            value = trim(value);
            if (!key.empty() && key[0] != '#' && !value.empty()) {
                (*this)[key] = value;
            }
        };
        ifs.close();
    } else {
        saved = false;
    }
}

//string Properties::trim(basic_string<char, char_traits<char>, allocator<char>> str) {
string Properties::trim(const string str) {
    string s = str;
    unsigned int p;
    while ((p = s.length()) > 0 && (unsigned char) s[p - 1] <= ' ')
        s.resize(p - 1);
    while ((p = s.length()) > 0 && (unsigned char) s[0] <= ' ')
        s.erase(0, 1);
    return s;
}

void Properties::save() throw(FileException){
    ofstream ofs((sFile).c_str(), std::ofstream::out);
    if (ofs.fail()){
        throw FileException(getFile());
    }
    for (const auto &p : *this) {
        ofs << p.first << " = " << p.second << endl;
    }
    ofs.flush();
    ofs.close();
    saved = true;
}

Properties::~Properties() {
//    sFile.~string();
}

bool Properties::operator==(const Properties &properties) const {
    return static_cast<const map<string, string> &>(*this) == static_cast<const map<string, string> &>(properties) &&
           sFile == properties.sFile;
}

bool Properties::operator!=(const Properties &properties) const {
    return !(properties == *this);
}

const string &Properties::getFile() const {
    return sFile;
}

ostream &operator<<(ostream &os, const Properties &properties) {
    os << "Properties file: " << properties.sFile << endl;
    for (const auto &p : properties) {
        os << "properties[" << p.first << "] = " << p.second << endl;
    }
    return os;
}

bool Properties::isSaved() {
    return saved;
}