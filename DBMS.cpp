#include <bits/stdc++.h>
using namespace std;
fstream schema;
fstream table;
const char *c;
string line;

vector<string> Table;

void createTable(vector<string> cmd)
{
    ofstream schema("Schema.txt",std::ios_base::app);
    string table_name = cmd[2];
    Table.push_back(table_name);
    schema<<table_name;
    if(cmd[3]!="(" || cmd[cmd.size()-1]!=";" || cmd[cmd.size()-2]!=")")
    {
            cout<<"\tNot a correct syntax check brackets and ; "<<endl;
    }

    int start = -1, end = -1;
    for (int i = 3; i < cmd.size(); i++) {
        if (cmd[i] == "(") {
            start = i;
        }
        if (cmd[i] == ")") {
            end = i;
        }
    }

    if (start == -1 || end == -1) {
        cout << "Error" << endl;
        return;
    }

    while (start < end - 1) {
        start++;
        if (cmd[start] != ",") {
            schema << "#" << cmd[start];
        }
    }
    schema <<""<< endl;
    // create file for table
    fstream table;
    table_name += ".txt";
    const char *c = table_name.c_str();
    table.open(c,ios_base::out);
    table.close();
    cout << "Table created successfully" << endl;
    schema.close();
}

//Drop table
void dropTable(vector<string> cmd)
{
    fstream temp;
    fstream table;
    temp.open("temp.txt", ios_base::out);

    string table_name = cmd[2];
    string line;

    while(getline(schema, line)) {
        string l1 = line.substr(0, line.find('#'));
        if (table_name != l1) {
            temp << line << endl;
        }
    }
    schema.close();
    temp.close();
    // deleting schema of table
    remove("Schema.txt");
    rename("temp.txt", "Schema.txt");
    // deleting data from table file
    table_name += ".txt";
    const char *c = table_name.c_str();
    ifstream file;
    file.open(c);
    if(file)
    {
        file.close();
        remove(c);

        cout << "Table dropped successfully" << endl;
    }
    else{
        cout<<"Table file not exist"<<endl;
    }

}

//Truncate table
void truncateTable(vector<string> cmd)
{
    string table_name = cmd[2];
    // Deleting whole data and table file but not schema
    table_name += ".txt";
    const char *c = table_name.c_str();
    ofstream file;
    file.open(c);
    if(file){
        file.open(c, std::ofstream::out | std::ofstream::trunc);
        file.close();
        cout << "Table truncate successfully" << endl;
    }
    else{
        cout<<"Table file not exist"<<endl;
    }
}

//Describe table
void describe(vector<string> cmd)
{
    if (cmd.size() > 1) {
        string table_name = cmd[1];
        string line;

        while (getline(schema, line)) {
            string l1 = line.substr(0, line.find('#'));

            if (table_name == l1) {
                cout<<" "<<endl;
                cout<<"   Table Name : "<<table_name<<endl;
                cout<<" "<<endl;
                cout<<" Column\t\t"<<" Data type"<<endl;
                cout<<"__________\t ___________"<<endl;
                string l2 = line.substr(line.find('#') + 1, line.size());
                int cnt = 1;

                for (int i=0; i<l2.length(); i++) {
                    if (l2[i] == '#') {
                        if (cnt % 2 != 0) {
                            cout << "\t  \t";
                        } else {
                            cout <<"\n "<< endl;
                        }
                        cnt++;
                    } else {
                        cout <<l2[i];
                    }
                }
            }
        }
    } else {
        cout << "please mention table name" << endl;
    }

}

//Insert function
void insert_Into(vector<string> cmd)
{
    if(cmd[3]!="values" || cmd[4]!="(" || cmd[cmd.size()-1]!=";" || cmd[cmd.size()-2]!=")")
    {cout<<" error check brackets ; or values may be missing "<<endl;}

    string table_name = cmd[2];
    table_name += ".txt";
    const char *c = table_name.c_str();
    ofstream table(c,std::ios_base::app);

    int start = -1, end = -1;
    for (int i=4; i<cmd.size(); i++) {
        if (cmd[i] == "(") {
            start = i;
        }
        if (cmd[i] == ")") {
            end = i;
        }
    }

    if (start == -1 || end == -1) {
        cout << "Error" << endl;
        return;
    }
    while (start < end - 1) {
        start++;
        if (cmd[start] != ",") {
            table << cmd[start];
            if (start != end - 1) {
                table << "#";
            } else {
                table << "\n";
            }
        }
    }
    table.close();
    cout << "Tuple inserted successfully" << endl;
}


