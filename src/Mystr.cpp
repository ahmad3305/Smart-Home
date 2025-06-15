#include <iostream>
#include "../include/Mystr.h"
using namespace std;


Mystr::Mystr() {
    len = 0;
    cap = 0;
    Cs = nullptr;
}

Mystr::Mystr(const Mystr& Str) {
    this->len = Str.len;
    this->cap = Str.cap;
    this->Cs = new char[len + 1];

    for (int i = 0; i < len; i++) {
        this->Cs[i] = Str.Cs[i];
    }
    this->Cs[len] = '\0';
}

Mystr::Mystr(const char* p) {
    len = strlen(p);
    cap = len;
    Cs = new char[len + 1];
    for (int i = 0; i < len; i++) {
        Cs[i] = p[i];
    }
    Cs[len] = '\0';
}

Mystr::Mystr(const char ch, int size) {
    len = size;
    cap = size;
    Cs = new char[len + 1];
    for (int i = 0; i < len; i++) {
        Cs[i] = ch;
    }
    Cs[len] = '\0';
}

Mystr::Mystr(int num) {
    *this = itos(num);
}

Mystr Mystr::itos(int num) const {
    int tempNum = num;
    int length = 0;

    if (num == 0) {
        length = 1;
    }
    else {
        while (tempNum != 0) {
            length++;
            tempNum /= 10;
        }
    }

    char* result = new char[length + 1];
    result[length] = '\0';

    for (int i = length - 1; i >= 0; i--) {
        result[i] = (num % 10) + '0';
        num /= 10;
    }

    return Mystr(result);
}

int Mystr::stoi(const Mystr s) const {
    int result = 0;

    for (int i = 0; i < s.len; i++) {
        if (s.Cs[i] >= '0' && s.Cs[i] <= '9') {
            result = result * 10 + (s.Cs[i] - '0');
        }
        else {
            cout << "Invalid characters" << endl;
            return 1;
        }
    }
    return result;
}

Mystr& Mystr:: operator=(const Mystr& str) {
    if (this != &str) {
        delete[] Cs;
        len = str.len;
        cap = str.cap;
        Cs = new char[len + 1];
        for (int i = 0; i < len; i++) {
            Cs[i] = str.Cs[i];
        }
        Cs[len] = '\0';
    }
    return *this;
}

void Mystr::insert_at(int i, char ch) {
    if (i < 0 || i > len) {
        return;
    }

    char* newCs = new char[len + 2];

    for (int j = 0; j < i; j++) {
        newCs[j] = Cs[j];
    }

    newCs[i] = ch;

    for (int j = i; j < len; j++) {
        newCs[j + 1] = Cs[j];
    }

    newCs[len + 1] = '\0';

    delete[] Cs;
    Cs = newCs;
    len++;
    cap = len;
}

void Mystr::insert_at(int i, const Mystr sub) {
    if (i < 0 || i > len) {
        return;
    }

    char* newCs = new char[len + sub.len + 1];

    for (int j = 0; j < i; j++) {
        newCs[j] = Cs[j];
    }

    for (int j = 0; j < sub.len; j++) {
        newCs[i + j] = sub.Cs[j];
    }

    for (int j = i; j < len; j++) {
        newCs[sub.len + j] = Cs[j];
    }

    newCs[len + sub.len] = '\0';

    delete[] Cs;
    Cs = newCs;
    len += sub.len;
    cap = len;
}

void Mystr::replace_first(char c) {
    if (len > 0) {
        Cs[0] = c;
    }
}

Mystr& Mystr::trim() {
    if (len == 0) {
        return *this;
    }

    int start = 0;
    while (start < len && (Cs[start] == ' ' || Cs[start] == '\t' || Cs[start] == '\n')) {
        start++;
    }

    int end = len - 1;
    while (end >= start && (Cs[end] == ' ' || Cs[end] == '\t' || Cs[end] == '\n')) {
        end--;
    }

    int newLen = end - start + 1;
    char* newCs = new char[newLen + 1];

    for (int i = 0; i < newLen; i++) {
        newCs[i] = Cs[start + i];
    }
    newCs[newLen] = '\0';

    delete[] Cs;
    Cs = newCs;
    len = newLen;
    cap = len;

    return *this;
}

bool Mystr::is_equal(const Mystr& M) const {
    if (len != M.len) {
        return false;
    }
    for (int i = 0; i < len; i++) {
        if (Cs[i] != M.Cs[i]) {
            return false;
        }
    }
    return true;
}

bool Mystr::is_less(const Mystr& M) const {
    int minLength;
    if (len < M.len) {
        minLength = len;
    }
    else {
        minLength = M.len;
    }
    for (int i = 0; i < minLength; i++) {
        if (Cs[i] < M.Cs[i]) {
            return true;
        }
        else if (Cs[i] > M.Cs[i]) {
            return false;
        }
    }
    return len < M.len;
}

