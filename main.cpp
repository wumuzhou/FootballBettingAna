#include <iostream>
#include "log.h"
#include <iostream>
#include <boost/random.hpp>
#include "utils.h"
#include <vector>
#include <algorithm>
#include "log.h"
#include "boost/filesystem/operations.hpp"

using namespace std;

class odd
{
public:
    string name;
    double value;

    odd()
    {
    };

    odd(string name_, double value_)
    {
        name = name_;
        value = value_;
    }
};

class oddcompute
{
public :
    vector<odd> vodd;
    int times_current;
    int times_total;
};

int get_all_odd_combination(const vector<vector<odd> >& vvods, vector<odd>& vret)
{
    if (vvods.size() == 0)
    {
        return -1;
    }
    oddcompute initone;
    initone.vodd = vvods[0];
    initone.times_total = vvods.size() - 1;
    initone.times_current = 0;
    deque<oddcompute> dodd;
    dodd.push_back(initone);
    while (true)
    {
        auto one = dodd.front();
        dodd.pop_front();
        if (one.times_current == one.times_total)
        {
            vret = one.vodd;
            //todo sort it
            sort(vret.begin(), vret.end(), [&](odd const& obj1, odd const& obj2)
            {
                return ((obj1.value - obj2.value) > 0);
            });
            return 0;
        }

        oddcompute newoddcom;
        newoddcom.times_total = one.times_total;

        for (auto x: one.vodd)
        {
            for (auto ele: vvods[one.times_current + 1])
            {
                odd temp;
                temp.name = x.name + ele.name;
                temp.value = x.value * ele.value;
                newoddcom.vodd.emplace_back(temp);
            }
        }
        newoddcom.times_current = ++one.times_current;
        dodd.push_back(newoddcom);
    }
    return -2;
}

string getodd_by_readfile(const string& file, vector<odd>& vodd_ret)
{
    ifstream infile;
    infile.open(file.data());   //将文件流对象与文件连接起来
    string stemp, str;
    vector<vector<odd>> vvodd;

    while (getline(infile, stemp))
    {
        vector<string> vstr;
        Common::SplitString(stemp, "#", vstr);

        vector<odd> vodd1;
        vodd1.emplace_back("胜", Common::str2double(vstr[0]));
        vodd1.emplace_back("平", Common::str2double(vstr[1]));
        vodd1.emplace_back("负", Common::str2double(vstr[2]));
        vvodd.emplace_back(vodd1);
    }
    infile.close();             //关闭文件输入流

    get_all_odd_combination(vvodd, vodd_ret);

    return str;
}


// 通过随机抓取的方式获取资金赔率分布 可以设置最大抓取的个数
void GetCfgMoney(int TOTAL, const int& OddSize, const vector<odd>& vodd_ret, int largezhushu_win,
        const vector<int>& vmust_hasthis, const vector<int>& vnotthis)
{
    boost::mt19937 rng(time(0));

    // 检查第三个参数是否合规
    if (vnotthis.size() >= OddSize)
    {
        LOG_INFO << "vnotthis  size input error";
        return;
    }
    for (auto one: vnotthis)
    {
        if (one >= OddSize)
        {
            LOG_INFO << "vnotthis  content input error";
            return;
        }
    }

    if (vmust_hasthis.size() >= OddSize)
    {
        LOG_INFO << "vmust_hasthis  size input error";
        return;
    }
    for (auto one: vmust_hasthis)
    {
        if (one >= OddSize)
        {
            LOG_INFO << "vmust_hasthis  content input error";
            return;
        }
    }

    //  开始统计分析
    while (true)
    {
        int SurPlus = TOTAL;
        vector<int> alloc_money;

        for (int i = 1; i < OddSize; i++)
        {
            //获取随机值 0到多少注数
            boost::uniform_int<> ui(0, SurPlus);
            int GetData = ui(rng);


            alloc_money.emplace_back(GetData);
            SurPlus = SurPlus - GetData;
        }

        alloc_money.emplace_back(SurPlus);


        // 打印信息
        std::string content("=====================================find one=========================\n");
        double total_money = 0.0;
        int large_total_0 = 0;
        int showwill = 1;
        vector<int> vwins;
        vector<int> vbuy0;
        for (int i = 0; i < OddSize; i++)
        {
            double valueget = alloc_money[i] * vodd_ret[i].value;
            if(alloc_money[i] == 0) {vbuy0.emplace_back(i);}
            if ((valueget - TOTAL) > 0)
            {
                ++large_total_0;
                vwins.emplace_back(i);
            }
            total_money += valueget;
            content += vodd_ret[i].name + "  odd:" + Common::doubleTostr(vodd_ret[i].value) + "   buy:";
            content += Common::intTostr(alloc_money[i]) + "  profit: " +
                       Common::doubleTostr((valueget / TOTAL - 1.0) * 100) + "%\n";
        }
        content += "=====================================end     =========================\n";

        // 观察是否满足过滤条件，若满足
        if (vnotthis.size() == 0)
        {
            // 检查是否存在于musthasthis
            int must_has = 0;
            for (auto xx: vmust_hasthis)
            {
                for (auto yy: vwins)
                {
                    if (xx == yy)
                    {
                        ++must_has;
                        continue;
                    }
                }
            }
            if ((large_total_0 - largezhushu_win) >= 0 and must_has == vmust_hasthis.size())
            {
                LOG_INFO << content;
            }
        }
        else
        {

            // 检查是否存在于not_hasthis
            int not_has_times = 0;
            for (auto xx: vnotthis)
            {
                for (auto yy: vbuy0)
                {
                    if (xx == yy)
                    {
                        not_has_times++;
                        continue;
                    }
                }
            }
            if ((large_total_0 - largezhushu_win) >= 0 and not_has_times == vnotthis.size())
            {
                LOG_INFO << content;
            }
        }
    }
}