//Delete
void delete_from(vector<string> cmd)
{
    //delete from D where ball = 67 ;
    string table_name = cmd[2];
    if(cmd[cmd.size()-1]!=";" )
    {
        cout<<"Invalid Termination of command"<<endl;
    }
    vector<string> Schema;
    schema.open("Schema.txt", ios_base::in);
    string line;
    int flag = 0;

    while (getline(schema, line))
    {
        string l1 = line.substr(0, line.find('#'));
        if (cmd[2] == l1){
            flag = 1;
            stringstream ss(line);

            while (ss.good()){
                string substr;
                getline(ss, substr, '#');
                Schema.push_back(substr);
            }
            break;
        }
    }
    if (flag == 0){
        cout << "table not found" << endl;
    }

    schema.close();
    //Inserting Table Schema into vector named Schema

    int count = 0;

    if (!Schema.empty())
    {
        if (cmd.size() == 4)
        {
            string table = cmd[2] + ".txt";
            const char *c = table.c_str();
            ofstream file;
            file.open(c, std::ofstream::out | std::ofstream::trunc);
            file.close();

        }
        else if (cmd[3] == "where")
        {
            string table_name = cmd[2];
            table_name+=".txt";
            const char *c = table_name.c_str();
            //fstream table(c,std::ios_base::app);
            fstream table;
            table.open(c, ios_base::in);

            string line;

            int i, j;
            j = 0;
            int flag = 0;
            vector<string> lineVec;

            //fstream temp;
            //temp.open("temp.txt", ios_base::out);
            ofstream temp("temp.txt",std::ios_base::out);

            while (getline(table, line))
            {
                stringstream ss(line);
                while (ss.good())
                {
                    string substr;
                    getline(ss, substr, '#');
                    lineVec.push_back(substr);
                }

                for (i = 1; i < Schema.size(); i += 2)
                {
                    if (cmd[4] == Schema[i])
                    {
                        if (cmd[5] == "=")
                        {
                            if (cmd[6] == lineVec[j])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (cmd[5] == ">")
                        {
                            if (lineVec[j] < cmd[6])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (cmd[5] == "<")
                        {
                            if (lineVec[j] < cmd[6] )
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (cmd[5] == ">=")
                        {
                            if ( lineVec[j] >= cmd[6])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (cmd[5] == "<=")
                        {
                            if (lineVec[j] <= cmd[6])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                        if (cmd[5] == "!=")
                        {
                            if (cmd[6] != lineVec[j])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                    }
                    j++;
                }

                if (flag != 1)
                {
                    temp << line << endl;
                }
                flag = 0;

            }
            table.close();
            temp.close();

            string table1 = cmd[2] + ".txt";
            const char *ch = table1.c_str();
            remove(ch);
            rename("temp.txt", ch);
        }
        cout << " row deleted successfully !!!" << endl;
    }
}

//check column number in table
void table_number(map<string, int>& table, vector<string> Schema){
    int cnt = 0;
    for(int j=1; j<Schema.size(); j+=2){
        table[Schema[j]] = cnt;
        cnt++;
    }
}

//Check column of table
void checkCommand(map<int, string>& column_change, vector<string> cmd, vector<string> Schema){
    int i;
    map<string, int> table;
    table_number(table, Schema);
    for(i=3; i<cmd.size() && cmd[i]!="where"; i+=3){
        int col = table[cmd[i]];
        column_change[col] = cmd[i+2];
    }
}

//update
void update(vector<string> cmd)
{
    vector<string> Schema;
    schema.open("Schema.txt", ios_base::in);
    string line;
    int flag = 0;

    while (getline(schema, line))
    {
        string l1 = line.substr(0, line.find('#'));
        if (cmd[1] == l1){
            flag = 1;
            stringstream ss(line);

            while (ss.good()){
                string substr;
                getline(ss, substr, '#');
                Schema.push_back(substr);
            }
            break;
        }
    }
    if (flag == 0){
        cout << "table not found" << endl;
    }

    schema.close();

    if(!Schema.empty())
    {
        string table_name = cmd[1] + ".txt";
        fstream table;
        const char *c = table_name.c_str();
        table.open(c, ios_base::in);
        vector<string>::iterator it;
        it = find(cmd.begin(), cmd.end(), "where");
        map<int, string> column_change;
        checkCommand(column_change, cmd, Schema);

        string line;
        vector<string> lineVec;
        int i, j=0;
        int flag = 0;
        int count = 0;
        fstream temp;
        temp.open("temp.txt", ios_base::out);


        //if where condition is in command
        if(it != cmd.end())
        {
            while (getline(table, line))
            {
                string tab = line;
                i=0;
                string curr = "";
                while(i<tab.length()){
                    if(tab[i] == '#'){
                        lineVec.push_back(curr);
                        curr = "";
                    }else{
                        curr += tab[i];
                    }
                    i++;
                }
                lineVec.push_back(curr);

                int idx = 0;
                idx = it - cmd.begin();
                j=0;
                for(i=1; i<Schema.size(); i+=2){
                    if(cmd[idx+1] == Schema[i]){
                        //Equality condition
                        if(cmd[idx+2] == "="){
                            if(lineVec[j] == cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //Greater than condition
                        if(cmd[idx+2] == ">"){
                            if(lineVec[j] > cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //Greater than or equal to
                        if(cmd[idx+2] == ">="){
                            if(lineVec[j] >= cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //smaller than condition
                        if(cmd[idx+2] == "<"){
                            if(lineVec[j] < cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //smaller than and equal to
                        if(cmd[idx+2] == "<="){
                            if(lineVec[j] <= cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                        //Not equal to condition
                        if(cmd[idx+2] == "!="){
                            if(lineVec[j] != cmd[idx+3]){
                                flag = 1;
                                count++;
                            }
                        }
                    }
                    j++;
                }
                //Not making any changes in the row
                if(flag != 1){
                    temp << line << endl;
                    lineVec.clear();
                }
                //make changes in a row
                else{
                    string new_line = "";
                    map<int, string>::iterator itr;
                    for(itr=column_change.begin(); itr!=column_change.end(); itr++){
                        lineVec[itr->first] = itr->second;
                    }

                    for(i=0; i<lineVec.size()-1; i++){
                        new_line += lineVec[i];
                        new_line += "#";
                    }
                    new_line += lineVec[i];
                    temp << new_line << endl;
                    lineVec.clear();
                }
                flag = 0;
            }
        }
        //if where condition is not in command
        else
        {
            while(getline(table, line))
            {
                string tab = line;
                i=0;
                string curr = "";
                while(i<tab.length()){
                    if(tab[i] == '#'){
                        lineVec.push_back(curr);
                        curr = "";
                    }else{
                        curr += tab[i];
                    }
                    i++;
                }
                lineVec.push_back(curr);

                for(int k=0;i<lineVec.size();k++)
                {
                    cout<<lineVec[i]<<endl;
                }

                string new_line = "";
                map<int, string>::iterator itr;
                for(itr=column_change.begin(); itr!=column_change.end(); itr++){
                    lineVec[itr->first] = itr->second;
                }

                for(i=0; i<lineVec.size()-1; i++){
                    new_line += lineVec[i];
                    new_line += "#";
                }
                new_line += lineVec[i];
                temp << new_line << endl;
                lineVec.clear();
            }
        }


        table.close();
        temp.close();
        remove(c);
        rename("temp.txt", c);
        cout<<"Rows updated successfully"<<endl;


    }
    else{
        cout<<"Table not Available "<<endl;
    }

}

//select
void select_from(vector<string> cmd)
{
     if(cmd[cmd.size()-1]!=";" ){
        cout<<"Invalid Termination of command"<<endl;
    }

    // iterator from
    vector<string>::iterator it_from;
    it_from = find(cmd.begin(), cmd.end(), "from");

    if(it_from != cmd.end())
    {
        int idx_table = 0;
        idx_table = it_from - cmd.begin();
        string table_name = cmd[idx_table+1];
        vector<string> Schema;
        schema.open("Schema.txt", ios_base::in);
        string line;
        int flag = 0;

       while (getline(schema, line))
       {
        string l1 = line.substr(0, line.find('#'));
        if (table_name == l1){
            flag = 1;
            stringstream ss(line);

            while (ss.good()){
                string substr;
                getline(ss, substr, '#');
                Schema.push_back(substr);
            }
            break;
        }
       }
       if (flag == 0){
        cout << "table not found" << endl;
       }

    schema.close();

        if(!Schema.empty())
        {
            table_name += ".txt";
            fstream table;
            const char *c = table_name.c_str();
            table.open(c, ios_base::in);
            // iterator where
            vector<string>::iterator it_where;
            it_where = find(cmd.begin(), cmd.end(), "where");
            vector<int> attribute;

            string line;
            int i, j=0;
            int flag = 0;
            int count = 0;
            vector<string> lineVec;
            map<string, int> table_num;
            //check column number in table
            int cnt = 0;
            for(int j=1; j<Schema.size(); j+=2){
                table_num[Schema[j]] = cnt;
                cnt++;
            }

            //Select all attributes
            if(cmd[1] == "*"){
                for(i=1; i<Schema.size(); i+=2)
                    attribute.push_back(table_num[Schema[i]]);
            }
            //Select attributes which mentioned in the command
            else{
                for(i=1; i<idx_table; i+=2){
                    attribute.push_back(table_num[cmd[i]]);
                }
            }

            //if where condition is not in command
            if(it_where == cmd.end())
            {
                while(getline(table, line))
                {
                    string tab = line;
                    i=0;
                    string curr = "";
                    while(i<tab.length()){
                        if(tab[i] == '#'){
                            lineVec.push_back(curr);
                            curr = "";
                        }else{
                            curr += tab[i];
                        }
                        i++;
                    }

                    lineVec.push_back(curr);

                    string new_line = "";
                    for(i=0; i<attribute.size(); i++){
                        new_line += lineVec[attribute[i]];
                        new_line += "    ";
                    }
                    cout<<new_line<<endl;
                    lineVec.clear();
                }

            }
            //where condition is in command
            else
            {
                while (getline(table, line))
                {
                    string tab = line;
                    i=0;
                    string curr = "";
                    while(i<tab.length()){
                        if(tab[i] == '#'){
                            lineVec.push_back(curr);
                            curr = "";
                        }else{
                            curr += tab[i];
                        }
                        i++;
                    }
                    lineVec.push_back(curr);

                    int idx = 0;
                    idx = it_where - cmd.begin();
                    for(i=1; i<Schema.size(); i+=2)
                    {
                        if(cmd[idx+1] == Schema[i]){
                            if(cmd[idx+2] == "="){
                                if(cmd[idx+3] == lineVec[j]){
                                    flag = 1;
                                    count++;
                                }
                            }
                            if(cmd[idx+2] == ">"){
                                if(cmd[idx+3] > lineVec[j]){
                                    flag = 1;
                                    count++;
                                }
                            }
                            if(cmd[idx+2] == ">="){
                                if(cmd[idx+3] >= lineVec[j]){
                                    flag = 1;
                                    count++;
                                }
                            }
                            if(cmd[idx+2] == "<"){
                                if(cmd[idx+3] < lineVec[j]){
                                    flag = 1;
                                    count++;
                                }
                            }
                            if(cmd[idx+2] == "<="){
                                if(cmd[idx+3] <= lineVec[j]){
                                    flag = 1;
                                    count++;
                                }
                            }
                            if(cmd[idx+2] == "!="){
                                if(cmd[idx+3] != lineVec[j]){
                                    flag = 1;
                                    count++;
                                }
                            }
                        }
                        j++;
                    }
                    // select proper rows from table
                    if(flag == 1){
                        string new_line = "";
                        for(i=0; i<attribute.size(); i++){
                            new_line += lineVec[attribute[i]];
                            new_line += "    ";
                        }
                        cout<<new_line<<endl;

                    }
                    flag = 0;
                    lineVec.clear();
                }
            }
            table.close();

        }
        else {
        cout<<"Table is not available"<<endl;
       }
    }
}

//help tables function
void help_tables(vector<string> cmd)
{
    if(cmd[2]!=";")
    {
        cout<<" Not a proper Termination of command "<<endl;
    }
    else{
        string line;

       schema.open("Schema.txt");

       int count = 0;
       while (getline(schema, line))
       {
        string l1 = line.substr(0, line.find('#'));
        cout << l1 << endl;
        count++;
       }
       if (!count)
       {
        cout << "No tables found" << endl;
       }
       schema.close();
    }
}

//help cmd function
void help_cmd(vector<string> cmd)
{
    if(cmd[1]=="create" && cmd[2]=="table" && cmd[3]==";")
    {
        cout<<" CREATE TABLE statement is used to create a new table in the database. "<<endl;
        cout<<" To create a table, you have to name that table and define its columns and datatype for each column."<<endl;
        cout<<" Syntax: CREATE TABLE table_name;"<<endl;
    }
    else if(cmd[1]=="drop" && cmd[2]=="table" && cmd[3]==";")
    {
        cout<<" DROP TABLE statement is used to remove or delete a table from the database."<<endl;
        cout<<" It deletes both the data and schema "<<endl;
        cout<<" Syntax: DROP TABLE table_name;"<<endl;
    }
    else if(cmd[1]=="truncate" && cmd[2]=="table" && cmd[3]==";")
    {
        cout<<" TRUNCATE TABLE statement is used to remove all records from a table"<<endl;
        cout<<" Warning: If you truncate a table, the TRUNCATE TABLE statement can not be rolled back."<<endl;
        cout<<" TRUNCATE TABLE table_name;"<<endl;
    }
    else if(cmd[1]=="describe" && cmd[2]==";")
    {
        cout<<" To show the structure of a database table"<<endl;
        cout<<" Syntax: DESCRIBE one; OR DESC one;"<<endl;
    }
    else if(cmd[1]=="insert"&& cmd[2] == "into" && cmd[3]==";")
    {
        cout<<"The insert command is used for inserting one or more rows into a database table with specified table column values."<<endl;
        cout<<" INSERT INTO TABLE_NAME (column1, column2, column3,...columnN) VALUES (value1, value2, value3,...valueN);"<<endl;
    }
    else if(cmd[1]=="delete" && cmd[2]==";")
    {
        cout<<"DELETE statement removes one or more records from a table."<<endl;
        cout<<" Syntax: DELETE FROM table_name WHERE condition; OR DELETE FROM table_name;"<<endl;
    }
    else if(cmd[1]=="update" && cmd[2]==";")
    {
        cout<<" UPDATE statement changes the data of one or more records in a table."<<endl;
        cout<<" Syntax: UPDATE table_name SET attr1_name = attr1_value,attr2_name = attr2_value… WHERE condition_list"<<endl;

    }
    else if(cmd[1]=="select" && cmd[2]==";")
    {
            cout<<" The SELECT statement is used to select data from a database."<<endl;
            cout<<" Syntax:  SELECT column1, column2, columnN FROM table_name; "<<endl;
            cout<<"     OR   SELECT column1, ...,columnN FROM table_name WHERE condition_list; "<<endl;
    }
    else
    {
            cout<<" Invalid command "<<endl;
    }
}

void handleCmd(vector<string> cmd){
    //Create table
    if (cmd[0] == "create" && cmd[1] == "table") {
        createTable(cmd);
    }
    //Drop table
    else if (cmd[0] == "drop" && cmd[1] == "table") {
        dropTable(cmd);
    }
    //Truncate table
    else if (cmd[0] == "truncate" && cmd[1] == "table") {
        truncateTable(cmd);
    }
    //Describe
    else if (cmd[0] == "describe" || cmd[0] == "desc") {
        describe(cmd);
    }
    //Insert
    else if (cmd[0] == "insert" && cmd[1] == "into") {
        insert_Into(cmd);
    }
    //Delete
    else if (cmd[0] == "delete" && cmd[1] == "from" ) {
        delete_from(cmd);
    }
    //Update
    else if (cmd[0] == "update" && cmd[2] == "set" ) {
        update(cmd);
    }
    //Select
    else if (cmd[0] == "select" && cmd[2] == "from" ) {
        select_from(cmd);
    }

    //Help tables
    else if ((cmd[0] == "help" || cmd[0]=="HELP")&& (cmd[1] == "tables" || cmd[1]=="TABLES")) {
        help_tables(cmd);
    }
    //Help CMD
    else if (cmd[0] == "help" || cmd[0]=="HELP") {
        help_cmd(cmd);
    }
    else if (cmd[0] == "exit" || cmd[0] == "Exit" || cmd[0]=="quit" || cmd[0]=="QUIT") {
            cout<<"Exit Successfully !!!"<<endl;
        exit(0);
    }
    //Wrong command
    else {
        cout <<"Syntax Error (Wrong command)";
    }
}

void convertToVector(string input, vector<string> &v){
    string word = "";
    int i=0;
    while(i<input.length()){
        if(input[i] == ' '){
            v.push_back(word);
            word = "";
        }else{
            word = word + input[i];
        }
        i++;
    }
    v.push_back(word);
}

int main()
{
    vector<string> cmd;
    string input;
    schema.open("Schema.txt");
    //table.open(c);

    cout<<"  "<<endl;
    cout<<"\t *** Welcome to our small-scale Relational Database Management System (DBMS) ***"<<endl;
    cout<<"\t\t\t_________________________________________________"<<endl;
    cout<<"  "<<endl;
    cout<<"Here you can perform following operations :"<<endl;
    cout<<"\t\t1. Create table\n\t\t2. Drop table\n\t\t3. Truncate table\n\t\t4. Describe"<<endl;
    cout<<"\t\t5. Insert into\n\t\t6. Delete table\n\t\t7. Update table\n\t\t8. Select "<<endl;
    cout<<"\t\t9. HELP TABLES\n\t\t10. HELP CMD \n\t\t11. Exit"<<endl;
    cout<<"Enter your SQL command: "<<endl;
    getline(cin, input);
    while (input != "Exit" || input!="exit") {
        convertToVector(input, cmd);
        handleCmd(cmd);
        cmd.clear();
        cout<<endl;
        cout<<"Enter your next SQL command: "<<endl;
        getline(cin, input);
    }
    //table.close();
    schema.close();
    return 0;
}
