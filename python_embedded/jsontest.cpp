#include <iostream>
#include <exception>

#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>

#include <boost/filesystem.hpp>

using namespace std;
namespace python = boost::python;

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout << "Usage: " << argv[0] << " <jsontest-python-script>" << endl;
        return 1;
    }

    if ( !boost::filesystem::exists( argv[1]) ) {
        cout << "File not found: " << argv[1] << endl;
        return 2;
    }

    // setup env and load script
    Py_Initialize();
    python::object main = python::import("__main__");
    python::object global = main.attr("__dict__");
    exec_file(argv[1], global, global);
    
    // map main object jst as instantiation of class JSonTest
    python::object jsontest_object = global["jst"];
    
    // call getip() text -> string
    python::object py_result = jsontest_object.attr("getip")();
    string ip = python::extract<string>(py_result);
    if(ip.length()) {
        cout << "IP: " << string(ip) << endl;
    } else {
        cout << "Emtpy value for getip()" << endl;
    }

    // call restecho() json -> dict[string]=string
    python::object py_result2 = jsontest_object.attr("restecho")("t1","t2","t3","t4");
    python::dict echo = python::extract<python::dict>(py_result2);
    
    typedef python::stl_input_iterator<python::object> pObjIter;
    if(pObjIter(echo.items()) == pObjIter()){ 
        cout << "Emtpy value for restecho()" << endl;
    }

    for(pObjIter i = pObjIter(echo.items()); i != pObjIter(); i++) {
        string key    = python::extract<string>((*i)[0]);
        string value  = python::extract<string>((*i)[1]);
        cout << key << " : " << value << endl;
    }

    
    // call getitems() json -> list(dict[string]=int/string)
    python::object py_result3 = jsontest_object.attr("getitems")();
    python::list items = python::extract<python::list>(py_result3);

    int status_code = python::extract<int>(global["jst"].attr("status_code"));
    cout << "getitems(): status code = " << status_code << endl;

    for(pObjIter i = pObjIter(items); i != pObjIter(); i++) {
        python::dict item = python::extract<python::dict>(*i);
        for(pObjIter j = pObjIter(item.items()); j != pObjIter(); j++) {
            string key = python::extract<string>((*j)[0]);
            cout << key << " : ";
            
            boost::python::extract<int> ival((*j)[1]);
            if(ival.check()) {
                cout << ival;
            } else {
                string sval = python::extract<string>((*j)[1]);
                cout << sval.substr(0,10) << "...";
            }
            cout << "\t";
        }
        cout << endl;
    }

    return 0;
}