int main(int argc, char* argv[])
{
//    GetCfgMoney(25, 9);
//    return 0;

    vector<odd> vodd_ret;

    if (argc == 1)
    {
        std::cout << "1.打印当前赔率组合 \n"
                     "football_ana.exe  1   \n"
                     "含义：\n"
                     "第一个参数 football_ana.exe 为进程名\n"
                     "第二个参数1 为选择类型，此处打印当前所有赔率，并按照从大到小排列 ，index数据可以作为后面筛选的参数\n"
                     "\n"
                     "2.获取希望多少赔率盈利\n"
                     "football_ana.exe  2    买入注数     希望多少个组合可盈利   包含命令1中的组合index，用#连接\n"
                     "比如：football_ana.exe 2 20 3 1#2\n"
                     "含义：\n"
                     "第一个参数 football_ana.exe 为进程名\n"
                     "第二个参数2 为选择类型\n"
                     "第三个参数 20 为买入的注数\n"
                     "第四个参数 3 希望多少个组合可盈利，比如2串1有9中组合，可以写2到5种组合\n"
                     "第五个参数 1#2 命令1中显示的index，就是index 1和2对应赔率一定要发生\n"
                     "\n"
                     "3.获取希望多少赔率盈利，且第多少注不能盈利\n"
                     "football_ana.exe  3    买入注数     希望多少个组合可盈利   不包含命令1中的组合index，用#连接\n"
                     "比如：football_ana.exe 3 20 3 1#2\n"
                     "含义：\n"
                     "第一个参数 football_ana.exe 为进程名\n"
                     "第二个参数2 为选择类型\n"
                     "第三个参数 20 为买入的注数\n"
                     "第四个参数 3 希望多少个组合可盈利，比如2串1有9中组合，可以写2到5种组合\n"
                     "第五个参数 1#2 命令1中显示的index，就是index 1和2对应赔率不能发生" << std::endl;
        exit(0);
    }
    namespace bf = boost::filesystem;
    bf::path FullPath(boost::filesystem::current_path());
    bf::path PathLog = FullPath / "config_log.txt";
    bf::path Pathodd_file = FullPath / "oddfile";

    // init log
    if (init_log_conf(PathLog.string()) == 1) exit(0);

    int cmd = Common::str2int(argv[1]);
    LOG_INFO << "compute is start cmd:" << cmd;
    if (cmd == 1)
    {
        getodd_by_readfile(Pathodd_file.string(), vodd_ret);
        int index = 0;
        for (auto one: vodd_ret)
        {
            std::cout << "index:" << index++ << "   name:" << one.name << "   value:" << one.value << std::endl;
        }
    }
    if (cmd == 2)
    {
        int cmd2 = Common::str2int(argv[2]);
        int cmd3 = Common::str2int(argv[3]);

        vector<string> vstr;
        Common::SplitString(argv[4], "#", vstr);

        getodd_by_readfile(Pathodd_file.string(), vodd_ret);
        vector<int> vnotthis;
        vector<int> vmust_hasthis;
        for(int i =0;i < vstr.size(); i++)
        {

            vmust_hasthis.push_back(Common::str2int(vstr[i]));
        }

        GetCfgMoney(cmd2, vodd_ret.size(), vodd_ret, cmd3, vmust_hasthis, vnotthis);
    }

    if (cmd == 3)
    {
        int cmd2 = Common::str2int(argv[2]);
        int cmd3 = Common::str2int(argv[3]);

        vector<string> vstr;
        Common::SplitString(argv[4], "#", vstr);

        getodd_by_readfile(Pathodd_file.string(), vodd_ret);
        vector<int> vnotthis;
        vector<int> vmust_hasthis;
        for(int i =0;i < vstr.size(); i++)
        {

            vnotthis.push_back(Common::str2int(vstr[i]));
        }

        GetCfgMoney(cmd2, vodd_ret.size(), vodd_ret, cmd3, vmust_hasthis, vnotthis);
    }
    return 0;
}
