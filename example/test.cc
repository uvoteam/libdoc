#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <thread>
#include <iostream>

#include <errno.h>

#include "../libdoc.h"

using namespace std;

vector<string> dsts(6);
void get_text(const string &filename, int i)
{
    std::ifstream t(filename);
    std::string str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
    char *buffer = NULL;
    int ret = doc2text(str.c_str(), str.length(), &buffer);
    dsts[i] = buffer;
    free(buffer);
}

int main()
{
    vector<thread> t;
    int i = 0;


    t.push_back(thread(bind(get_text, "/home/kasha/Downloads/test.doc", i++)));
    t.push_back(thread(bind(get_text, "/home/kasha/Downloads/test2.doc", i++)));
    t.push_back(thread(bind(get_text, "/home/kasha/Downloads/test3.doc", i++)));
    t.push_back(thread(bind(get_text, "/home/kasha/Downloads/test4.doc", i++)));
    t.push_back(thread(bind(get_text, "/home/kasha/Downloads/test5.doc", i++)));
    t.push_back(thread(bind(get_text, "/home/kasha/Downloads/test6.doc", i++)));



    for (thread &th : t) th.join();

    for (const string &s : dsts)
        cout << s << endl << endl;
}
