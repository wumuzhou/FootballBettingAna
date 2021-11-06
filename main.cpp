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
    infile.open(file.data());   //���ļ����������ļ���������
    string stemp, str;
    vector<vector<odd>> vvodd;

    while (getline(infile, stemp))
    {
        vector<string> vstr;
        Common::SplitString(stemp, "#", vstr);

        vector<odd> vodd1;
        vodd1.emplace_back("ʤ", Common::str2double(vstr[0]));
        vodd1.emplace_back("ƽ", Common::str2double(vstr[1]));
        vodd1.emplace_back("��", Common::str2double(vstr[2]));
        vvodd.emplace_back(vodd1);
    }
    infile.close();             //�ر��ļ�������

    get_all_odd_combination(vvodd, vodd_ret);

    return str;
}


// ͨ�����ץȡ�ķ�ʽ��ȡ�ʽ����ʷֲ� �����������ץȡ�ĸ���
void GetCfgMoney(int TOTAL, const int& OddSize, const vector<odd>& vodd_ret, int largezhushu_win,
        const vector<int>& vmust_hasthis, const vector<int>& vnotthis)
{
    boost::mt19937 rng(time(0));

    // �������������Ƿ�Ϲ�
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

    //  ��ʼͳ�Ʒ���
    while (true)
    {
        int SurPlus = TOTAL;
        vector<int> alloc_money;

        for (int i = 1; i < OddSize; i++)
        {
            //��ȡ���ֵ 0������ע��
            boost::uniform_int<> ui(0, SurPlus);
            int GetData = ui(rng);


            alloc_money.emplace_back(GetData);
            SurPlus = SurPlus - GetData;
        }

        alloc_money.emplace_back(SurPlus);


        // ��ӡ��Ϣ
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

        // �۲��Ƿ��������������������
        if (vnotthis.size() == 0)
        {
            // ����Ƿ������musthasthis
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

            // ����Ƿ������not_hasthis
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
        std::cout << "1.��ӡ��ǰ������� \n"
                     "football_ana.exe  1   \n"
                     "���壺\n"
                     "��һ������ football_ana.exe Ϊ������\n"
                     "�ڶ�������1 Ϊѡ�����ͣ��˴���ӡ��ǰ�������ʣ������մӴ�С���� ��index���ݿ�����Ϊ����ɸѡ�Ĳ���\n"
                     "\n"
                     "2.��ȡϣ����������ӯ��\n"
                     "football_ana.exe  2    ����ע��     ϣ�����ٸ���Ͽ�ӯ��   ��������1�е����index����#����\n"
                     "���磺football_ana.exe 2 20 3 1#2\n"
                     "���壺\n"
                     "��һ������ football_ana.exe Ϊ������\n"
                     "�ڶ�������2 Ϊѡ������\n"
                     "���������� 20 Ϊ�����ע��\n"
                     "���ĸ����� 3 ϣ�����ٸ���Ͽ�ӯ��������2��1��9����ϣ�����д2��5�����\n"
                     "��������� 1#2 ����1����ʾ��index������index 1��2��Ӧ����һ��Ҫ����\n"
                     "\n"
                     "3.��ȡϣ����������ӯ�����ҵڶ���ע����ӯ��\n"
                     "football_ana.exe  3    ����ע��     ϣ�����ٸ���Ͽ�ӯ��   ����������1�е����index����#����\n"
                     "���磺football_ana.exe 3 20 3 1#2\n"
                     "���壺\n"
                     "��һ������ football_ana.exe Ϊ������\n"
                     "�ڶ�������2 Ϊѡ������\n"
                     "���������� 20 Ϊ�����ע��\n"
                     "���ĸ����� 3 ϣ�����ٸ���Ͽ�ӯ��������2��1��9����ϣ�����д2��5�����\n"
                     "��������� 1#2 ����1����ʾ��index������index 1��2��Ӧ���ʲ��ܷ���" << std::endl;
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
