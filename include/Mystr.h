#include<iostream>
using namespace std;

#ifndef Mystr_H
#define Mystr_H

class Mystr {
    int len;
    int cap;
    char* Cs;

    int strlen(const char* str) const;


    bool isDelimiter(char ch, const char* delim) const;

public:
    Mystr();

    Mystr(const Mystr& Str);

    Mystr(const char* p);

    Mystr(const char ch, int size);

    Mystr(int num);

    Mystr itos(int num) const;
    int stoi(const Mystr s) const;

    Mystr& operator=(const Mystr& str);
    void insert_at(int i, char ch);

    void insert_at(int i, const Mystr sub);

    void replace_first(char c);

    Mystr& trim();
    bool is_equal(const Mystr& M) const;

    bool is_less(const Mystr& M) const;
    bool is_greater(const Mystr& M) const;

    Mystr concat(const Mystr& s2) const;

    Mystr& append(const Mystr& s2);
    int* all_sub_Mystrs(const char* sub, int& count) const;
    Mystr* split(char delim, int& count) const;

    Mystr* tokenize(const char* delim, int& count) const;

    int find_first(const Mystr& substr) const;
    int find_first(char ch) const;
    int find_last(char ch) const;
    int find_last(const Mystr& substr) const;
    int* find_all(char ch, int& C) const;

    int* find_all(const Mystr& substr, int& C) const;

    void remove_at(int i);

    void remove_first(char ch);

    void remove_last(char ch);

    void remove_all(char ch);

    void to_upper();

    void to_lower();

    char& operator[](int index);

    const char& operator[](int index) const;

    const char* getstr() const;

    ~Mystr();

    friend istream& operator>>(istream& is, Mystr& str);

    friend ostream& operator<<(ostream& os, const Mystr& str);

};

#endif

ostream& operator<<(ostream& os, const Mystr& str);


Mystr operator+(const Mystr& s1, const Mystr& s2);

Mystr& operator+=(Mystr& s1, const Mystr& s2);

