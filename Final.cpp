#include <iostream>
#include<set>
#include<cstdio>
#include <iomanip>
#include <string>
#include <fstream>
#include <random>
#include <chrono>
#include <vector>
using namespace std;

class income;
void HOME(string id);

string GenTid(int length = 16)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int charsetSize = sizeof(charset) - 1;
    string randomString;

    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < length; ++i)
    {
        int randomIndex = rand() % charsetSize;
        randomString += charset[randomIndex];
    }

    return randomString;
}

string todayDate()
{
    chrono::system_clock::time_point now = chrono::system_clock::now();

    time_t time = chrono::system_clock::to_time_t(now);
    tm *date = localtime(&time);

    int year = date->tm_year + 1900;
    int month = date->tm_mon + 1;
    int day = date->tm_mday;

    string today =to_string(year) + "-" + to_string(month) + "-" +  to_string(day);
    return today;
}

struct USER
{
    string UserId;
    string UserName;
    string Password;
    string Email;
    string MobNo;
    string Profession;
    string DefaultAccId;
    string balance;
};

struct Transaction
{
    string userId, transactionId, transactionDate, title, description, accountId, entryDate;
    float amount;
    string recieveFrom_or_SentTo;
};

class transaction
{
protected:
    string userId, transactionId = GenTid(), transactionDate, title, description, accountId, entryDate = todayDate();
    float amount;

public:
    virtual void Trigger()=0; //Pure Virtual Function
};

class income : public transaction
{
protected:
    // string recievedFrom="Null";

public:
    income(string uid)
    {
        userId = uid;

        
        getIncomeInfo()->printFile()->Trigger();

        cout<<"\nTransaction is  Done Succesfully.....\n\n";
        // printConsole();
    }
    void printConsole()
    {
        cout.fill('-');
        cout.width(100);
        cout << "" << endl;

        cout.fill(' ');
        cout << title;
        cout.width(20);
        cout << description;
        cout.width(10);
        cout << accountId;
        cout.width(15);
        cout << transactionDate;
        cout.width(20);
        // cout << recievedFrom;
        // cout.width(20);
        cout << amount;
        cout << endl;
    }

    income * printFile()
    {
        ofstream file("income.txt",ios::app);
        file << userId << " " << transactionId << " " << transactionDate << " " << title << " " << description << " " << accountId << " " << entryDate << " " << amount << " " << endl;
        file.close();
        return this;
    }

    income *getIncomeInfo()
    {
        cout << "Enter Income Title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Enter Description: ";
        getline(cin, description);
        cout << "Enter Account ID: ";
        cin >> accountId;
        cout << "Enter Transaction Date (yyyy-mm-dd): ";
        cin.ignore();
        getline(cin, transactionDate);
        // cout << "Received From: ";
        // cin >> recievedFrom;
        cout << "Enter Amount: ";
        cin >> amount;
        
        return this;
    }

    // RunTime polymorphism
    void Trigger() override 
    {
        // ofstream file("income.txt", ios::app);
        // file << sentTo << " " << transactionId << " " << transactionDate << " " << title << " " << description << " " << accountId << " " << entryDate << " " << amount << " " << userId << " " << endl;
        // file.close();

        ifstream in("users.txt");
        USER temp;
        vector<USER> U;
        while (in >> temp.UserId >> temp.UserName >> temp.Password >> temp.Email >> temp.MobNo >> temp.Profession >> temp.DefaultAccId >> temp.balance)
        {
            U.push_back(temp);
        }
        in.close();
        
        ofstream out("users.txt");

        for (auto i : U)
        {
            if (i.UserId == userId)
            {
                out << i.UserId << " " << i.UserName << " " << i.Password << " " << i.Email << " " << i.MobNo << " " << i.Profession << " " << i.DefaultAccId << " " << to_string(stof(i.balance) + (amount)) << endl;
            }
            // else if (sentTo != "0" && i.UserId == sentTo)
            // {
            //     out << i.UserId << " " << i.UserName << " " << i.Password << " " << i.Email << " " << i.MobNo << " " << i.Profession << " " << i.DefaultAccId << " " << to_string(stof(i.balance) + (amount)) << endl;
            // }
            else
            {
                out << i.UserId << " " << i.UserName << " " << i.Password << " " << i.Email << " " << i.MobNo << " " << i.Profession << " " << i.DefaultAccId << " " << i.balance << endl;
            }
        }
        out.close();
    }





};

