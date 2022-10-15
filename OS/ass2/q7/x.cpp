#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void printdata(vector<int> &available, vector<vector<int>> &max, vector<vector<int>> &allocation, vector<vector<int>> &need)
{
    int p = max.size();
    int r = max[0].size();

    cout << "Process\t\t"
         << "Allocation\t"
         << "Max\t\t"
         << "Need\t\t"
         << "Available\t\t" << endl;

    for (int i = 0; i < p; i++)
    {
        cout << "P" << i << "\t\t";

        for (int j = 0; j < r; j++)
        {
            cout << allocation[i][j] << " ";
        }
        cout << "\t";

        for (int j = 0; j < r; j++)
        {
            cout << max[i][j] << " ";
        }

        cout << "\t";

        for (int j = 0; j < r; j++)
        {
            cout << need[i][j] << " ";
        }

        cout << "\t";

        if (i == 0)
        {
            for (int j = 0; j < r; j++)
            {
                cout << available[j] << " ";
            }
        }
        cout << endl;
    }
}

int main()
{

    // read from a file

    ifstream fin("input.txt");

    // number of resources is in the first line
    int r;
    fin >> r;

    vector<int> available(r);

    // available resources are in the second line
    for (int i = 0; i < r; i++)
    {
        fin >> available[i];
    }

    // number of processes is in the third line
    int p;
    fin >> p;

    vector<vector<int>> max(p, vector<int>(r));

    // max resources are in the fourth line
    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < r; j++)
        {
            fin >> max[i][j];
        }
    }

    // initializing an allocation matrix with all zeros

    vector<vector<int>> allocation(p, vector<int>(r, 0));

    // initializing a need matrix with all the needs of the processes

    vector<vector<int>> need(p, vector<int>(r));

    for (int i = 0; i < p; i++)
    {
        for (int j = 0; j < r; j++)
        {
            need[i][j] = max[i][j];
        }
    }

    printdata(available, max, allocation, need);

    

    int count = 0;
    while (count < p)
    {
    again:
        int p_num;
        vector<int> req(r);

        cout << "Enter allocation request for process: ";
        cin >> p_num;
        for (int i = 0; i < r; i++)
        {
            cin >> req[i];
        }

        for (int i = 0; i < r; i++)
        {
            if (req[i] > need[p_num][i])
            {
                cout << "Error! Request is greater than need" << endl;
                goto again;
            }
            else if (req[i] > available[i])
            {
                cout << "Error! Request <= need but greater than available resources" << endl;
                goto again;
            }
        }

        printf("Now checking if the system is in safe state or not after granting the request\n");

        // declaring duplicate vectors to check if the system is in safe state or not

        vector<int> dup_available(available.begin(), available.end());

        vector<vector<int>> dup_allocation(allocation.begin(), allocation.end());

        vector<vector<int>> dup_need(need.begin(), need.end());

        // granting the request

        for (int i = 0; i < r; i++)
        {
            dup_available[i] -= req[i];
            dup_allocation[p_num][i] += req[i];
            dup_need[p_num][i] -= req[i];
        }

        vector<int> work(r);

        work = dup_available;

        vector<bool> finish(p, false);
        vector<int> safe_seq(p);
        int ind = 0;
        bool flag = false;
        while (true)
        {

            for (int i = 0; i < p; i++)
            {
                if (finish[i] == false)
                {
                    bool canfinish = true;
                    for (int j = 0; j < r; j++)
                    {
                        if (dup_need[i][j] > work[j])
                        {
                            canfinish = false;
                            break;
                        }
                    }

                    if (canfinish == false)
                        continue;

                    for (int j = 0; j < r; j++)
                    {
                        work[j] += dup_allocation[i][j];
                    }

                    finish[i] = true;
                    safe_seq[ind++] = i;
                    flag = true;
                }
            }

            if (flag == false)
                break;

            flag = false;
        }

        for (int i = 0; i < p; i++)
        {
            if (finish[i] == false)
            {
                cout << "System is not in safe state" << endl;
                cout << "Request cannot be granted" << endl;
                goto again;
            }
        }

        cout << "System is in safe state" << endl;

        cout << "Safe sequence is: ";
        for (int i = 0; i < p - 1; i++)
        {
            cout << safe_seq[i] << " -> ";
        }
        cout << safe_seq[p - 1] << endl;

        cout << "Request granted" << endl;

        // updating the original vectors

        for (int i = 0; i < r; i++)
        {
            available[i] -= req[i];
            allocation[p_num][i] += req[i];
            need[p_num][i] -= req[i];
        }

        // checking if need of a process is zero

        int sum = 0;

        for (int i = 0; i < r; i++)
        {
            sum += need[p_num][i];
        }

        if (sum == 0)
        {
            cout << "Process " << p_num << " has finished" << endl;

            // freeing allocated resources

            for (int i = 0; i < r; i++)
            {
                available[i] += allocation[p_num][i];
                allocation[p_num][i] = 0;
            }
            count++;
        }

        printdata(available, max, allocation, need);
    }

    cout << "All Processes have finished" << endl;

    return 0;
}