bool Mystr::is_greater(const Mystr& M) const {
    int minLength;
    if (len < M.len) {
        minLength = len;
    }
    else {
        minLength = M.len;
    }
    for (int i = 0; i < minLength; i++) {
        if (Cs[i] > M.Cs[i]) {
            return true;
        }
        else if (Cs[i] < M.Cs[i]) {
            return false;
        }
    }
    return len > M.len;
}

Mystr Mystr::concat(const Mystr& s2) const {
    int newLen = len + s2.len;
    char* newCs = new char[newLen + 3];

    for (int i = 0; i < len; i++) {
        newCs[i] = Cs[i];
    }
    for (int i = 0; i < s2.len; i++) {
        newCs[len + i] = s2.Cs[i];
    }
    newCs[newLen] = '\0';

    Mystr result(newCs);
    delete[] newCs;
    return result;
}

Mystr& Mystr::append(const Mystr& s2) {
    int newLen = len + s2.len;
    char* newCs = new char[newLen + 3];

    for (int i = 0; i < len; i++) {
        newCs[i] = Cs[i];
    }
    for (int i = 0; i < s2.len; i++) {
        newCs[len + i] = s2.Cs[i];
    }
    newCs[newLen] = '\0';

    delete[] Cs;
    Cs = newCs;
    len = newLen;
    cap = newLen;
    return *this;
}

