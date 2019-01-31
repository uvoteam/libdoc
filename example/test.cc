#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <thread>
#include <iostream>
#include <assert.h>

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

int main(int argc, char *argv[])
{
    vector<thread> t;
    int i = 0;

    assert(argc == 7);
    t.push_back(thread(bind(get_text, argv[1], i++)));
    t.push_back(thread(bind(get_text, argv[2], i++)));
    t.push_back(thread(bind(get_text, argv[3], i++)));
    t.push_back(thread(bind(get_text, argv[4], i++)));
    t.push_back(thread(bind(get_text, argv[5], i++)));
    t.push_back(thread(bind(get_text, argv[6], i++)));



    for (thread &th : t) th.join();

    for (const string &s : dsts)
        cout << s << endl << endl;
}