class expense : public transaction
{
protected:
    string sentTo;

public:
    expense(string uid)
    {
        userId = uid;
        getExpenseInfo();
        Trigger();
        printFile();
        // printConsole();
        cout<<"\nTransaction is  Done Succesfully.....\n\n";

    }
    void printConsole()
    {
        cout.fill('-');
        cout.width(100);
        cout << "" << endl;

        cout.fill(' ');
        cout << title;
        cout.width(20);
        cout << description;
        cout.width(10);
        cout << accountId;
        cout.width(15);
        cout << transactionDate;
        cout.width(20);
        cout << sentTo;
        cout.width(20);
        cout << amount;
        cout << endl;
    }

    void printFile()
    {
        ofstream file("expense.txt", ios::app);
        file << userId << " " << transactionId << " " << transactionDate << " " << title << " " << description << " " << accountId << " " << entryDate << " " << amount << " " << sentTo << " " << endl;
        file.close();
    }

    void getExpenseInfo()
    {
        cout << "Enter Expense Title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Enter Description: ";
        getline(cin, description);
        cout << "Enter Account ID: ";
        cin >> accountId;
        cout << "Enter Transaction Date (yyyy-mm-dd): ";
        cin.ignore();
        getline(cin, transactionDate);
        cout << "Sent To : /enter '0' if it is not a user to user Transaction : ";
        cin >> sentTo;
        // file.close();
        cout << "Enter Amount: ";
        cin >> amount;
    }

    //RunTime Polymorphism
    void Trigger() override
    {
        if(sentTo != "0")
        {ofstream file("income.txt", ios::app);
        file << sentTo << " " << transactionId << " " << transactionDate << " " << title << " " << description << " " << accountId << " " << entryDate << " " << amount << " "  << endl;
        file.close();}

        ifstream in("users.txt");
        USER temp;
        vector<USER> U;
        while (in >> temp.UserId >> temp.UserName >> temp.Password >> temp.Email >> temp.MobNo >> temp.Profession >> temp.DefaultAccId >> temp.balance)
        {
            U.push_back(temp);
        }
        in.close();

        ofstream out("users.txt");

        for (auto i : U)
        {
            if (i.UserId == userId)
            {
                out << i.UserId << " " << i.UserName << " " << i.Password << " " << i.Email << " " << i.MobNo << " " << i.Profession << " " << i.DefaultAccId << " " << to_string(stof(i.balance) - (amount)) << endl;
            }
            else if (sentTo != "0" && i.UserId == sentTo)
            {
                out << i.UserId << " " << i.UserName << " " << i.Password << " " << i.Email << " " << i.MobNo << " " << i.Profession << " " << i.DefaultAccId << " " << to_string(stof(i.balance) + (amount)) << endl;
            }
            else
            {
                out << i.UserId << " " << i.UserName << " " << i.Password << " " << i.Email << " " << i.MobNo << " " << i.Profession << " " << i.DefaultAccId << " " << i.balance << endl;
            }
        }
        out.close();
    }
};

class user
{
protected:
    string UserId;
    string UserName;
    string Password;
    string Email;
    string MobNo;
    string Profession;
    string DefaultAccId;
    string balance;

public:
    user()
    {
        setUserDetails();
    }

    user(USER &u1)
    {
        UserId = u1.UserId;
        UserName = u1.UserName;
        Password = u1.Password;
        Email = u1.Email;
        MobNo = u1.MobNo;
        Profession = u1.Profession;
        DefaultAccId = u1.DefaultAccId;
        balance = u1.balance;
    }

    void setUserDetails()
    {
        cout << "Enter UserId: ";
        cin >> UserId;
        if (check())
        {
            cout << "Enter UserName: ";
            cin >> UserName;
            cout << "Enter Password: ";
            cin >> Password;
            cout << "Enter Email: ";
            cin >> Email;
            cout << "Enter Mobile Number: ";
            cin >> MobNo;
            cout << "Enter Profession: ";
            cin >> Profession;
            cout << "Enter Default Account Id: ";
            cin >> DefaultAccId;
            cout << "Enter Opening Balance : ";
            cin >> balance;
            writeToFile();
        }
    }

    bool check()
    {
        ifstream in("users.txt");
        USER temp;
        while (in >> temp.UserId >> temp.UserName >> temp.Password >> temp.Email >> temp.MobNo >> temp.Profession >> temp.DefaultAccId >> temp.balance)
        {
            if (temp.UserId == UserId)
            {
                cout << "\nUser Already Exist.....\n\n";
                return false;
            }
        }
        return true;
    }

    void writeToFile()
    {
        ofstream file("users.txt", ios::app);
        if (file.is_open())
        {
            file << UserId << " " << UserName << " " << Password << " " << Email << " " << MobNo << " " << Profession << " " << DefaultAccId << " " << balance << endl;
            file.close();
        }
        else
        {
            cout << "Error opening the file." << endl;
        }
        file.close();
    }
};

