//
// Created by kpilavov on 28.03.23.
//


#include <ctime>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include "DataBase.h"


const char DataBase::m_charset[] =
"0123456789"
"abcdefghijklmnopqrstuvwxyz"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ";


DataBase::DataBase() {
    // initialize random number generator
    srand(time(nullptr));
}

// generate string with random symbols
std::string DataBase::generate_random_string() const{
    std::string result;
    result.resize(m_symbols_quantity);

    for (int i = 0; i < m_symbols_quantity; ++i) {
        result[i] = m_charset[rand() % (sizeof(m_charset) - 1)];
    }

    return result;
}

void DataBase::fill_file(const std::string &filename) const {
    std::ofstream outfile(filename);

    // generate 1024 m_rows
    for (int i = 0; i < m_rows; ++i) {
        // generate 6 m_columns
        for (int j = 0; j < m_columns; ++j) {
            // generate string and put in file
            // comma is m_separator
            outfile << generate_random_string() << m_separator;
        }
        outfile << "\n";
    }

    outfile.close();

}

bool DataBase::is_vowel(char c) const {
    std::string vowels = "AEIOUaeiou";
    return vowels.find(c) != std::string::npos;
}

void DataBase::edit_file(const std::string &input_filename, const std::string &output_filename) const {
    TwoDimVector data = read_csv(input_filename);

    edit_data_in_file(data);

    save_edited_file(data,output_filename);
}

TwoDimVector DataBase::read_csv(const std::string &input_filename) const {
    TwoDimVector data;
    std::ifstream infile(input_filename);

    // read rows from file
    std::string line;
    while (getline(infile, line)) {
        std::vector<std::string> row;
        size_t pos = 0;
        while ((pos = line.find(m_separator)) != std::string::npos) {
            row.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }

        data.push_back(row);
    }
    infile.close();
    return data;
}

void DataBase::edit_data_in_file(TwoDimVector &data) const {
    for (auto& row : data) {
        // changing odd numbers with #
        for (auto& col : row) {
            for (char& c : col) {
                if (isdigit(c) && (c - '0') % 2 != 0) {
                    c = '#';
                }
            }
        }

        // deleting row if all entries starts with vowel
        bool remove_row = true;
        for (const auto& col : row) {
            if (!is_vowel(col[0])) {
                remove_row = false;
                break;
            }
        }
        if (remove_row) {
            row.clear();
        }
    }
}

void DataBase::save_edited_file(const TwoDimVector &data, const std::string &output_file) const {
    std::ofstream outfile(output_file);

    for (const auto& row : data)
    {
        if (!row.empty())
        {
            for (const auto & column : row)
            {
                outfile << column << m_separator;
            }
            outfile << std::endl;
        }
    }

    outfile.close();
}

void DataBase::create_sql_from_csv(const std::string &filename) const {
    sqlite3 *db;
    char* zErrMsg = 0;

    int rc = sqlite3_open("database.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(1);
    }

    const char *sql_delete = "DROP TABLE IF EXISTS data;";
    rc = sqlite3_exec(db, sql_delete, 0, 0, &zErrMsg);
    check_possible_sql_error(rc, zErrMsg, db);

    const char *sql_create = "CREATE TABLE data ("
                             "column1 TEXT, "
                             "column2 TEXT, "
                             "column3 TEXT, "
                             "column4 TEXT, "
                             "column5 TEXT, "
                             "column6 TEXT);";

    rc = sqlite3_exec(db, sql_create, 0, 0, &zErrMsg);
    check_possible_sql_error(rc, zErrMsg, db);

    TwoDimVector data = read_csv(filename);
    for (const auto &row : data) {
        std::stringstream ss;
        ss << "INSERT INTO data (column1, column2, column3, column4, column5, column6) VALUES ("
           << "'" << row[0] << "', "
           << "'" << row[1] << "', "
           << "'" << row[2] << "', "
           << "'" << row[3] << "', "
           << "'" << row[4] << "', "
           << "'" << row[5] << "');";

        rc = sqlite3_exec(db, ss.str().c_str(), 0, 0, &zErrMsg);
        check_possible_sql_error(rc, zErrMsg, db);
    }

    edit_database(db);

    sqlite3_close(db);
}

void DataBase::edit_database(sqlite3 *db) const {
    const char *sql_delete = "DELETE FROM data WHERE substr(column2, 1, 1) IN ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9');";
    char* zErrMsg = 0;
    int rc = sqlite3_exec(db, sql_delete, 0, 0, &zErrMsg);
    check_possible_sql_error(rc, zErrMsg, db);
}

void DataBase::check_possible_sql_error(int rc, const char *zErrMsg, sqlite3 *db) const {
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(&zErrMsg);
        sqlite3_close(db);
        exit(1);
    }
}