int* Mystr::all_sub_Mystrs(const char* sub, int& count) const {
    int sub_len = strlen(sub);
    if (sub_len == 0) {
        count = 0;
        return nullptr;
    }

    int* tempidx = new int[len + 1];
    count = 0;

    for (int i = 0; i <= len - sub_len; i++) {
        bool match = true;
        for (int j = 0; j < sub_len; j++) {
            if (Cs[i + j] != sub[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            tempidx[count++] = i;
        }
    }

    if (count == 0) {
        delete[] tempidx;
        return nullptr;
    }

    int* result = new int[count];
    for (int i = 0; i < count; i++) {
        result[i] = tempidx[i];
    }
    delete[] tempidx;

    return result;
}
Mystr* Mystr::split(char delim, int& count) const {
    count = 0;
    if (len == 0) {
        return nullptr;
    }

    bool inSubstr = false;
    for (int i = 0; i < len; i++) {
        if (Cs[i] != delim) {
            if (!inSubstr) {
                inSubstr = true;
                count++;
            }
        }
        else {
            inSubstr = false;
        }
    }

    Mystr* result = new Mystr[count];
    int start = 0;
    int substridx = 0;
    inSubstr = false;

    for (int i = 0; i < len; i++) {
        if (Cs[i] != delim) {
            if (!inSubstr) {
                start = i;
                inSubstr = true;
            }
        }
        else {
            if (inSubstr) {
                int substrLen = i - start;
                char* substr = new char[substrLen + 1];
                for (int j = 0; j < substrLen; j++) {
                    substr[j] = Cs[start + j];
                }
                substr[substrLen] = '\0';

                result[substridx] = Mystr(substr);
                substridx++;
                delete[] substr;
                inSubstr = false;
            }
        }
    }

    if (inSubstr) {
        int substrLen = len - start;
        char* substr = new char[substrLen + 1];
        for (int j = 0; j < substrLen; j++) {
            substr[j] = Cs[start + j];
        }
        substr[substrLen] = '\0';

        result[substridx] = Mystr(substr);
        delete[] substr;
    }

    return result;
}

Mystr* Mystr::tokenize(const char* delim, int& count) const {
    count = 0;
    if (len == 0) {
        return nullptr;
    }

    bool inSubstr = false;
    for (int i = 0; i < len; i++) {
        if (!isDelimiter(Cs[i], delim)) {
            if (!inSubstr) {
                inSubstr = true;
                count++;
            }
        }
        else {
            inSubstr = false;
        }
    }

    Mystr* result = new Mystr[count];
    int start = 0;
    int substridx = 0;
    inSubstr = false;

    for (int i = 0; i < len; i++) {
        if (!isDelimiter(Cs[i], delim)) {
            if (!inSubstr) {
                start = i;
                inSubstr = true;
            }
        }
        else {
            if (inSubstr) {
                int substrLen = i - start;
                char* substr = new char[substrLen + 1];
                for (int j = 0; j < substrLen; j++) {
                    substr[j] = Cs[start + j];
                }
                substr[substrLen] = '\0';

                result[substridx] = Mystr(substr);
                substridx++;
                delete[] substr;
                inSubstr = false;
            }
        }
    }

    if (inSubstr) {
        int substrLen = len - start;
        char* substr = new char[substrLen + 1];
        for (int j = 0; j < substrLen; j++) {
            substr[j] = Cs[start + j];
        }
        substr[substrLen] = '\0';

        result[substridx] = Mystr(substr);
        delete[] substr;
    }

    return result;
}

int Mystr::find_first(char ch) const {
    for (int i = 0; i < len; i++) {
        if (Cs[i] == ch) {
            return i;
        }
    }
    return -1;
}

int Mystr::find_first(const Mystr& substr) const {
    if (substr.len == 0 || substr.len > len) {
        return -1;
    }

    for (int i = 0; i <= len - substr.len; i++) {
        bool found = true;
        for (int j = 0; j < substr.len; j++) {
            if (Cs[i + j] != substr.Cs[j]) {
                found = false;
                break;
            }
        }
        if (found) {
            return i;
        }
    }
    return -1;
}

int Mystr::find_last(char ch) const {
    for (int i = len - 1; i >= 0; i--) {
        if (Cs[i] == ch) {
            return i;
        }
    }
    return -1;
}

int Mystr::find_last(const Mystr& substr) const {
    if (substr.len == 0 || substr.len > len) {
        return -1;
    }

    for (int i = len - substr.len; i >= 0; i--) {
        bool found = true;
        for (int j = 0; j < substr.len; j++) {
            if (Cs[i + j] != substr.Cs[j]) {
                found = false;
                break;
            }
        }
        if (found) {
            return i;
        }
    }
    return -1;
}

int* Mystr::find_all(char ch, int& C) const {
    C = 0;
    for (int i = 0; i < len; i++) {
        if (Cs[i] == ch) {
            C++;
        }
    }

    if (C == 0) {
        return nullptr;
    }

    int* indices = new int[C];
    int index = 0;
    for (int i = 0; i < len; i++) {
        if (Cs[i] == ch) {
            indices[index++] = i;
        }
    }
    return indices;
}

int* Mystr::find_all(const Mystr& substr, int& C) const {
    C = 0;
    if (substr.len == 0 || substr.len > len) {
        return nullptr;
    }

    for (int i = 0; i <= len - substr.len; i++) {
        bool found = true;
        for (int j = 0; j < substr.len; j++) {
            if (Cs[i + j] != substr.Cs[j]) {
                found = false;
                break;
            }
        }
        if (found) {
            C++;
        }
    }

    if (C == 0) {
        return nullptr;
    }

    int* indices = new int[C];
    int index = 0;
    for (int i = 0; i <= len - substr.len && index < C; i++) {
        bool found = true;
        for (int j = 0; j < substr.len; j++) {
            if (Cs[i + j] != substr.Cs[j]) {
                found = false;
                break;
            }
        }
        if (found) {
            indices[index++] = i;
        }
    }
    return indices;
}

void Mystr::remove_at(int i) {
    if (i < 0 || i >= len) {
        return;
    }

    char* newCs = new char[len + 1];
    for (int j = 0; j < i; j++) {
        newCs[j] = Cs[j];
    }
    for (int j = i + 1; j < len; j++) {
        newCs[j - 1] = Cs[j];
    }
    newCs[len - 1] = '\0';

    delete[] Cs;
    Cs = newCs;
    len--;
    cap = len;
}

int Mystr::strlen(const char* str) const {
    int count = 0;
    while (str[count] != '\0') {
        count++;
    }
    return count;
}

bool Mystr::isDelimiter(char ch, const char* delim) const {
    for (int i = 0; delim[i] != '\0'; i++) {
        if (ch == delim[i]) {
            return true;
        }
    }
    return false;
}


void Mystr::remove_first(char ch) {
    int index = find_first(ch);
    if (index != -1) {
        remove_at(index);
    }
}

void Mystr::remove_last(char ch) {
    int index = find_last(ch);
    if (index != -1) {
        remove_at(index);
    }
}

void Mystr::remove_all(char ch) {
    int index;
    while ((index = find_first(ch)) != -1) {
        remove_at(index);
    }
}

void Mystr::to_upper() {
    for (int i = 0; i < len; i++) {
        if (Cs[i] >= 'a' && Cs[i] <= 'z') {
            Cs[i] = Cs[i] - ('a' - 'A');
        }
    }
}

void Mystr::to_lower() {
    for (int i = 0; i < len; i++) {
        if (Cs[i] >= 'A' && Cs[i] <= 'Z') {
            Cs[i] = Cs[i] + ('a' - 'A');
        }
    }
}

char& Mystr:: operator[](int index) {
    if (index >= 0 && index < len) {
        return Cs[index];
    }
}


const char& Mystr:: operator[](int index) const {
    if (index >= 0 && index < len) {
        return Cs[index];
    }
}

Mystr::~Mystr() {
    delete[] Cs;
}

ostream& operator<<(ostream& os, const Mystr& str) {
    for (int i = 0; i < str.len; i++) {
        os << str.Cs[i];
    }
    return os;
}



Mystr operator+(const Mystr& s1, const Mystr& s2) {
    return s1.concat(s2);
}

Mystr& operator+=(Mystr& s1, const Mystr& s2) {
    return s1.append(s2);
}

istream& operator>>(istream& is, Mystr& str) {
    char temp[1024];
    is >> temp;
    str = Mystr(temp);
    return is;
}

const char* Mystr::getstr() const {
    return Cs;
}