class login
{
    string id;
    string password;

public:
    login()
    {
        getinfo();
        check();
    }

    void getinfo()
    {
        cout << "Enter Your Id : ";
        cin >> id;
        cout << "Enter Your Password : ";
        cin >> password;
    }
    void check()
    {
        ifstream file("users.txt");
        USER u1;
        bool flag = false;
        while (file >> u1.UserId >> u1.UserName >> u1.Password >> u1.Email >> u1.MobNo >> u1.Profession >> u1.DefaultAccId >> u1.balance)
        {
            if (u1.UserId == id && u1.Password == password)
            {
                cout << "\n**************************************************  HOME  ******************************************************\n\n";
                flag = true;
                HOME(id);
            }
        }
        if (!flag)
            cout << "\nInvalid ID/PassWord.....\n\n";
        file.close();
    }
};

void profile(string id)
{
    ifstream in("users.txt");
    USER temp;
    while (in >> temp.UserId >> temp.UserName >> temp.Password >> temp.Email >> temp.MobNo >> temp.Profession >> temp.DefaultAccId >> temp.balance)
    {
        if (temp.UserId == id)
        {
            cout << "                         User Id       : "
                 << "   " << temp.UserId << endl;
            cout << "                         Balance       : "
                 << "   " << temp.balance << endl;
            cout << "                         Name          : "
                 << "   " << temp.UserName << endl;
            cout << "                         Email Id      : "
                 << "   " << temp.Email << endl;
            cout << "                         Mobile No     : "
                 << "   " << temp.MobNo << endl;
            cout << "                         Profession    : "
                 << "   " << temp.Profession << endl;
            break;
        }
    }
    in.close();
}


// void SORT()
// {
//     ifstream in("income.txt");
//     Transaction t1;
//     vector<Transaction> v;
//     while (in >> t1.userId >> t1.transactionId >> t1.transactionDate >> t1.title >> t1.description >> t1.accountId >> t1.entryDate >> t1.amount >> t1.recieveFrom_or_SentTo)
//     {
//         string date;
//         date=t1.transactionDate.substr(6,4)+t1.transactionDate.substr(3,2)+t1.transactionDate.substr(0,2);
//         t1.transactionDate=date;  
//         v.push_back(t1);      
//     }
//     in.close();
//     for(int i=0;i<v.size()-1;i++)
//     {
//         bool f=false;
//         for(int j=0;j<v.size()-i-1;j++)
//         {
//             if(v[j].transactionDate > v[j+1].transactionDate)
//             {
//                 auto t=v[j];
//                 v[j]= v[j+1];
//                 v[j+1]=t;
//                 f=true;
//             }

//         }if(!f)
//         {
//             break;
//         }
//     }

//     ofstream out("income.txt");
//     for(int i=0;i<v.size();i++)
//     {
//         string date;
//         date=v[i].transactionDate.substr(6,2)+"-"+v[i].transactionDate.substr(4,2)+"-"+v[i].transactionDate.substr(0,4);
    

//         out<<v[i].userId<<" "<<v[i].transactionId<<" "<<v[i].title<<" " <<v[i].description<<" "<<v[i].accountId<<" "<<v[i].entryDate<<" "<<v[i].amount<<" "<<v[i].recieveFrom_or_SentTo<<endl; 
//     }out.close();


//     ifstream inn("expense.txt");
//     Transaction t2;
//     vector<Transaction> v1;
//     while (inn >> t2.userId >> t2.transactionId >> t2.transactionDate >> t2.title >> t2.description >> t2.accountId >> t2.entryDate >> t2.amount >> t2.recieveFrom_or_SentTo)
//     {
//         string date;
//         date=t2.transactionDate.substr(6,4)+t2.transactionDate.substr(3,2)+t2.transactionDate.substr(0,2);
//         t2.transactionDate=date;  
//         v1.push_back(t1);      
//     }inn.close();
//     for(int i=0;i<v1.size()-1;i++)
//     {
//         bool f=false;
//         for(int j=0;j<v1.size()-i-1;j++)
//         {
//             if(v1[j].transactionDate > v1[j+1].transactionDate)
//             {
//                 auto t=v1[j];
//                 v1[j]= v1[j+1];
//                 v1[j+1]=t;
//                 f=true;
//             }

//         }if(!f)
//         {S
//             break;
//         }
//     }

//     ofstream outt("expense.txt");
//     for(int i=0;i<v1.size();i++)
//     {
//         string date;
//         date=v1[i].transactionDate.substr(6,2)+"-"+v1[i].transactionDate.substr(4,2)+"-"+v1[i].transactionDate.substr(0,4);
    

