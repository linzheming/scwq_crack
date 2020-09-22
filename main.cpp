#include <iostream>
#include <cstdio>
#include <algorithm>
#include <fstream>
#include "httplib.h"
#include <Windows.h>

using namespace std;


//添加本地域名映射，劫持http请求
#define HOSTS "C:\\Windows\\System32\\drivers\\etc\\hosts"

void change_hosts()
{
    bool ok=false;//是否已经包含 cnbook 域名

    fstream f (HOSTS);
    string line;
    if ( f.is_open() )
    {
        while ( f.good() )
        {
            getline(f,line);
            cout << line << endl;
            if(line.find("cnbook.cn")!=string::npos)
                ok=true;
        }

        f.close();
    } else
        cout << "Error" << endl;

    //已经有域名记录
    if(ok) return;

    //没有记录就添加
    ofstream out;
    out.open(HOSTS, ios::app);
    if(!out)
        cout<<"File Not Found or Permission Denied";
    else
    {
        out<<"\n127.0.0.1"<<"\t" << "freebook.cnbook.cn";
        out<<"\n127.0.0.1"<<"\t" << "www.cnbook.cn";
        cout<<"\n";
        cout<<"cnbook is blocked";
    }
    out.close();
}
int main()
{
    change_hosts();
    httplib::Server svr;

    svr.Get("/scwq/reg.php", [](const httplib::Request &req, httplib::Response &res) {
        cout << req.target << endl;
        res.set_content("OK ok", "text/plain");
    });

    bool ret = svr.listen("127.0.0.1", 80);
    cout << "Hello World!" << ret << endl;
    return 0;
}
