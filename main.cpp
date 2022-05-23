#include <iostream>

using namespace std;

#include "OrgChart.hpp"
using namespace ariel;

int main()
{
    OrgChart org;
    cout << "Enter root name: ";
    string root;
    cin >> root;
    org.add_root(root);
    while (true)
    {
        string super, sub;
        cout << "Write 'exit exit' to stop adding subs and see the chart" << endl;
        cout << "Enter superviser name: " << endl;
        cin >> super;
        cout << "Enter sub name: " << endl;
        cin >> sub;
        if (super == "exit" && sub == "exit")
        {
            cout << "Here is your organization: \n"
                 << org << endl;
            cout << "---------- LEVEL ORDER ----------" << endl;
            for (auto it = org.begin_level_order(); it != org.end_level_order(); ++it)
            {
                cout << (*it) << " ";
            }
            cout << endl;
            cout << "------ REVERSE LEVEL ORDER ------" << endl;
            for (auto it = org.begin_reverse_order(); it != org.reverse_order(); ++it)
            {
                cout << (*it) << " ";
            }
            cout << endl;
            cout << "----------- PRE-ORDER -----------" << endl;
            for (auto it = org.begin_preorder(); it != org.end_preorder(); ++it)
            {
                cout << (*it) << " ";
            }
            cout << endl;
            break;
        }
        org.add_sub(super, sub);
    }

    return 0;
}