//         outt<<v1[i].userId<<" "<<v1[i].transactionId<<" "<<v1[i].title<<" " <<v1[i].description<<" "<<v1[i].accountId<<" "<<v1[i].entryDate<<" "<<v1[i].amount<<" "<<v1[i].recieveFrom_or_SentTo<<endl; 
//     }outt.close();
    
// }


void SORT()
{
    set<string> SetDate;
    vector<Transaction> t1(0);
    Transaction temp;
    ifstream in("income.txt");
    
    while (in >> temp.userId >> temp.transactionId >> temp.transactionDate >> temp.title >> temp.description >> temp.accountId >> temp.entryDate >> temp.amount)
    {
        t1.push_back(temp);
        SetDate.insert(temp.transactionDate);
    }
    in.close();
    ofstream out("sorted_transaction.txt");

    for(auto i =SetDate.begin() ; i != SetDate.end() ; i++)
    {
        for(int j=0;j<t1.size();j++)
        {
            if(t1[j].transactionDate == *i)
            {
                out << t1[j].userId << " " << t1[j].transactionId << " " << t1[j].transactionDate << " " << t1[j].title << " " << t1[j].description << " " << t1[j].accountId << " " << t1[j].entryDate << " " << t1[j].amount << " " << endl;
            }
            else{}
        }
    }
    out.close();
    

}







void dailyTransaction(string id)
{
    ifstream in("sorted_transaction.txt");
    Transaction t1;
    cout << "\nIncome Info :-\n\n";
    cout<<setw(20)<<"Title"<<setw(20)<<"Description"<<setw(20)<<"Amount"<<setw(20)<<"Date \n";
    cout<<"------------------------------------------------------------------------------------------------------\n";
    while (in >> t1.userId >> t1.transactionId >> t1.transactionDate >> t1.title >> t1.description >> t1.accountId >> t1.entryDate >> t1.amount )
    {
        if(id==t1.userId)
        {
            cout <<setw(20)<< t1.title <<setw(20)<< t1.description <<setw(20)<< t1.amount  <<setw(20)<<t1.transactionDate<< endl;
        }   
    }
    in.close();

    ifstream innn("expense.txt");
    Transaction t2;
    cout << "\nExpense Info :-\n\n";
    cout<<setw(20)<<"Title"<<setw(20)<<"Description"<<setw(20)<<"Amount"<<setw(20)<<"SendTo"<<setw(20)<<"Date \n";
    cout<<"------------------------------------------------------------------------------------------------------\n";

    while (innn >> t2.userId >> t2.transactionId >> t2.transactionDate >> t2.title >> t2.description >> t2.accountId >> t2.entryDate >> t2.amount >> t2.recieveFrom_or_SentTo)
    {
        if(id==t2.userId)
        {
            cout << setw(20)<<t2.title <<setw(20)<< t2.description <<setw(20)<< t2.amount <<setw(20)<< t2.recieveFrom_or_SentTo <<setw(20)<<t2.transactionDate<< endl;
        }   
    }
    innn.close();
}

void HOME(string id)
{
    income *i;
    expense *e;

    cout << "1.Profile \n";
    cout << "2.Income Entry\n";
    cout << "3.Expense Entry\n";
    cout << "4.Daily Transaction\n";
    cout << "5.Sort Transaction\n";
    cout << "6.Log Out\n";

    char c;
    cin >> c;
    while (c != '6')
    {
        switch (c)
        {
        case '1':
            cout << "\nProfile :- \n\n";
            profile(id);
            break;

        case '2':
            i = new income(id);
            delete (i);
            break;

        case '3':
            e = new expense(id);
            delete (e);
            break;

        case '4':
            dailyTransaction(id);
            break;
        case '5':
            SORT();
            cout<<"\nSuccessfully Sorted .....\n\n";
            break;
        }

        cin >> c;
    }

    cout << "\nLogging Out.......\n\n";
    cout << "1.New Registration\n2.Log_In\n3.Exit\n";
}

int main()
{
    cout << "1.New Registration\n2.Log_In\n3.Exit\n";
    char choice;
    cin >> choice;
    

    user *u;
    login *l;
    while (choice != '3')
    {
        switch (choice)
        {
        case '1':
            u = new user;
            break;
        case '2':
            l = new login;
            break;

        default:
            cout<<"\nInvalid Choice ....\n\n";
            fflush(stdin);
            break;
        }
        cin >> choice;
    }
    cout << "\n-----X-----X-----X-----X-----Exited-----X-----X-----X-----X-----\n\n";

    return 0;
}