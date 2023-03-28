#include "DataBase.h"


int main()
{
    DataBase db;
    db.fill_file("data.csv");

    db.edit_file("data.csv", "new_data.csv");

    db.create_sql_from_csv("data.csv");

    return 0;
}
